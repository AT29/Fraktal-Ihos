/////////////////////////////////////////////////////////////////////////////////////
//
//  MIT License
//  
//  Fraktal Ihos - Copyright(c) 2022 Antoni Tretiakov - antonitre29@gmail.com
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//  
//  The above copyright noticeand this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//
/////////////////////////////////////////////////////////////////////////////////////

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


// not pow - it should be rootC(...)
vec2 powC(vec2 z, float n)
{
	float mod_z = sqrt(z.x*z.x+z.y*z.y);
	float mod_to_n = pow(mod_z, n);

	float fi1 = 0.;
	float fi2 = 0.;
	if(mod_z > 0.) {fi1 = acos(z.x / mod_z);}
	if(mod_z > 0.) {fi2 = asin(z.y / mod_z);}

	return vec2( mod_to_n*cos(n*fi1), mod_to_n*sin(n*fi2) );
}

uniform float sh_scale;
uniform float sh_time_0;
uniform float sh_music; // Range: <0;1>
uniform float sh_musicAccumulator;
uniform float sh_precision;
uniform vec2  sh_relativeCenter;
uniform vec2  sh_resolutionCenter;
uniform vec2  sh_displacement;
uniform vec2  sh_windowResolution;

void main(void)
{	
	vec4 color;
	
	vec2 z = {2.*3.14159*fract(sh_time_0/32. + sh_musicAccumulator*0.003),0.5};
	vec2 z2 = vec2(0.0,0.0);
	
	vec2 c = vec2(
		sh_scale * (gl_FragCoord.yx - sh_resolutionCenter.yx) - sh_displacement.yx - sh_relativeCenter.yx
	);
	
	float cScale = 6.;
	float cTileScale = 0.5;
	
	c.x += sh_windowResolution.x*0.015625*0.5; // 1/64
	c.y += sh_windowResolution.y*0.125;
	
	bool activate = true;
	if((int)floor(c.x*cTileScale)%4<=1 && (int)floor(c.y*cTileScale)%4<=1)
	{vec2 temp=c.x; c.x = c.y; c.y = temp;activate=false;}
	
	vec2 c2 = fract(vec2(c.x,c.y)*cTileScale/4.0)*cScale-cScale*0.25;
	c2.x -=0.8;
	
	c = fract(vec2(c.x,c.y)*cTileScale)*cScale-cScale*0.5;
	
	if(activate) c = divC(vec2(1.,0.),c);
	
	float maxprecision = sh_precision;
	
	float number = 0.;
	int iteration = 0;
	while (
		(z2.x *z2.x * z.x * z.x + z2.y *z2.y * z.y * z.y) * 0.05 < 128.0 &&
		++iteration < maxprecision
	)
	{
		if(activate) z = addC(mulC(c,cosC(z))*sin(sh_time_0*0.0625),mulC(c,sinC(z))*cos(sh_time_0*0.0625)); // Fusion
		else z2 = addC(powZC(z2,2),c2);
		number = min(number, (-1.125+0.6*sin(number+sin(sh_time_0*0.0625))-sh_music*0.01) * log( length( cosC(z) ) *4.) );
	}
	
	if (iteration >= maxprecision)
		color = vec4(0.6 + 0.3 * sin(3.8* number+4.5),0.6 + 0.3 * sin(2.5 * number),0.2 + 0.1 * sin(3.8 * number),1.);
	else 
		color = vec4(
			0.4 + 0.15 * sin( (3.0 + float(iteration) / 5.0 + sh_musicAccumulator * 0.1) ),
			0.5 + 0.25 * cos( (5.0 + float(iteration) / 6.0 + sh_musicAccumulator * 0.1) ),
			0.6 + 0.35 * sin( (6.0 + float(iteration) / 7.0 + sh_musicAccumulator * 0.1) ),
			1.
		);
	
	if(activate) gl_FragColor = color;
	else gl_FragColor = vec4(1.-color.xyz,1.);
}
