#ifndef FABLE_ENGINE_CFLOATQUAD_H
#define FABLE_ENGINE_CFLOATQUAD_H
// Descriptive libpk value type: no original name found in supplied PDBs or labels.
// Layout: native00c462e0 reads four floats at0,4,8,12 and writes none.
// No callers or RTTI reference; only the accessed 16-byte value record is known.
class CFloatQuad {
public:
    float m_corner[4];
    float Interpolate(float u,float v)const;
};
#endif
