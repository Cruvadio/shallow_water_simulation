uniform sample2D texture;

uniform float width;
uniform float height;

in vec2 TexCoord;

void main(void)
{

     vec4 info = texture2D(texture, TexCoord);
     vec2 delta(1/width, 1/height);
     vec3 dx = vec3(delta.x, 0.0, texture2D(texture, vec2(TexCoord.x + delta.x, TexCoord.y)).b - info.b);
     vec3 dy = vec3(0.0, delta.y, texture2D(texture, vec2(TexCoord.x, TexCoord.y + delta.y)).b - info.b);
     
     
     info.rgb = normalize(cross(dx, dy));

    gl_FragData[0] = info;
}