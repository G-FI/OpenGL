#version 330 core

out vec4 FragColor;

//ͨ�������ʶ������
struct Material{
    //ʹ������������kd��Ϊka, һ������²��ʻ�������ɫ�����������
    sampler2D kd; //��������ͼ
    sampler2D ks; //�������ͼ
    float shininess;
};

struct DirLight{
    vec3 direction;
    vec3 ka;
    vec3 kd;
    vec3 ks;
};
struct PointLight{
    vec3 position;
    vec3 ka;
    vec3 kd;
    vec3 ks;

    //���Դ˥������
    float constant;
    float linear;
    float quadratic; 
};
struct SpotLight{
    vec3 ka;
    vec3 kd;
    vec3 ks;

    //���Դ˥������
    float constant;
    float linear;
    float quadratic;

    //�۹����
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//���붥�������Լ����㷨��
in vec3 FragPos;    //����ͨ��model�任�����������е�λ��,���������㶥�㵽��Դ�ķ����Լ����룬ƽ�й��ò���
in vec3 Normal;
in vec2 TexCoords;



uniform Material material;
uniform DirLight dirLight;
uniform SpotLight spotLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];




//��ʱʹ��(viewpos - FragPos)��Ϊ�۲췽�򣬲��ܱ�ʾ����ͷ��ת�����ĸ߹�
uniform vec3 viewPos;


void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 normal = normalize(Normal);
    vec3 result = vec3(0.0,0.0,0.0);

    result += CalDirLight(dirLight, normal, viewDir);
    result += CalSpotLight(spotLight, normal, FragPos, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalPointLight(pointLights[i], normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalDirLight(DirLight light, vec3 normal,vec3 viewDir)
{
    //���߷�������, �Ӷ���ָ���Դ
    vec3 lightDir = normalize(-light.direction);

    //������
    vec3 ambient = light.ka * vec3(texture(material.kd, TexCoords));

    //�������
    vec3 kd = vec3(texture(material.kd, TexCoords));  
    vec3 diffuse =  kd* light.kd * max(dot(lightDir, normal), 0.0);

    //�߹���
    vec3 reflectDir = reflect(-lightDir, normal);//���㷴���
    vec3 ks = vec3(texture(material.ks, TexCoords));
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * ks * light.ks;
    
    //������ֻ�����仯���������͸߹��һ������׶֮���ǿ�ȱ仯
    vec3 result = (ambient + diffuse + specular);
    return result;  
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1/(light.constant + light.linear * distance + light.quadratic * distance * distance);
    
    vec3 lightDir = normalize(light.position - fragPos);


    //������
    vec3 ambient = light.ka * vec3(texture(material.kd, TexCoords));

    //�������
    vec3 kd = vec3(texture(material.kd, TexCoords));  
    vec3 diffuse =  kd* light.kd * max(dot(lightDir, normal), 0.0);

    //�߹���
    vec3 reflectDir = reflect(-lightDir, normal);//���㷴���
    vec3 ks = vec3(texture(material.ks, TexCoords));
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * ks * light.ks;
    
    //������ֻ�����仯���������͸߹��һ������׶֮���ǿ�ȱ仯
    vec3 result = (ambient + diffuse + specular) * attenuation;
    return result;  
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    //����Ƭ���Ƿ��ھ۹���,ʹ��intensity��ʾ
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(light.direction, -lightDir);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0); //��׶֮��Ϊ1������׶֮��(0,1)����׶֮��Ϊ0


    //������
    vec3 ambient = light.ka * vec3(texture(material.kd, TexCoords));

    //�������
    vec3 kd = vec3(texture(material.kd, TexCoords));  
    vec3 diffuse =  kd* light.kd * max(dot(lightDir, normal), 0.0);

    //�߹���
    vec3 reflectDir = reflect(-lightDir, normal);//���㷴���
    vec3 ks = vec3(texture(material.ks, TexCoords));
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * ks * light.ks;
    
    //������ֻ�����仯���������͸߹��һ������׶֮���ǿ�ȱ仯
    vec3 result = (ambient + diffuse + specular) * intensity* attenuation;
    return result;  
}