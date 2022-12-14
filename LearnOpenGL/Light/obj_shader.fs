#version 330 core

out vec4 FragColor;

//通过反射率定义材质
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

//传入顶点坐标以及顶点法线
in vec3 FragPos;    //顶点通过model变换到世界坐标中的位置
in vec3 Normal;



uniform Material material;
uniform Light light;


//暂时使用(viewpos - FragPos)作为观察方向，不能表示摄像头旋转产生的高光
uniform vec3 viewPos;


void main()
{
    //距离递减参数参数, 需要等比例缩放到（0，1）之间
    //float decArg = 1/pow(dot(viewPos - FragPos),2);


    //环境光
    vec3 ambient = light.ka * material.ka;
    //漫反射光
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = material.kd * light.kd * max(dot(lightDir, normal), 0.0);

    //高光项
    vec3 reflectDir = reflect(-lightDir, normal);//计算反射光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.ks * light.ks;
    
    vec3 result = (ambient + diffuse + specular);
  

    FragColor = vec4(result, 1.0);
}