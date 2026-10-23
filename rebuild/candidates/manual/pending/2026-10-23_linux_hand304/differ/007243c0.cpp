// Grade: DIFFER(620v617) -- 140 difflines: retail loop head is unaligned and tested at the bottom with esi = &table[i].Y (`jl`); ours aligns it (jmp over `lea esp,[esp]` padding); the height-above-ground local is stored after the 0.7 compare in retail (fcom then fstp) and the frame is 4 bytes larger. Tried index, pointer and const-ref offset loops.
// CTCPhysicsStandard::UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity) -- when the body is within 0.7 of the ground, sample the impassability factor of the nine squares around it, build a push-away direction weighted by each factor over the squared distance (full weight within 1, fading out to 1.4), and if the velocity runs into it, cancel that component and (without navigation layers) re-seat the Z velocity on the ground at the predicted position
// Leaf: UpdateVelocityForImpassability
// Retail: Fable.exe (TLC, VC7.1) @ 0x007243C0, 615 bytes, `ret 8`.
// Callees: the ground-height query 0x00880200 (as the landed CTCPhysicsStandard code), vector<float>::reserve 0x00724E30,
//   CTCPhysicsBase::GetWorldMap 0x006AF620, CWorldMap::GetImpassabilityFactorAround 0x0063E3C0 (PDB),
//   _CIfmod 0x00BFEB28, C3DVector::Normalise (float result) 0x00A14480, free 0x00BFEA14; virtual slot 55 (+0xdc).
// Globals: the nine neighbour offsets (C2DCoordI) 0x01385978; 0.7f 0x01255318, 1.0 (double) 0x01237F00, 0.5f 0x0122F59C,
//   1.4f 0x01265AAC, 2.5f 0x01241D68, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
#include <math.h>
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
    void operator*=(float s) { X *= s; Y *= s; Z *= s; }
    void Clear() { X = Y = Z = 0.0f; }
    float Normalise();                              // 0x00A14480
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct C2DCoordI { long X, Y; };
extern const C2DCoordI g_NeighbourOffsets[9];       // 0x01385978

struct CFloatVector {                               // retail STLport vector<float> (12 bytes)
    float* first;
    float* last;
    float* endOfStorage;
    CFloatVector() : first(0), last(0), endOfStorage(0) { }
    ~CFloatVector() { if (first) free(first); }
    void reserve(unsigned int n);                   // 0x00724E30
    float& operator[](unsigned int i) { return first[i]; }
};
struct CWorldMap {
    void GetImpassabilityFactorAround(const C3DVector& pos, CFloatVector& factors, bool navigationLayers) const;   // 0x0063E3C0
};

#define V4(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d();
struct CTCPhysicsStandard {
    V4(0) V4(1) V4(2) V4(3) V4(4) V4(5) V4(6) V4(7) V4(8) V4(9) V4(10) V4(11) V4(12)   // 0-51
    virtual void v52(); virtual void v53(); virtual void v54();
    virtual bool UsesNavigationLayers();            // slot 55 (+0xdc)
    CWorldMap* GetWorldMap();                       // 0x006AF620
    float GetGroundHeight(const C3DVector& pos);    // 0x00880200
    void UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity);
};

void CTCPhysicsStandard::UpdateVelocityForImpassability(const C3DVector& position, C3DVector& velocity)
{
    float heightAboveGround = position.Z - GetGroundHeight(position);
    if (heightAboveGround < 0.7f) {
        CFloatVector factors;
        factors.reserve(9);
        GetWorldMap()->GetImpassabilityFactorAround(position, factors, UsesNavigationLayers());
        float offsetX = 0.5f - (float)fmod((double)position.X, 1.0);
        float offsetY = 0.5f - (float)fmod((double)position.Y, 1.0);
        C3DVector push;
        push.Clear();
        long i = 0;
        for (const C2DCoordI* offset = g_NeighbourOffsets; offset < g_NeighbourOffsets + 9; ++offset, ++i) {
            C3DVector dir(offset->X + offsetX, offset->Y + offsetY, 0.0f);
            float dist = dir.Normalise();
            if (dist != 0.0f && dist < 1.4f) {
                float weight = (dist > 1.0f) ? (1.4f - dist) * 2.5f : 1.0f;
                push += dir * (weight * factors[i] / (dist * dist));
            }
        }
        float pushLength = push.Normalise();
        float along = DotProduct(push, velocity);
        push *= -(along * pushLength);
        if (pushLength > 0.0f && along > 0.0f) {
            velocity += push;
            if (!UsesNavigationLayers()) {
                C3DVector predicted = position + velocity;
                velocity.Z = GetGroundHeight(predicted) + heightAboveGround - position.Z;
            }
        }
    }
}
