#ifndef FABLE_PASS29_AMBIENT_CUBE_H
#define FABLE_PASS29_AMBIENT_CUBE_H
#ifdef FABLE_RETAIL_AMBIENT_CUBE_INTERPOLATE_PASS29
#include "engine/C3DVector.h"
// Descriptive: ambient cube, apex and level names follow the landed constructor and dump routine.
// Layout: eight 28-byte apex records with distance at +24, then the 52-byte level parameter at +224.
#include "libpk/CFader.h"

struct SAmbientApex {
    C3DVector m_offset;
    C3DVector m_position;
    float     m_minDistance;
    SAmbientApex() : m_offset(0.0f, 0.0f, 0.0f), m_position(0.0f, 0.0f, 0.0f) {}
};

class CAmbientCube {
public:
    SAmbientApex m_apexes[8];
    CRampedParam m_level;
    CAmbientCube(float size);
    void SetCentre(const C3DVector& centre);
    void InterpolateLevel(const C3DVector& factors,float time);
};


#endif
#endif
