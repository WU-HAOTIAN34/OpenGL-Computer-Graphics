#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glad.h>
#include <vector>
#include "../vmlib/vec2.hpp"
#include "../vmlib/vec3.hpp"

struct Particle {
    Vec3f position;
    Vec3f velocity;
    Vec3f color;
    float lifeTime;

    Particle()
        : position({0.f,0.f,0.f}), velocity({ 0.f,0.f,0.f }), color({ 0.f,0.f,0.f }), lifeTime(0.0f) { }

};

int findParticle(std::vector<Particle>& particles);

void resetParticle(std::vector<Particle>& particles, int i, Vec3f position, Vec3f velocity);

GLuint create_vao_particles(std::vector<Particle>& particles);

#endif 
