#ifndef __SueloFILE__
#define __SueloFILE__

//Describimos un cubo
//*******************
//Se replican vértices para que cada cara 
//tenga una normal distinta.

//Número de vértices
const int sueloNVertex = 4; // 4 vértices en total, 2 triángulos
const int sueloNTriangleIndex = 2; // 2 triángulos en total

const int sueloNGodRayVertex = 4; // 4 vértices en total, 2 triángulos
const int sueloNGodRayTriangleIndex = 2; // 2 triángulos en total

const unsigned int sueloTriangleIndex[] = {
	0, 1, 2, 2, 3, 0,
};
const unsigned int sueloTriangleGodRayIndex[] = {
	0,3,2,2,1,0,
};
//Posición de los vértices suelo
const float sueloVertexPos[] = {
	//Cara suelo
	4.000000f, 0.000000f, 4.000000f,
	-4.000000f, 0.000000f, 4.000000f,
	-4.000000f, 0.000000f, -4.000000f,
	4.000000f, 0.000000f, -4.000000f,
};
const float sueloVertexNormal[] = {
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f,  
};
const float sueloVertexColor[] = {
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f,  
};
const float sueloVertexTexCoord[] = { 	
	1.000000f, 1.000000f,
	0.000000f, 1.000000f,
	0.000000f, 0.000000f,
	1.000000f, 0.000000f,

};

const float sueloVertexGodRayPos[] = {
	6.000000f, 7.000000f,0.000000f, 
	-6.000000f,7.000000f, 0.000000f, 
	-6.000000f, -7.000000f,0.000000f, 
	6.000000f,  -7.000000f,0.000000f,

};
const float sueloVertexGodRayNormal[] = {
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f,  
};
const float sueloVertexGodRayColor[] = {
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
};
const float sueloVertexGodRayTexCoord[] = { 
	1.000000f, 1.000000f,
	0.000000f, 1.000000f,
	0.000000f, 0.000000f,
	1.000000f, 0.000000f,
};

#endif


