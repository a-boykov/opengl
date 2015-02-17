
varying vec2 position;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D mask;
uniform float time;
uniform float maskWidth;
uniform float maskHeight;


void main()
{
//	newCoords.x = 0.5;
//	newCoords.y = 0.5;
	vec4 color1 = texture2D(tex1,gl_TexCoord[0].st);
	vec4 color2 = texture2D(tex2,gl_TexCoord[1].st);
	vec4 maskColor = texture2D(mask,gl_TexCoord[2].st);

	float beginX = (1 + maskWidth)*time - maskWidth;
	float endX = beginX + maskWidth;
	if(beginX < 0)
	    beginX = 0;
	if(endX > 1)
	    endX = 1;
	    
	float beginY = 0 ;
	float endY = 1.0;

	
	if(position.x >  beginX && position.x < endX && position.y >  beginY && position.y < endY)
	{
	  gl_FragColor = color2*maskColor.w + color1*(1-maskColor.w);
	}
	  
	else
	  gl_FragColor = color1;
	  

}
