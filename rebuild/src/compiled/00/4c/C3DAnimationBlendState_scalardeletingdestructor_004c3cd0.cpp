// C3DAnimationBlendState::`scalar deleting destructor'(unsigned int) -- release the shared-data counted pointer Data (+4), run the CBaseClass destructor, free when bit 0 of the flags is set
// Leaf: ??_GC3DAnimationBlendState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3CD0, length 56 bytes (+ int3 padding). C3DAnimationBlendState vtable
// 0x01239838 slot 0.
// Callees: the shared data's virtual slot 1 (`call [eax+4]`, destroy at count zero), CBaseClass::~CBaseClass @ 0x0099A2E0 (it stores the
// CBaseClass vtable 0x01231710; the manifest's "CBase::CBase" label names the constructor),
// operator delete @ 0x00BFE9BC.
// Label correction: the manifest files this row as "CThingHolySite::`vector_deleting_destructor'" (propagated over
// byte-identical bodies); the vtable slot names C3DAnimationBlendState.
// Layout: PDB C3DAnimationBlendState { CIVCountedPointer<CSharedData> Data; } at +4 behind the CBaseClass vptr.
// Shape: the ??_G stores no vtable and runs the base destructor on `this` AFTER the member release, so the class is
// modelled non-polymorphic with the polymorphic CBaseClass as its FIRST MEMBER (wave-24g/24m model; VC7.1 emits the
// non-virtual `??_G` for a `delete`). The counted pointer's inline destructor is `p = m_p; if (p) { if (--p->count == 0)
// p->Destroy(); m_p = 0; }` (`dec dword ptr [ecx+4] ; jne`); both it and the class destructor are `__forceinline` (a
// generated destructor plus a forceinlined release is called out of line from the ??_G; a polymorphic derived class
// with no destructor left the release out of line).
class CBaseClass { public:
    CBaseClass();
    virtual ~CBaseClass();                             // 0x0099A2E0
};

struct CSharedData {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +0x04
};

template <class T> class CIVCountedPointer { public:
    T* m_p;
    CIVCountedPointer() : m_p(0) {}
    __forceinline ~CIVCountedPointer()
    {
        T* p = m_p;
        if (p) {
            if (--p->m_refCount == 0)
                p->Destroy();
            m_p = 0;
        }
    }
};

class C3DAnimationBlendState { public:
    CBaseClass                     m_base;          // +0x00 (the polymorphic base, modelled as the first member)
    CIVCountedPointer<CSharedData> Data;            // +0x04
    __forceinline ~C3DAnimationBlendState() {}
};

void DeleteC3DAnimationBlendState(C3DAnimationBlendState* p)
{
    delete p;
}