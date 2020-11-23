#version 330

// vertex shader input  
in vec3 passUVs;						//< frag Uv's
in vec3 passNormal;						//< frag normal in world space
in vec3 passPosition;					//< frag world space position 

// uniform inputs
uniform sampler2D inModelTexture;		//< Model Texture

// output
out vec4 out_Color;

void main() 
{
	// Get the color and alpha from the texture
	vec3  model_color = texture(inModelTexture, passUVs.xy).rgb;
	float model_alpha = texture(inModelTexture, passUVs.xy).a;

	// Set fragment color output
	out_Color =  vec4(model_color, model_alpha);
}
