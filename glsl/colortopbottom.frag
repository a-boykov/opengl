#version 330

uniform sampler2D tex1;
out vec4 outputColor;

void main(void)
{
    vec4 color = texture2D(tex1, gl_TexCoord[0].st);
    float lerpValue = gl_FragCoord.y / 500.0f;

    outputColor = mix( vec4(color.r, color.g, color.b, color.a),
		       vec4(0.2f, 0.2f, 0.2f, 1.0f),
		       lerpValue );
}

