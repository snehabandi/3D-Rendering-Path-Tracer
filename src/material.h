#ifndef MATERIAL_H
#define MATERIAL_H

#include "../lib/rand48/erand48.h"
#include "vector.h"
#include "ray.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Material {

private:
	bool emits;
	Vec m_colour;
	Vec m_emission;
    Texture m_texture;
	


public:
	Material(bool emits=false, Vec c=Vec(1,1,1), Vec e=Vec(0,0,0), Texture tex=Texture() ): emits{emits}, m_colour{c}, m_emission{e}, m_texture{tex}{};
	bool does_emit() const {
		return emits;
	}
	Vec get_colour() const;
	Vec get_colour_at(double u, double v) const;
	Vec get_emission() const;
	virtual Ray get_reflected_ray( const Ray &r, Vec &p, const Vec &n, unsigned short *Xi ) const = 0;
};

class DiffuseMaterial: public  Material {
	public:
	DiffuseMaterial(bool emits, Vec c=Vec(1,1,1), Vec e=Vec(0,0,0), Texture tex=Texture() ): Material(emits, c, e, tex){};
	Ray get_reflected_ray( const Ray &r, Vec &p, const Vec &n, unsigned short *Xi ) const override;
};

class SpecularMaterial: public Material {
	public:
	SpecularMaterial(bool emits, Vec c=Vec(1,1,1), Vec e=Vec(0,0,0), Texture tex=Texture() ): Material(emits, c, e, tex){};
	Ray get_reflected_ray( const Ray &r, Vec &p, const Vec &n, unsigned short *Xi ) const override;
};


#endif // MATERIAL_H