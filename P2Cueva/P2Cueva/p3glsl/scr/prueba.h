#ifndef _BLENDER_EXPORT_H_
#define _BLENDER_EXPORT_H_

#define OBJECTS_COUNT 1
#define PLANE 0
/***************************************
 *          local transformations
 ***************************************/

float transformations[][16]={
	{1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f},
 };
/***************************************
 *	faces count for each mesh 
 ***************************************/
unsigned int faces_count[]={32};

/***************************************
 *	vertices count for each mesh 
 ***************************************/
unsigned int vertex_count[]={25};

/***************************************
 *	offset tables for each mesh  
 ***************************************/
unsigned int vertex_offset_table []={
	0, 
};
unsigned int indices_offset_table []={
	0, 
};
/***************************************
 *	vertices definition
 ***************************************/

struct vertex_struct {
	float x,y,z;
	float nx,ny,nz;
	float u,v;
};
struct vertex_struct vertices[]={
	/* PLANE: 25 vertices */
	{0.500000f, 0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, 0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.500000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-0.500000f, 0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-0.500000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.500000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.500000f, -0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.000000f, -0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.500000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-0.500000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-0.500000f, -0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-0.500000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, -0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, -0.500000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
};

#define INX_TYPE GL_UNSIGNED_SHORT
unsigned short indexes[]={
/* PLANE 32 faces */
0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 5, 8, 9, 5, 9, 6, 8, 0, 3, 8, 3, 9, 10, 11, 12, 10, 12, 13, 13, 12, 14, 13, 14, 15, 15, 14, 0, 15, 0, 8, 16, 10, 13, 16, 13, 17, 17, 13, 15, 17, 15, 18, 18, 15, 8, 18, 8, 5, 19, 16, 17, 19, 17, 20, 20, 17, 18, 20, 18, 21, 21, 18, 5, 21, 5, 4, 11, 22, 23, 11, 23, 12, 12, 23, 24, 12, 24, 14, 14, 24, 1, 14, 1, 0, 
};
#endif