#version 410 core 

//in vec4 vs_color;
in VS_OUT
{
    vec4 color;
} fs_in;
                                                                  
out vec4 color;                                                   
                                                                  
void main(void)                                                   
{                                                                 
    //color = vec4(0.0, 0.8, 1.0, 1.0);   
    color = fs_in.color;                          
}                                                                 