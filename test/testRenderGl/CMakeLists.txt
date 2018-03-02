# CMAKE FILE : testRenderGl/moennen/2018-03-02

CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

PROJECT(testRenderGl)
ADD_DEFINITIONS( -DCOMPILER_GCC -std=gnu++0x )
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(MOENNEN_LOCAL /mnt/p4/favila/moennen/local)
SET(MOENNEN_WKSPCE /mnt/p4/avila/moennen_wkspce)
SET(M_INC ${MOENNEN_LOCAL}/include)
SET(M_LIBS ${MOENNEN_LOCAL}/lib)
SET(M_LIBS64 ${MOENNEN_LOCAL}/lib64)
SET(CUDA /usr/local/cuda-8.0)

INCLUDE_DIRECTORIES(
	${MOENNEN_WKSPCE}
	${M_INC}
	${M_INC}/GL
	${M_INC}/eigen3
	${CUDA}/include
)
LINK_DIRECTORIES(
	${M_LIBS}
	${M_LIBS64}
	${CUDA}/lib64
)

ADD_EXECUTABLE(testRenderGl testRenderGl_main.cpp)

TARGET_LINK_LIBRARIES( testRenderGl
	boost_thread boost_system GL GLU glut cudart cublas
	opencv_world )

