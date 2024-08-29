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
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067



void CrearLetrasyFiguras()
{
	GLfloat vertices_C[] = {	

		//C
		//X			Y			Z			R		G		B

		//Primer triangulo, primer lateral
		-1.0f, 0.0f, 0.0f, 0.6f,	1.0f,	0.6f,
		-.85f, 1.f, 0.0f,  0.60f,	1.0f,	0.60f,
		-1.0f, 1.0f, 0.0f, 0.60f,	1.0f,	0.60f,

		-1.0f, 0.0f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.85f, 0.0f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.85f, 1.0f, 0.0f, 0.60f,	1.0f,	0.60f,

		//Parte superior C
		-.51f, 1.0f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.85f, .85f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.85f, 1.0f, 0.0f, 0.60f,	1.0f,	0.60f,

		-.51f, 1.0f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.85f, .85f, 0.0f, 0.60f,	1.0f,	0.60f,
		-.51f, .85f,0.0f,  0.60f,	1.0f,	0.60f,

		//Parte inferior C
		-.85f, 0.0f, 0.0f,  0.60f,	1.0f,	0.60f,
		-.51f,0.0f,0.0f,    0.60f,	1.0f,	0.60f,
		-.51f, .15f, 0.0f,  0.60f,	1.0f,	0.60f,

		-.85f, 0.0f, 0.0f,  0.60f,	1.0f,	0.60f,
		-.51f,.15f,0.0f,    0.60f,	1.0f,	0.60f,
		-.85f, .15f, 0.0f,  0.60f,	1.0f,	0.60f,
		
	};
	MeshColor * C = new MeshColor();
	C->CreateMeshColor( vertices_C,108);
	meshColorList.push_back(C);

	GLfloat vertices_A[] = {

		//A
		//Rombo izquierdo
		-.50f, -.25f, 0.0f, 0.680f,	0.850f,	.90f,
		-.40f,.60f,0.0f,	0.680f,	0.850f,	.90f,
		-.35f,-.25f, 0.0f,  0.680f,	0.850f,	.90f,

		-.25f, .60f, 0.0f,	0.680f,	0.850f,	.90f,
		-.40f,.60f,0.0f,	0.680f,	0.850f,	.90f,
		-.35f,-.25f, 0.0f,	0.680f,	0.850f,	.90f,

		//Rombo derecho

		-.25f, .60f, 0.0f,	0.680f,	0.850f,	.90f,
		0.0f,-.25f,0.0f,	0.680f,	0.850f,	.90f,
		-.15f,-.25f, 0.0f,	0.680f,	0.850f,	.90f,

		-.25f, .60f, 0.0f,	0.680f,	0.850f,	.90f,
		-.10f,.60f,0.0f,	0.680f,	0.850f,	.90f,
		0.0f,-.25f, 0.0f,	0.680f,	0.850f,	.90f,

		//Trapecio superior

		-.40f,.60f,0.0f,	0.680f,	0.850f,	.90f,
		-.38f,.75,0.0f,		0.680f,	0.850f,	.90f,
		-.10f,.60f,0.0f,	0.680f,	0.850f,	.90f,

		-.115f,.75f,0.0f,	0.680f,	0.850f,	.90f,
		-.38f,.75,0.0f,		0.680f,	0.850f,	.90f,
		-.10f,.60f,0.0f,	0.680f,	0.850f,	.90f,

		//Trapecio parte inferior

		-.32f,0.22f,0.0f,	0.680f,	0.850f,	.90f,
		-.29,.37f,0.0f,		0.680f,	0.850f,	.90f,
		-.19,.22,0.0f,		0.680f,	0.850f,	.90f,

		-.21f,.37f,0.0f,	0.680f,	0.850f,	.90f,
		-.29,.37f,0.0f,		0.680f,	0.850f,	.90f,
		-.19,.22,0.0f,		0.680f,	0.850f,	.90f,

	};
	MeshColor* A = new MeshColor();
	A->CreateMeshColor(vertices_A, 144);
	meshColorList.push_back(A);
	
	GLfloat vertices_R[] = {
		//R

		//Primer lateral
		0.05f, .5f, 0.0f,	1.0f,	.80f,	0.86f,
		.20f,-.50f,0.0f,	1.0f,	.80f,	0.86f,
		0.05f,-.50f, 0.0f,	1.0f,	.80f,	0.86f,

		.05f, .50f, 0.0f,	1.0f,	.80f,	0.86f,
		.20f,-.50f,0.0f,	1.0f,	.80f,	0.86f,
		.20f,.50f, 0.0f,	1.0f,	.80f,	0.86f,

		//Rectangulo superior

		.20f,.50f, 0.0f,	1.0f,	.80f,	0.86f,
		.20f,.35f, 0.0f,	1.0f,	.80f,	0.86f,
		.5f,.50f, 0.0f,		1.0f,	.80f,	0.86f,

		.20f,.35f, 0.0f,	1.0f,	.80f,	0.86f,
		.5f,.35f, 0.0f,		1.0f,	.80f,	0.86f,
		.5f,.50f, 0.0f,		1.0f,	.80f,	0.86f,


		//Rectangulo inferior

		.20f,.20f, 0.0f,	1.0f,	.80f,	0.86f,
		.20f,.05f, 0.0f,	1.0f,	.80f,	0.86f,
		.5f,.05f, 0.0f,		1.0f,	.80f,	0.86f,

		.20f,.20f, 0.0f,	1.0f,	.80f,	0.86f,
		.5f,.05f, 0.0f,		1.0f,	.80f,	0.86f,
		.5f,.20f, 0.0f,		1.0f,	.80f,	0.86f,

		//Rectangulo interno

		0.5f,.20f,0.0f,		1.0f,	.80f,	0.86f,
		0.5f,.35f,0.0f,		1.0f,	.80f,	0.86f,
		0.35f,.35f,0.0f,	1.0f,	.80f,	0.86f,

		0.5f,.20f,0.0f,		1.0f,	.80f,	0.86f,
		0.35f,.20f,0.0f,	1.0f,	.80f,	0.86f,
		0.35f,.35f,0.0f,	1.0f,	.80f,	0.86f,

		//Rombo inferior

		.35f,.05f, 0.0f,	1.0f,	.80f,	0.86f,
		.5f,-.50f, 0.0f,	1.0f,	.80f,	0.86f,
		.35f,-.50f, 0.0f,	1.0f,	.80f,	0.86f,

		.35f,-.50f, 0.0f,	1.0f,	.80f,	0.86f,
		.35f,.05f, 0.0f,	1.0f,	.80f,	0.86f,
		.20f,.05f, 0.0f,	1.0f,	.80f,	0.86f,
		

	};
	MeshColor* R = new MeshColor();
	R->CreateMeshColor(vertices_R, 180);
	meshColorList.push_back(R);

	GLfloat vertices_J[] = {
		//J
		//Rectangulo inferior
		.55f,-.75f,0.0f, 0.78f,	0.64f,	0.78f,
		.85f,-.75f,0.0,	 0.78f,	0.64f,	0.78f,
		.85f,-.60f,0.0,	 0.78f,	0.64f,	0.78f,

		.55f,-.75f,0.0f,	   0.78f,	0.64f,	0.78f,
		.85f,-.60f,0.0,		0.78f,	0.64f,	0.78f,
		.55f,-.60f,0.0,		0.78f,	0.64f,	0.78f,

		//Rectangulo superior
		.55f,.25f,0.0f,		0.78f,	0.64f,	0.78f,
		.55f,.10f,0.0,		0.78f,	0.64f,	0.78f,
		.98f,.25f,0.0,		0.78f,	0.64f,	0.78f,

		.98f, .10f, 0.0f,		0.78f,	0.64f,	0.78f,
		.55f, .10f, 0.0,		0.78f,	0.64f,	0.78f,
		.98f, .25f, 0.0,		0.78f,	0.64f,	0.78f,

		//Rectangulo medio

		.85f,-.60f,0.0f,	0.78f,	0.64f,	0.78f,
		.70f,-.60f,0.0f,	0.78f,	0.64f,	0.78f,
		.85f,.10f,0.0f,		0.78f,	0.64f,	0.78f,

		.70f,-.60f, 0.0f,	0.78f,	0.64f,	0.78f,
		.70f, .10f, 0.0f,	0.78f,	0.64f,	0.78f,
		.85f,.10f, 0.0f,	0.78f,	0.64f,	0.78f,
	
	};
	MeshColor* J = new MeshColor();
	J->CreateMeshColor(vertices_J, 110);
	meshColorList.push_back(J);

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(1020, 720);
	mainWindow.Initialise();
	
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
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
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		
		//C
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		//R
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		//J
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();


	

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