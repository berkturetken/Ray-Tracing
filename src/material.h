#pragma once
#ifndef MATERIALH
#define MATERIALH 

struct HitRecord;

#include "ray.h"
#include "hitable.h"
#include <random>

namespace rt {

	float random() {
		float r = (float)rand() / (float)RAND_MAX;
		return r;
	}

	float dot(glm::vec3 a, glm::vec3 b) {
		float x = a.x*b.x + a.y*b.y + a.z*b.z;
		return x;
	}

	glm::vec3 unit_vector(glm::vec3 v) {
		float length = sqrtf((v.x*v.x + v.y*v.y+ v.z*v.z));
		glm::vec3 w = glm::vec3(v.x /length, v.y/length, v.z/length);
		return w;
	}

	float schlick(float cosine, float ref_idx) {
		float r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0)*pow((1 - cosine), 5);
	}


	bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted) {
		glm::vec3 uv = unit_vector(v);
		float dt = dot(uv, n);
		float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
		if (discriminant > 0) {
			refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		else
			return false;
	}

	glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) {
		return v - 2 * dot(v, n)*n;
	}

	glm::vec3 random_in_unit_sphere() {
		glm::vec3 p;
		do {
			p = (float)2.0*glm::vec3(random(), random(), random()) - glm::vec3(1, 1, 1);
		} while ((p.x*p.x + p.y*p.y + p.z*p.z) >= 1.0);
		return p;
	}
	

	class material {
	public:
		virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const = 0;
	};

	class lambertian : public material {
	public:
		lambertian(const glm::vec3& a) : albedo(a) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
			glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			scattered = Ray(rec.p, target - rec.p);
			attenuation = albedo;
			return true;
		}

		glm::vec3 albedo;
	};
	
	class metal : public material {
	public:
		metal(const glm::vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
		virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
			glm::vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
			attenuation = albedo;
			return (dot(scattered.direction(), rec.normal) > 0);
		}
		glm::vec3 albedo;
		float fuzz;
	};

	class dielectric : public material {
	public:
		dielectric(float ri) : ref_idx(ri) {}
		virtual bool scatter(const Ray& r_in, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const {
			glm::vec3 outward_normal;
			glm::vec3 reflected = reflect(r_in.direction(), rec.normal);
			float ni_over_nt;
			attenuation = glm::vec3(1.0, 1.0, 1.0);
			glm::vec3 refracted;
			float reflect_prob;
			float cosine;
			if (dot(r_in.direction(), rec.normal) > 0) {
				outward_normal = -rec.normal;
				ni_over_nt = ref_idx;
				//         cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
				cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
				cosine = sqrt(1 - ref_idx * ref_idx*(1 - cosine * cosine));
			}
			else {
				outward_normal = rec.normal;
				ni_over_nt = 1.0 / ref_idx;
				cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
			}
			if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
				reflect_prob = schlick(cosine, ref_idx);
			else
				reflect_prob = 1.0;
			if (random() < reflect_prob)
				scattered = Ray(rec.p, reflected);
			else
				scattered = Ray(rec.p, refracted);
			return true;
		}

		float ref_idx;
	};
}
#endif
