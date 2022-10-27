#version 330 core

out vec4 FragColor;

//通过反射率定义材质
struct Material{
    //使用漫反射纹理kd作为ka, 一帮情况下材质环境光颜色等于漫反射光
    sampler2D kd; //环境光贴图
    sampler2D ks; //镜面光贴图
    float shininess;
};
struct Light{
    vec3 ka;
    vec3 kd;
    vec3 ks;

    //点光源衰减参数
    float constant;
    float linear;
    float quadratic;

    //聚光参数
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};
//传入顶点坐标以及顶点法线
in vec3 FragPos;    //顶点通过model变换到世界坐标中的位置
in vec3 Normal;
in vec2 TexCoords;



uniform Material material;
uniform Light light;



//暂时使用(viewpos - FragPos)作为观察方向，不能表示摄像头旋转产生的高光
uniform vec3 viewPos;


void main()
{


    float distance = length(light.position - FragPos);
    float attenuation = 1/(light.constant + light.linear * distance + light.quadratic * distance * distance);

    //计算片段是否在聚光内,使用intensity表示
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(light.direction, -lightDir);
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0); //内锥之内为1，内外锥之间(0,1)，外锥之外为0


    //环境光
    vec3 ambient = light.ka * vec3(texture(material.kd, TexCoords));

    //漫反射光
    vec3 normal = normalize(Normal);
    vec3 kd = vec3(texture(material.kd, TexCoords));
    
    vec3 diffuse =  kd* light.kd * max(dot(lightDir, normal), 0.0);

    //高光项
    vec3 reflectDir = reflect(-lightDir, normal);//计算反射光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 ks = vec3(texture(material.ks, TexCoords));
    vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * ks * light.ks;
    
    //环境光只随距离变化，漫反射光和高光多一个内外锥之间的强度变化
    vec3 result = ((ambient + diffuse + specular) * intensity)* attenuation;

   
  

    FragColor = vec4(result, 1.0);
}