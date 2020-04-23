in vec2 TexCoord;

uniform vec2 randPosition;

void main(void)
{

     vec3 pos;
    pos.xy = (TexCoord.st - vec2(0.5,0.5))*2.0;
    pos.z = 0.5;
    gl_FragData[0] = vec4(pos, 1.0);
    if (pos.xy == randPosition)
        pos.z = 1.0;
    gl_FragData[1] = vec4(pos, 1.0);
}