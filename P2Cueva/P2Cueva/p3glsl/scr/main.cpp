#include "BOX.h"
#include "Suelo.h"
#include "auxiliar.h"
#include <time.h>

#include <windows.h> //Para meter sleep en el idle, y realentizar. Solo para Windows

#include <gl/glew.h> //Si lo incluimos, no hace falta incluir OpenGL. SIEMPRE SIEMPRE antes de OpenGl
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp> //libreria matematica
#include <glm/gtc/matrix_transform.hpp>

//variables para crear los planos grandes
int width = 50;
int height = 50;
int widthA = 10;
int heightA = 10;
float* vertices = 0;
int* indices = 0;
int* indicestecho = 0;
float * color = 0;
float * normal = 0;
float * UV = 0;
//variables para spline
std::vector<glm::vec3> spline;
std::vector<glm::vec3> tangente;
int numCam = 0;

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f); //Una por objeto.


//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////

//Identificador para shader de vertices
unsigned int vshader;
//Identificador para shader de fragmentos
unsigned int fshader;
//Identificador para el programa
unsigned int program;

//Tenemos dos shader en esta practica, uno con texturas y otro sin texturas

//Variables Uniform
//Matriz modelview de Shader sin textura
int uModelViewMat;
//Matriz modelviewProj de Shader sin textura
int uModelViewProjMat;
//Matriz normal de Shader sin textura
int uNormalMat;

//Variable uniforme - Vector intensidad de Luz
int uIntMat;
//Variable uniforme - Matriz posicion de Luz
int uPosMat;
int uFocMat;
int uDirMat;
int uPunMat;
//TipoLuz de Luz
int uTipoMat;


//Identificadores de las variables que van a acceder a las texturas (Texturas Uniform)
int uColorTex;
int uEmiTex;
int uSpecTex;

//Atributos
//Identificadores de las variables uniformes del shader sin textura
std::vector<unsigned int> inPos;
std::vector<unsigned int> inColor;
std::vector<unsigned int> inNormal;
std::vector<unsigned int> inTexCoord;

float angle = 0.0f;
float posx = 0.0f;
int tipoLuz = 1;


//Catmull-ROM
float paso = 0.0f;
float cantPas = 0.01f;

//Movimiento teclado
bool enMovimiento = true;
glm::vec3 posRelativa;
glm::vec3 perp;
glm::vec3 arriba = glm::vec3(0.0f, 1.0f, 0.0f);
float ejeY = 0.0f;
float ejeX = 0.0f;
float Zoom = 0.0f;

float angulo;

//Catmull-ROM 8 puntos
glm::vec3 p0 = glm::vec3(100.0f, 0.0f, 25.0f);
glm::vec3 p1 = glm::vec3(137.0f, 0.0f, 62.0f);
glm::vec3 p2 = glm::vec3(175.0f, 0.0f, 100.0f);
glm::vec3 p3 = glm::vec3(137.0f, 0.0f, 137.0f);
glm::vec3 p4 = glm::vec3(100.0f, 0.0f, 150.0f);
glm::vec3 p5 = glm::vec3(62.0f, 0.0f, 137.0f);
glm::vec3 p6 = glm::vec3(25.0f, 0.0f, 100.0f);
glm::vec3 p7 = glm::vec3(62.0f, 0.0f, 62.0f);
const int numPuntos = 8;
glm::vec3 matrizPuntos[numPuntos] = { p0, p1, p2, p3, p4, p5, p6, p7 };
glm::vec3 matrizPuntosN[numPuntos] = { p0, p1, p2, p3, p4, p5, p6, p7 };
//Variables para moverlos por la spline
glm::vec3 poscat;
glm::vec3 poscatA;
glm::vec3 poscatP = p0;
glm::vec3 tang;


//Vector de intensidad de luz (X - Puntual, Y - Focal, Z - Direccional)
glm::vec4 intenLuz(1.0f);
glm::vec4 PosPuntual;
glm::vec4 PosFocal;
glm::vec4 PosDireccional;
glm::mat4 posiLuz(0.0f);
glm::mat4 posiDireccional;
glm::mat4 posiFocal;
glm::mat4 posiPuntual;

//VAO
unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

//Texturas
unsigned int colorTexId;
unsigned int colorTexIdNube;
unsigned int colorTexIdgodRay;
unsigned int emiTexId;
unsigned int emiTexIdNube;
unsigned int specTexId;


//Creacion de planos
// https://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips 
//Variable para ordenar los objetos a renderizar
int ob = 0;
//Funcion que devuelve el numero de vertices
int getVerticesCount(int width, int height) {
	return width * height * 3;
}
//Funcion que devuelve el numero de indices
int getIndicesCount(int width, int height) {
	return (width*height) + (width - 1)*(height - 2);
}
//Funcion que crea los vertices del plano
float* getVertices(int width, int height) {
	if (vertices) vertices=0;

	vertices = new float[getVerticesCount(width, height)];
	int i = 0;

	for (int row = 0; row<height; row++) {
		for (int col = 0; col<width; col++) {
			vertices[i++] = (float)col*4;
			vertices[i++] = 0.0f;
			vertices[i++] = (float)row*4;
		}
	}

	return vertices;
}
//Funcion que crea los indices del plano
int* getIndices(int width, int height) {
	if (indices) indices = 0;

	indices = new int[getIndicesCount(width, height)];
	int i = 0;

	for (int row = 0; row<height - 1; row++) {
		if ((row & 1) == 0) { 
			for (int col = 0; col<width; col++) {
				indices[i++] = col + row * width;
				indices[i++] = col + (row + 1) * width;
			}
		}
		else {
			for (int col = width - 1; col>0; col--) {
				indices[i++] = col + (row + 1) * width;
				indices[i++] = col - 1 + +row * width;
			}
		}
	}
	return indices;
}
//Funcion que crea los indices del plano techo
int* getIndicesTecho(int width, int height) {
	if (indices) indices = 0;

	indices = new int[getIndicesCount(width, height)];
	indicestecho = new int[getIndicesCount(width, height)];
	int i = 0;

	for (int row = 0; row<height - 1; row++) {
		if ((row & 1) == 0) { 
			for (int col = 0; col<width; col++) {
				indices[i++] = col + row * width;
				indices[i++] = col + (row + 1) * width;
			}
		}
		else { 
			for (int col = width - 1; col>0; col--) {
				indices[i++] = col + (row + 1) * width;
				indices[i++] = col - 1 + +row * width;
			}
		}
	}

	int n = getIndicesCount(width, height);
	for (int i = 0; i < n; i++){
		indicestecho[i] = indices[n - i];
	}
	return indices;
}

//Funcion que crea los colores del plano
float* getColor(int width, int height) {
	if (color) color = 0;

	color = new float[getVerticesCount(width, height)];
	int i = 0;

	for (i = 0; i < getVerticesCount(width, height) / 3; i++) {
		color[3 * i] = 0.0;
		color[3 * i + 1] = 1.0;
		color[3 * i + 2] = 0.0;
	}

	return color;
}
//Funcion que crea las normales del plano
float* getNormal(int width, int height) {
	if (normal) normal = 0;

	normal = new float[getVerticesCount(width, height)];
	int i = 0;

	for (i = 0; i < getVerticesCount(width, height) / 3; i++) {
		normal[3 * i] = 0.0;
		normal[3 * i + 1] = 1.0;
		normal[3 * i + 2] = 0.0;
	}

	return normal;
}
//Funcion que crea las UV del plano
float* getUV(int width, int height) {
	if (UV) UV = 0;

	UV = new float[getVerticesCount(width, height)];
	int i = 0;

	for (int row = 0; row<height; row++) {
		for (int col = 0; col<width; col++) {
			UV[i++] = (float)row / (width - 1);
			UV[i++] = (float)col / (height - 1);
		}
	}
	return UV;
}

//Variables para el plano Suelo de la cueva
int numeroVerticesSuelo = getVerticesCount(width, height);
int numeroIndicesSuelo = getIndicesCount(width, height);
float* sueloVertices = getVertices(width, height);
int* sueloIndices = getIndices(width, height);
float* sueloColor = getColor(width, height);
float* sueloNormal = getNormal(width, height);
float* sueloUV = getUV(width, height);

//Variables para el plano Agua de la cueva
int numeroVerticesAgua = getVerticesCount(width, height);
int numeroIndicesAgua = getIndicesCount(width, height);
float* AguaVertices = getVertices(width, height);
int* AguaIndices = getIndices(width, height);
float* AguaColor = getColor(width, height);
float* AguaNormal = getNormal(width, height);
float* AguaUV = getUV(width, height);

//Variables para el plano Techo de la cueva
int numeroVerticesTecho = getVerticesCount(width, height);
int numeroIndicesTecho = getIndicesCount(width, height);
float* TechoVertices = getVertices(width, height);
int* TechoIndices = getIndicesTecho(width, height);
float* TechoColor = getColor(width, height);
float* TechoNormal = getNormal(width, height);
float* TechoUV = getUV(width, height);

//Variables para el plano Agujero de la cueva
int numeroVerticesAgujero = getVerticesCount(widthA, heightA);
int numeroIndicesAgujero = getIndicesCount(widthA, heightA);
float* AgujeroVertices = getVertices(widthA, heightA);
int* AgujeroIndices = getIndicesTecho(widthA, heightA);
float* AgujeroColor = getColor(widthA, heightA);
float* AgujeroNormal = getNormal(widthA, heightA);
float* AgujeroUV = getUV(widthA, heightA);

//Deformamos el plano del suelo
void deformarSuelo(float* vertices, std::vector<glm::vec3> spline) {
	//Por cada vertice del plano, calculamos la distancia
	//A cada punto de la spline y nos guardamos la distancia minima.
	//Esta distancia, aplicandole el coseno, sera nuestra nueva posicion en el eje Y en el vertice del plano
	for (int v = 0; v < numeroVerticesSuelo * 3; v += 3) {
		float minDistance = 10000.0;
		float distancia = 0;
		float valor = 0;
		for (int i = 0; i< spline.size(); i++) {
			distancia = sqrt(pow((vertices[v] - spline[i].x), 2) +
				pow((vertices[v + 1] - spline[i].y), 2) +
				pow((vertices[v + 2] - spline[i].z), 2));
			if (distancia< minDistance) {
				minDistance = distancia;
			}
		}
		if (minDistance <= 15) {
			vertices[v + 1] = -cos(minDistance*0.3) * 4;
		}
	}
}

void deformarTecho(float* vertices, std::vector<glm::vec3> spline) {
	//Por cada vertice del plano, calculamos la distancia
	//A cada punto de la spline y nos guardamos la distancia minima.
	//Esta distancia, aplicandole el coseno, sera nuestra nueva posicion en el eje Y en el vertice del plano
	for (int v = 0; v < numeroVerticesSuelo * 3; v += 3) {
		float minDistance = 10000.0;
		float distancia = 0;
		float valor = 0;
		for (int i = 0; i< spline.size(); i++) {
			distancia = sqrt(pow((vertices[v] - spline[i].x), 2) +
				pow((vertices[v + 1] - spline[i].y), 2) +
				pow((vertices[v + 2] - spline[i].z), 2));
			if (distancia< minDistance) {
				minDistance = distancia;
			}
		}
		if (minDistance <= 15) {
			vertices[v + 1] = cos(minDistance*0.3) * 6;
		}
	}
}

//Función normalizar
glm::vec3 normalizar(glm::vec3 vc) {
	float length = abs(sqrt(vc.x * vc.x + vc.y * vc.y + vc.z * vc.z));
	vc.x = vc.x / length;
	vc.y = vc.y / length;
	vc.z = vc.z / length;

	return vc;
}

//Clase SCENE
class Scene {
public:
	//Atributos
	std::vector<glm::vec3> arrayPosicionesDireccional;
	std::vector<float> arrayIntensidadDireccional;
	std::vector<glm::vec3> arrayPosicionesPuntual;
	std::vector<float> arrayIntensidadPuntual;
	std::vector<glm::vec3> arrayPosicionesFocal;
	std::vector<float> arrayIntensidadFocal;
	std::vector<glm::vec3> posicionesMallas;
	std::vector<unsigned int> programs;
	std::vector<unsigned int> vaos;


	//Funciones
	//Ponemos los valores de la camara
	void setCamera(int x, int y, int z, glm::vec4 datos) {
		view[3].x = x;
		view[3].y = y;
		view[3].z = z;
		proj = glm::perspective(glm::radians(datos.x), datos.y, datos.z, datos.w);
	}
	//Leemos la malla de un CUBO, guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMalla(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjcubo(n);
	}
	//Leemos la malla del SUELO, guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMallasuelo(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjsuelo(n);
	}
	//Leemos la malla del AGUA, guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMallaAgua(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjAgua(n);
	}
	//Leemos la malla del AGUJERO en la pared , guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMallaAgujero(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjAgujero(n);
	}
	//Leemos la malla del God Ray , guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMallaGodRay(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjGodRay(n);
	}
	//Leemos la malla del TECHO , guardando su posicion en el array posiciones
	// inicializando su shader y variables del objeto
	void loadMallaTecho(glm::vec3 pos, const char* s1, const char* s2, int n) {
		posicionesMallas.push_back(pos);
		initShader(s1, s2);
		initObjTecho(n);
	}

	//Guardamos las posiciones e intensidades de las luces que hemos definido en el main
	void setLuzDireccional(glm::vec3 luz, float intensidad) {
		arrayPosicionesDireccional.push_back(luz);
		arrayIntensidadDireccional.push_back(intensidad);
	}
	void setLuzPuntual(glm::vec3 luz, float intensidad) {
		arrayPosicionesPuntual.push_back(luz);
		arrayIntensidadPuntual.push_back(intensidad);
	}
	void setLuzFocal(glm::vec3 luz, float intensidad) {
		arrayPosicionesFocal.push_back(luz);
		arrayIntensidadFocal.push_back(intensidad);
	}

	void render() {
		//Limpia los buffer de color y profundidad antes de cada renderizado
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < posicionesMallas.size(); i++) {
			//Activar programa antes de renderizar
			glUseProgram(programs[i]);

			//Se necesitan primero las variables uniformes
			//Creamos las matrices necesarias para los shaders
			//Es lo unico que se sube, todo lo demas se realiza en local
			glm::mat4 modelView = view * model;
			glm::mat4 modelViewProj = proj * view * model;
			glm::mat4 normal = glm::transpose(glm::inverse(modelView));
			int ocupado = 0;
			int j = 0;
			for (int j = 0; j < arrayPosicionesPuntual.size(); j++) {
				posiLuz[ocupado] = glm::vec4(arrayPosicionesPuntual[j], 1.0f);
				intenLuz[ocupado] = arrayIntensidadPuntual[j];
				ocupado++;

			}
			for (int j = 0; j < arrayPosicionesFocal.size(); j++) {
				posiLuz[ocupado] = glm::vec4(arrayPosicionesFocal[j], 2.0f);
				intenLuz[ocupado] = arrayIntensidadFocal[j];
				ocupado++;

			}
			for (int j = 0; j < arrayPosicionesDireccional.size(); j++) {
				posiLuz[ocupado] = glm::vec4(arrayPosicionesDireccional[j], 3.0f);
				intenLuz[ocupado] = arrayIntensidadDireccional[j];
				ocupado++;

			}

			if (uIntMat != -1)
				glUniform4fv(uIntMat, 1, &(intenLuz[0]));
			if (uPosMat != -1)
				glUniformMatrix4fv(uPosMat, 1, GL_FALSE, &(posiLuz[0][0]));
			if (uFocMat != -1)
				glUniformMatrix4fv(uFocMat, 1, GL_FALSE, &(posiFocal[0][0]));
			if (uPunMat != -1)
				glUniformMatrix4fv(uPunMat, 1, GL_FALSE, &(posiPuntual[0][0]));
			if (uDirMat != -1)
				glUniformMatrix4fv(uDirMat, 1, GL_FALSE, &(posiDireccional[0][0]));
			if (uTipoMat != -1)
				glUniform1i(uTipoMat, tipoLuz);


			//PONER ANTES DE LA LLAMADA AL PINTADO, Y DESPUES DE ACTIVAR EL SHADER
			//Texturas
			//Comprueba que nuestro shader este utilizando la variable ColorTex
			if (uColorTex != -1)
			{
				//Activo el textureUnit0
				glActiveTexture(GL_TEXTURE0);
				//Activo la textura de color en el textureUnit0
				if (i == 3 || i == 5){ //Si tenemos el plano del agujero en la pared, subimos la textura del cielo
					glBindTexture(GL_TEXTURE_2D, colorTexIdNube);

				}
				else if (i == 4 || i==6){//Si tenemos el plano del god ray, subimos la textura del god ray
					//activamos la textura para blend
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
					glBindTexture(GL_TEXTURE_2D, colorTexIdgodRay);


				}
				else{
					glBindTexture(GL_TEXTURE_2D, colorTexId);
				}
				//Le digo al shader que la textura de color esta en textureUnit0
				glUniform1i(uColorTex, 0);
			}
			//Textura emisiva igual que el color, pero con textureUnit1
			if (uEmiTex != -1)
			{
				glActiveTexture(GL_TEXTURE0 + 1);
				if (i == 3 || i== 5){ //Si tenemos el plano del agujero, subimos la textura emisiva del cielo
					glBindTexture(GL_TEXTURE_2D, emiTexIdNube);

				}
				else{
					glBindTexture(GL_TEXTURE_2D, emiTexId);
				}
				glUniform1i(uEmiTex, 1);
			}
			if (uSpecTex != -1)
			{
				glActiveTexture(GL_TEXTURE0 + 2);
				glBindTexture(GL_TEXTURE_2D, specTexId);
				glUniform1i(uSpecTex, 2);
			}

			model = glm::mat4(1.0f);
			model[3].w = 1.0f;
			glm::vec3 axis(posicionesMallas[i].x, posicionesMallas[i].y, posicionesMallas[i].z);
			//model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, axis);
			//model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

			modelView = view * model;
			modelViewProj = proj * view * model;
			normal = glm::transpose(glm::inverse(modelView));

			if (uModelViewMat != -1)
				glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE,
				&(modelView[0][0]));
			if (uModelViewProjMat != -1)
				glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
				&(modelViewProj[0][0]));
			if (uNormalMat != -1)
				glUniformMatrix4fv(uNormalMat, 1, GL_FALSE,
				&(normal[0][0]));
			glBindVertexArray(vaos[i]);
			if (i == 0) { //Dibujamos el suelo
				glDrawElements(GL_TRIANGLE_STRIP, numeroIndicesSuelo * 3,
					GL_UNSIGNED_INT, (void*)0);
			}
			else if (i == 1){ //Dibujamos el agua
				glDrawElements(GL_TRIANGLE_STRIP, numeroIndicesAgua * 3,
					GL_UNSIGNED_INT, (void*)0);
			}
			else if (i == 2) { //Dibujamos el techo
				glDrawElements(GL_TRIANGLE_STRIP, numeroIndicesTecho * 3,
					GL_UNSIGNED_INT, (void*)0);
			}
			else if (i == 3 || i == 5) { //Dibujamos el Agujero
				glDrawElements(GL_TRIANGLES, sueloNTriangleIndex * 3,
					GL_UNSIGNED_INT, (void*)0);
			}
			else if (i == 4  || i == 6){
				//Dibujamos el god ray
				//Renderizamos el god ray mediante GL_BLEND para que sea transparente.
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glDrawElements(GL_TRIANGLES, sueloNGodRayTriangleIndex * 3,
					GL_UNSIGNED_INT, (void*)0);
				glDisable(GL_BLEND);

			}
			else{
				glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
					GL_UNSIGNED_INT, (void*)0);
			}
			glUseProgram(NULL);

		}


		//Siempre la ultima funcion para renderizar
		glutSwapBuffers();

	}
	void destroy() {

		for (int i = 0; i < posicionesMallas.size(); i++) {
			//Liberamos recursos
			//Desasignamos shader de vertices al programa
			glDetachShader(program, vshader);
			//Desasignamos shader de fragmentos al programa
			glDetachShader(program, fshader);
			//Borramos el shader de vertices
			glDeleteShader(vshader);
			//Borramos el shader de fragmentos
			glDeleteShader(fshader);
			//Borramos el programa
			glDeleteProgram(program);

			//Activamos un array buffer 0
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//Activamos un element array buffer 0
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//Borramos los VBO
			if (inPos[i] != -1) glDeleteBuffers(1, &posVBO);
			if (inColor[i] != -1) glDeleteBuffers(1, &colorVBO);
			if (inNormal[i] != -1) glDeleteBuffers(1, &normalVBO);
			if (inTexCoord[i] != -1) glDeleteBuffers(1, &texCoordVBO);
			glDeleteBuffers(1, &triangleIndexVBO);
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &vaos[i]);
			//Borramos las texturas
			glDeleteTextures(1, &colorTexId);
			glDeleteTextures(1, &colorTexIdNube);
			glDeleteTextures(1, &colorTexIdgodRay);
			glDeleteTextures(1, &emiTexId);
			glDeleteTextures(1, &specTexId);
		}
	}
private:

	void initShader(const char *vname, const char *fname) {
		//Crea shader de vertices
		vshader = loadShader(vname, GL_VERTEX_SHADER);
		//Crea shader de fragmentos
		fshader = loadShader(fname, GL_FRAGMENT_SHADER);

		//Crea un programa
		program = glCreateProgram();
		//Asocia los shaders al programa
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);


		//Antes de linkado
		//Identificadores de los atributos(Se propone valores)
		glBindAttribLocation(program, 0, "inPos");
		glBindAttribLocation(program, 1, "inColor");
		glBindAttribLocation(program, 2, "inNormal");
		glBindAttribLocation(program, 3, "inTexCoord");

		//Linkamos el programa
		glLinkProgram(program);

		int linked;
		//Preguntamos el estatus del linkado
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(program, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteProgram(program);
			program = 0;
			exit(-1);
		}

		//Despues del Linkado
		//Se obtiene los valores id de las matrices
		uNormalMat = glGetUniformLocation(program, "normal");
		uModelViewMat = glGetUniformLocation(program, "modelView");
		uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
		uIntMat = glGetUniformLocation(program, "intenLuz");
		uPosMat = glGetUniformLocation(program, "posiLuz");
		uDirMat = glGetUniformLocation(program, "posiDireccional");
		uFocMat = glGetUniformLocation(program, "posiFocal");
		uPunMat = glGetUniformLocation(program, "posiPuntual");
		uTipoMat = glGetUniformLocation(program, "tipoLuz");

		//PONER DESPUES DE LINKAR
		//Cuales son los identificadores que van a recoger la textura
		uColorTex = glGetUniformLocation(program, "colorTex");
		uEmiTex = glGetUniformLocation(program, "emiTex");
		uSpecTex = glGetUniformLocation(program, "specTex");

		//Obtiene los id de los identificadores(valores finales). Pero por ejemplo, mi shader no usa texturas, por lo que inTexCoord seria -1
		inPos.push_back(glGetAttribLocation(program, "inPos"));
		inColor.push_back(glGetAttribLocation(program, "inColor"));
		inNormal.push_back(glGetAttribLocation(program, "inNormal"));
		inTexCoord.push_back(glGetAttribLocation(program, "inTexCoord"));
		programs.push_back(program);


	}
	void initObjcubo(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float)* 3,
				cubeVertexPos, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float)* 3, cubeVertexColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float)* 3, cubeVertexNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float)* 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}

		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int)* 3, cubeTriangleIndex,
			GL_STATIC_DRAW);

		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		//colorTexId = loadTex("../img/cueva.jpg");
		//emiTexId = loadTex("../img/cuevaEmisive.jpg");
		//specTexId = loadTex("../img/specMap.png");
		vaos.push_back(vao);

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}

	void initObjsuelo(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesSuelo * sizeof(float)* 3, sueloVertices, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesSuelo * sizeof(float)* 3, sueloColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesSuelo * sizeof(float)* 3, sueloNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesSuelo * sizeof(float)* 2, sueloUV, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}
		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numeroIndicesSuelo * sizeof(unsigned int)* 3, sueloIndices, GL_STATIC_DRAW);

		vaos.push_back(vao);
		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		colorTexId = loadTex("../img/cueva.jpg");
		emiTexId = loadTex("../img/cuevaEmisive.jpg");
		specTexId = loadTex("../img/specMap.png");

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}

	void initObjAgua(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesAgua * sizeof(float)* 3, AguaVertices, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesAgua * sizeof(float)* 3, AguaColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesAgua * sizeof(float)* 3, AguaNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesAgua * sizeof(float)* 2, AguaUV, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}
		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numeroIndicesAgua * sizeof(unsigned int)* 3, AguaIndices, GL_STATIC_DRAW);

		vaos.push_back(vao);
		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		//colorTexId = loadTex("../img/cueva.jpg");
		//emiTexId = loadTex("../img/cuevaEmisive.jpg");
		//specTexId = loadTex("../img/specMap.png");

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}
	void initObjAgujero(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNVertex * sizeof(float)* 3, sueloVertexPos, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNVertex * sizeof(float)* 3, sueloVertexColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNVertex * sizeof(float)* 3, sueloVertexNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNVertex * sizeof(float)* 2, sueloVertexTexCoord, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}
		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sueloNTriangleIndex * sizeof(unsigned int)* 3, sueloTriangleIndex, GL_STATIC_DRAW);

		vaos.push_back(vao);
		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		colorTexIdNube = loadTex("../img/nube.jpg");
		emiTexIdNube = loadTex("../img/nubeEmisive.jpg");
		specTexId = loadTex("../img/specMap.png");

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}
	void initObjGodRay(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNGodRayVertex * sizeof(float)* 3, sueloVertexGodRayPos, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNGodRayVertex * sizeof(float)* 3, sueloVertexGodRayColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNGodRayVertex * sizeof(float)* 3, sueloVertexGodRayNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sueloNGodRayVertex * sizeof(float)* 2, sueloVertexGodRayTexCoord, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}
		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sueloNGodRayTriangleIndex * sizeof(unsigned int)* 3, sueloTriangleGodRayIndex, GL_STATIC_DRAW);

		vaos.push_back(vao);
		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		colorTexIdgodRay = loadTex("../img/godRay03.png");
		//emiTexIdNube = loadTex("../img/nubeEmisive.jpg");
		//specTexId = loadTex("../img/specMap.png");

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}

	void initObjTecho(int i) {
		//Iniciar VAO
		glGenVertexArrays(1, &vao);
		//Activar VAO
		glBindVertexArray(vao);
		// Miro si InPos(localizador del atributo que tiene la posicion de los vertices) se utiliza dentro del shader
		if (inPos[i] != -1)
		{
			//Iniciar VBO de posicion
			glGenBuffers(1, &posVBO);
			//Activar VBO de posicion
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			//Subir los datos al VBO (como array de bits sin configuracion)
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesTecho * sizeof(float)* 3, TechoVertices, GL_STATIC_DRAW);
			//(Ahora se guarda en VAO) Quiero meter el VBO que esta activo quiero meterlo en InPos, cogiendo vectores de 3 elementos, cada uno de esos elementos es un float, antes de pasarselo al shader NO lo tiene que normaliza, por ultimo Obset, strike.
			glVertexAttribPointer(inPos[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			//Configuro en el VAO que el atributo inPos lo tiene que utilizar
			glEnableVertexAttribArray(inPos[i]);
		}
		//Igual que InPos
		if (inColor[i] != -1)
		{
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesTecho * sizeof(float)* 3, TechoColor, GL_STATIC_DRAW);
			glVertexAttribPointer(inColor[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inColor[i]);
		}	
		//Igual que InPos
		if (inNormal[i] != -1)
		{
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesTecho * sizeof(float)* 3, TechoNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(inNormal[i], 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inNormal[i]);
		}
		//Igual que InPos
		if (inTexCoord[i] != -1)
		{
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			//glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, numeroVerticesTecho * sizeof(float)* 2, TechoUV, GL_STATIC_DRAW);
			glVertexAttribPointer(inTexCoord[i], 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(inTexCoord[i]);
		}
		//Creo buffer de posiciones
		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numeroIndicesTecho * sizeof(unsigned int)* 3, TechoIndices, GL_STATIC_DRAW);

		vaos.push_back(vao);
		//PONER ANTES DE MATRIZ MODEL
		//Cargar texturas
		//colorTexId = loadTex("../img/cueva.jpg");
		//emiTexId = loadTex("../img/cuevaEmisive.jpg");
		//specTexId = loadTex("../img/specMap.png");

		//inicializamos la matriz model como la matriz identidad
		model = glm::mat4(1.0f);
	}

	//Funcion que se pasa el nombre del shader y el tipo del shader(vertices o fragmentos).
	//Se coge el codigo del fichero, se carga en String, se compila y devolvemos el identificador
	GLuint loadShader(const char *fileName, GLenum type) {
		//Variable donde nos devuelven la longitud del fichero
		unsigned int fileLen;
		//Ponemos el nombre del fichero y nos devuelve la cadena de caracteres con su longitud. Funcion de auxiliar
		char *source = loadStringFromFile(fileName, fileLen);

		//////////////////////////////////////////////
		//Creación y compilación del Shader

		//Creamos un shader del tipo que se nos indique en la funcion.
		GLuint shader;
		shader = glCreateShader(type);
		//Asignamos al shader, la cadena de caracteres que acabamos de cargar, con su longitud.
		//El 1 es la posicion donde lee. Ya que se puede meter un array de shaders.
		glShaderSource(shader, 1,
			(const GLchar **)&source, (const GLint *)&fileLen);
		//Compilamos el shader
		glCompileShader(shader);
		//Liberamos el recurso de la cadena del shader
		delete source;

		//Comprobamos que se compilo bien
		//Variable compiled
		GLint compiled;
		//En compiled se guarda el estado de compilacion de shader
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		//Si el estado es incorrecto...
		if (!compiled)
		{
			//Calculamos una cadena de error
			GLint logLen;
			//Reservar espacio para la cadena de error
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
			//Metemos el error en la variable de error
			char *logString = new char[logLen];
			//Mostramos el error por pantalla
			glGetShaderInfoLog(shader, logLen, NULL, logString);
			std::cout << "FILE NAME: " << fileName << std::endl;
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteShader(shader);
			exit(-1);
		}

		return shader;

	}
	unsigned int loadTex(const char *fileName) {

		unsigned char *map;
		unsigned int w, h;
		//Cargamos la textura desde fichero. 
		map = loadTexture(fileName, w, h);
		if (!map) //no ha sido capaz de cargar la textura
		{
			std::cout << "Error cargando el fichero: "
				<< fileName << std::endl;
			exit(-1);
		}
		//Creamos una variable. Identificador donde vamos a almacenar la textura
		unsigned int texId;
		//Pedimos a OpenGL que nos devuelva un identificador  de textura
		glGenTextures(1, &texId);
		//Activamos la textura
		glBindTexture(GL_TEXTURE_2D, texId);
		//Subimos la textura
		//GL_TEXTURE_2D es el tipo de textura
		//0 voy a subir los datos al nivel 0 del bitmap
		//GL_RGBA8 8 bits por canal, 4 canales. Formato interno de la tarj. grafica. Como se almacenan lso datos en la tarj grafica 
		//w ancho
		//h alto
		//0 borde
		//GL_RGBA Canales de lso datos que le voy a pasar. Formato RGBA
		//GL_UNSIGNED_BYTE Cada canal es de unsigned_char
		//(GLvoid*)map Puntero a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLvoid*)map);



		//Ya esta subido a la  tarjeta grafica, por lo que liberamos memoria
		delete[] map;
		// Genero todos los bitmaps a partir del bitmap de nivel 0
		glGenerateMipmap(GL_TEXTURE_2D);

		//Indicar el modo de acceso a la textura.
		//Filtrado trilinear para min filter
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		//Filtrado linear para mag filter
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//No puedo direccionar mas del 1
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Cambiado de GL_CLAMP por un fallo en que los edges eran negros
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Cambiado de GL_CLAMP por un fallo en que los edges eran negros

		//if (glewIsSupported("GL_EXT_texture_filter_anisotropic")){
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		//}

		//Devolver el identificador de la textura que acabo de subir a opengl y que acabo de configurar
		return texId;
	}

};



//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void funSpecialKeyboard(int key, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj();
void destroy();
void menu();


Scene scene;

int main(int argc, char** argv)
{
	//Esto se queda igual
	std::locale::global(std::locale("spanish"));// acentos ;)
	initContext(argc, argv);
	initOGL(); //Al hacerlo funcionar, quitar la camara y la posicion de las luces/objeto de aqui.
	//Meter en la clase Scene las luces, camara y mallas
	scene.setCamera(matrizPuntos[0].x, matrizPuntos[0].y, matrizPuntos[0].z, glm::vec4(60.0f, 1.0f, 0.1f, 50.0f));

	//Puntos aleatorios
	srand(time(NULL));

	//Cambiamos los puntos moviendolos un nuero aleatorio de -15 a +15 en el eje X y Z
	for (int a = 0; a < numPuntos; a++) {
		matrizPuntos[a].x = matrizPuntos[a].x + (rand() % 41) - 20;
		matrizPuntos[a].z = matrizPuntos[a].z + (rand() % 41) - 20;
	}

	//Variable para controlar la spline
	int pasadas = 0;

	while (pasadas <= 8) {
		//Realizamos las pasadas necesarias para guardar todos los puntos de la spline en unos vectores para su posterior utizacion
		//en la deformacion de los planos y el movimiento de la camara

		//Cálculo de posición futura (posterior) a partir de puntos
		poscatP = 0.5f * ((2.0f * p1) + (-p0 + p2) * paso + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * pow(paso, 2) + (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * pow(paso, 3));
		//Calculamos vector tangente
		tang = normalizar((poscatP - poscatA));

		//Guardamos la tangente y la posicion
		spline.push_back(poscatP);
		tangente.push_back(tang);

		//Actualizo posiciones
		poscatA = poscat;
		poscat = poscatP;
		paso += cantPas;
		if (paso >= 1.0) {

			for (int i = 0; i < numPuntos; i++) {
				if (i < 7) {
					matrizPuntosN[i] = matrizPuntos[i + 1];
				}
				else {
					matrizPuntosN[i] = matrizPuntos[0];
				}
			}
			for (int j = 0; j < numPuntos; j++) {
				matrizPuntos[j] = matrizPuntosN[j];
			}
			p0 = matrizPuntosN[0];
			p1 = matrizPuntosN[1];
			p2 = matrizPuntosN[2];
			p3 = matrizPuntosN[3];
			p4 = matrizPuntosN[4];
			p5 = matrizPuntosN[5];
			p6 = matrizPuntosN[6];
			p7 = matrizPuntosN[7];
			paso = 0.0f;
			pasadas++;

		}

	}
	//Deformamos suelo y techo
	deformarSuelo(sueloVertices, spline);
	deformarTecho(TechoVertices, spline);
	//Creamos un numero aleatorio entre 0 y 7, para elegir esta posicion en los puntos de control de la Spline
	//para poner el agujero en la pared para la visibilidad del cielo y su godray
	int agujeroAleatorio = (rand() % 7);
	int agujeroAleatorio2 = (rand() % 7);
	while (agujeroAleatorio == agujeroAleatorio2){
		agujeroAleatorio2 = (rand() % 7);
	}

	//MAXIMO 4 LUCES
	//	scene.setLuzPuntual(glm::vec3(0.0, 0.0, 1.0), 0.5f);;
	//scene.setLuzDireccional(glm::vec3(matrizPuntos[agujeroAleatorio].x, 0.0, matrizPuntos[agujeroAleatorio].z), 1.0f);
	//scene.setLuzFocal(glm::vec3(0.0, 2.0, 1.0), 1.0f);
	//scene.setLuzFocal(glm::vec3(0.0, -2.0, 1.0), 1.0f);

	//Cargamos los objetos necesarios con su posicion y sus shaders
	scene.loadMallasuelo(glm::vec3(0.0, -2.0, 0.0), "../shaders_P3/shaderPared.v1.vert", "../shaders_P3/shaderPared.v1.frag", ob++);
	scene.loadMallaAgua(glm::vec3(0.0, -1.75, 0.0), "../shaders_P3/shaderAgua.v1.vert", "../shaders_P3/shaderAgua.v1.frag", ob++);
	scene.loadMallaTecho(glm::vec3(0.0, 0.0, 0.0), "../shaders_P3/shaderPared.v1.vert", "../shaders_P3/shaderPared.v1.frag", ob++);
	scene.loadMallaAgujero(glm::vec3(matrizPuntos[agujeroAleatorio].x, 5.0, matrizPuntos[agujeroAleatorio].z), "../shaders_P3/shaderAgujero.v1.vert", "../shaders_P3/shaderAgujero.v1.frag", ob++);
	scene.loadMallaGodRay(glm::vec3(matrizPuntos[agujeroAleatorio].x, -1.0, matrizPuntos[agujeroAleatorio].z), "../shaders_P3/shadergodRay.v1.vert", "../shaders_P3/shadergodRay.v1.frag", ob++);
	scene.loadMallaAgujero(glm::vec3(matrizPuntos[agujeroAleatorio2].x, 5.0, matrizPuntos[agujeroAleatorio2].z), "../shaders_P3/shaderAgujero.v1.vert", "../shaders_P3/shaderAgujero.v1.frag", ob++);
	scene.loadMallaGodRay(glm::vec3(matrizPuntos[agujeroAleatorio2].x, -1.0, matrizPuntos[agujeroAleatorio2].z), "../shaders_P3/shadergodRay.v1.vert", "../shaders_P3/shadergodRay.v1.frag", ob++);
	//scene.loadMallaGodRay(glm::vec3(matrizPuntos[agujeroAleatorio].x+0.25, -1.0, matrizPuntos[agujeroAleatorio].z+0.01), "../shaders_P3/shadergodRay.v1.vert", "../shaders_P3/shadergodRay.v1.frag", ob++);
	//scene.loadMallaGodRay(glm::vec3(matrizPuntos[agujeroAleatorio].x-0.25, -1.0, matrizPuntos[agujeroAleatorio].z-0.01), "../shaders_P3/shadergodRay.v1.vert", "../shaders_P3/shadergodRay.v1.frag", ob++);

	//Mostramos el menu
	menu();

	glutMainLoop(); //Bucle que espere a eventos

	scene.destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv) {
	glutInit(&argc, argv); //Crea contexto basico
	glutInitContextVersion(3, 3); //Decimos que el contexto es de opengl 3.3
	// glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); Se comenta debido a que tenemos la version 3.3, y no es necesario
	glutInitContextProfile(GLUT_CORE_PROFILE); //no hay compatibilidad para atras
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // Elementos de framebuffer
	glutInitWindowSize(500, 500); //Creacion ventana 500x500
	glutInitWindowPosition(0, 0); //Inicializada en 0,0 del S.O.
	glutCreateWindow("Prácticas OpenGL"); //Crea la ventana con el titulo Practicas OpenGL

	glewExperimental = GL_TRUE; //Se pone proque la libreria Glew es antigua, y aunque hay funciones nuevas, en glew son "experimentales" aun.
	GLenum err = glewInit(); //inicializar glew
	if (GLEW_OK != err) //Si hay error, sale por pantalla
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte *oglVersion = glGetString(GL_VERSION); // Que version Opengl utilizamos(contexto 3.3)
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	//Se puede copiar tambien en el Main. Da igual donde ponerlo, siempre que sea despues de inicializar
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(funSpecialKeyboard);
	glutMouseFunc(mouseFunc);


}

void initOGL() {
	//Activamos el test de profundidad
	glEnable(GL_DEPTH_TEST);
	//Color de fondo
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	//Mi modelo esta definido en Counter Clockwise (sentido antihorario)
	glFrontFace(GL_CCW);
	//Quiero pintar la cara front y back rellenas, no como lineas o puntos
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Activar Culling
	glEnable(GL_CULL_FACE);

}

void renderFunc() {

	scene.render();

}
void resizeFunc(int width, int height) {
	glViewport(0, 0, width, height);
	proj = glm::perspective(glm::radians(60.0f), float(width) / float(height), 1.0f, 50.0f);
	glutPostRedisplay();
}
void idleFunc() {
	if (enMovimiento == true) {
	//Borro lo que habia antes en la matriz model y cargo la matriz identidad
	model = glm::mat4(1.0f);
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;


	//Definimos una rotacion en model
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 0.0f));

	//Catmull-ROM	--  MOVEMOS LA CAMARA POR LA FUNCION SPLINE
	//Cálculo de posición futura (posterior) a partir de puntos
	poscatP = 0.5f * ((2.0f * p1) +
		(-p0 + p2) * paso +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * pow(paso, 2) +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * pow(paso, 3));

	//Calculamos vector tangente
	tang = normalizar((poscatP - poscatA));

	perp = glm::cross(tang, arriba);
	perp = normalizar(perp);

	view = glm::lookAt(poscat, poscat + tang, glm::vec3(0.0f, 1.0f, 0.0f));

	//Actualizo posiciones
	poscatA = poscat;
	poscat = poscatP;
	paso += cantPas;

	Sleep(40);

	if (paso >= 1.0) {
		for (int i = 0; i < numPuntos; i++) {
			if (i < 7) {
				matrizPuntosN[i] = matrizPuntos[i + 1];
			}
			else {
				matrizPuntosN[i] = matrizPuntos[0];
			}
		} 
		for (int j = 0; j < numPuntos; j++) {
			matrizPuntos[j] = matrizPuntosN[j];
		}
		p0 = matrizPuntosN[0];
		p1 = matrizPuntosN[1];
		p2 = matrizPuntosN[2];
		p3 = matrizPuntosN[3];
		p4 = matrizPuntosN[4];
		p5 = matrizPuntosN[5];
		p6 = matrizPuntosN[6];
		p7 = matrizPuntosN[7];
		paso = 0.0f;
	}

	glutPostRedisplay();
	}
}


void keyboardFunc(unsigned char key, int x, int y) {
	
	switch (key)
	{
	case '1':
		std::cout << std::endl << "Renderizado por facetas" << std::endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case '2':
		std::cout << std::endl << "Renderizado por lineas" << std::endl ;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case '3':
		std::cout << std::endl << "Renderizado por puntos" << std::endl;
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;

	case 'p':

		if (enMovimiento == true) {
			std::cout << std::endl << "Movimiento Pausado" << std::endl;
				  enMovimiento = false;
				  posRelativa = poscatA;
				  break;
			}
			  else {
				  std::cout << std::endl << "Movimiento Reanudado" << std::endl;
				  enMovimiento = true;
				  ejeY = 0.0f;
				  ejeX = 0.0f;
				  break;
			  }

			  //Acelerar y decelerar
	case 'o': if (enMovimiento == true && cantPas < 0.5) {
				  std::cout << std::endl << "Acelerando" << std::endl;
				  cantPas += 0.01;
				  break;
	} if (enMovimiento == false) {
				  break;
	}
	case 'l': if (enMovimiento == true && cantPas > 0.006) {
				  std::cout << std::endl << "Desacelerando" << std::endl;
				  cantPas -= 0.005;
				  break;
	} if (enMovimiento == false) {
				  break;
	}
			  //Giros
	case 'w': if (enMovimiento == false) {
				 // if (ejeY < 1.0) {
					  ejeY += 0.023f;
				  //}
				//  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*glm::vec3(perp.x, perp.y, perp.z), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) + glm::lookAt(posRelativa, posRelativa + tang + (ejeX)*glm::vec3(perp.x, perp.y, perp.z), glm::vec3(0.0f, 1.0f, 0.0f));
				  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*(perp.x), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				  glutPostRedisplay();
				  break;
	}
	case 's': if (enMovimiento == false) {
				  //if (ejeY > -1.0) {
					  ejeY -= 0.023f;
				//  }
				  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*(perp.x), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				//  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*glm::vec3(perp.x, perp.y, perp.z), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) + glm::lookAt(posRelativa, posRelativa + tang + (ejeX)*glm::vec3(perp.x, perp.y, perp.z), glm::vec3(0.0f, 1.0f, 0.0f));
				  glutPostRedisplay();
				  break;
	}
			  
	case 'a': if (enMovimiento == false) {
				 // if (ejeX > -1.0) {
					  ejeX -= 0.023f;
				  //}
				  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*(perp.x), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				  //view = glm::lookAt(posRelativa, posRelativa + tang + (ejeX) *glm::vec3(perp.x, perp.y, perp.z), glm::vec3(0.0f, 1.0f, 0.0f)) + glm::lookAt(posRelativa, posRelativa + tang + glm::vec3(0.0f, ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				  glutPostRedisplay();
				  break;
	}
	case 'd': if (enMovimiento == false) {
				 // if (ejeX < 1.0) {
					  ejeX += 0.023f;
				 // }
				  view = glm::lookAt(posRelativa, posRelativa + tang + glm::vec3((ejeX)*(perp.x), ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

				//  view = glm::lookAt(posRelativa, posRelativa + tang + (ejeX)*glm::vec3(perp.x, perp.y, perp.z), glm::vec3(0.0f, 1.0f, 0.0f)) + glm::lookAt(posRelativa, posRelativa + tang + glm::vec3(0.0f, ejeY, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				  glutPostRedisplay();
				  break;
	}
	default:
		break;
	}
}
void mouseFunc(int button, int state, int x, int y) {}
void funSpecialKeyboard(int key, int x, int y) {}
void menu(){
	printf("\n");
	printf("              Menu\n");
	printf("--------------------------------\n");
	printf("Tecla 1: Renderizado por facetas\n");
	printf("Tecla 2: Renderizado por lineas\n");
	printf("Tecla 3: Renderizado por puntos\n");
	printf("Tecla P: Pausar / Reanudar movimiento\n");
	printf("Tecla O: Aumentar velocidad\n");
	printf("Tecla L: Disminuir velocidad\n");
	printf("Teclas WASD: Movimiento de camara\n");
	printf("\n");
	printf("Puntos de Control de Catmun Roll\n");
	for (int a = 0; a < numPuntos; a++) {
	printf("Punto %i : (%f,%f)\n", a, matrizPuntos[a].x, matrizPuntos[a].z);
	}
}