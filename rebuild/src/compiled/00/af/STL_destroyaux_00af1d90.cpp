// std::__destroy_aux(NParticleEngine::CParticleEmitterInfo* first, NParticleEngine::CParticleEmitterInfo* last, const __false_type&) -- destroy a run of 12-byte particle-emitter info records: release each record's counted emitter-data pointer (virtual delete at a zero count) and null it
// Leaf: ??$__destroy_aux@PAVCParticleEmitterInfo@NParticleEngine@@@std@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF1D90, 50 bytes, `ret 4` (link-time __fastcall: first in ecx, last in edx, the
//   __false_type tag reference on the stack, never read).
// Identity: the element-range destroy of NParticleEngine::CParticleDataBank's ParticleEmitters (CArray<CParticleEmitterInfo>):
//   the landed CParticleDataBank::Close 0x00AF15B0 (erase(begin(), end())) and ~CParticleDataBank 0x00AF1600 call it, as
//   do the vector's insert / resize COMDATs 0x00AF1E90 / 0x00AF2170. Byte twin of 0x00A43A00 at a 12-byte stride.
// Callees: virtual slot 1 of the pointee (CIVCountedPointeeBase delete) when the reference count reaches zero.
// Layout: Ego_r.pdb CParticleEmitterInfo (12 bytes): Data CIVCountedPointer<CParticleEmitterInfo::CEmitterData> +0,
//   Bank CParticleDataBank* +4, Index +8; CIVCountedPointeeBase: vptr +0, IVRefCount +4. Local model: the generated
//   engine/CIVCountedPointeeBase.h carries the vptr as a data word, and the release needs the real virtual slot 1.
// Shape: STLport 4.x `for (; first != last; ++first) _Destroy(&*first);` with the record's implicit destructor inlined:
//   ~CIVCountedPointer = `T* old = Object; if (old) { if (--old->IVRefCount == 0) old->Delete(); Object = 0; }`
//   (the pointer copied to a local, or VC7.1 re-reads it after the decrement).
// Label correction: catalogued as `vector<NGameText::CDataBankGroupEntry>::_Destroy` (propagated); the element is
//   NParticleEngine::CParticleEmitterInfo.
class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);                 // slot 0
    virtual void Delete();                           // slot 1
    unsigned long IVRefCount;                        // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                       // +0x00
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
};

namespace NParticleEngine {
class CParticleDataBank;

class CParticleEmitterInfo {                         // 12 bytes
public:
    class CEmitterData : public CIVCountedPointeeBase { };
    CIVCountedPointer<CEmitterData> Data;            // +0x00
    CParticleDataBank*              Bank;            // +0x04
    unsigned long                   Index;           // +0x08
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __destroy_aux<NParticleEngine::CParticleEmitterInfo*>(NParticleEngine::CParticleEmitterInfo*, NParticleEngine::CParticleEmitterInfo*, const __false_type&);
}