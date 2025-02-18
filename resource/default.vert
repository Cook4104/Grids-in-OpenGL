#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in int tile;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out flat int inTile;
out vec2 oUV;
out float oHeight;

void main(){
    gl_Position = projection * view * model * vec4(aPos,0,1);
    inTile = tile-1;
    oUV = aUV;
    oHeight = aHeight;
}