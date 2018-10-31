//DrawOjbect_calls.cpp
#include "globalOpenGLStuff.h"		// For GLFW and glad (OpenGL calls)
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "cMeshObject.h"


void DrawObject(cMeshObject* pCurrentMesh,
	glm::mat4x4 &matModel,
	GLuint shaderProgramID)
{

	// Is this object visible
	if (!pCurrentMesh->bIsVisible)
	{
		return;
	}

	//************************************
	matModel = glm::mat4x4(1.0f);		// mat4x4_identity(m);


//glm::mat4 preRot_X = glm::rotate( glm::mat4(1.0f), 
//									pCurrentMesh->preRotation.x, 
//									glm::vec3( 1.0f, 0.0, 0.0f ) );
//matModel = matModel * preRot_X;
//
//// Calculate some rotation matrix values;
//glm::mat4 preRot_Y = glm::rotate( glm::mat4(1.0f), 
//									pCurrentMesh->preRotation.y, 
//									glm::vec3( 0.0f, 1.0, 0.0f ) );
//matModel = matModel * preRot_Y;
//
//
//glm::mat4 preRot_Z = glm::rotate( glm::mat4(1.0f), 
//									pCurrentMesh->preRotation.z, 
//									glm::vec3( 0.0f, 0.0, 1.0f ) );
//matModel = matModel * preRot_Z;

	////mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	//glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f), 
	//								 (float) glfwGetTime(), 
	//								 glm::vec3( 0.0f, 0.0, 1.0f ) );
	//m = m * rotateZ;

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
		pCurrentMesh->position);
	matModel = matModel * matTranslation;		// matMove

	glm::quat qRotation = pCurrentMesh->getQOrientation();
	// Generate the 4x4 matrix for that
	glm::mat4 matQrotation = glm::mat4(qRotation);

	matModel = matModel * matQrotation;

	//glm::mat4 postRot_X = glm::rotate( glm::mat4(1.0f), 
	//									pCurrentMesh->postRotation.x, 
	//									glm::vec3( 1.0f, 0.0, 0.0f ) );
	//matModel = matModel * postRot_X;
	//
	//glm::mat4 postRot_Y = glm::rotate( glm::mat4(1.0f), 
	//									pCurrentMesh->postRotation.y, 
	//									glm::vec3( 0.0f, 1.0, 0.0f ) );
	//matModel = matModel * postRot_Y;
	//
	//glm::mat4 postRot_Z = glm::rotate( glm::mat4(1.0f), 
	//									pCurrentMesh->postRotation.z, 
	//									glm::vec3( 0.0f, 0.0, 1.0f ) );
	//matModel = matModel * postRot_Z;

		// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));

	// And now scale

	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		pCurrentMesh->nonUniformScale);
	matModel = matModel * matScale;


	//************************************

		//mat4x4_mul(mvp, p, m);
		//mvp = p * view * m; 

	glUseProgram(shaderProgramID);

	// HACK: We wil deal with these uniform issues later...

	// Loading the uniform variables here (rather than the inner draw loop)
//	GLint objectColour_UniLoc = glGetUniformLocation( shaderProgramID, "objectColour" );
	GLint objectDiffuse_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	GLint objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");

	//uniform vec3 lightPos;
	//uniform float lightAtten;
	GLint lightPos_UniLoc = glGetUniformLocation(shaderProgramID, "lightPos");
	GLint lightBrightness_UniLoc = glGetUniformLocation(shaderProgramID, "lightBrightness");
	GLint useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");

	//	// uniform mat4 MVP;	THIS ONE IS NO LONGER USED	
	//uniform mat4 matModel;	// M
	//uniform mat4 matView;		// V
	//uniform mat4 matProj;		// P
	//GLint mvp_location = glGetUniformLocation(program, "MVP");
	GLint matModel_location = glGetUniformLocation(shaderProgramID, "matModel");
	GLint matModelInvTrans_location = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	GLint matView_location = glGetUniformLocation(shaderProgramID, "matView");
	GLint matProj_location = glGetUniformLocation(shaderProgramID, "matProj");

	GLint bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");

	glUniformMatrix4fv(matModel_location, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(matModelInvTrans_location, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));


	// Set the object to "wireframe"
//			glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );	//GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//GL_FILL

	//GLint objectColour_UniLoc 
	//	= glGetUniformLocation( program, "objectColour" );

	//glUniform3f( objectColour_UniLoc, 
	//				pCurrentMesh->objColour.r, 
	//				pCurrentMesh->objColour.g, 
	//				pCurrentMesh->objColour.b ); 
	glUniform4f(objectDiffuse_UniLoc,
		pCurrentMesh->materialDiffuse.r,
		pCurrentMesh->materialDiffuse.g,
		pCurrentMesh->materialDiffuse.b,
		pCurrentMesh->materialDiffuse.a);
	glUniform4f(objectSpecular_UniLoc,
		pCurrentMesh->materialSpecular.r,
		pCurrentMesh->materialSpecular.g,
		pCurrentMesh->materialSpecular.b,
		pCurrentMesh->materialSpecular.a);

	if (pCurrentMesh->bUseVertexColour)
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (pCurrentMesh->bDontLight)
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(bDontUseLighting_UniLoc, (float)GL_FALSE);
	}

	if (pCurrentMesh->bIsWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}


	sModelDrawInfo modelInfo;
	modelInfo.meshFileName = pCurrentMesh->meshName;

	if (g_pTheVAOMeshManager->FindDrawInfoByModelName(modelInfo))
	{
		//glDrawArrays(GL_TRIANGLES, 0, bunnyInfo.numberOfIndices );

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

	return;
}//void DrawObject(void)
