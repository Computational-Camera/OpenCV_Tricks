
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <immintrin.h>

/* A simile example (dot and cross product) showing no performance gain with AVX, complier = GCC, CLANG need to be checked.*/

using namespace std;
using namespace cv;
static timespec ts;
	void tic(){
		clock_gettime(CLOCK_REALTIME, &ts);
	}

	double toc(){
		timespec cur;
		clock_gettime(CLOCK_REALTIME, &cur);
		float tt = (cur.tv_nsec - ts.tv_nsec);
		tt = (tt>0) ? tt/1000000000.f +(cur.tv_sec-ts.tv_sec): tt/1000000000.f+1 + (cur.tv_sec - ts.tv_sec)-1;
		return tt;
	}

typedef struct {
	double x, y, z;
}Vector;
	
inline double Dot(const Vector &v1, const Vector &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline void Cross(const Vector &v1, const Vector &v2, Vector&v3) {
	v3.x = (v1.y * v2.z) - (v1.z * v2.y);
	v3.y = (v1.z * v2.x) - (v1.x * v2.z);
	v3.z = (v1.x * v2.y) - (v1.y * v2.x);
}

#define CROSS_PRODUCT(a,b) _mm256_permute4x64_pd(\
		_mm256_sub_pd(\
			_mm256_mul_pd(a, _mm256_permute4x64_pd(b, _MM_SHUFFLE(3, 0, 2, 1))),\
			_mm256_mul_pd(b, _mm256_permute4x64_pd(a, _MM_SHUFFLE(3, 0, 2, 1)))\
		), _MM_SHUFFLE(3, 0, 2, 1)\
	)


int main() {

	int Size = 4574*708;

	double* array_a     = aligned_alloc( 64, Size*4*sizeof(double));
	double* array_b     = aligned_alloc( 64, Size*4*sizeof(double));
	double* array_a2     = aligned_alloc( 64, Size*4*sizeof(double));
	double* array_b2     = aligned_alloc( 64, Size*4*sizeof(double));	
	
	double* array_a_avx = aligned_alloc( 64, Size*4*sizeof(double));
	double* array_b_avx = aligned_alloc( 64, Size*4*sizeof(double));
	float* array_a2_avx = aligned_alloc( 64, Size*8*sizeof(float));
	float* array_b2_avx = aligned_alloc( 64, Size*8*sizeof(float));	

	for (int k=0; k<4*Size; k++){
		array_a[k]      = k;
		array_b[k]      = k+1;
		array_a2[k]     = k+2;
		array_b2[k]     = k+3;	
		array_a_avx[k]  = k;
		array_b_avx[k]  = k+1;

		
		if ((k+1)%4==0){ //forth element should be zero
		array_a[k]      = 0;
		array_b[k]      = 0;
		array_a2[k]     = 0;
		array_b2[k]     = 0;	
		array_a_avx[k]  = 0;
		array_b_avx[k]  = 0;		
		}					
	}
	for (int k=0; k<4*Size; k=k+4){	
		array_a2_avx[2*k]   = array_a2[k];
		array_a2_avx[2*k+1] = array_a2[k+1];		
		array_a2_avx[2*k+2] = array_a2[k+2];		
		array_a2_avx[2*k+3] = array_a2[k+3];			
		array_b2_avx[2*k]   = array_b2[k];
		array_b2_avx[2*k+1] = array_b2[k+1];		
		array_b2_avx[2*k+2] = array_b2[k+2];		
		array_b2_avx[2*k+3] = array_b2[k+3];				
	}
	tic();
	
	double sum = 0;
	Vector v5;
	//normal  
	for (int k=0; k<Size; k++){	

		double dot_product = Dot( *((Vector*)(array_a2)),  *((Vector*)(array_b2))); 
		Cross( *((Vector*)(array_a)),  *((Vector*)(array_b)), v5);
		 
		sum = sum + v5.x + dot_product; 

		array_a2 = array_a2 +4; 
		array_b2 = array_b2 +4;
		array_a  = array_a  +4;
		array_b  = array_b  +4;		

	}
	cout<<"time spent "<<toc()<<endl;
	cout<<sum<<endl;
		
	sum = 0;
	//AVX DOT and CORSS product
	tic();
	for (int k=0; k<Size; k++){	
		double cross_product[4] __attribute__((aligned(64)));
		float  dot_product[4]   __attribute__((aligned(64)));		
		//dot product	
		_mm256_store_ps (dot_product, _mm256_dp_ps(_mm256_load_ps(array_a2_avx), _mm256_load_ps(array_b2_avx), 255));			
		//cross product	
		_mm256_store_pd (cross_product, CROSS_PRODUCT(_mm256_load_pd( array_a_avx),_mm256_load_pd( array_b_avx)));
		sum = sum+cross_product[0]+dot_product[0];		

		array_a2_avx = array_a2_avx +8; 
		array_b2_avx = array_b2_avx +8;
		array_a_avx  = array_a_avx  +4;
		array_b_avx  = array_b_avx  +4;		
		
	}
	cout<<"time spent "<<toc()<<endl;
	cout<<sum<<endl;
	
	
	//CROSS_PRODUCT(a,b);
    return 0;
}

