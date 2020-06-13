#version 410 core
in vec3 normal;
in vec2 uv;

uniform vec3 colour;
uniform vec3 lightPos;
uniform vec3 ambient;

//uniform sampler2D tex;

out vec4 fragColour;

void main()
{
    float diffuseFactor=max(dot(normal, lightPos), 0);
    //vec4 texColour = texture(tex, uv);
    //vec3 ambient = colour.rgb * 0.8;
    fragColour = vec4(ambient + colour * diffuseFactor * 0.5, 1);
}

