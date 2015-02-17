
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
	vec4 color1 = texture2D(tex1,gl_TexCoord[2].st);
	vec4 color2 = texture2D(tex2,gl_TexCoord[1].st);
	vec4 maskColor = texture2D(mask,gl_TexCoord[2].st);

	float beginX = -1.0  + (2.0 - maskWidth*2)*time;
	float endX = beginX + 2*maskWidth;
	if(beginX < -1)
	    beginX = -1;
	if(endX > 1)
	    endX = 1;
	    
	float beginY = -1.0 ;//+ (2.0 - maskHeight*2);
	float endY = 1.0;//beginY + 2*maskHeight;

	
//	if(position.x >  beginX && position.x < endX && position.y >  beginY && position.y < endY)
	{
	    //gl_FragColor = color1*maskColor.x + color2*(1-maskColor.x);
	  gl_FragColor = color1;
	}
	  
//	else
//	  gl_FragColor = color2;
	  

}
