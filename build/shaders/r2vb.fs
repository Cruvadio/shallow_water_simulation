//uniform float scalefactor = 0.0;
//uniform float invtexsize;
in vec2 TexCoord;



void main(void)
{
  /*vec4 h = texture2D(HeightMap, TexCoord);
  vec4 h2 = texture2D(HeightMap, vec2(TexCoord.s,TexCoord.t+invtexsize)); 
  vec4 h3 = texture2D(HeightMap, TexCoord+vec2(invtexsize,invtexsize)); 
       h.xy=vec2(0.0,0.0);
       h2.xy=vec2(0.0,1.0);
       h3.xy=vec2(1.0,1.0);
  vec4 d1, d2;
       d1=h2-h;
       d2=h3-h;
  vec3 n = cross(d1.xyz,d2.xyz);         
  vec3 pos;
       pos.xy = (TexCoord.st - vec2(0.5,0.5))*2.0;
       pos.z = h.z*scalefactor;
  gl_FragData[0] = vec4(pos,1.0);
  gl_FragData[1] = vec4(-n,1.0);
  gl_FragData[2] = vec4(TexCoord,0.0, 0.0);*/
     vec3 pos;
    pos.xy = (TexCoord.st - vec2(0.5,0.5))*2.0;
    pos.z = 0.;
    gl_FragData[0] = vec4(pos, 1.0);
    gl_FragData[1] = vec4(0.0, 0.0, 1.0, 1.0);
    gl_FragData[2] = vec4(TexCoord,0.0, 1.0);

}