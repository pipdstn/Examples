#ifndef SNOWFLAKE_H_
#define SNOWFLAKE_H_

#include <ngl/Vec3.h>
#include <ngl/AbstractVAO.h>

struct Snowflake {
    int num_particles     = 0;
    int max_particles     = 300;
    float particle_radius = 0.03f;
    float bias            = 0.03f;
    float biasMax         = 0.06f;
    
    bool exported            = false;
    std::string exportedName = "";
    
    std::vector<ngl::Vec3>            m_particles;
    std::unique_ptr<ngl::AbstractVAO> m_vao;

    Snowflake();
    void update();
    void render();
    void reset();
    void exportFlake();
    bool checkCollisions(ngl::Vec3 position);
    std::string getUniqueFileName();
};

#endif