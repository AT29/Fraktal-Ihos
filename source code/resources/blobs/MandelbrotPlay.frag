
#ifdef GL_ES
precision highp float;
#endif

#extension GL_OES_standard_derivatives : enable

// Complex calculation algorithms

vec2 addC(vec2 a, vec2 b)
{
	return vec2(a.x+b.x, a.y+b.y);
}

vec2 subC(vec2 a, vec2 b)
{
	return vec2(a.x-b.x, a.y-b.y);
}

vec2 mulC(vec2 a, vec2 b)
{
	return vec2(
		a.x*b.x - a.y*b.y,
		a.x*b.y + a.y*b.x
	);
}

vec2 divC(vec2 a, vec2 b)
{
	float denominator = b.x*b.x + b.y * b.y;
	return vec2(
		(a.x*b.x + a.y*b.y) / denominator,
		(a.y*b.x - a.x*b.y) / denominator
	);
}

vec2 decimalToCommon(float decimal)
{
	return vec2(0,0);
}

vec2 powZC(vec2 a, int p)
{
	vec2 z = a;

	if(p<1) return vec2(1,1);
	else if(p<2) return a;

	for(int i = 0; i<p-1; i++)
	{
		z = mulC(z, a);
	}
	return z;
}

vec2 sinC(vec2 a)
{
	return vec2(sin(a.x)*cosh(a.y),cos(a.x)*sinh(a.y));
}

vec2 cosC(vec2 a)
{
	return vec2(cos(a.x)*cosh(a.y),-sin(a.x)*sinh(a.y));
}

uniform float sh_scale;
uniform float sh_time;
uniform float sh_music; // Range: <0;1>
uniform float sh_musicAccumulator;
uniform float sh_precision;
uniform vec2  sh_relativeCenter;
uniform vec2  sh_resolutionCenter;
uniform vec2  sh_displacement;

void main(void)
{
	vec2 z = vec2(0,0);
	
	vec2 c = vec2(
		sh_scale * (gl_FragCoord.yx - sh_resolutionCenter.yx) - sh_displacement.yx - sh_relativeCenter.yx
	);

	c.x = -c.x;
	
	float maxprecision = sh_precision;

	float number = 0.;
	int iteration = 0;
	while (
		z.x * z.x * z.y * z.y * (0.75+0.25*sin((z.x * z.y) +sh_musicAccumulator/4.)) < 8. * (1.5+sh_music) &&
		++iteration < maxprecision
	)
	{
		z = addC(powZC(z,2),c);

		number = min(number, -0.5+0.5*sin(number+sh_time)-0.5+0.5*cos(sh_music) + log( length(z) ) );
	}

	float color_optimal_multiplier = sin(0.75+0.1*log(1+sh_music));

	vec4 color;

	if (iteration >= maxprecision)
		color = vec4(0.3 + 0.1 * sin(0.3* number-0.5),0.4 + 0.2 * sin((1+0.2*sh_music)*0.5 * number-0.5),0.5 + 0.3 * sin(0.8 * number),1.);
	else 
		color = vec4(
			0.3 + 0.15 * sin(3.14 + float(iteration) / 1.0 + sh_musicAccumulator * 0.02 + color_optimal_multiplier),
			0.4 + 0.15 * sin(4.2 + float(iteration) / 2.0 + sh_musicAccumulator * 0.01 + color_optimal_multiplier),
			0.5 + 0.15 * sin(6.0 + float(iteration) / 3.0 + sh_musicAccumulator * 0.03 + color_optimal_multiplier),
			1.
		);
	gl_FragColor = color;
}