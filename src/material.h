#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Material {

private:
	bool emits;
	glm::vec3 m_colour;
	glm::vec3 m_emission;
    Texture m_texture;
	


public:
	Material(bool emits=false, glm::vec3 c=glm::vec3(1,1,1), glm::vec3 e=glm::vec3(0,0,0), Texture tex=Texture(), bool temp=false): emits{emits}, m_colour{c}, m_emission{e}, m_texture{tex}, temp{temp}{};
	bool does_emit() const {
		return emits;
	}
	glm::vec3 get_colour() const;
	glm::vec3 get_colour_at(double u, double v) const;
	glm::vec3 get_emission() const;
	virtual Ray get_reflected_ray( const Ray &r, glm::vec3 &p, const glm::vec3 &n, unsigned short *Xi ) const = 0;
	virtual ~Material()=default;
	bool temp;
};

class DiffuseMaterial: public  Material {
	public:
	DiffuseMaterial(bool emits, glm::vec3 c=glm::vec3(1,1,1), glm::vec3 e=glm::vec3(0,0,0), Texture tex=Texture(), bool temp=false): Material(emits, c, e, tex, temp){};
	Ray get_reflected_ray( const Ray &r, glm::vec3 &p, const glm::vec3 &n, unsigned short *Xi ) const override;
};

class SpecularMaterial: public Material {
	public:
	SpecularMaterial(bool emits, glm::vec3 c=glm::vec3(1,1,1), glm::vec3 e=glm::vec3(0,0,0), Texture tex=Texture(), bool temp = false): Material(emits, c, e, tex, temp){};
	Ray get_reflected_ray( const Ray &r, glm::vec3 &p, const glm::vec3 &n, unsigned short *Xi ) const override;
};


#endif // MATERIAL_H