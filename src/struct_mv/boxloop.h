/*BHEADER**********************************************************************
 * Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of HYPRE.  See file COPYRIGHT for details.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * $Revision$
 ***********************************************************************EHEADER*/

/******************************************************************************
 *
 * Header info for the BoxLoop
 *
 *****************************************************************************/

/*--------------------------------------------------------------------------
 * BoxLoop macros:
 *--------------------------------------------------------------------------*/

#ifndef HYPRE_NEWBOXLOOP_HEADER
#define HYPRE_NEWBOXLOOP_HEADER

#ifdef HYPRE_USING_OPENMP
#define HYPRE_BOX_REDUCTION 
#ifdef WIN32
#define Pragma(x) __pragma(#x)
#else
#define Pragma(x) _Pragma(#x)
#endif
/* #define OMP1 Pragma(omp parallel for private(HYPRE_BOX_PRIVATE) HYPRE_SMP_SCHEDULE) */
#define OMP1 Pragma(omp parallel for private(HYPRE_BOX_PRIVATE) HYPRE_BOX_REDUCTION HYPRE_SMP_SCHEDULE)
#else
#define OMP1
#endif

typedef struct hypre_Boxloop_struct
  {
    HYPRE_Int lsize0,lsize1,lsize2;
    HYPRE_Int strides0,strides1,strides2;
    HYPRE_Int bstart0,bstart1,bstart2;
    HYPRE_Int bsize0,bsize1,bsize2;
  }hypre_Boxloop;

#define zypre_newBoxLoop0Begin(ndim, loop_size)				\
{\
   zypre_BoxLoopDeclare();									\
   zypre_BoxLoopInit(ndim, loop_size);						\
   OMP1\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++)\
   {\
      zypre_BoxLoopSet();\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)\
      {\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)\
         {

#define zypre_newBoxLoop0End()\
         }\
         zypre_BoxLoopInc1();\
         zypre_BoxLoopInc2();\
      }\
   }\
}

#define zypre_newBoxLoop1Begin(ndim, loop_size,				\
                               dbox1, start1, stride1, i1)              \
{									\
   HYPRE_Int i1;							\
   zypre_BoxLoopDeclare();						\
   zypre_BoxLoopDeclareK(1);						\
   zypre_BoxLoopInit(ndim, loop_size);					\
   zypre_BoxLoopInitK(1, dbox1, start1, stride1, i1);			\
   OMP1									\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++) \
   {									\
      zypre_BoxLoopSet();						\
      zypre_BoxLoopSetK(1, i1);						\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)		\
      {								\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)	\
	 {

#define zypre_newBoxLoop1End(i1)			       \
            i1 += hypre__i0inc1;			       \
	 }						       \
	 zypre_BoxLoopInc1();				       \
	 i1 += hypre__ikinc1[hypre__d];			       \
	 zypre_BoxLoopInc2();				       \
      }							       \
   }							\
}


#define zypre_newBoxLoop2Begin(ndim, loop_size,				\
			       dbox1, start1, stride1, i1,		\
			       dbox2, start2, stride2, i2)		\
{									\
   HYPRE_Int i1,i2;				\
   zypre_BoxLoopDeclare();\
   zypre_BoxLoopDeclareK(1);\
   zypre_BoxLoopDeclareK(2);\
   zypre_BoxLoopInit(ndim, loop_size);\
   zypre_BoxLoopInitK(1, dbox1, start1, stride1, i1);\
   zypre_BoxLoopInitK(2, dbox2, start2, stride2, i2);\
   OMP1\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++)	\
   {\
      zypre_BoxLoopSet();\
      zypre_BoxLoopSetK(1, i1);\
      zypre_BoxLoopSetK(2, i2);\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)\
      {\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)\
         {

#define zypre_newBoxLoop2End(i1, i2)\
            i1 += hypre__i0inc1;\
            i2 += hypre__i0inc2;\
         }\
         zypre_BoxLoopInc1();\
         i1 += hypre__ikinc1[hypre__d];\
         i2 += hypre__ikinc2[hypre__d];\
         zypre_BoxLoopInc2();\
      }\
   }\
}


#define zypre_newBoxLoop3Begin(ndim, loop_size,\
			       dbox1, start1, stride1, i1,		\
			       dbox2, start2, stride2, i2,		\
			       dbox3, start3, stride3, i3)		\
{									\
   HYPRE_Int i1,i2,i3;							\
   zypre_BoxLoopDeclare();									\
   zypre_BoxLoopDeclareK(1);								\
   zypre_BoxLoopDeclareK(2);								\
   zypre_BoxLoopDeclareK(3);								\
   zypre_BoxLoopInit(ndim, loop_size);						\
   zypre_BoxLoopInitK(1, dbox1, start1, stride1, i1);		\
   zypre_BoxLoopInitK(2, dbox2, start2, stride2, i2);		\
   zypre_BoxLoopInitK(3, dbox3, start3, stride3, i3);		\
   OMP1\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++)	\
   {\
      zypre_BoxLoopSet();\
      zypre_BoxLoopSetK(1, i1);\
      zypre_BoxLoopSetK(2, i2);\
      zypre_BoxLoopSetK(3, i3);\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)\
      {\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)\
         {

#define zypre_newBoxLoop3End(i1, i2, i3)\
            i1 += hypre__i0inc1;\
            i2 += hypre__i0inc2;\
            i3 += hypre__i0inc3;\
         }\
         zypre_BoxLoopInc1();\
         i1 += hypre__ikinc1[hypre__d];\
         i2 += hypre__ikinc2[hypre__d];\
         i3 += hypre__ikinc3[hypre__d];\
         zypre_BoxLoopInc2();\
      }\
   }\
}

#define zypre_newBoxLoop4Begin(ndim, loop_size,\
                            dbox1, start1, stride1, i1,\
                            dbox2, start2, stride2, i2,\
                            dbox3, start3, stride3, i3,\
                            dbox4, start4, stride4, i4)\
{\
   HYPRE_Int i1,i2,i3,i4;\
   zypre_BoxLoopDeclare();\
   zypre_BoxLoopDeclareK(1);\
   zypre_BoxLoopDeclareK(2);\
   zypre_BoxLoopDeclareK(3);\
   zypre_BoxLoopDeclareK(4);\
   zypre_BoxLoopInit(ndim, loop_size);\
   zypre_BoxLoopInitK(1, dbox1, start1, stride1, i1);\
   zypre_BoxLoopInitK(2, dbox2, start2, stride2, i2);\
   zypre_BoxLoopInitK(3, dbox3, start3, stride3, i3);\
   zypre_BoxLoopInitK(4, dbox4, start4, stride4, i4);\
   OMP1\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++)\
   {\
      zypre_BoxLoopSet();\
      zypre_BoxLoopSetK(1, i1);\
      zypre_BoxLoopSetK(2, i2);\
      zypre_BoxLoopSetK(3, i3);\
      zypre_BoxLoopSetK(4, i4);\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)\
      {\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)\
         {

#define zypre_newBoxLoop4End(i1, i2, i3, i4)\
            i1 += hypre__i0inc1;\
            i2 += hypre__i0inc2;\
            i3 += hypre__i0inc3;\
            i4 += hypre__i0inc4;\
         }\
         zypre_BoxLoopInc1();\
         i1 += hypre__ikinc1[hypre__d];\
         i2 += hypre__ikinc2[hypre__d];\
         i3 += hypre__ikinc3[hypre__d];\
         i4 += hypre__ikinc4[hypre__d];\
         zypre_BoxLoopInc2();\
      }\
   }\
}

#define zypre_newBasicBoxLoop2Begin(ndim, loop_size,\
				    stride1, i1,		\
				    stride2, i2)		\
{\
   HYPRE_Int i1,i2;\
   zypre_BoxLoopDeclare();\
   zypre_BoxLoopDeclareK(1);\
   zypre_BoxLoopDeclareK(2);\
   zypre_BoxLoopInit(ndim, loop_size);\
   zypre_BasicBoxLoopInitK(1, stride1);\
   zypre_BasicBoxLoopInitK(2, stride2);\
   OMP1\
   for (hypre__block = 0; hypre__block < hypre__num_blocks; hypre__block++)	\
   {\
      zypre_BoxLoopSet();\
      zypre_BoxLoopSetK(1, i1);\
      zypre_BoxLoopSetK(2, i2);\
      for (hypre__J = 0; hypre__J < hypre__JN; hypre__J++)\
      {\
         for (hypre__I = 0; hypre__I < hypre__IN; hypre__I++)\
         {


#define hypre_LoopBegin(size,idx)			\
{									\
   HYPRE_Int idx;							\
   for (idx = 0;idx < size;idx ++)					\
   {

#define hypre_LoopEnd()					\
  }							\
}

#define hypre_newBoxLoopGetIndex zypre_BoxLoopGetIndex  
#define hypre_BoxLoopGetIndex    zypre_BoxLoopGetIndex
#define hypre_BoxLoopSetOneBlock zypre_BoxLoopSetOneBlock
#define hypre_BoxLoopBlock       zypre_BoxLoopBlock
#define hypre_BoxLoop0Begin      zypre_newBoxLoop0Begin
#define hypre_BoxLoop0End        zypre_newBoxLoop0End
#define hypre_BoxLoop1Begin      zypre_newBoxLoop1Begin
#define hypre_BoxLoop1End        zypre_newBoxLoop1End
#define hypre_BoxLoop2Begin      zypre_newBoxLoop2Begin
#define hypre_BoxLoop2End        zypre_newBoxLoop2End
#define hypre_BoxLoop3Begin      zypre_newBoxLoop3Begin
#define hypre_BoxLoop3End        zypre_newBoxLoop3End
#define hypre_BoxLoop4Begin      zypre_newBoxLoop4Begin
#define hypre_BoxLoop4End        zypre_BoxLoop4End
#define hypre_BasicBoxLoop2Begin zypre_newBasicBoxLoop2Begin

#endif
