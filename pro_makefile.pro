#Flags
QMAKE_CXXFLAGS += -O3 -march=native  -std=c++11 -m64 -ffast-math -Waggressive-loop-optimizations -Wall -fpermissive



#include directory besides system default directory
INCLUDEPATH +=  /usr/local/include/eigen3/
INCLUDEPATH +=  /usr/local/include/opencv4/
#include library besides system default directory
LIBS += ./xxx/lib/libcaffe2.so
#in this case, you also need to 
#export LD_LIBRARY_PATH=/xxxx, when you run the binary
export LD_LIBRARY_PATH=./xxx/lib/:LD_LIBRARY_PATH





#example  cuda
LIBS += -lcudnn -lcuda -lcudart -lcublas -lcurand -lcusolver

#example  ceres


#example  opengl


#example  realsence
