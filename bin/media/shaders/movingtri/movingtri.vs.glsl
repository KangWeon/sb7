#version 430 core                                                 
                                                                  
layout (location = 0) in vec4 offset; 
layout (location = 1) in vec4 color;

//out vec4 vs_color;
out VS_OUT 
{
    vec4 color;
} vs_out;
                                                                  
void main(void)                                                   
{                                                                 
    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  
                                   vec4(-0.25, -0.25, 0.5, 1.0),  
                                   vec4( 0.25,  0.25, 0.5, 1.0)); 
                                                                  
    // Add 'offset' to our hard-coded vertex position             
    gl_Position = vertices[gl_VertexID] + offset;

    vs_out.color = color;                 
}                                                                 