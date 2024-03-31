#version 330 core



layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal; 
out vec3 FragPos;
uniform float offset;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * transform* vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model * transform))) * aNormal;  
    
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}