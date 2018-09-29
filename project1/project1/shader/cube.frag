#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
	vec3 aNormal;
} fs_in;

 
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform sampler2D diffuseMapSampler;
uniform sampler2D normalMapSampler;

void main()
{
	//�����������tangentƽ������ϵ��ļ���
	// ��ȡ����ͷ�����
	vec3 diffuseColor = texture(diffuseMapSampler, fs_in.TexCoords).rgb;
	vec3 tangentNormal = texture(normalMapSampler, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1],this normal is in tangent space
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);

	// ����tangentLightDir��tangentViewDir 
	vec3 tangentLightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 tangentViewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	
    // ������
    vec3 ambient = light.ambient;
  	
    // ������ 
    float diff = max(dot(tangentLightDir, tangentNormal), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;
	
	//debug
	//vec3 diffuse = light.diffuse * diffuseColor;


	//debug
	//mat3 TBN = mat3(fs_in.VT, fs_in.VB, fs_in.VN);    
	//vec3 color = TBN * tangentNormal;

	//vec3 diffuse = light.diffuse  * color;

	
    // ���淴��
    float specularStrength = 0.6;
    

	// ��ʽ1��ֱ�����䷽��
    //vec3 reflectDir = reflect(-tangentLightDir, tangentNormal);
    //float spec = pow(max(dot(tangentViewDir, reflectDir), 0.0), 128);
    //vec3 specular = specularStrength * spec *  light.specular * diffuseColor;  
    
	// ��ʽ2,��L��V���м�����H
	vec3 hDir = normalize(tangentLightDir + tangentViewDir);
	float spec = pow(max(dot(hDir, tangentNormal), 0.0), 128);
    vec3 specular = specularStrength * spec *  light.specular * diffuseColor;
	
    FragColor = vec4(ambient + diffuse + specular, 1.0);
} 

