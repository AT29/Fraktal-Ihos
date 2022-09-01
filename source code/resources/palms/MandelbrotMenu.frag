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

// Basic complex numbers algorithms

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

uniform float sh_scale;
uniform float sh_time;
uniform float sh_music;
uniform float sh_precision;
uniform vec2  sh_relativeCenter;
uniform vec2  sh_resolutionCenter;
uniform vec2  sh_displacement;

void main(void)
{	
	vec2 z = vec2(0,0);
	
	vec2 c = vec2(
		sh_scale * (gl_FragCoord.y - sh_resolutionCenter.y) - sh_displacement.y - sh_relativeCenter.y,
		sh_scale * (gl_FragCoord.x - sh_resolutionCenter.x) - sh_displacement.x - sh_relativeCenter.x
	);

	c.x = -c.x; // invert to rotate image by 90 degrees

	float number = 0;
	int iteration = 0;

	while ( z.x * z.x + z.y * z.y < 16.0 && ++iteration < sh_precision)
	{
		z = addC(mulC(z,z),c);

		if(z.x * z.x + z.y * z.y > 16 + z.x * cos(sh_time)*2. + z.y * sin(sh_time)*2.)
		number = (number + 1.0 - log(log(length(vec2(z.y,z.x))))/log(2.0));
	}

	float intensity = float(iteration) / float(-100*number+sh_precision);

	if (iteration == sh_precision)
		gl_FragColor = vec4(0,0,0,1);
	else 
		gl_FragColor = vec4(0.,0,1.-intensity,1.); // Acidbrot
}