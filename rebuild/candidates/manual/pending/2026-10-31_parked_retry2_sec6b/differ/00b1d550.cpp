// NParticleEngine::CPSCDecalRenderer::DecalCollides(CParticleSystem& system, const list<CDecal*>::iterator& decal, float radius,
//   list<CDecal*>::iterator& collider, float& colliderSize) -- find the first other active, live decal of the system whose
//   current pooled size (start/end render size rounded to two places, lerped by life fraction inside the fade band, times the
//   decal's size factor, capped at MaxPoolSize) overlaps `decal` grown by `radius`; returns it and its size.
// Leaf: DecalCollides
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1D550, 576 bytes, `ret 0x14`. PDB (Ego_r) ?DecalCollides@CPSCDecalRenderer@NParticleEngine@@IAE_N...
// Retail-asm: 0x00B1D5A5 `fld dword ptr [esp+10h] ; fistp dword ptr [esp+14h]` and 0x00B1D605 `fld dword ptr [esp+18h] ; fistp dword ptr [esp+14h]` -- the engine's inline-assembly float-to-int helper (bare dword fistp into a named long); VC7.1 C++ `(long)x` calls __ftol2.
// Layout: PDB struct_layouts_egor.tsv CPSCDecalRenderer (bitfields FadeInEndInteger +0x20 bit 16, FadeOutBeginInteger +0x24 bit 0,
//   StartRenderSize +0x24 bit 10, EndRenderSize +0x28 bit 0; MaxPoolSize +0x38); engine/CDecal.h (Position +0x04, Lifetimer +0x28,
//   MaxLife +0x30, SizeFactor +0x38, Active bit 0 of +0x58). No engine header for CPSCDecalRenderer / CParticleSystem: local views.
// Grade: DIFFER(546v576) 150 diff lines (parked: no body). Retry sec6b first reconstruction (retail_asm candidate): the list walk,
//   the clamped life fraction, the fade band, the capped pooled size, the overlap test and both exits follow retail; the rounding
//   helper keeps retail's `mov eax,2 ; fld 1.0 ; dec ; fmul 10.0 ; jne` loop (a for-loop folds to x*100 / *0.01: 503 bytes, 107 lines).
//   Left: VC7.1 folds `* (1/2047) * 20.0` into one constant (retail multiplies twice), `this` stays in ecx (retail ebp) and the
//   frame is 0x14 (retail 0x18) so every esp offset shifts. The decal's Active flag is bit 0 of CDecal.h's +0x58 group (the header
//   spells the byte group LandscapeOnlyDecal; PDB bit order Active, SizeCollisionThisFrame, AlphaCollisionThisFrame, LandscapeOnlyDecal).
#include "engine/CDecal.h"

template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& x) const { return _M_node == x._M_node; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> struct list {
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    iterator begin() { iterator it; it._M_node = _M_node->_M_next; return it; }
    iterator end() { iterator it; it._M_node = _M_node; return it; }
};
template <class T> inline const T& (min)(const T& a, const T& b) { return b < a ? b : a; }

__forceinline long FloatToInt(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
inline float RoundToDecimalPlaces(float value, long places)
{
    float scale = 1.0f;
    do {
        scale *= 10.0f;
    } while (--places);
    return FloatToInt(value * scale) / scale;
}

namespace NParticleEngine {
class CParticleSystem {
public:
    unsigned char _pad00[0x20];
    list<CDecal*> Decals;                                    // +0x20
};
class CPSCDecalRenderer {
public:
    void*         __vftable;                                 // +0x00
    unsigned char _pad04[0x20 - 0x04];
    unsigned long BlendMode : 3;                             // +0x20
    unsigned long BlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;                  // +0x24
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long EndRenderSize : 11;                        // +0x28
    unsigned long AnimationTimeSecs : 14;
    unsigned long SizeFadeMinimum : 7;
    unsigned char _pad2c[0x38 - 0x2c];
    float         MaxPoolSize;                               // +0x38
protected:
    bool DecalCollides(CParticleSystem& system, const list<CDecal*>::iterator& decal, float radius,
                       list<CDecal*>::iterator& collider, float& colliderSize);
};

bool CPSCDecalRenderer::DecalCollides(CParticleSystem& system, const list<CDecal*>::iterator& decal, float radius,
                                      list<CDecal*>::iterator& collider, float& colliderSize)
{
    float startSize = RoundToDecimalPlaces(StartRenderSize * (1.0f / 2047.0f) * 20.0f, 2);
    float endSize = RoundToDecimalPlaces(EndRenderSize * (1.0f / 2047.0f) * 20.0f, 2);
    float fadeOutBegin = FadeOutBeginInteger * 0.001f;
    float fadeInEnd = FadeInEndInteger * 0.001f;
    for (list<CDecal*>::iterator it = system.Decals.begin(); it != system.Decals.end(); ++it) {
        if (it == decal)
            continue;
        CDecal* other = *it;
        if (other->MaxLife > -1 && (other->LandscapeOnlyDecal[0] & 1)) {
            float life = (min)((float)other->Lifetimer / other->MaxLife, 1.0f);
            if (life >= fadeInEnd && life <= fadeOutBegin) {
                float size = (min)(((endSize - startSize) * life + startSize) * other->SizeFactor[0], MaxPoolSize);
                CDecal* self = *decal;
                float dx = self->Position_X - other->Position_X;
                float dy = self->Position_Y - other->Position_Y;
                float dz = self->Position_Z - other->Position_Z;
                float reach = size + radius;
                if (dx * dx + dy * dy + dz * dz < reach * reach) {
                    collider = it;
                    colliderSize = size;
                    return true;
                }
            }
        }
    }
    return false;
}
}
