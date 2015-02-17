
varying vec2 position;
uniform float time;
uniform float maskWidth;
uniform float maskHeight;
void main()
{	

	// the following three lines provide the same result
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_MultiTexCoord2;	
//	gl_TexCoord[2].x = (gl_TexCoord[2].x - time*maskWidth*(10*(1-maskWidth)))*(1/maskWidth);
	gl_TexCoord[2].x = gl_TexCoord[2].x*1.5 - 1;
	gl_Position = ftransform();
	
	
	if(gl_Vertex.x > 0)			position.x = 1;
	else if(gl_Vertex.x < 0) 		position.x = -1;
	else					position.x = 0;
	
	if(gl_Vertex.y > 0)			position.y = 1;
	else if(gl_Vertex.y < 0) 		position.y = -1;
	else 					position.y = 0;
	//position = position/position.w;
}
