//uniform float scalefactor = 0.0;
//uniform float invtexsize;
uniform sample2D Hprev;
uniform sample2D Hcurr;

uniform float width;
uniform float height;
uniform float omega;

in vec2 TexCoord;

void main(void)
{

     vec4 info = texture2D(Hcurr, TexCoord);
     vec2 delta(1/width, 1/height);
     vec2 dx = vec2(delta.x, 0);
     vec2 dy = vec2(0, delta.y);

     float average = (
        texture2D(Hcurr, TexCoord - dx).b +
        texture2D(Hcurr, TexCoord - dy).b +
        texture2D(Hcurr, TexCoord + dx).b +
        texture2D(Hcurr, TexCoord + dy).b
      ) * 0.25;
     average += (1 - omega) * texture2D(Hprev, TexCoord).b;
     info.a += (average - info.b) * 2.0;

     info.a *= 0.995;

     info.b += info.a;

     vec3 pos;
    pos.xy = (TexCoord.st - vec2(0.5,0.5))*2.0;
    gl_FragData[0] = vec4(pos.xy, info.ba);
}