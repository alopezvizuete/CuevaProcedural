#ifndef _BLENDER_EXPORT_H_
#define _BLENDER_EXPORT_H_

#define OBJECTS_COUNT 1
#define PLANE 0
/***************************************
 *          local transformations
 ***************************************/

float transformations[][16]={
	{1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, 0.000000f, 0.000000f, -1.000000f, -0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f},
 };
/***************************************
 *	faces count for each mesh 
 ***************************************/
unsigned int faces_count[]={18};

/***************************************
 *	vertices count for each mesh 
 ***************************************/
unsigned int vertex_count[]={16};

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
	/* PLANE: 16 vertices */
	{1.000000f, 0.000000f, -0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.333333f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.333333f, 0.000000f, -0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.333333f, 0.000000f, 0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{0.333333f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{1.000000f, 0.000000f, 0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-0.333333f, 0.000000f, 0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.000000f, -0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.000000f, 0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-0.333333f, 0.000000f, -0.333333f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-0.333333f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{-0.333333f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
	{1.000000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f},
};

#define INX_TYPE GL_UNSIGNED_SHORT
unsigned short indexes[]={
/* PLANE 18 faces */
0, 1, 2, 3, 4, 5, 6, 2, 4, 7, 8, 9, 4, 10, 7, 11, 9, 12, 5, 7, 11, 10, 13, 8, 2, 14, 10, 0, 15, 1, 3, 6, 4, 6, 0, 2, 7, 10, 8, 4, 2, 10, 11, 7, 9, 5, 4, 7, 10, 14, 13, 2, 1, 14, 
};
#endif