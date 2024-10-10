/*
Pr�ctica 7: Iluminaci�n 1
318026644
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture decaedro;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model llanta1;
Model llanta2;
Model llanta3;
Model llanta4;
Model parabrisas;
Model cofre;
Model parrilla;
Model chazis;
Model lamp;
Model star;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];



SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightssec[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//funci�n de calculo de normales por promedio de v�rtices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//DODECAEDRO
void CreateDecaedro()
{
	///dado
	unsigned int cubo_indices[] = {

		0, 1, 2,
		3, 4, 5,


		6, 7, 8,
		9, 10, 11,


		12, 13, 14,
		15, 16 ,17,


		18, 19, 20,
		21, 22, 23,


		24, 25, 26,
		27, 28, 29,

	};

	GLfloat cubo_vertices[] = {
		//7


		//x		y		z				S		T			NX		NY		NZ
		0.0f, 0.0f, 0.0f,				0.0f,	0.50f,		1.0f,	1.0f,	1.0f,	//
		3.0f, 0.0f, 0.0f,				0.19f,	0.5f,		1.0f,	1.0f,	1.0f, 	//
		1.5f, 2.064572880f, 3.0f,		0.1f,	1.0f,		1.0f,	1.0f,	1.0f,	//


		3.0f, 0.0f, 0.0f,				0.19f,   0.5f,		1.0f,	1.0f,	1.0f,	// coincide
		3.9270509f, 2.8531695f, 0.0f,	0.40f,	0.5f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, 3.0f,		0.3f,	1.00f,		1.0f,	1.0f,	1.0f,	// coincide


		3.9270509f, 2.8531695f, 0.0f,	0.4f, 0.5f,			1.0f,	1.0f,	1.0f,	// coincide
		1.5f, 4.6165253057f, 0.0f,		0.60f,	0.5f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, 3.0f,		0.5f,	1.0f,		1.0f,	1.0f,	1.0f,	// coincide 

		1.5f, 4.6165253057f, 0.0f,			0.60f, 0.50f,		1.0f,	1.0f,	1.0f,	// coincide
		-0.92705098f, 2.853169f, 0.0f,		0.8f,	0.50f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, 3.0f,			0.7f,	1.0,		1.0f,	1.0f,	1.0f,	// coincide

		//x		y		z					S		T
		-0.92705098f, 2.853169f, 0.0f,		0.8f, .50f,		1.0f,	1.0f,	1.0f,	// coincide
		0.0f, 0.0f, 0.0f,					1.0f,	0.50f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, 3.0f,			0.9f,	1.0f,		1.0f,	1.0f,	1.0f,	// coincide

		//PARTE INFERIOR

		0.0f, 0.0f, 0.0f,				0.0f,	0.5f,		1.0f,	1.0f,	1.0f,	// 
		3.0f, 0.0f, 0.0f,				0.2f,	0.5f,		1.0f,	1.0f,	1.0f, 	// 
		1.5f, 2.064572880f, -3.0f,		0.1f,	0.0f,		1.0f,	1.0f,	1.0f,	//



		3.0f, 0.0f, 0.0f,				0.2f,   0.5f,		1.0f,	1.0f,	1.0f,	//
		3.9270509f, 2.8531695f, 0.0f,	0.4f,	0.5f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, -3.0f,		0.3f,	0.0f,		1.0f,	1.0f,	1.0f,	//


		3.9270509f, 2.8531695f, 0.0f,	0.4f, 0.50f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 4.6165253057f, 0.0f,		0.6f,	0.50f,		1.0f,  1.0f,	1.0f,	//
		1.5f, 2.064572880f, -3.0f,		0.5f,	0.0f,		1.0f,	1.0f,	1.0f,	//

		1.5f, 4.6165253057f, 0.0f,			0.6f, 0.50f,		1.0f,	1.0f,	1.0f,	//
		-0.92705098f, 2.853169f, 0.0f,		0.8f,	0.50f,		1.0f,	1.0f,	1.0f,	//
		1.5f, 2.064572880f, -3.0f,			0.7f,	0.0f,		1.0f,	1.0f,	1.0f,	//


		//x		y		z				S		T

	   -0.92705098f, 2.853169f, 0.0f,		0.8f, 0.50f,		1.0f,	1.0f,	1.0f,	//
	   0.0f, 0.0f, 0.0f,					1.0f,	0.50f,	   1.0f,	1.0f,	1.0f,	//
	   1.5f, 2.064572880f, -3.0f,			0.9f,	0.0f,		1.0f,	1.0f,	1.0f,	//




	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 240, 36);
	////dado->CreateMesh(cub_vertices,cub_indices,232,29);
	meshList.push_back(dado);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateDecaedro();
	CreateShaders();


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	decaedro = Texture("Textures/decaedro.png");
	decaedro.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	llanta1 = Model();
	llanta1.LoadModel("Models/llanta1.obj");
	llanta2 = Model();
	llanta2.LoadModel("Models/llanta2.obj");
	llanta3 = Model();
	llanta3.LoadModel("Models/llanta3.obj");
	llanta4 = Model();
	llanta4.LoadModel("Models/llanta4.obj");

	parabrisas = Model();
	parabrisas.LoadModel("Models/parabrisas.obj");

	cofre = Model();
	cofre.LoadModel("Models/cofreblue.obj");


	parrilla = Model();
	parrilla.LoadModel("Models/parrillayluces.obj");


	chazis = Model();
	chazis.LoadModel("Models/chazis_blue.obj");

	lamp = Model();
	lamp.LoadModel("Models/lampara.obj");

	star = Model();
	star.LoadModel("Models/star_texturizada.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales

	unsigned int pointLightCount = 0;
	
	
	unsigned int pointLightCount2 = 0;

	//LUZ LAMPARA
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, .90f, 1.0f, //UBICACION --> LAMPARA (0,0,1)
		0.1f, 0.1f, 0.1f);
	pointLightCount++;

	//LUZ STAR
	pointLights2[1] = PointLight(1.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f, 5.0f, //UBICACION --> LAMPARA (0,0,1)
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//INICIAN LOS SPOTLIGHTS

	unsigned int spotLightCount = 0;
	unsigned int spotLightCountsec = 4;
	//linterna
	spotLights[0] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f); //valor de apertura , quue tan grande quero que sea ekpunto de iluminacion 
	spotLightCount++;


	///CARRO
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f, //color 
		1.0f, 2.0f,
		4.0f, 4.0f, .5f, //posicion
		///4.0f, 2.0f, -3.0f,
		-5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		///0.1f, 0.1f, 00f
		15.0f); ///angulo
	spotLightCount++;

	//HELICOPTERO


	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f, ////color 
		1.0f, 2.0f, ///intensidad
		10.0f, 2.0f, -2.5f, ///posicion
		///4.0f, 2.0f, -2.5f,
		0.0f, -5.0f, 0.0f, ////direccion
		1.0f, 0.0f, 0.0f, ///  coeficientes
		///0.1f, 0.1f, 0.0f
		15.0f); ///angulo
	spotLightCount++;

	
	//LUZ CARRO
	spotLightssec[0] = spotLights[0];
	spotLightssec[1] = spotLights[1];
	spotLightssec[2] = SpotLight(1.0f, 0.0f, 1.0f, //color 
		1.0f, 2.0f,
		4.0f, 4.0f, .5f, //posicion
		///4.0f, 2.0f, -3.0f,
		5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		///0.1f, 0.1f, 00f
		15.0f); ///angulo
	spotLightCount++;
	spotLightssec[3] = spotLights[3];






	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		pointLights2[0] = pointLights[1];
		pointLights2[1] = pointLights[0];
		//pointLights2[2] = pointLights[1];
		//pointLights2[3] = pointLights[3];

		if (mainWindow.getonoff() == 2.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount-1); //apago
		}

		if (mainWindow.getonoff() == 4.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2); //apago (con 4 apago lampara)
		}
		
	
		else if (mainWindow.getonoff() == 1.0f || mainWindow.getonoff() == 3.0f) { //prendo (con 2 lampara)
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}

		

		//idea para el carro
		/*
		pointLights2[0] = pointLights[2];
		pointLights2[1] = pointLights[1];
		pointLights2[2] = pointLights[0];
		//pointLights2[3] = pointLights[3];

		if (mainWindow.getonoff() == 0.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getonoff() == 1.0f) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}*/

		/*idea 2 para el carro
		* if (mainWindow.getonoff() == 0.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getonoff() == 1.0f) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}*/
		


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess); //puedo cambiar a mmaterial brillante 

		meshList[2]->RenderMesh();


		//Dado ejercicio

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decaedro.UseTexture();
		meshList[4]->RenderMesh();

		//HELICOPTERO
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::translate(model, glm::vec3(mainWindow.getmuevexhel(), 0.0f, 0.0));
		spotLights[3].SetPos(glm::vec3(mainWindow.getmuevexhel(), 5.0, 6.0f));
		//spotLights[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(mainWindow.getmuevexSEC(), 0.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		// LAMPRA
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0));

		//spotLights[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();

		// STAR
		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 5.0));

		//spotLights[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		star.RenderModel();
		//EJERCICIO 07
		
		///CHAZIS

		//CARRO LUCES 

		


		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(4.0f, 0.70f, -1.5f));
		///0.0f,0.5f,-1.5f
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0));

		if (mainWindow.getonposneg() == false) {
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
			
			spotLights[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));


		}
		else if (mainWindow.getonposneg() == true) {

			shaderList[0].SetSpotLights(spotLightssec, spotLightCountsec);
			spotLightssec[2].SetPos(glm::vec3(8.0f + mainWindow.getmuevex(), -1.0f, 0.0f));

		}

		/*idea 2 para el carro
		* if (mainWindow.getonoff() == 0.0f) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else if (mainWindow.getonoff() == 1.0f) {
			shaderList[0].SetPointLights(pointLights2, pointLightCount2);
		}*/
		

		modelaux = model;
		modelaux2 = model;
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chazis.RenderModel();



		//LLANTA 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.8f, -1.0f, -2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta1.RenderModel();

		//LLANTA2
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.60f, 0.0f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta2.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(0.10f, 0.0f, 3.850f));

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta3.RenderModel();
		//

		model = modelaux;
		model = glm::translate(model, glm::vec3(5.750f, 0.05f, 3.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta4.RenderModel();


		//cofre
		//model=modelaux2;
		model = modelaux2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.8f, -1.0f));
		model = glm::translate(model, glm::vec3(-3.2f, 0.55f, 0.1f));
		//model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre.RenderModel();
		/*
		//para cofre
		if (mainWindow.getonposneg() == false) {
			shaderList[0].SetSpotLights(spotLights, spotLightCount);

			spotLights[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));


		}
		else if (mainWindow.getonposneg() == true) {

			shaderList[0].SetSpotLights(spotLightssec, spotLightCountsec);
			spotLightssec[2].SetPos(glm::vec3(0.0f + mainWindow.getmuevex(), -1.0f, 0.0f));

		}
		*/
		//parabrisas
		//model=modelaux2;
		model = modelaux2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.8f, -1.0f));
		model = glm::translate(model, glm::vec3(-.75f, 1.035f, -0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parabrisas.RenderModel();

		//parrilla
			//parabrisas
		//model=modelaux2;
		model = modelaux2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.8f, -1.0f));
		model = glm::translate(model, glm::vec3(-4.29f, -.30f, 0.30f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parrilla.RenderModel();
		

		/*
		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		*/


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}