/*
Práctica 9: Animacion 1
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
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
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

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//variables para animación
float movCoche;
float movCocheY;
float movOffset;
float movDado;
float movOffdado;
float rotllanta;
float rotllantaOffset;
bool avanza;
bool avanza2; 
int direccion = 0; // 0: X positivo, 1: Y positivo, 2: X negativo, 3: Y negativo
const float rotacion_angulo_helicoptero = 90.0f; // Ángulo en grados


GLfloat arrancar;
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
Model tablero;
Model elice;



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
PointLight arreglo2puntual[MAX_SPOT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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
		
	   -0.92705098f, 2.853169f, 0.0f,		0.8f, 0.50f,		-1.0f,	-1.0f,	-1.0f,	//
	   0.0f, 0.0f, 0.0f,					1.0f,	0.50f,		-1.0f,	-1.0f,	-1.0f,	//
	   1.5f, 2.064572880f, -3.0f,			0.9f,	0.0f,		-1.0f,	-1.0f,	-1.0f,	//

		


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
	Kitt_M.LoadModel("Models/helicoptero.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	tablero = Model();
	tablero.LoadModel("Models/tablero_pretexturizado.obj");
	elice = Model();
	elice.LoadModel("Models/elice_2.obj");

	



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


	

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	
	movCoche = 0.0f;
	movOffset = 0.3f;
	movDado = 0.0f;
	movOffdado = 0.3;
	rotllanta = 0.0f;
	rotllantaOffset = 5.0f;
	avanza = true;
	avanza2 = true;
	while (!mainWindow.getShouldClose())
	{
		

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//arrancar = glfwGetTime();
		//printf("arrancar %f \n", arrancar);
		if (mainWindow.getarrancar() == true)
		{
			if (avanza)
			{
				switch (direccion)
				{
				case 0: // X positivo
					if (movCoche < 100.0f)
					{
						movCoche += movOffset * deltaTime;
						rotllanta += rotllantaOffset * deltaTime;
					}
					else
					{
						direccion = 1; // Cambiar a Y positivo
						movCoche = 100.0f; // Asegurar que no sobrepase el límite
					}
					break;

				case 1: // Y positivo
					if (movCocheY < 100.0f)
					{
						movCocheY += movOffset * deltaTime;
						rotllanta += rotllantaOffset * deltaTime;
					}
					else
					{
						direccion = 2; // Cambiar a X negativo
						movCocheY = 100.0f; // Asegurar que no sobrepase el límite
					}
					break;

				case 2: // X negativo
					if (movCoche > -100.0f)
					{
						movCoche -= movOffset * deltaTime;
						rotllanta += rotllantaOffset * deltaTime;
					}
					else
					{
						direccion = 3; // Cambiar a Y negativo
						movCoche = -100.0f; // Asegurar que no sobrepase el límite
					}
					break;

				case 3: // Y negativo
					if (movCocheY > -100.0f)
					{
						movCocheY -= movOffset * deltaTime;
						rotllanta += rotllantaOffset * deltaTime;
					}
					else
					{
						direccion = 0; // Cambiar a X positivo
						movCocheY = -100.0f; // Asegurar que no sobrepase el límite
					}
					break;
				}
			}
		}

		//ANIMACION DADO
		/*
		
		if (mainWindow.getarrancar2() == true) {
			if (avanza2) {
				if (movDado > -8.0f) {
					movDado -= movOffdado * deltaTime;
					//printf("avanza%f \n ",movCoche);
					//rotllanta += rotllantaOffset * deltaTime;
				}
				else {
					avanza2 = !avanza2;
				}
			} */
		/*
			else {
				if (movDado < 5.0f) {
					//movDado += movOffdado * deltaTime;
					//printf("avanza%f \n ",movCoche);
					//rotllanta -= rotllantaOffset * deltaTime;
				}
				else {
					avanza2 = !avanza2;
				}
			}
		}*/

		
		std::srand(static_cast<unsigned>(std::time(0)));

		
		int rotaciones = std::rand() % 20 + 1;

	
		float angulo = rotaciones * 360.0f * toRadians;

	
		float tiempo = 7.0f;

		
		float  velocidad= angulo  / tiempo;

		float rotacion = 0.0f;
		float lapso = 0.0f;
		bool rotaciontf = false; // Indica si el objeto está en fase de rotación
		bool rota = false; // Indica si el objeto está en fase de rotación
		if (mainWindow.getarrancar2() == true) {
			if (avanza2) {
				if (movDado > -8.0f) {
					movDado -= movOffdado * deltaTime;
					//printf("avanza%f \n ",movCoche);
					//rotllanta += rotllantaOffset * deltaTime;
				}
				else if (movDado == -8.0f) {
					// Al llegar a la posición -8, comienza a girar
					avanza2 = false; // Detener el avance
					rota = true; // Activar la rotación
					lapso = 0.0f; // Reiniciar el tiempo para la rotación
				}
			}
			else if (rota) {
				// Actualizar el tiempo transcurrido para la rotación
				lapso += deltaTime;

				// Actualizar la rotación actual
				rotacion = velocidad * lapso;

				if (lapso >= tiempo) {
					rotacion = angulo; // Limitar a la rotación total calculada
					rota = false; // Finalizar la rotación
				}
			}
		}

		

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		




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
		model = glm::translate(model, glm::vec3(0.0f, 10+movDado, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotacion, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decaedro.UseTexture();
		meshList[4]->RenderMesh();

		//Instancia del helicoptero
		// Aplicar transformaciones de modelo
		model = glm::mat4(1.0);

		switch (direccion)
		{
		case 0: // X positivo

			model = glm::translate(model, glm::vec3(movCoche, 45.0f, -100.0f));
			model = glm::rotate(model, 2 * rotacion_angulo_helicoptero * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 1: // Y positivo
			model = glm::translate(model, glm::vec3(100.0f, 45.0f, movCocheY));
			model = glm::rotate(model, rotacion_angulo_helicoptero * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 2: // X negativo ssi entra
			model = glm::translate(model, glm::vec3(movCoche, 45.0f, 100.0)); //si entra
			model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			break;

		case 3: // Y negativo
			model = glm::translate(model, glm::vec3(-100.0f, 45.0f, movCocheY));
			model = glm::rotate(model, 3 * rotacion_angulo_helicoptero * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			break;


		}

		// Guardar el modelo transformado para uso adicional si es necesario
		modelaux = model;

		// Escalar el modelo
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		// Enviar la matriz de modelo al shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Renderizar el modelo
		Kitt_M.RenderModel();

		//Instancia helices 
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.20f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		elice.RenderModel();

		//Instancia del tablero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 2.0, -120.0f)); //el movimiento lo cambio al eje y para que suba y baje 
		modelaux = model;
		model = glm::scale(model, glm::vec3(15.0f, 5.0f, 10.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tablero.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
