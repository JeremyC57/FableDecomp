// _global::CopyBackward_ParticleEmitterInfo_00af1b30(NParticleEngine::CParticleEmitterInfo*, NParticleEngine::CParticleEmitterInfo*, NParticleEngine::CParticleEmitterInfo*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the particle bank's 12-byte emitter infos
// Leaf: CopyBackward_ParticleEmitterInfo_00af1b30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF1B30, length 111 bytes (+ int3 padding).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); count = (last - first)
// / 12 (`imul 2AAAAAABh ; sar 1`), both pointers step back one element, the moved-back result is returned.
// Element (Ego_r.pdb): NParticleEngine::CParticleEmitterInfo, 12 bytes = CIVCountedPointer<CEmitterData> Data +0,
// CParticleDataBank* Bank +4, unsigned long Index +8 (CParticleDataBank::ParticleEmitters is a CArray of them; the TU
// is the particle data bank's, 0x00AF1670 CParticleDataBank). The manifest's `CDataBankTextEntry` label is propagated.
// The implicit operator= runs the inlined intrusive CIVCountedPointer::operator= (same-object skip, release through
// virtual slot 1 at refcount 0 and clear, adopt + add a reference), then copies Bank and Index.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);         // slot 0
    virtual void Delete();                   // slot 1 (+0x04)
    unsigned long IVRefCount;                // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

struct CIVCountedPointer {                   // CIVCountedPointer<T>, 4 bytes
    CIVCountedPointeeBase* Object;           // +0x00
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

struct CParticleDataBank;

struct CParticleEmitterInfo {                // NParticleEngine::CParticleEmitterInfo, 12 bytes
    CIVCountedPointer  Data;                 // +0x00 CIVCountedPointer<CParticleEmitterInfo::CEmitterData>
    CParticleDataBank* Bank;                 // +0x04
    unsigned long      Index;                // +0x08
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CParticleEmitterInfo* __fastcall CopyBackward_ParticleEmitterInfo_00af1b30(CParticleEmitterInfo* first, CParticleEmitterInfo* last, CParticleEmitterInfo* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}