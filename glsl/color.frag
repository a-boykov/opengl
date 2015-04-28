
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float time;

void main()
{
	vec4 color1 = texture2D(tex1,gl_TexCoord[0].st);
	vec4 color2 = texture2D(tex2,gl_TexCoord[1].st);
	gl_FragColor = time*color1 + (1-time)*color2;
}
