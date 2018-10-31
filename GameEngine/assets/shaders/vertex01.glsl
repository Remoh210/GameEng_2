#version 420 
// vertex01.glsl

//uniform mat4 MVP;		
uniform mat4 matModel;		// M
uniform mat4 matModelInvTrans;	// inverse(transpose(matModel))
uniform mat4 matView;		// V
uniform mat4 matProj;		// P

in vec3 vColour;		// rgb   	was "attribute"
in vec3 vPosition;		// xyz		was "attribute"
in vec3 vNormal;		// normal xyz

out vec3 color;			// exit to fragment
out vec4 vertPosWorld;	// "World space"
out vec3 vertNormal;	// "Model space"

void main()
{
	vec3 posTemp = vPosition;
	
	// Note these are 'backwards'
	mat4 MVP = matProj * matView * matModel;
    gl_Position = MVP * vec4(posTemp, 1.0);			// ON SCREEN
	
	vertPosWorld = matModel * vec4(posTemp, 1.0);
	
	// Transforms the normal into "world space"
	// Remove all scaling and transformation from model
	// Leaving ONLY rotation... 
	vertNormal = vec3( matModelInvTrans * vec4(normalize(vNormal),1.0f) );
	
    color = vColour;
}



