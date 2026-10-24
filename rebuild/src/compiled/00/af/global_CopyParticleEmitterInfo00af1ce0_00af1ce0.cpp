// _global::Copy_ParticleEmitterInfo_00af1ce0(const CParticleEmitterInfo* first, const CParticleEmitterInfo* last, CParticleEmitterInfo* result, const random_access_iterator_tag&, int*) -- assign [first, last) of particle emitter info records onto result (counted data pointer + bank + index), return the advanced result
// Leaf: Copy_ParticleEmitterInfo_00af1ce0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1CE0, length 107 bytes (`ret 0Ch`).
// Identity: catalog `_Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*>` is propagated from the byte twin
//   0x00708C20; the landed caller NParticleEngine::CParticleDataBank::Close 0x00AF15B0 runs it over its
//   CArray<CParticleEmitterInfo> (Ego_r.pdb, 12 bytes: CIVCountedPointer<CEmitterData> Data +0, Bank +4, Index +8). That
//   caller declares it with a (tag reference, bool) tail; the two trailing stack words are unused either way.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The implicit operator= runs the inlined intrusive pointer assignment (same-object skip; release through virtual
//   slot 1 when the count reaches zero and clear the slot; adopt + add a reference) and then the plain words.
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CIVCountedPointeeBase {                  // intrusive pointee: vtable +0 (slot 1 deletes), reference count +4
    virtual void Destroy(int flags);            // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    unsigned long IVRefCount;                   // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

template <class T>
struct CIVCountedPointer {                      // 4 bytes; operator= as the landed 0x00A43720 / 0x0083CE60
    CIVCountedPointeeBase* Object;              // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CIVCountedPointeeBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

namespace NParticleEngine {
struct CParticleDataBank;
struct CParticleEmitterInfo {                   // Ego_r.pdb, 12 bytes
    struct CEmitterData;
    CIVCountedPointer<CEmitterData> Data;       // +0x00
    CParticleDataBank*              Bank;       // +0x04
    unsigned long                   Index;      // +0x08
};
}
using NParticleEngine::CParticleEmitterInfo;

CParticleEmitterInfo* __fastcall Copy_ParticleEmitterInfo_00af1ce0(const CParticleEmitterInfo* first, const CParticleEmitterInfo* last, CParticleEmitterInfo* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}