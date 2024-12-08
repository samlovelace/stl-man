#ifndef STLMODEL_H
#define STLMODEL_H

#include <vector>
#include <stddef.h>

struct Vector3 {
    float x, y, z;
};

struct Triangle {
    Vector3 normal;
    size_t vertex1, vertex2, vertex3; // Indices into a vertex array
};

struct Chunk {
    Vector3 normal; 
    std::vector<Vector3> vertices; 
};

struct STLModel {
    std::vector<Chunk> chunks; 
};

struct STLModelFast {
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
};
#endif
