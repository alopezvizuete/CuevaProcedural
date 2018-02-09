#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specTex;

uniform vec4 intenLuz;
uniform mat4 posiLuz;
uniform int tipoLuz;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 5000.0;
vec3 Ke;

//Propiedades de la luz
vec3 Ia = vec3 (0.3);
vec3 IdP = vec3 (intenLuz.x);
vec3 IsP = vec3 (intenLuz.x);
vec3 IdF = vec3 (intenLuz.y);
vec3 IsF = vec3 (intenLuz.y);
vec3 IdD = vec3 (intenLuz.z);
vec3 IsD = vec3 (intenLuz.z);
vec3 lposP = posiLuz[0].xyz;
vec4 Plfoc = posiLuz[1];
vec4 Pldir = posiLuz[2];

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	Ke = texture(emiTex, texCoord).rgb;
	//Ks = vec3 (1.0);
	Ks = texture(specTex,texCoord).xyz;

	N = normalize (norm);
	
	outColor = vec4(shade(), 1.0);   
}

vec3 shade()
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;
	for(int i=0;i<posiLuz.length();i++){
		if(posiLuz[i].w==1.0f){
			// PUNTUAL
			vec3 L = normalize (posiLuz[i].xyz - pos);
			vec3 diffuse = intenLuz[i] * Kd * dot (L,N);
			c += clamp(diffuse, 0.0, 1.0);
			vec3 V = normalize (-pos);
			vec3 R = normalize (reflect (-L,N));
			float factor = max (dot (R,V), 0.01);
			vec3 specular = intenLuz[i]*Ks*pow(factor,alpha);
			c += clamp(specular, 0.0, 1.0);
		}
		if(posiLuz[i].w==2.0f){
			//FOCAL
			vec3 Lfoc = normalize(posiLuz[i].xyz - pos);
			//Atenuacion
			float dis_foc = length(Lfoc);
			float c1 = 1.0;
			float c2 = 0.05;	
			float c3 = 0.025;
			float fatt_foc = min( (1/(c1+(c2*dis_foc)+(c3*dis_foc*dis_foc))),1); 
			vec3 V = normalize (-pos);
			vec3 dir_foc = vec3(0.0,0.0,1.0);
			float Ip = (dot(dir_foc,Lfoc) - cos(0.3) ) / (1-cos(0.3));
			Ip = pow(Ip,20);
			vec3 diff_foc = intenLuz[i] * Kd * dot(N,Lfoc)*fatt_foc*Ip;
			c += clamp(diff_foc,0.0,1.0);
			vec3 Rfoc = reflect(-Lfoc,N);
			float Sfactor_foc = max (dot(Rfoc,V),0.0001);
			Sfactor_foc = pow(Sfactor_foc,alpha);
			vec3 spec_foc = intenLuz[i] * Ks * Sfactor_foc*fatt_foc*Ip;
			c += clamp(spec_foc,0.0,1.0);
		}
		if(posiLuz[i].w==3.0f){
		//DIRECCIONAL 
			vec3 V = normalize (-pos);
			vec3 Ldir = normalize(posiLuz[i].xyz);
			vec3 diff_dir = intenLuz[i] * Kd * dot(N,Ldir);
			c += clamp(diff_dir,0.0,1.0);
			vec3 Rdir = reflect(-Ldir,N);
			float Sfactor_dir = max (dot(Rdir,V),0.0001);
			Sfactor_dir = pow(Sfactor_dir,alpha);
			vec3 spec_dir = intenLuz[i] * Ks * Sfactor_dir;
			c += clamp(spec_dir,0.0,1.0);
		}
	}
	//NIEBLA
	float distance = length(pos);
	vec3 fogColor = vec3 (0.2,0.2,0.2);
	float fCor = 0.1;
	float el = pow(distance*fCor,2);
	float f = exp(-el);
	f=clamp(f,0.0,1.0);
	c = mix(fogColor,c,f);
	c+=Ke;

	//return c;
	return vec3(1.0,0.0,1.0);
}
