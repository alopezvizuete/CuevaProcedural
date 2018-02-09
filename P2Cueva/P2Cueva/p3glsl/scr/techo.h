#ifndef _BLENDER_EXPORT_H_
#define _BLENDER_EXPORT_H_

#define OBJECTS_COUNT 1
#define PLANE 0
/***************************************
 *          local transformations
 ***************************************/

float transformations[][16]={
	{1.984932f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.984932f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.984932f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f},
 };
/***************************************
 *	faces count for each mesh 
 ***************************************/
unsigned int faces_count[]={2};

/***************************************
 *	vertices count for each mesh 
 ***************************************/
unsigned int vertex_count[]={4};

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
	/* PLANE: 4 vertices */
	{-1.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
	{-1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f},
};

#define INX_TYPE GL_UNSIGNED_SHORT
unsigned short indexes[]={
/* PLANE 2 faces */
0, 1, 2, 0, 2, 3, 
};
#endif