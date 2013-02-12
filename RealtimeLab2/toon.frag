//lighthouse 3d 


varying vec3 lightDir, normal;
uniform float tresh;

void main() {

	float intensity;
	vec4 colour;

	vec3 n = normalize(normal);

	intensity = dot (lightDir, n);

	if (intensity > tresh) {
		colour = vec4(1.0, 0.5, 0.5, 1.0);
	} else if (intensity > tresh - 0.5) {
		colour = vec4(0.6, 0.3, 0.3, 1.0);
	} else if (intensity > tresh -  0.25) {
		colour = vec4(0.4, 0.2, 0.2, 1.0);
	} else {
		colour = vec4(0.2, 0.1, 0.1, 1.0);
	}

	gl_FragColor = colour;
}