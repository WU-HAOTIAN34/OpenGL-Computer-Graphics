# include "particle.hpp"
#include <cstddef>

int findParticle(std::vector<Particle>& particles)
{

    for (GLuint i = 0; i < 500; ++i) {
        if (particles[i].lifeTime <= 0.0f) {
            return i;
        }
    }
    return 0;
}

void resetParticle(std::vector<Particle>& particles, int i, Vec3f position, Vec3f velocity) {

    
    float randomFloat = (rand() % 4001) / 10000.0f;
    float randomFloat1 = (rand() % 4001) / 10000.0f-0.1;
    float randomFloat2 = (rand() % 4001) / 10000.0f-0.1;
    particles[i].position = position;
    particles[i].position.z = randomFloat;
    particles[i].position.x += randomFloat1;
    particles[i].position.y += randomFloat2;
    particles[i].color = {0.8f, 0.1f, 0.1f};
    particles[i].lifeTime = 25.0f;
    particles[i].velocity =  0.1f * velocity;

}

GLuint create_vao_particles(std::vector<Particle>& particles) {


    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * particles.size(), particles.data(), GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, lifeTime));
    
    
    return VAO;
}

