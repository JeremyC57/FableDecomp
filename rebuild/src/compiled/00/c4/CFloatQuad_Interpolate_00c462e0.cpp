// CFloatQuad::Interpolate(float,float) -- bilinear interpolation of four scalar corners.
// Retail: 0x00C462E0, 51 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?Interpolate@CFloatQuad@@QBEMMM@Z
// Descriptive identity; source searches and layout evidence in NAMING_EVIDENCE.md.
#include "engine/CFloatQuad.h"
#include "engine/C2DVector.h"
float CFloatQuad::Interpolate(float u,float v)const
{
    C2DVector top(m_corner[0],m_corner[1]);
    C2DVector bottom(m_corner[2],m_corner[3]);
    C2DVector edge;
    edge.X=(bottom.Y-bottom.X)*u+bottom.X;
    edge.Y=(top.Y-top.X)*u+top.X;
    return(edge.Y-edge.X)*v+edge.X;
}