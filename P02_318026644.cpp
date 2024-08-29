//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader

//RED
static const char* vShaderR = "shaders/shaderR.vert";
static const char* fShaderR = "shaders/shader.frag";

//GREEN 
static const char* vShaderG = "shaders/shaderG.vert";
static const char* fShaderG = "shaders/shader.frag";

//BLUE
static const char* vShaderB = "shaders/shaderB.vert";
static const char* fShaderB = "shaders/shader.frag";

//GREENF
static const char* vShaderGF = "shaders/shaderGF.vert";
static const char* fShaderGF = "shaders/shader.frag";

//CAFE
static const char* vShaderC = "shaders/shaderC.vert";
static const char* fShaderC = "shaders/shader.frag";

//static const char* vShaderColor = "shaders/shadercolor.vert";
//static const char* fShaderColor = "shaders/shadercolor.frag";

//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);


}


void CreateShaders()
{
	//RED SHADER
	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShaderR, fShaderR);
	shaderList.push_back(*shader1);

	//GREEN 
	Shader* shader2 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader2->CreateFromFiles(vShaderG, fShaderG);
	shaderList.push_back(*shader2);

	//BLUE
	Shader* shader3 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader3->CreateFromFiles(vShaderB, fShaderB);
	shaderList.push_back(*shader3);

	//GREEN FUERTE
	Shader *shader4 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader4->CreateFromFiles(vShaderGF, fShaderGF);
	shaderList.push_back(*shader4);

	//CAFE
	Shader* shader5 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader5->CreateFromFiles(vShaderC, fShaderC);
	shaderList.push_back(*shader5);

	
}


int main()
{
	mainWindow = Window(1020, 720);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	
	
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//CUBO ROJO CASA

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.0;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, -.30f, -3.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.45f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();
		
		//TRIANGULO AZUL CASA
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.0;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, .70f, -3.0f));
		model = glm::scale(model, glm::vec3(1.50f, 0.55f, 2.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//CUBO VERDE PUERTA

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[1].useShader(); 
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		angulo += 0.0;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		
		model = glm::translate(model, glm::vec3(0.0f, -0.80f, -2.0f));
		//model = glm::rotate(model,90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.40f, 0.5f, 0.40f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//CUBO VERDE VENTANAS
		//CUBO VERDE VENTANA 1

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		angulo += 0.0;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-0.30f, 0.10f, -2.0f));
		model = glm::rotate(model, 95 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.5f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//CUBO VERDE VENTANA 2

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		angulo += 0.0;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(.30f, 0.10f, -2.0f));
		model = glm::rotate(model, 95 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.350f, 0.5f, 0.350f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		
		//CUBO CAFE
		//PINO DERECHO
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.0;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-.77f, -.85f, -3.0f));
		//model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.0;
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.770f, -.300f, -3.0f));
		model = glm::scale(model, glm::vec3(.35f, 0.80f, 1.0f));
		//model = glm::rotate(model, 220 * toRadians, glm::vec3(0.0f, 5.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//PINO IZQUIERDO
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.0;
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(.770f, -.850f, -3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0-5 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.0;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.770f, -.300f, -3.0f));
		model = glm::scale(model, glm::vec3(.35f, 0.80f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		

		glUseProgram(0);
		mainWindow.swapBuffers();

		





	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/