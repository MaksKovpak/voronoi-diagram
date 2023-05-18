#version 330
precision mediump float;

#define MAX_SIZE 512

uniform int size;
uniform vec2 seeds[MAX_SIZE];

float voronoi(in vec2 p) {
    vec2 currentVector;
    float minDist = 1e6;

    for (int i = 0; i < size; i++) {
        vec2 v = seeds[i] - p;
        float d = dot(v, v);

        if(d < minDist) minDist = d, currentVector = v;        
    }

    minDist = 1e6;
    for (int i = 0; i < size; i++) {
        vec2 v = seeds[i] - p;

        // Distance between points A and B is more than 10px
        if(dot(currentVector - v, currentVector - v) > 10) {
            minDist = min(minDist, dot(0.5 * (currentVector + v), normalize(v - currentVector)));
        }
    }

    return minDist;
}

void main() {
    vec3 color = mix(vec3(0.0), vec3(1.0), smoothstep(1, 3, voronoi(gl_FragCoord.xy)));

    gl_FragColor = vec4(color, 1.0);
}
