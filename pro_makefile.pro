

#include directory besides system default directory
INCLUDEPATH +=  /usr/local/include/eigen3/

#include library besides system default directory
#in this case, you also need to 
#export LD_LIBRARY_PATH=/xxxx, when you run the binary
LIBS += -L/xxxx
