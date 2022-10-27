#version 330 core

out vec4 FragColor;

//ͨ�������ʶ������
struct Material{
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ka;
    vec3 kd;
    vec3 ks;
};

//���붥�������Լ����㷨��
in vec3 FragPos;    //����ͨ��model�任�����������е�λ��
in vec3 Normal;



uniform Material material;
uniform Light light;


//��ʱʹ��(viewpos - FragPos)��Ϊ�۲췽�򣬲��ܱ�ʾ����ͷ��ת�����ĸ߹�
uniform vec3 viewPos;


void main()
{
    //����ݼ���������, ��Ҫ�ȱ������ŵ���0��1��֮��
    //float decArg = 1/pow(dot(viewPos - FragPos),2);


    //������
    vec3 ambient = light.ka * material.ka;
    //�������
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = material.kd * light.kd * max(dot(lightDir, normal), 0.0);

    //�߹���
    vec3 reflectDir = reflect(-lightDir, normal);//���㷴���
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.ks * light.ks;
    
    vec3 result = (ambient + diffuse + specular);
  

    FragColor = vec4(result, 1.0);
}