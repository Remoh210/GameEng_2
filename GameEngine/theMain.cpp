//     ___                 ___ _     
//    / _ \ _ __  ___ _ _ / __| |    
//   | (_) | '_ \/ -_) ' \ (_ | |__  
//    \___/| .__/\___|_||_\___|____| 
//         |_|                       
//
#include "globalOpenGLStuff.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "fmodStuff.h"
#include <stdlib.h>
#include <stdio.h>		// printf();
#include <iostream>		// cout (console out)

#include <vector>		// "smart array" dynamic array

#include "cShaderManager.h"

#include "cMeshObject.h"
#include "cVAOMeshManager.h"
#include "cLightHelper.h"


//FMOD Globals /***********************************************
FMOD_RESULT _result = FMOD_OK;
FMOD::System *_system = NULL;
FMOD::Sound *_sound[NUM_OF_SOUNDS];
FMOD::Channel *_channel[NUM_OF_SOUNDS];
FMOD::ChannelGroup *_channel_groups[NUM_OF_CHANNEL_GROUPS];
FMOD::DSP *_dsp_echo;

FMOD_VECTOR _channel_position1;
FMOD_VECTOR _channel_position2;
FMOD_VECTOR _channel_position3;
FMOD_VECTOR _channel_velocity = { 0.0f, 0.0f, 0.0f };
FMOD_VECTOR _listener_position = { 0.0f, 0.0f, 0.0f };
FMOD_VECTOR _forward = { 0.0f, 0.0f, 0.0f };
FMOD_VECTOR _up = { 0.0f, 1.0f, 0.0f };

//Functions
bool init_fmod();
bool shutdown_fmod();
void LoadFromFile();
void SetUpSound();
void UpdateSound();


unsigned int _channel_position = 0;
unsigned int _sound_lenght = 0;
float _channel_frequency = 0.0f;
float _channel_volume = 1.0f;
float _channel_pan = 0.0f;
char _songname[128];
float _channel_pitch = 1.0f;

//FMOD Globals /***********************************************



void UpdateWindowTitle(void);


void DoPhysicsUpdate( double deltaTime, 
					  std::vector< cMeshObject* > &vec_pObjectsToDraw );

std::vector< cMeshObject* > vec_pObjectsToDraw;

// To the right, up 4.0 units, along the x axis
glm::vec3 g_lightPos = glm::vec3( 4.0f, 4.0f, 0.0f );
float g_lightBrightness = 400000.0f;

unsigned int numberOfObjectsToDraw = 0;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;


glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_CameraEye = glm::vec3( 0.0, 0.0, 250.0f );




std::string inputFile = "assets/music/songlist_compressed.txt";

cShaderManager* pTheShaderManager = NULL;	
cVAOMeshManager* g_pTheVAOMeshManager = NULL;

cLightManager* LightManager = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


int main(void)
{
	GLFWwindow* window;
	
	init_fmod();
	LoadFromFile();
	SetUpSound();

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Light", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	//glfwSetKeyCallback()
	

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	pTheShaderManager = new cShaderManager();
	pTheShaderManager->setBasePath("assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pTheShaderManager->createProgramFromFile("BasicUberShader",
		vertexShader,
		fragmentShader))
	{		// Shaders are OK
		std::cout << "Compiled shaders OK." << std::endl;
	}
	else
	{
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pTheShaderManager->getLastError() << std::endl;
	}



	GLuint program = pTheShaderManager->getIDFromFriendlyName("BasicUberShader");


	::g_pTheVAOMeshManager = new cVAOMeshManager();


	GLint objectColour_UniLoc = glGetUniformLocation(program, "objectColour");

	GLint lightPos_UniLoc = glGetUniformLocation(program, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(program, "lightBrightness");

	GLint matModel_location = glGetUniformLocation(program, "matModel");
	GLint matView_location = glGetUniformLocation(program, "matView");
	GLint matProj_location = glGetUniformLocation(program, "matProj");
	GLint eyeLocation_location = glGetUniformLocation(program, "eyeLocation");


	// Loading models was moved into this function
	LoadModelTypes(::g_pTheVAOMeshManager, program);
	LoadModelsIntoScene(::vec_pObjectsToDraw);

	// Get the current time to start with
	double lastTime = glfwGetTime();


	//***************************************************************

	LightManager = new cLightManager();

	{
		sLight* pTheMainLight = new sLight();
		pTheMainLight->position = glm::vec4(1.0f, 400.0f, 0.0f, 1.0f);
		pTheMainLight->atten.x = 0.0f;	// 			float constAtten = 0.0f;
		pTheMainLight->atten.y = 0.0001f;	//			float linearAtten = 0.01f;
		pTheMainLight->atten.z = 0.00001f;	//			float quadAtten = 0.001f;
		pTheMainLight->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// White light
		pTheMainLight->param2.x = 1.0f;
		pTheMainLight->SetLightType(sLight::SPOT_LIGHT);
		pTheMainLight->SetSpotConeAngles(15.0f, 35.0f);
		//	pTheOneLight->SetSpotConeAngles( 15.0f, 45.0f );
			// Direction is RELATIVE to the LIGHT (for spots)
			// Straight down... 
		pTheMainLight->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 1.0f));
		//pTheForthLight->AtenSphere - false;
		pTheMainLight->lightName = "MainLight";
		LightManager->vecLights.push_back(pTheMainLight);
		LightManager->LoadUniformLocations(program);
	}

	for(int light_count = 0; light_count < 4;  light_count++)
	{
		sLight* pTorch = new sLight();
		pTorch->position = glm::vec4(light_count * 10.0f, 400.0f, 0.0f, 1.0f);
		pTorch->atten.x = 0.0f;	// 			float constAtten = 0.0f;
		pTorch->atten.y = 0.0001f;	//			float linearAtten = 0.01f;
		pTorch->atten.z = 0.000015f;	//			float quadAtten = 0.001f;
		pTorch->diffuse = glm::vec4(232 /250.0f, 109 / 250.0f, 27/250.0f, 1.0f);// White light
		pTorch->param2.x = 1.0f;
		pTorch->SetLightType(sLight::POINT_LIGHT);
		//pTheForthLight->AtenSphere - false;
		pTorch->lightName = "Torch_Light" + std::to_string(light_count);
		LightManager->vecLights.push_back(pTorch);
		LightManager->LoadUniformLocations(program);
	}


	//saveLightInfo("Default.txt")
	cLightHelper* pLightHelper = new cLightHelper();

	
	loadModels("Models.txt", vec_pObjectsToDraw);
	loadLights("lights.txt", LightManager->vecLights);


	
	// Draw the "scene" (run the program)
	while (!glfwWindowShouldClose(window))
    {

		// Switch to the shader we want
		::pTheShaderManager->useShaderProgram( "BasicUberShader" );

        float ratio;
        int width, height;


		glm::mat4x4 matProjection = glm::mat4(1.0f);
		glm::mat4x4	matView = glm::mat4(1.0f);
 

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);


		glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
		glEnable( GL_CULL_FACE );	// Discared "back facing" triangles

		// Colour and depth buffers are TWO DIFF THINGS.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		matProjection = glm::perspective( 1.0f,			// FOV
			                                ratio,		// Aspect ratio
			                                0.1f,			// Near clipping plane
			                                5000.0f );	// Far clipping plane

		matView = glm::lookAt(g_CameraEye, g_CameraEye + cameraFront, cameraUp);


		glUniform3f(eyeLocation_location, ::g_CameraEye.x, ::g_CameraEye.y, ::g_CameraEye.z);

		//matView = glm::lookAt( g_CameraEye,	// Eye
		//	                    g_CameraAt,		// At
		//	                    glm::vec3( 0.0f, 1.0f, 0.0f ) );// Up

		glUniformMatrix4fv( matView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv( matProj_location, 1, GL_FALSE, glm::value_ptr(matProjection));
		// Do all this ONCE per frame
		LightManager->CopyLightValuesToShader();
			




		// Draw all the objects in the "scene"
		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
			
			glm::mat4x4 matModel = glm::mat4(1.0f);			// mat4x4 m, p, mvp;

			DrawObject(pCurrentMesh, matModel, program);

		}//for ( unsigned int objIndex = 0; 


		// High res timer (likely in ms or ns)
		double currentTime = glfwGetTime();		
		double deltaTime = currentTime - lastTime; 

		for ( unsigned int objIndex = 0; 
			  objIndex != (unsigned int)vec_pObjectsToDraw.size(); 
			  objIndex++ )
		{	
			cMeshObject* pCurrentMesh = vec_pObjectsToDraw[objIndex];
			
			pCurrentMesh->Update( deltaTime );

		}//for ( unsigned int objIndex = 0; 


		// Call the debug renderer call
//#ifdef _DEBUG
//#endif 


		// update the "last time"
		lastTime = currentTime;

		// The physics update loop
		DoPhysicsUpdate( deltaTime, vec_pObjectsToDraw );

		UpdateSound();

		for (std::vector<sLight*>::iterator it = LightManager->vecLights.begin(); it != LightManager->vecLights.end(); ++it)
		{

			sLight* CurLight = *it;
			if (CurLight->AtenSphere == true)
			{


				cMeshObject* pDebugSphere = findObjectByFriendlyName("DebugSphere");
				pDebugSphere->bIsVisible = true;
				pDebugSphere->bDontLight = true;
				glm::vec4 oldDiffuse = pDebugSphere->materialDiffuse;
				glm::vec3 oldScale = pDebugSphere->nonUniformScale;
				pDebugSphere->setDiffuseColour(glm::vec3(255.0f / 255.0f, 105.0f / 255.0f, 180.0f / 255.0f));
				pDebugSphere->bUseVertexColour = false;
				pDebugSphere->position = glm::vec3(CurLight->position);
				glm::mat4 matBall(1.0f);


				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->setUniformScale(0.1f);			// Position
				DrawObject(pDebugSphere, matBall, program);

				const float ACCURACY_OF_DISTANCE = 0.0001f;
				const float INFINITE_DISTANCE = 10000.0f;

				float distance90Percent =
					pLightHelper->calcApproxDistFromAtten(0.90f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);

				pDebugSphere->setUniformScale(distance90Percent);			// 90% brightness
				//pDebugSphere->objColour = glm::vec3(1.0f,1.0f,0.0f);
				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
				DrawObject(pDebugSphere, matBall, program);

				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
				float distance50Percent =
					pLightHelper->calcApproxDistFromAtten(0.50f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance50Percent);
				DrawObject(pDebugSphere, matBall, program);

				pDebugSphere->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
				float distance25Percent =
					pLightHelper->calcApproxDistFromAtten(0.25f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setUniformScale(distance25Percent);
				DrawObject(pDebugSphere, matBall, program);

				float distance1Percent =
					pLightHelper->calcApproxDistFromAtten(0.01f, ACCURACY_OF_DISTANCE,
						INFINITE_DISTANCE,
						CurLight->atten.x,
						CurLight->atten.y,
						CurLight->atten.z);
				pDebugSphere->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
				pDebugSphere->setUniformScale(distance1Percent);
				DrawObject(pDebugSphere, matBall, program);
				pDebugSphere->materialDiffuse = oldDiffuse;
				pDebugSphere->nonUniformScale = oldScale;
				pDebugSphere->bIsVisible = false;
			}
		}




		UpdateWindowTitle();

		glfwSwapBuffers(window);		// Shows what we drew

        glfwPollEvents();
		ProcessAsynKeys(window);




    }

	delete pTheShaderManager;
	delete ::g_pTheVAOMeshManager;

	shutdown_fmod();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}





void UpdateWindowTitle(void)
{



	return;
}

cMeshObject* findObjectByFriendlyName(std::string theNameToFind)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		// Is this it? 500K - 1M
		// CPU limited Memory delay = 0
		// CPU over powered (x100 x1000) Memory is REAAAAALLY SLOW
		if ( vec_pObjectsToDraw[index]->friendlyName == theNameToFind )
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}


cMeshObject* findObjectByUniqueID(unsigned int ID_to_find)
{
	for ( unsigned int index = 0; index != vec_pObjectsToDraw.size(); index++ )
	{
		if ( vec_pObjectsToDraw[index]->getUniqueID() == ID_to_find )
		{
			return vec_pObjectsToDraw[index];
		}
	}

	// Didn't find it.
	return NULL;	// 0 or nullptr
}



bool init_fmod() {

	//Create the main system object
	_result = FMOD::System_Create(&_system);
	//TODO: CHECK FOR FMOD ERRORS, IMPLEMENT YOUR OWN FUNCTION
	assert(!_result);
	//Initializes the system object, and the msound device. This has to be called at the start of the user's program
	_result = _system->init(512, FMOD_INIT_3D_RIGHTHANDED, NULL);
	assert(!_result);


	return true;
}

bool shutdown_fmod() {

	for (unsigned int i = 0; i < NUM_OF_SOUNDS; i++)
	{
		if (_sound[i]) {
			_result = _sound[i]->release();
			assert(!_result);
		}
	}


	if (_system) {
		_result = _system->close();
		assert(!_result);
		_result = _system->release();
		assert(!_result);
	}

	return true;
}



void LoadFromFile()
{
	std::string files[NUM_OF_SOUNDS];
	int count = 0;

	std::ifstream inputfile;
	inputfile.open(inputFile);
	if (!inputfile.is_open())			// More "c" or "C++" ish
	{
		std::cout << "Didn't open file" << std::endl;
	}
	while (!inputfile.eof())
	{

		inputfile >> files[count];
		std::cout << files[count] << std::endl;
		if (count < 3) {
			_result = _system->createSound(files[count].c_str(), FMOD_3D, 0, &_sound[count]);
			assert(!_result);
		}
		else 
		{
		_system->createStream(files[count].c_str(), FMOD_DEFAULT, 0, &_sound[count]);
		assert(!_result);
		}
		count++;
	}

}


void SetUpSound() 
{	
	//Master Group
	_result = _system->getMasterChannelGroup(&_channel_groups[0]);
	assert(!_result);

	//
	_result = _system->createChannelGroup("Group 3D Sound", &_channel_groups[1]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 1", &_channel_groups[2]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 2", &_channel_groups[3]);
	assert(!_result);
	_result = _system->createChannelGroup("Music Group 3", &_channel_groups[4]);
	assert(!_result);

	//Set groups children of master group.
	_result = _channel_groups[0]->addGroup(_channel_groups[1]);
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[2]);
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[3]); 
	assert(!_result);
	_result = _channel_groups[0]->addGroup(_channel_groups[4]);
	assert(!_result);


	//3d Sound 
	_result = _sound[0]->set3DMinMaxDistance(30.0f, 10000.0f);
	assert(!_result);
	_result = _sound[0]->setMode(FMOD_LOOP_NORMAL);
	assert(!_result);
	_result = _system->playSound(_sound[0], _channel_groups[1], true, &_channel[0]);
	assert(!_result);
	_channel_position1 = { 0.0f,0.0f,0.0f };
	_result = _channel[0]->set3DAttributes(&_channel_position1, &_channel_velocity);
	assert(!_result);
	//second
	_result = _sound[1]->set3DMinMaxDistance(30.0f, 100000.0f);
	assert(!_result);
	_result = _sound[1]->setMode(FMOD_LOOP_NORMAL);
	assert(!_result);
	_result = _system->playSound(_sound[1], _channel_groups[1], true, &_channel[1]);
	assert(!_result);
	_channel_position2 = { 0.0f,0.0f,0.0f };
	_result = _channel[1]->set3DAttributes(&_channel_position2, &_channel_velocity);
	assert(!_result);
	//Third
	_result = _sound[2]->set3DMinMaxDistance(30.0f, 100000.0f);
	assert(!_result);
	_result = _sound[2]->setMode(FMOD_LOOP_NORMAL);
	assert(!_result);
	_result = _system->playSound(_sound[2], _channel_groups[1], true, &_channel[2]);
	assert(!_result);
	_channel_position3 = { 0.0f,0.0f,0.0f };
	_result = _channel[2]->set3DAttributes(&_channel_position2, &_channel_velocity);
	assert(!_result);


	_result = _system->playSound(_sound[3], _channel_groups[2], true, &_channel[3]);
	assert(!_result);
	_result = _system->playSound(_sound[4], _channel_groups[2], true, &_channel[4]);
	assert(!_result);
	_result = _system->playSound(_sound[5], _channel_groups[2], true, &_channel[5]);
	assert(!_result);

	_result = _system->playSound(_sound[6], _channel_groups[3], true, &_channel[6]);
	assert(!_result);
	_result = _system->playSound(_sound[7], _channel_groups[3], true, &_channel[7]);
	assert(!_result);
	_result = _system->playSound(_sound[8], _channel_groups[3], true, &_channel[8]);
	assert(!_result);

	_result = _system->playSound(_sound[9], _channel_groups[4], true, &_channel[9]);
	assert(!_result);
	_result = _system->playSound(_sound[10], _channel_groups[4], true, &_channel[10]);
	assert(!_result);
	_result = _system->playSound(_sound[11], _channel_groups[4], true, &_channel[11]);
	assert(!_result);
	//Streaming Sounds
	//for (int i = 3; i < 12; i++) {
	//	if (i < 6) {
	//		_result = _system->playSound(_sound[i], _channel_groups[2], true, &_channel[i]);
	//		assert(!_result);
	//	}
	//	if (i > 6 || i < 9) {
	//		_result = _system->playSound(_sound[i], _channel_groups[3], true, &_channel[i]);
	//		assert(!_result);
	//	}
	//	if (i > 9) {
	//		_result = _system->playSound(_sound[i], _channel_groups[4], true, &_channel[i]);
	//		assert(!_result);
	//	}
	//}

	//TODO7: Create dsp echo
	_result = _system->createDSPByType(FMOD_DSP_TYPE_ECHO, &_dsp_echo);
	assert(!_result);

	_result = _channel_groups[1]->addDSP(0, _dsp_echo);
	assert(!_result);
	_result = _dsp_echo->setBypass(true);
	assert(!_result);
}

void UpdateSound() {
	//listener Position
	_listener_position = { g_CameraEye.x, g_CameraEye.y, g_CameraEye.z };
	glm::vec3 horizon = glm::normalize(glm::cross(cameraFront, cameraUp));
	glm::vec3 newup = glm::normalize(glm::cross(horizon, cameraFront));
	_up = { newup.x, newup.y, newup.z };
	_forward = { cameraFront.x, cameraFront.y, cameraFront.z };
	_result = _system->set3DListenerAttributes(0, &_listener_position, &_channel_velocity, &_forward, &_up);
	assert(!_result);



	//Sound 1 position
	cMeshObject* bonfire = findObjectByFriendlyName("bonfire");
	_channel_position1 = { bonfire->position.x, bonfire->position.y, bonfire->position.z };
	_result = _channel[0]->set3DAttributes(&_channel_position1, &_channel_velocity);

	//Sound 2 position
	cMeshObject* speaker = findObjectByFriendlyName("speaker");
	_channel_position2 = { speaker->position.x, speaker->position.y, speaker->position.z };
	_result = _channel[1]->set3DAttributes(&_channel_position2, &_channel_velocity);


	//Sound 3 position
	cMeshObject* wolf = findObjectByFriendlyName("wolf");
	_channel_position2 = { wolf->position.x, wolf->position.y, wolf->position.z };
	_result = _channel[2]->set3DAttributes(&_channel_position2, &_channel_velocity);


	_result = _system->update();
	assert(!_result);
}