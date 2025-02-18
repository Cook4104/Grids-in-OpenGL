#version 460 core
out vec4 fragColor;

in flat int inTile;
in vec2 oUV;
uniform sampler2D atlas1;
uniform int swap;

in float oHeight;


void main(){
    // fragColor = vec4(inTile-1,inTile,0,1);
    if(swap == 1)
        fragColor = texture(atlas1,oUV + vec2(inTile*(1.0/16),1.0/16*floor(float(inTile)/16)));
    else if(swap == 0)
        fragColor = vec4(oHeight);
}