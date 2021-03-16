#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	vec3 lightPos;
	vec3 viewPos;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 normal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 lightPosition;
layout(location = 3) out vec3 CameraPositon;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    normal = inNormal;
	lightPosition = ubo.lightPos;
	CameraPositon = ubo.viewPos;
    fragTexCoord = inTexCoord;
}