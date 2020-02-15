#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include <Eigen/Geometry>
#include <Eigen/Dense>

template<typename T> inline
T DotProduct(const T x[3], const T y[3]) {
  return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);
}

template <typename T> inline
void QuaternionToScaledRotation(const T q[4], T R[3 * 3]) {
  // Make convenient names for elements of q.
  T a = q[0];
  T b = q[1];
  T c = q[2];
  T d = q[3];
  // This is not to eliminate common sub-expression, but to
  // make the lines shorter so that they fit in 80 columns!
  T aa = a * a;
  T ab = a * b;
  T ac = a * c;
  T ad = a * d;
  T bb = b * b;
  T bc = b * c;
  T bd = b * d;
  T cc = c * c;
  T cd = c * d;
  T dd = d * d;

  R[0] =  aa + bb - cc - dd; R[1] = T(2) * (bc - ad); R[2] = T(2) * (ac + bd);  // NOLINT
  R[3] = T(2) * (ad + bc); R[4] =  aa - bb + cc - dd; R[5] = T(2) * (cd - ab);  // NOLINT
  R[6] = T(2) * (bd - ac); R[7] = T(2) * (ab + cd); R[8] =  aa - bb - cc + dd;  // NOLINT
}

template <typename T> inline
void QuaternionToRotation(const T q[4], T R[3 * 3]) {
  QuaternionToScaledRotation(q, R);

  T normalizer = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
  //CHECK_NE(normalizer, T(0));
  normalizer = T(1) / normalizer;

  for (int i = 0; i < 9; ++i) {
    R[i] *= normalizer;
  }
}

template <typename T> //Sequence Z Y X
inline void EulerAnglesToRotationMatrix(const T* euler,
                                        const int row_stride,
                                        T* R) {
  //const T degrees_to_radians(M_PI / 180.0);

  const T pitch(euler[0]);// * degrees_to_radians);
  const T roll(euler[1]);// * degrees_to_radians);
  const T yaw(euler[2]);// * degrees_to_radians);

  const T c1 = cos(yaw);
  const T s1 = sin(yaw);
  const T c2 = cos(roll);
  const T s2 = sin(roll);
  const T c3 = cos(pitch);
  const T s3 = sin(pitch);

  // Rows of the rotation matrix.
  T* R1 = R;
  T* R2 = R1 + row_stride;
  T* R3 = R2 + row_stride;

  R1[0] = c1*c2;
  R1[1] = -s1*c3 + c1*s2*s3;
  R1[2] = s1*s3 + c1*s2*c3;

  R2[0] = s1*c2;
  R2[1] = c1*c3 + s1*s2*s3;
  R2[2] = -c1*s3 + s1*s2*c3;

  R3[0] = -s2;
  R3[1] = c2*s3;
  R3[2] = c2*c3;
}

template <typename T>
inline void AngleAxisToRotationMatrix(const T * angle_axis, T * R) {
  static const T kOne = T(1.0);

  const T theta2 = DotProduct(angle_axis, angle_axis);
  if (theta2 > 0.0) {
    // We want to be careful to only evaluate the square root if the
    // norm of the angle_axis vector is greater than zero. Otherwise
    // we get a division by zero.
    const T theta = sqrt(theta2);
    const T wx = angle_axis[0] / theta;
    const T wy = angle_axis[1] / theta;
    const T wz = angle_axis[2] / theta;

    const T costheta = cos(theta);
    const T sintheta = sin(theta);
    //change ceres column major to row major
    R[0] =     costheta   + wx*wx*(kOne -    costheta);
    R[3] =  wz*sintheta   + wx*wy*(kOne -    costheta);
    R[6] = -wy*sintheta   + wx*wz*(kOne -    costheta);
    R[1] =  wx*wy*(kOne - costheta)     - wz*sintheta;
    R[4] =     costheta   + wy*wy*(kOne -    costheta);
    R[7] =  wx*sintheta   + wy*wz*(kOne -    costheta);
    R[2] =  wy*sintheta   + wx*wz*(kOne -    costheta);
    R[5] = -wx*sintheta   + wy*wz*(kOne -    costheta);
    R[8] =     costheta   + wz*wz*(kOne -    costheta);
  } else {
    // At zero, we switch to using the first order Taylor expansion.
    R[0] =  kOne;
    R[1] = -angle_axis[2];
    R[2] =  angle_axis[1];
    R[3] =  angle_axis[2];
    R[4] =  kOne;
    R[5] = -angle_axis[0];
    R[6] = -angle_axis[1];
    R[7] =  angle_axis[0];
    R[8] = kOne;
  }
}

template<typename T>
inline void AngleAxisToQuaternion(const T* angle_axis, T* quaternion) {
  const T &a0 = angle_axis[0];
  const T &a1 = angle_axis[1];
  const T &a2 = angle_axis[2];
  const T theta_squared = a0 * a0 + a1 * a1 + a2 * a2;

  // For points not at the origin, the full conversion is numerically stable.
  if (theta_squared > T(0.0)) {
    const T theta = sqrt(theta_squared);
    const T half_theta = theta * T(0.5);
    const T k = sin(half_theta) / theta;
    quaternion[0] = cos(half_theta);
    quaternion[1] = a0 * k;
    quaternion[2] = a1 * k;
    quaternion[3] = a2 * k;
  } else {
    // At the origin, sqrt() will produce NaN in the derivative since
    // the argument is zero.  By approximating with a Taylor series,
    // and truncating at one term, the value and first derivatives will be
    // computed correctly when Jets are used.
    const T k(0.5);
    quaternion[0] = T(1.0);
    quaternion[1] = a0 * k;
    quaternion[2] = a1 * k;
    quaternion[3] = a2 * k;
  }
}


void print_matrix33(double* R){

    cout<<R[0]<<" "<<R[1]<<" "<<R[2]<<endl;
    cout<<R[3]<<" "<<R[4]<<" "<<R[5]<<endl;
    cout<<R[6]<<" "<<R[7]<<" "<<R[8]<<endl;
}

template<typename T>
inline void QuaternionToAngleAxis(const T* quaternion, T* angle_axis) {
  const T &q1 = quaternion[1];
  const T &q2 = quaternion[2];
  const T &q3 = quaternion[3];
  const T sin_squared = q1 * q1 + q2 * q2 + q3 * q3;

  // For quaternions representing non-zero rotation, the conversion
  // is numerically stable.
  if (sin_squared > T(0.0)) {
    const T sin_theta = sqrt(sin_squared);
    const T k = T(2.0) * atan2(sin_theta, quaternion[0]) / sin_theta;
    angle_axis[0] = q1 * k;
    angle_axis[1] = q2 * k;
    angle_axis[2] = q3 * k;
  } else {
    // For zero rotation, sqrt() will produce NaN in the derivative since
    // the argument is zero.  By approximating with a Taylor series,
    // and truncating at one term, the value and first derivatives will be
    // computed correctly when Jets are used.
    const T k(2.0);
    angle_axis[0] = q1 * k;
    angle_axis[1] = q2 * k;
    angle_axis[2] = q3 * k;
  }
}

template <typename T>
inline void RotationMatrixToAngleAxis(const T * R, T * angle_axis) {
  // x = k * 2 * sin(theta), where k is the axis of rotation.
  angle_axis[0] = R[5] - R[7];
  angle_axis[1] = R[6] - R[2];
  angle_axis[2] = R[1] - R[3];

  static const T kOne = T(1.0);
  static const T kTwo = T(2.0);

  // Since the right hand side may give numbers just above 1.0 or
  // below -1.0 leading to atan misbehaving, we threshold.
  T costheta = std::min(std::max((R[0] + R[4] + R[8] - kOne) / kTwo,
                                 T(-1.0)),
                        kOne);

  // sqrt is guaranteed to give non-negative results, so we only
  // threshold above.
  T sintheta = std::min(sqrt(angle_axis[0] * angle_axis[0] +
                             angle_axis[1] * angle_axis[1] +
                             angle_axis[2] * angle_axis[2]) / kTwo,
                        kOne);

  // Use the arctan2 to get the right sign on theta
  const T theta = atan2(sintheta, costheta);

  // Case 1: sin(theta) is large enough, so dividing by it is not a
  // problem. We do not use abs here, because while jets.h imports
  // std::abs into the namespace, here in this file, abs resolves to
  // the int version of the function, which returns zero always.
  //
  // We use a threshold much larger then the machine epsilon, because
  // if sin(theta) is small, not only do we risk overflow but even if
  // that does not occur, just dividing by a small number will result
  // in numerical garbage. So we play it safe.
  static const double kThreshold = 1e-12;
  if ((sintheta > kThreshold) || (sintheta < -kThreshold)) {
    const T r = theta / (kTwo * sintheta);
    for (int i = 0; i < 3; ++i) {
      angle_axis[i] *= r;
    }
    return;
  }

  // Case 2: theta ~ 0, means sin(theta) ~ theta to a good
  // approximation.
  if (costheta > 0) {
    const T kHalf = T(0.5);
    for (int i = 0; i < 3; ++i) {
      angle_axis[i] *= kHalf;
    }
    return;
  }

  // Case 3: theta ~ pi, this is the hard case. Since theta is large,
  // and sin(theta) is small. Dividing by theta by sin(theta) will
  // either give an overflow or worse still numerically meaningless
  // results. Thus we use an alternate more complicated formula
  // here.

  // Since cos(theta) is negative, division by (1-cos(theta)) cannot
  // overflow.
  const T inv_one_minus_costheta = kOne / (kOne - costheta);

  // We now compute the absolute value of coordinates of the axis
  // vector using the diagonal entries of R. To resolve the sign of
  // these entries, we compare the sign of angle_axis[i]*sin(theta)
  // with the sign of sin(theta). If they are the same, then
  // angle_axis[i] should be positive, otherwise negative.
  for (int i = 0; i < 3; ++i) {
    angle_axis[i] = theta * sqrt((R[i*4] - costheta) * inv_one_minus_costheta);
    if (((sintheta < 0) && (angle_axis[i] > 0)) ||
        ((sintheta > 0) && (angle_axis[i] < 0))) {
      angle_axis[i] = -angle_axis[i];
    }
  }
}

template <typename T, int row_stride, int col_stride>
struct MatrixAdapter {
  T* pointer_;
  explicit MatrixAdapter(T* pointer) : pointer_(pointer) {}

  T& operator()(int r, int c) const {
    return pointer_[r * row_stride + c * col_stride];
  }
};

template <typename T>
MatrixAdapter<T, 1, 3> ColumnMajorAdapter3x3(T* pointer) {
  return MatrixAdapter<T, 1, 3>(pointer);
}

template <typename T>
MatrixAdapter<T, 3, 1> RowMajorAdapter3x3(T* pointer) {
  return MatrixAdapter<T, 3, 1>(pointer);
}

// This algorithm comes from "Quaternion Calculus and Fast Animation",
// Ken Shoemake, 1987 SIGGRAPH course notes
template <typename T, int row_stride, int col_stride>
void RotationMatrixToQuaternion(
    const MatrixAdapter<const T, row_stride, col_stride>& R, T* quaternion) {
  const T trace = R(0, 0) + R(1, 1) + R(2, 2);
  if (trace >= 0.0) {
    T t = sqrt(trace + T(1.0));
    quaternion[0] = T(0.5) * t;
    t = T(0.5) / t;
    quaternion[1] = (R(2, 1) - R(1, 2)) * t;
    quaternion[2] = (R(0, 2) - R(2, 0)) * t;
    quaternion[3] = (R(1, 0) - R(0, 1)) * t;
  } else {
    int i = 0;
    if (R(1, 1) > R(0, 0)) {
      i = 1;
    }

    if (R(2, 2) > R(i, i)) {
      i = 2;
    }

    const int j = (i + 1) % 3;
    const int k = (j + 1) % 3;
    T t = sqrt(R(i, i) - R(j, j) - R(k, k) + T(1.0));
    quaternion[i + 1] = T(0.5) * t;
    t = T(0.5) / t;
    quaternion[0] = (R(k, j) - R(j, k)) * t;
    quaternion[j + 1] = (R(j, i) + R(i, j)) * t;
    quaternion[k + 1] = (R(k, i) + R(i, k)) * t;
  }
}

template <typename T, int row_stride, int col_stride>
void RotationMatrixToAngleAxis2(
    const MatrixAdapter<const T, row_stride, col_stride>& R, T* angle_axis) {
  T quaternion[4];
  RotationMatrixToQuaternion(R, quaternion);
  QuaternionToAngleAxis(quaternion, angle_axis);
  return;
}


template <typename T>
inline void RotationMatrixToAngleAxis2(const T* R, T* angle_axis) {
  RotationMatrixToAngleAxis2(RowMajorAdapter3x3(R), angle_axis);
}

int main( int argc, char** argv ){
  
    double euler[3]      = { 0.1, -0.5, 0.3};
    double q[4]          = { 0.2217227, 0, 0, 0.9751098};
    double angle_axis[3] = {0.1722057, -0.4814122, 0.3184683};
    double R[9];
    Eigen::Matrix3d rotation_matrix2;
    rotation_matrix2 << 0.838387, -0.339769, -0.426222, 0.259343, 0.936419, -0.236347, 0.479426, 0.0876121, 0.873198;
    //=== Quaternion    =>  Rotation Matrix  ===
    QuaternionToRotation(q,R);
    print_matrix33(R);

    //=== Euler Angles  =>  Rotation Matrix  ===  OK
    cout<<"-----Euler angles to Rotation matrix, Ceres and Eigen------------"<<endl;
    EulerAnglesToRotationMatrix(euler, 3, R); //ceres is faster~
    print_matrix33(R);

    Eigen::AngleAxisd rollAngle(euler[2], Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(euler[1], Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(euler[0], Eigen::Vector3d::UnitX());

    Eigen::Quaternion<double> qqq = rollAngle * yawAngle * pitchAngle;
    Eigen::Matrix3d rotationMatrix = qqq.matrix();
    cout<<rotationMatrix<<endl;

    //=== AngleAxis    =>  Rotation Matrix  === OK 
    cout<<"-----AngleAxis to Rotation matrix, Ceres and Eigen------------"<<endl;
    AngleAxisToRotationMatrix(angle_axis, R);
    print_matrix33(R);
    
    Eigen::AngleAxisd angle_axis_2 (0.6023575, Eigen::Vector3d(0.2858863, -0.7992134, 0.5287031));
    Eigen::Matrix3d   rotationMatrix2 = angle_axis_2.toRotationMatrix();
    //std::cout<<"IsRowMajor?: "<<rotationMatrix2.IsRowMajor;    
    cout<<rotationMatrix2<<endl;

    //=== AngleAxis    => Quaternion
    double qq[4];
    AngleAxisToQuaternion(angle_axis, qq);
    cout<<qq[0]<<" "<<qq[1]<<" "<<qq[2]<<" "<<qq[3]<<endl;

    //=== Quaternion To AngleAxis
    QuaternionToAngleAxis(qq, angle_axis);
    cout<<angle_axis[0]<<" "<<angle_axis[1]<<" "<<angle_axis[2]<<endl;

    //=== Rotation Matrix to Angle Axis
    RotationMatrixToAngleAxis2(R, angle_axis);
    cout<<angle_axis[0]<<" "<<angle_axis[1]<<" "<<angle_axis[2]<<endl;

    angle_axis_2.fromRotationMatrix(rotation_matrix2);
    cout<<angle_axis_2.angle()<<endl;
    cout<<angle_axis_2.axis()<<endl;
}

