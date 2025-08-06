#version 120
varying vec4 fragColor;
varying vec2 texCoord;

void main() {

    fragColor = gl_Color;
    texCoord = gl_MultiTexCoord0.xy;

    gl_Position = ftransform();

}