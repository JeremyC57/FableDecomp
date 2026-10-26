// std::pair::pair() -- default constructor of pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>: type 0, then an empty enabled component instance
// Leaf: ??0?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF67B0, 49 bytes, __fastcall `ret`, returns `this`.
// Identity: the element of CParticleSystem's Components map (CVectorMap<EPSCType, CComponentInstance>); the same
//   default pair is built inline by the landed one-argument resize 0x00AF7A10 (`resize(n, value_type())`). No direct
//   callers (an emitted COMDAT). The vtable 0x0129F478 is NParticleEngine::CComponentInstance (RTTI), base CBaseClass.
// Callee: CBaseClass::CBaseClass() @ 0x0099A2D0.
// Layout (Ego_r.pdb): first (EPSCType) +0x00; CComponentInstance +0x04 (20 bytes): vptr, Component
//   (CCountedPointer<CPSCBase>: data +4, count block +8), Enabled +0xC (true), InstanceName +0x10 (left unset).
// Shape: STLport's `pair() : first(_T1()), second(_T2())`; the instance's inline constructor (base call, own vtable,
//   counted pointer zeroed, Enabled = true) runs on `esi = &second`, and the enabled byte is stored after `mov eax,edi`.
class CBaseClass {                                      // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                       // 0x0099A2D0
};

template <class T> class CCountedPointer { public:
    T*    Data;                                         // +0x00
    void* m_pCount;                                     // +0x04
    CCountedPointer() : Data(0), m_pCount(0) {}
};

namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {          // vtable 0x0129F478
public:
    CCountedPointer<CPSCBase> Component;                // +0x04
    bool                      Enabled;                  // +0x0C
    unsigned long             InstanceName;             // +0x10
    virtual void v000();
    CComponentInstance() : Enabled(true) {}
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};
template pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>::pair();
}