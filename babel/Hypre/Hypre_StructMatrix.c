/*#*****************************************************
#
#	File:  Hypre_StructMatrix.c
#
#********************************************************/

#include "Hypre_StructMatrix_Skel.h" 
#include "Hypre_StructMatrix_Data.h" /*gkk: added. (automatic after 0.3.0) */

#include "Hypre_StructuredGrid_Skel.h" /* gkk: added */
#include "Hypre_StructuredGrid_Data.h" /* gkk: added */
#include "Hypre_StructStencil_Skel.h"  /* gkk: added */
#include "Hypre_StructStencil_Data.h"  /* gkk: added */
#include "Hypre_StructVector_Skel.h"   /* gkk: added */
#include "Hypre_StructVector_Data.h"   /* gkk: added */
#include "Hypre_Box_Skel.h"            /* gkk: added */
#include "Hypre_Box_Data.h"            /* gkk: added */


/*#************************************************
#	Constructor
#**************************************************/

void Hypre_StructMatrix_constructor(Hypre_StructMatrix this) {

/* JFP: Allocates Memory */
   this->d_table = (struct Hypre_StructMatrix_private_type *)
      malloc( sizeof( struct Hypre_StructMatrix_private_type ) );

   this->d_table->hsmat = (HYPRE_StructMatrix *)
      malloc( sizeof( HYPRE_StructMatrix ) );

}


/*#************************************************
#	Destructor
#**************************************************/

void Hypre_StructMatrix_destructor(Hypre_StructMatrix this) {

   /* JFP: Deallocates memory. */

   struct Hypre_StructMatrix_private_type *SMp = this->d_table;
   HYPRE_StructMatrix *M = SMp->hsmat;

   HYPRE_StructMatrixDestroy( *M );
   free(this->d_table);
}

Hypre_StructMatrix  impl_Hypre_StructMatrix_NewMatrix(
   Hypre_StructMatrix this, Hypre_StructuredGrid grid,
   Hypre_StructStencil stencil, int symmetric ) {

   struct Hypre_StructMatrix_private_type *SMp = this->d_table;
   HYPRE_StructMatrix *M = SMp->hsmat;

   struct Hypre_StructuredGrid_private_type *Gp = grid->d_table;
   HYPRE_StructGrid *G = Gp->hsgrid;
   hypre_StructGrid *g = (hypre_StructGrid *) *G;

   struct Hypre_StructStencil_private_type *SSp = stencil->d_table;
   HYPRE_StructStencil *SS = SSp->hsstencil;

   MPI_Comm comm = hypre_StructGridComm( g );

   HYPRE_StructMatrixCreate( comm, *G, *SS, M );

   HYPRE_StructMatrixSetSymmetric( *M, symmetric );

   HYPRE_StructMatrixInitialize( *M );

   /* I don't want to put this in the interface (makes it too unnatural or
      complicated for a user), so I'm trying to call it multiple times.
      This may not work. (JfP 130100) */
   HYPRE_StructMatrixAssemble( *M );

   return this;
}

void  impl_Hypre_StructMatrix_print(Hypre_StructMatrix this) {

   int boxarray_size;
   FILE * file;

   struct Hypre_StructMatrix_private_type *SMp = this->d_table;
   HYPRE_StructMatrix *M = SMp->hsmat;
   hypre_StructMatrix *m = (hypre_StructMatrix *) *M;

   if ( m->data_space==NULL )
      boxarray_size = -1;
   else
      boxarray_size = m->data_space->size;

   printf( "StructMatrix, data size =%i, BoxArray size=%i, symmetric=%i\n",
           m->data_size, boxarray_size, m->symmetric );

   /* c.f. struct_matrix.c, line 717... */
   file = fopen( "testum.out", "w" );
   fprintf( file, "\nMatrix Data:\n");
   hypre_PrintBoxArrayData(
      file, hypre_StructMatrixDataSpace(m),
      hypre_StructMatrixDataSpace(m), m->num_values,
      hypre_StructMatrixData(m) );
   fflush(file);
   fclose(file);

}

int  impl_Hypre_StructMatrix_SetGrid(Hypre_StructMatrix this, Hypre_StructuredGrid grid) {

/* not implemented; this functionality isn't in Hypre (though doesn't
   look too hard to put in)
   */
   printf( "unimplemented function, Hypre_StructMatrix_SetGrid, was called" );

}

int  impl_Hypre_StructMatrix_SetStencil(Hypre_StructMatrix this, Hypre_StructStencil stencil) {

/* not implemented; this functionality isn't in Hypre (though doesn't
   look too hard to put in)
   */
   printf( "unimplemented function, Hypre_StructMatrix_SetStencil, was called" );

}

int  impl_Hypre_StructMatrix_SetValues(
   Hypre_StructMatrix this, Hypre_Box box,
   array1int stencil_indices, array1double values) {

   int i, ssize, lower[3], upper[3];

   struct Hypre_StructMatrix_private_type *SMp = this->d_table;
   HYPRE_StructMatrix *M = SMp->hsmat;
   hypre_StructMatrix *m = (hypre_StructMatrix *) *M;

   struct Hypre_Box_object_ BO = *box;
   struct Hypre_Box_private_type *Bp = BO.d_table;
   hypre_Box *B = Bp->hbox;

   for ( i=0; i<Bp->dimension; ++i ) {
      lower[i] = B->imin[i];
      upper[i] = B->imax[i];
   };

   ssize = stencil_indices.upper[0] - stencil_indices.lower[0];
   
   /*   printf(
      "lower[0]=%i, upper[0]=%i, stencil size=%i, first stencil data=%i, first matrix value=%f\n",
      lower[0], upper[0], ssize, stencil_indices.data[*(stencil_indices.lower)],
      values.data[*(values.lower)]  );
   */

   HYPRE_StructMatrixSetBoxValues(
      *M, lower, upper, ssize,
      &(stencil_indices.data[*(stencil_indices.lower)]),
      &(values.data[*(values.lower)]) );

   /* I don't want to put this in the interface (makes it too unnatural or
      complicated for a user), so I'm trying to call it multiple times.
      This may not work. (JfP 130100) */
   HYPRE_StructMatrixAssemble( *M );
}

int  impl_Hypre_StructMatrix_Setup(
   Hypre_StructMatrix this, Hypre_StructuredGrid grid,
   Hypre_StructStencil stencil, int symmetric) {

   impl_Hypre_StructMatrix_NewMatrix( this, grid, stencil, symmetric );
   return 0;
}

void  impl_Hypre_StructMatrix_Apply(Hypre_StructMatrix this, Hypre_StructVector x, Hypre_StructVector* b) {

   /* b = A * x   where this = A  */

   struct Hypre_StructMatrix_private_type *SMp = this->d_table;
   HYPRE_StructMatrix *M = SMp->hsmat;
   hypre_StructMatrix *hA = (hypre_StructMatrix *) *M;

   struct Hypre_StructVector_private_type *SVxp = x->d_table;
   HYPRE_StructVector *Vx = SVxp->hsvec;
   hypre_StructVector *hx = (hypre_StructVector *) *Vx;

   struct Hypre_StructVector_private_type *SVyp = (*b)->d_table;
   HYPRE_StructVector *Vy = SVyp->hsvec;
   hypre_StructVector *hy = (hypre_StructVector *) *Vy;

   hypre_StructMatvec( 1.0, hA, hx, 1.0, hy );  /* y = A*x */

}

Hypre_StructMatrix  impl_Hypre_StructMatrix_GetConstructedObject(Hypre_StructMatrix this) {

   return this;
}


