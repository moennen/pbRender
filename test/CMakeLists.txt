# CMAKE FILE 

CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

PROJECT(pbTest)
set(CMAKE_C_COMPILER gcc)
ADD_DEFINITIONS( -DCOMPILER_GCC -std=gnu++0x )
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(MOENNEN_LOCAL /mnt/p4/favila/moennen/local)
SET(MOENNEN_WKSPCE /mnt/p4/avila/moennen_wkspce)
SET(M_INC ${MOENNEN_LOCAL}/include)
SET(M_LIBS ${MOENNEN_LOCAL}/lib)
SET(M_LIBS64 ${MOENNEN_LOCAL}/lib64)

INCLUDE_DIRECTORIES(
	${MOENNEN_WKSPCE}/pbRender/include
	${M_INC}
	${M_INC}/GL
	${M_INC}/eigen3
)
LINK_DIRECTORIES(
	${M_LIBS}
	${M_LIBS64}
)

ADD_EXECUTABLE(pbTest pbTest.cpp)

TARGET_LINK_LIBRARIES( pbTest opencv_world boost_graph )

