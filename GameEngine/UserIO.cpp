#include "globalOpenGLStuff.h"
#include "fmodStuff.h"
#include "globalStuff.h"
//#include <vector>
#include <iostream>

int index = 0;
// This has all the keyboard, mouse, and controller stuff
float lastX;
float lastY;
bool bIsPaused;
float pitch;
float yaw;
int grIndx = 1;
extern sLight* pTheOneLight;	//  = NULL;
extern cLightManager* LightManager;
int lightIndex = 0;
bool firstMouse = true;

bool bIsAnimation = false;
bool bMoveForward = true;

glm::vec3 Front;
glm::vec3 Horizontal;

void SwitchToWireFrame(std::vector<cMeshObject*> models);


void SwitchToSolid(std::vector<cMeshObject*> models);
void soundControl(int numb, GLFWwindow* window);

void key_callback(GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods)
{
	cMeshObject* Skull = findObjectByFriendlyName("skull");
	cMeshObject* bonfire = findObjectByFriendlyName("bonfire");
	cMeshObject* speaker = findObjectByFriendlyName("speaker");
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//SAVE MODELS
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		saveModelInfo("Models.txt", vec_pObjectsToDraw);
		saveLightInfo("lights.txt", LightManager->vecLights);
	}


	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		SwitchToWireFrame(vec_pObjectsToDraw);
	}


	if (glfwGetKey(window, GLFW_KEY_K))
	{
		SwitchToSolid(vec_pObjectsToDraw);
	}

	if (glfwGetKey(window, GLFW_KEY_L))
	{
		loadModels("Models.txt", vec_pObjectsToDraw);
		loadLights("lights.txt", LightManager->vecLights);
	}


	//Chose the model
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (index < (vec_pObjectsToDraw.size() - 1)) {

			index = index + 1;
		}
		else { index = 0; }
		std::cout << "Model " << vec_pObjectsToDraw.at(index)->friendlyName << " is Chosen" << std::endl;
	}


	//Chose the light
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (lightIndex < (LightManager->vecLights.size() - 1)) {

			lightIndex = lightIndex + 1;
		}
		else { lightIndex = 0; }
		std::cout << "Light " << LightManager->vecLights.at(lightIndex)->lightName << " is Chosen" << std::endl;
	}


	//TURN ON The Light

	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		LightManager->vecLights.at(lightIndex)->param2.x = 1.0f;
	}
	//TURN OFF The Light
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		LightManager->vecLights.at(lightIndex)->param2.x = 0.0f;
	}


	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (bIsPaused == false)
		{
			_result = _channel_groups[grIndx]->setPaused(true);
			bIsPaused = true;
			std::cout << "channel group " << grIndx << " paused" << std::endl;
		}
		else if (bIsPaused == true) 
		{
			_result = _channel_groups[grIndx]->setPaused(false); bIsPaused = false; 
			std::cout << "channel group " << grIndx << " unpaused" << std::endl;
		}
	}
	
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{

	}





	//TURN ON AND OFF SPHERES THAT SHOW LIGHT ATTENUATION

	if (glfwGetKey(window, GLFW_KEY_O))
	{
		LightManager->vecLights.at(lightIndex)->AtenSphere = true;
	}
	if (glfwGetKey(window, GLFW_KEY_P))
	{
		LightManager->vecLights.at(lightIndex)->AtenSphere = false;
	}





	if(glm::distance(g_CameraEye, vec_Sound_Obj.at(0)->position) < 200)
	{
		_result = _channel[0]->setPaused(false);
	}
	//else { _result = _channel[0]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(1)->position) < 400)
	{
		_result = _channel[1]->setPaused(false);
	}
	//else { _result = _channel[1]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(2)->position) < 400)
	{
		_result = _channel[2]->setPaused(false);
	}
	//else { _result = _channel[2]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(3)->position) < 300)
	{
		_result = _channel[3]->setPaused(false);
	}
	else { _result = _channel[3]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(4)->position) < 300)
	{
		_result = _channel[4]->setPaused(false);
	}
	else { _result = _channel[4]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(5)->position) < 300)
	{
		_result = _channel[5]->setPaused(false);
	}
	else { _result = _channel[5]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(6)->position) < 300)
	{
		_result = _channel[6]->setPaused(false);
	}
	else { _result = _channel[6]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(7)->position) < 300)
	{
		_result = _channel[7]->setPaused(false);
	}
	else { _result = _channel[7]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(8)->position) < 300)
	{
		_result = _channel[8]->setPaused(false);
	}
	else { _result = _channel[8]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(9)->position) < 300)
	{
		_result = _channel[9]->setPaused(false);
	}
	else { _result = _channel[9]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(10)->position) < 300)
	{
		_result = _channel[10]->setPaused(false);
	}
	else { _result = _channel[10]->setPaused(true); }

	if (glm::distance(g_CameraEye, vec_Sound_Obj.at(11)->position) < 300)
	{
		_result = _channel[11]->setPaused(false);
	}
	else { _result = _channel[11]->setPaused(true); }






	return;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = SCR_WIDTH / 2.0f;
		lastY = 20.0f;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;


	//to prevent camera reverse pitch
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front.y = sin(glm::radians(pitch));
	Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(Front);
	Horizontal = glm::normalize(glm::cross(Front, cameraUp));

}


bool IsShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool IsCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool IsAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool AreAllModifiersUp(GLFWwindow* window)
{
	if (IsShiftDown(window)) { return false; }
	if (IsCtrlDown(window)) { return false; }
	if (IsAltDown(window)) { return false; }
	return true;
}

void ProcessAsynKeys(GLFWwindow* window)
{
	const float CAMERA_SPEED_SLOW = 3.0f;
	const float CAMERA_SPEED_FAST = 10.0f;




	if (bIsAnimation)
	{

	}

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	// If no keys are down, move the camera
	if (AreAllModifiersUp(window))
	{
		// Note: The "== GLFW_PRESS" isn't really needed as it's actually "1" 
		// (so the if() treats the "1" as true...)

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			g_CameraEye.x += cameraSpeed * Front.x;
			g_CameraEye.z += cameraSpeed * Front.z;
			//g_CameraEye.x += Front.x * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)	// "backwards"
		{
			g_CameraEye.x -= cameraSpeed * Front.x;
			g_CameraEye.z -= cameraSpeed * Front.z;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)	// "left"
		{
			g_CameraEye -= Horizontal * cameraSpeed;
			//g_CameraEye.x -= cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)	// "right"
		{
			g_CameraEye += Horizontal * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)	// "up"
		{
			g_CameraEye.y += cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)	// "down"
		{
			g_CameraEye.y -= cameraSpeed;
		}



	}//if(AreAllModifiersUp(window)

	//const float MIN_LIGHT_BRIGHTNESS = 0.001f;

	//LIGHT CONTROL*********************************************************************************************************
	if (IsCtrlDown(window))
	{



		if (glfwGetKey(window, GLFW_KEY_W)) { LightManager->vecLights.at(lightIndex)->position.z += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { LightManager->vecLights.at(lightIndex)->position.z -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { LightManager->vecLights.at(lightIndex)->position.x -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { LightManager->vecLights.at(lightIndex)->position.x += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { LightManager->vecLights.at(lightIndex)->position.y += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { LightManager->vecLights.at(lightIndex)->position.y -= cameraSpeed; }


		//change colour of the light
		if (glfwGetKey(window, GLFW_KEY_Z)) { LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); }//Red
		if (glfwGetKey(window, GLFW_KEY_X)) { LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); }//Green
		if (glfwGetKey(window, GLFW_KEY_C)) { LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); }//Blue
		if (glfwGetKey(window, GLFW_KEY_V)) { LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); }//Yellow
		if (glfwGetKey(window, GLFW_KEY_B)) { LightManager->vecLights.at(lightIndex)->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); }//White


		//CHANGE LIGHTYPE
				//change colour of the light
		if (glfwGetKey(window, GLFW_KEY_1)) { LightManager->vecLights.at(lightIndex)->SetLightType(sLight::POINT_LIGHT); }//Red
		if (glfwGetKey(window, GLFW_KEY_2)) { LightManager->vecLights.at(lightIndex)->SetLightType(sLight::SPOT_LIGHT); }//Green

		//this->param1.y = innerAngleDegrees;
	//	this->param1.z = outerAngleDegrees;

		//Change SpotCone
		if (glfwGetKey(window, GLFW_KEY_T))
		{
			LightManager->vecLights.at(lightIndex)->param1.y -= 0.1f;

		}//Blue


		if (glfwGetKey(window, GLFW_KEY_Y))
		{
			LightManager->vecLights.at(lightIndex)->param1.y += 0.1f;

		}//Blue


		if (glfwGetKey(window, GLFW_KEY_U))
		{
			LightManager->vecLights.at(lightIndex)->param1.z -= 0.1f;
		}//Blue


		if (glfwGetKey(window, GLFW_KEY_I))
		{
			LightManager->vecLights.at(lightIndex)->param1.z += 0.1f;
		}//Blue

		if (glfwGetKey(window, GLFW_KEY_R)) { LightManager->vecLights.at(lightIndex)->SetRelativeDirectionByLookAt(vec_pObjectsToDraw.at(index)->position); }
	







		//change attenuation:

		//Linear
		if (glfwGetKey(window, GLFW_KEY_LEFT)) { LightManager->vecLights.at(lightIndex)->atten.y *= 1.05f; }
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) { LightManager->vecLights.at(lightIndex)->atten.y *= 0.95f; }

		//Quadratic
		if (glfwGetKey(window, GLFW_KEY_DOWN)) { LightManager->vecLights.at(lightIndex)->atten.z *= 0.95f; }
		if (glfwGetKey(window, GLFW_KEY_UP)) { LightManager->vecLights.at(lightIndex)->atten.z *= 1.05f; }


	}

	//Some Sound controls
	if(IsShiftDown(window))
	{
		if (glfwGetKey(window, GLFW_KEY_1))
		{ grIndx = 1;}//Red
		if (glfwGetKey(window, GLFW_KEY_2)) { grIndx = 2; }//Green
		if (glfwGetKey(window, GLFW_KEY_3)) { grIndx = 3; }//Green
		if (glfwGetKey(window, GLFW_KEY_4)) { grIndx = 4; }//Green
		soundControl(grIndx, window);

	}


	//OBJECT CONTROL***********************************************************
	if (IsAltDown(window))
	{	//Object Postiton
		if (glfwGetKey(window, GLFW_KEY_W)) { vec_pObjectsToDraw.at(index)->position.z -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_S)) { vec_pObjectsToDraw.at(index)->position.z += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_A)) { vec_pObjectsToDraw.at(index)->position.x -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_D)) { vec_pObjectsToDraw.at(index)->position.x += cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_Q)) { vec_pObjectsToDraw.at(index)->position.y -= cameraSpeed; }
		if (glfwGetKey(window, GLFW_KEY_E)) { vec_pObjectsToDraw.at(index)->position.y += cameraSpeed; }

		////Object Rotation
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) { vec_pObjectsToDraw.at(index)->adjMeshOrientationEulerAngles(0.0f, 0.1f, 0.0f, false); }
		if (glfwGetKey(window, GLFW_KEY_LEFT)) { vec_pObjectsToDraw.at(index)->adjMeshOrientationEulerAngles(0.0f, -0.1f, 0.0f, false); }


		if (glfwGetKey(window, GLFW_KEY_V)) { vec_pObjectsToDraw.at(index)->nonUniformScale += 0.01f; }
		if (glfwGetKey(window, GLFW_KEY_B)) { vec_pObjectsToDraw.at(index)->nonUniformScale -= 0.01f; }
		//Object Scale



	}

	return;
}




void SwitchToWireFrame(std::vector<cMeshObject*> models)
{
	for (std::vector<cMeshObject*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		cMeshObject* CurModel = *it;
		CurModel->bIsWireFrame = true;
	}
}

void SwitchToSolid(std::vector<cMeshObject*> models)
{
	for (std::vector<cMeshObject*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		cMeshObject* CurModel = *it;
		CurModel->bIsWireFrame = false;

	}
}


void soundControl(int numb, GLFWwindow* window)
{
	if (_channel_groups[numb])
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER))
		{
			_channel_groups[numb]->getPaused(&bIsPaused);
			if(bIsPaused == true){
			_result = _channel_groups[numb]->setPaused(true);
			}
			if (bIsPaused == false) {
				_result = _channel_groups[numb]->setPaused(false);
			}
		}
		//Volume
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			//_result = _channel[numb]->getVolume(&_channel_volume);
			//assert(!_result);
			_channel_volume = (_channel_volume >= 1.0f) ? 1.0f : (_channel_volume + 0.05f);

			_result = _channel_groups[numb]->setVolume(_channel_volume);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			//_result = _channel[numb]->getVolume(&_channel_volume);
			//assert(!_result);
			_channel_volume = (_channel_volume <= 0.0f) ? 0.01f : (_channel_volume - 0.05f);
			_result = _channel_groups[numb]->setVolume(_channel_volume);
			assert(!_result);
		}
		//PAN
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			_channel_pan = (_channel_pan <= -1.0f) ? -1.0f : (_channel_pan - 0.01f);
			_result = _channel_groups[numb]->setPan(_channel_pan);
			assert(!_result);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			_channel_pan = (_channel_pan >= 1.0f) ? 1.0f : (_channel_pan + 0.01f);
			_result = _channel_groups[numb]->setPan(_channel_pan);
			assert(!_result);
		}

		//Pitch
		if (glfwGetKey(window, GLFW_KEY_Z))
		{


			_channel_pitch = (_channel_pitch <= 0.0f) ? 0.01f : (_channel_pitch - 0.00005f);
			_result = _channel_groups[numb]->setPitch((_channel_pitch));
			assert(!_result);
		}
		if (glfwGetKey(window, GLFW_KEY_X))
		{
			_channel_pitch = (_channel_pitch >= 2.0f) ? 2.0f : (_channel_pitch + 0.00005f);
			_result = _channel_groups[numb]->setPitch((_channel_pitch));
			assert(!_result);
		}

		//DSP EFFECTS
		if (glfwGetKey(window, GLFW_KEY_6))
		{
			//_result = _channel_groups[numb]->addDSP();
			//assert(!_result);
		}

	}
}

