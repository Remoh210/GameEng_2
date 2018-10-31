#include "globalOpenGLStuff.h"
#include "globalStuff.h"		// for g_pRogerRabbit

#include "cVAOMeshManager.h"
#include "cMeshObject.h"


#include <iostream>

// Loading models was moved into this function
void LoadModelTypes( cVAOMeshManager* pTheVAOMeshManager, GLuint shaderProgramID )
{

	sModelDrawInfo tableInfo;
	tableInfo.meshFileName = "table.ply";					// "mig29_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(tableInfo, shaderProgramID);

	sModelDrawInfo torchInfo;
	torchInfo.meshFileName = "torch.ply";					// "mig29_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(torchInfo, shaderProgramID);

	sModelDrawInfo teapotInfo;
	teapotInfo.meshFileName = "Utah_Teapot_xyz_n.ply";			// "Utah_Teapot_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(teapotInfo, shaderProgramID);

	sModelDrawInfo terrainInfo;
	terrainInfo.meshFileName = "MeshLab_Fractal_Terrain_xyz_n.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	terrainInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(terrainInfo, shaderProgramID);
	
	sModelDrawInfo bluntInfo;
	bluntInfo.meshFileName = "blunt.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	bluntInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(bluntInfo, shaderProgramID);

	sModelDrawInfo chest_bottomInfo;
	chest_bottomInfo.meshFileName = "chest_bottom.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	chest_bottomInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(chest_bottomInfo, shaderProgramID);

	sModelDrawInfo chest_topInfo;
	chest_topInfo.meshFileName = "chest_top.ply";	
	chest_topInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(chest_topInfo, shaderProgramID);

	sModelDrawInfo sphereInfo;
	sphereInfo.meshFileName = "Sphere_320_faces_xyz_n.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(sphereInfo, shaderProgramID);

	sModelDrawInfo roomInfo;
	roomInfo.meshFileName = "room.ply";			// "Sphere_320_faces_xyz.ply";
	pTheVAOMeshManager->LoadModelIntoVAO(roomInfo, shaderProgramID);

	sModelDrawInfo skullInfo;
	skullInfo.meshFileName = "skull.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	skullInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(skullInfo, shaderProgramID);

	sModelDrawInfo spiderInfo;
	spiderInfo.meshFileName = "spider.ply";	// "MeshLab_Fractal_Terrain_xyz.ply";
	// Will alow me to update the vertex data in the mesh
	spiderInfo.bVertexBufferIsDynamic = true;
	pTheVAOMeshManager->LoadModelIntoVAO(spiderInfo, shaderProgramID);

	// At this point, mesh in in GPU
	std::cout << "Mesh was loaded OK" << std::endl;

	return;
}

	
// Loads the models we are drawing into the vector
void LoadModelsIntoScene( std::vector<cMeshObject*> &vec_pObjectsToDraw )
{

	{
		cMeshObject* pTeapot = new cMeshObject();
		pTeapot->position = glm::vec3( -90.0f, 60.0f, 0.0f );
	//	pTeapot->objColour = glm::vec3( 0.2f, 0.8f, 0.3f );
	//	pTeapot->postRotation = glm::vec3(glm::radians(-20.0f), 0.0f, 0.0f);
		pTeapot->friendlyName = "teapot";
		pTeapot->meshName = "Utah_Teapot_xyz_n.ply";		
		pTeapot->setUniformScale( 0.4f );
		vec_pObjectsToDraw.push_back( pTeapot );
	}

	{
		cMeshObject* pSkull = new cMeshObject();
		pSkull->position = glm::vec3(0.0f, 50.0f, 0.0f);
		//pSkull->setSpecularColour(glm::vec3(1.0f, 0.4f, 0.1f));
		pSkull->setDiffuseColour(glm::vec3(1.0f, 0.4f, 0.1f));
		pSkull->setSpecularPower(100.0f);
		pSkull->friendlyName = "skull";
	//	pSkull->bIsInteractable = true;
		pSkull->meshName = "skull.ply";		
		pSkull->setUniformScale(1.5f);
		vec_pObjectsToDraw.push_back(pSkull);
	}


	for (int countsp = 0; countsp < 3; countsp++) {

		cMeshObject* pSpider = new cMeshObject();
		pSpider->position = glm::vec3(80.0f, 50.0f, 0.0f);
		//pSpider->postRotation = glm::vec3(0.0f, glm::radians(-20.0f), 0.0f);
	//	pSpider->objColour = glm::vec3(0.24f, 0.56f, 1.0f);
		pSpider->friendlyName = "spider" + std::to_string(countsp);
		pSpider->meshName = "spider.ply";
		pSpider->setUniformScale(0.2f);
		vec_pObjectsToDraw.push_back(pSpider);
	}



	{
		cMeshObject* pRoom = new cMeshObject();
		pRoom->position = glm::vec3(0.0f, 0.0f, 300.0f);
		//pSpider->postRotation = glm::vec3(0.0f, glm::radians(-20.0f), 0.0f);
	//	pRoom->objColour = glm::vec3(0.74f, 0.86f, 1.0f);
		pRoom->friendlyName = "room";
		pRoom->meshName = "room.ply";
		pRoom->setUniformScale(50.0f);
		pRoom->bIsVisible = true;
		vec_pObjectsToDraw.push_back(pRoom);
	}

	for (int count = 0; count < 4; count++)
	{
		cMeshObject* pBlunt = new cMeshObject();
		pBlunt->position = glm::vec3(0.0f, 0.0f, 300.0f);
	//	pBlunt->postRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	//	pBlunt->objColour = glm::vec3(0.74f, 0.86f, 1.0f);
		pBlunt->friendlyName = "blunt" + std::to_string(count);
		pBlunt->meshName = "blunt.ply";
		pBlunt->setUniformScale(1.5f);
		pBlunt->bIsVisible = true;
		vec_pObjectsToDraw.push_back(pBlunt);
	}

	{
		cMeshObject* pTable = new cMeshObject();
		pTable->position = glm::vec3(0.0f, 0.0f, 0.0f);
	//	pTable->objColour = glm::vec3(85 / 255.0f, 33 / 255.0f, 18 / 255.0f);
		//pTable->setUniformScale(0.9f);
		pTable->friendlyName = "table";
		pTable->meshName = "table.ply";
		vec_pObjectsToDraw.push_back(pTable);
	}



	{
		cMeshObject* pChest_b = new cMeshObject();
		pChest_b->position = glm::vec3(-675.0f, -20.0f, 520.0f);
	//	pChest_b->objColour = glm::vec3(85 / 255.0f, 33 / 255.0f, 18 / 255.0f);
		//pChest_b->setUniformScale(0.9f);
		pChest_b->friendlyName = "chest_bottom";
		pChest_b->meshName = "chest_bottom.ply";
		vec_pObjectsToDraw.push_back(pChest_b);
	}

	{
		cMeshObject* pChest_top = new cMeshObject();
		pChest_top->position = glm::vec3(-740.0f, 30.0f, 505.0f);
	//	pChest_top->objColour = glm::vec3(85 / 255.0f, 33 / 255.0f, 18 / 255.0f);
		//pChest_top_b->setUniformScale(0.9f);
		pChest_top->friendlyName = "chest_top";
		pChest_top->meshName = "chest_top.ply";
		vec_pObjectsToDraw.push_back(pChest_top);
	}

	for(int torch_count = 0; torch_count < 4; torch_count++)
	{
		cMeshObject* pTorch = new cMeshObject();
		pTorch->position = glm::vec3(torch_count * 10.0f, 0.0f, 300.0f);
	//	pTorch->postRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	//	pTorch->objColour = glm::vec3(0.74f, 0.86f, 1.0f);
		pTorch->friendlyName = "torch" + std::to_string(torch_count);
		pTorch->meshName = "torch.ply";
		pTorch->setUniformScale(1.0f);
		pTorch->bIsVisible = true;
		vec_pObjectsToDraw.push_back(pTorch);
	}




	//cMeshObject* pRogerRabbit = NULL;





	{	// This sphere is the tiny little debug sphere
		cMeshObject* pDebugSphere = new cMeshObject();
		pDebugSphere->position = glm::vec3(0.0f, 0.0f, 0.0f);
	//	pDebugSphere->objColour = glm::vec3(0.0f, 1.0f, 0.0f);
		float scale = 30.0f;
		pDebugSphere->friendlyName = "DebugSphere";
		pDebugSphere->nonUniformScale = glm::vec3(scale, scale, scale);
		pDebugSphere->meshName = "Sphere_320_faces_xyz_n.ply";		
		pDebugSphere->bIsWireFrame = true;
		pDebugSphere->bIsVisible = false;

		vec_pObjectsToDraw.push_back(pDebugSphere);
	}


	// ENDOF: Updated physics object





	return;
}