#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
	float ambval;
	float powval;
} ubo;
layout(binding = 1)uniform sampler2D texSampler;
layout(binding = 2)uniform BlinnPhongObject{
	bool Blinn;
	vec3 lightPosition;
	vec3 CameraPositon;
}bpo;

layout(location = 0) in vec3 normal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;


layout(location = 0) out vec4 outColor;

void main() {
	vec3 Color = texture(texSampler, fragTexCoord).rgb;
	//ambient
	vec3 ambient = ubo.ambval * Color;
	//diffuse
	vec3 lightDir = normalize(bpo.lightPosition - fragPos);
	vec3 normal = normalize(normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * Color;
	//specular
	vec3 viewDir = normalize(bpo.CameraPositon - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir),0.0), ubo.powval);
	vec3 specular = vec3(0.3) * spec;
	
	if(bpo.Blinn)
	{
		outColor = vec4(ambient + diffuse + specular, 1.0);
	}
	else
	{
		outColor = texture(texSampler, fragTexCoord);
	}

}