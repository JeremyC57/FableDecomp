// std::fill(NParticleEngine::CParticleEmitterInfo* first, NParticleEngine::CParticleEmitterInfo* last, const NParticleEngine::CParticleEmitterInfo& val) -- assign val to every emitter info of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00AF1C70, 78 bytes, `ret 4`: __fastcall (first ecx -> esi, last edx -> ebx, the value reference
// on the stack -> edi). Only call site 0x00AF210D in the vector resize path at 0x00AF2010 (catalogued Vector_ResizeN).
// Identity: the catalog's `_Fill<NGameText::CDataBankTextEntry*>` is propagated from the 8-byte twin 0x00A43890; this
// body strides 12 bytes and copies two words after the counted pointer: NParticleEngine::CParticleEmitterInfo
// (Ego_r.pdb, 12 bytes) = Data CIVCountedPointer<CParticleEmitterInfo::CEmitterData> +0, Bank +4, Index +8
// (CMeshEffectPriorityEntry, the other 12-byte layout, has byte flags at +8..+0xA that would not copy as one dword).
// Callees: the emitter data's virtual destroy (vtable slot 1) when its reference count (+4) drops to zero.
// Shape: as 0x00A43890: the implicit copy assignment = the intrusive pointer's operator= (self test, release through the
// local copy, store, count bump) then Bank and Index.
// Local base model, not engine/CIVCountedPointeeBase.h: the generated header flattens the vptr into a plain
// `void* __vftable`, which cannot make the slot-1 virtual destroy call the release runs.
struct CIVCountedPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
struct CIVCountedPointer {
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

namespace NParticleEngine {
struct CParticleDataBank;
struct CParticleEmitterInfo {                   // 12 bytes
    struct CEmitterData : CIVCountedPointeeBase { };
    CIVCountedPointer<CEmitterData> Data;       // +0x00
    CParticleDataBank*              Bank;       // +0x04
    unsigned long                   Index;      // +0x08
};
}
using NParticleEngine::CParticleEmitterInfo;

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(CParticleEmitterInfo*, CParticleEmitterInfo*, const CParticleEmitterInfo&);
}