#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec3 lightPos;
	vec3 viewPos;
} ubo;
layout(binding = 1)uniform sampler2D texSampler;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 lightPosition;
layout(location = 3) in vec3 CameraPositon;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragTexCoord);
}