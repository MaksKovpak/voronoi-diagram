#version 330
precision mediump float;

#define MAX_SIZE 512
// #define MANHATTAN

uniform int size;
uniform vec2 seeds[MAX_SIZE];
uniform vec3 colors[MAX_SIZE];

// Manhattan distance
float distance_to(vec2 v1, vec2 v2) {
	#ifndef MANHATTAN
		return distance(v1, v2);
	#else
		return abs(v1.x - v2.x) + abs(v1.y - v2.y);
	#endif
}

void main() {
	float dist = distance_to(seeds[0], gl_FragCoord.xy);
	vec3 color = colors[0];

	for (int i = 1; i < size; i++) {
		float current = distance_to(seeds[i], gl_FragCoord.xy);

		if (current < dist) {
			color = colors[i], dist = current;
		}
	}

	gl_FragColor = vec4(color, 1.0);
}
