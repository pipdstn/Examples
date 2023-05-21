#include "snowflake.h"
#include <stdio.h>
#include <ngl/Vec3.h>
#include <ngl/AbstractVAO.h>
#include <ngl/VAOFactory.h>
#include <ngl/ShaderLib.h>
#include <ngl/Mat3.h>
#include <ngl/Util.h>
#include <ngl/Random.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

#define PI 3.14159265

Snowflake::Snowflake() {
    m_vao = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_POINTS);
    bias = ngl::Random::randomPositiveNumber(biasMax);
}

void Snowflake::update() {
    bool inBounds = true;
    bool isColliding = false;
    ngl::Vec3 startPos = ngl::Vec3(10.0f, 0.0f, 0.0f);
    ngl::Vec3 currentPos = startPos;
    
    //Random walk
    while (inBounds && !isColliding) {
        float dx = -particle_radius;
        float dy = ngl::Random::randomNumber(bias);

        float x = currentPos.m_x;
        float y = currentPos.m_y;

        if (abs(atan(y/x)) >= PI/6) {
            ngl::Vec3 addvec = ngl::Vec3(-x, -y, 0);
            addvec.normalize();
            addvec *= particle_radius;

            currentPos += addvec;
        } else {
            currentPos += ngl::Vec3(dx, dy, 0.0f);
        }

        inBounds = (currentPos.m_x > 0.0f);
        isColliding = checkCollisions(currentPos);
    }

    //Add particle to Snowflake for rendering
    m_particles.push_back(currentPos);

    //add reflections/rotations here...
    
    for (int i = 1; i < 6; i ++) {
        ngl::Mat3 R = ngl::Mat3::rotateZ(i * 60.0f);
        ngl::Vec3 rotatedPos = currentPos * R;
        m_particles.push_back(rotatedPos);
    }

    currentPos = currentPos.reflect({0, 1, 0});

    for (int i = 0; i < 6; i ++) {
        ngl::Mat3 S = ngl::Mat3::rotateZ(i * 60.0f);
        ngl::Vec3 reflectedPos = currentPos * S;
        m_particles.push_back(reflectedPos);
    }

    num_particles +=1;
}

void Snowflake::render() {
    m_vao->bind();

    m_vao->setData(ngl::AbstractVAO::VertexData(m_particles.size() * sizeof(ngl::Vec3), m_particles.at(0).m_x));
    m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, sizeof(ngl::Vec3), 0);

    m_vao->setNumIndices(m_particles.size());
    
    m_vao->setMode(GL_POINTS);
    ngl::ShaderLib::setUniform("inColour", ngl::Vec3(1.0f, 1.0f, 1.0f));
    ngl::ShaderLib::setUniform("pointSize", 5.0f);
    m_vao->draw();

    m_vao->unbind();
}

bool Snowflake::checkCollisions(ngl::Vec3 position) {
    bool output = false;

    if (m_particles.size() == 0) {return output;}

    for (int i = 0; i < m_particles.size(); i++) {
        ngl::Vec3 diff = position - m_particles.at(i);
        float dist = diff.length();
        if (dist < 2 * particle_radius) {
                return true;
            }
    }

    return output;
}

void Snowflake::reset() {
    m_particles.clear();
    num_particles = 0;
    bias = ngl::Random::randomPositiveNumber(biasMax);
    exported = false;
    exportedName = "";
}

void Snowflake::exportFlake() {
    printf("Exporting flake!\n");

    std::string       filename = getUniqueFileName();
    std::ofstream     file(filename);
    std::stringstream data;

    //Header
    data << "PGEOMETRY V5\n";
    data << "NPoints " << m_particles.size() << " NPrims 0\n";
    data << "NPointGroups 0 NPrimGroups 0\n";
    data << "NPointAttrib 1 NVertexAttrib 0 NPrimAttrib 0 NAttrib 0\n";
    data << "PointAttrib\n";
    data << "pscale 1 float 1\n";

    //Point Data
    for (int i = 0; i < m_particles.size(); i++) {
        data << m_particles.at(i).m_x << " " << m_particles.at(i).m_y << " " << m_particles.at(i).m_z << " 1 ";
        data << "(" << particle_radius << ")\n";
    }

    //Footer
    data << "beginExtra\n";
    data << "endExtra";

    if (file.is_open()) {
	    file << data.rdbuf();
	    file.close();
        std::cout << "Exported " << filename << "!\n";
        exported = true;
        exportedName = filename;
        return;
    }
    else {
        std::cout << "Attempted to export" << filename << " but failed!\n";
        return;
    }
}

std::string Snowflake::getUniqueFileName() {
    bool exists = true;
    int filecount = 0;
    std::string uniqueName;

    while (exists) {
        std::stringstream fname;
        fname << "snowflake_" << std::setw(4) << std::setfill('0') << filecount << ".geo";
        std::ifstream fin;
        fin.open(fname.str(), std::ios::in);
        if (fin) {
            filecount++;
        }
        else {
            uniqueName = fname.str();
            exists = false;
        }
    }

    return uniqueName;
}