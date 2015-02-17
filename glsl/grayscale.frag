uniform sampler2D tex1;
uniform float time;

void main()
{
	vec4 color = texture2D(tex1, gl_TexCoord[0].st);
	//float color2 = (color.r*0.21+color.g*0.72+color.b*0.07);
	float color_red = mix( color.r, color.r*0.21, clamp(time, 0.0f, 1.0f) );
	float color_green = mix( color.g, color.g*0.72, clamp(time, 0.0f, 1.0f) );
	float color_blue = mix( color.b, color.r*0.07, clamp(time, 0.0f, 1.0f) );

	gl_FragColor= vec4(color_red,color_green,color_blue, color.a);
}
