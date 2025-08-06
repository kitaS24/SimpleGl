#version 120
uniform sampler2D tex;
varying vec4 fragColor;
varying vec2 texCoord;

void main() {
vec4 Color = texture2D(tex,texCoord);
Color.x = 1-Color.x;
Color.y = 1-Color.y;
Color.z = 1-Color.z;
gl_FragColor = Color;
}