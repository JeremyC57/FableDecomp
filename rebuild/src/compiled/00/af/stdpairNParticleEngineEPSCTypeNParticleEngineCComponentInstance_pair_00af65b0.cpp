// std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>::~pair() -- destroy a particle-system component slot: release the instance's counted component, then the CBaseClass root of the instance
// Leaf: ??1?$pair@W4EPSCType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF65B0, length 61 bytes, `jmp` tail. Callers: 0x00AF768D / 0x00AF784A in
// the vector insert helpers 0x00AF7570 / 0x00AF7730 of the CParticleSystem Components vector (the landed resize(n)
// 0x00AF7A10 names the element: pair<EPSCType, CComponentInstance>, vtable 0x0129F478, base CBaseClass). The manifest label
// (_Dest_val<allocator<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>>) is propagated and wrong.
// Layout: pair.first (EPSCType) +0; CComponentInstance +4 = vptr +0, Component (CCountedPointer<CPSCBase>) +4/+8,
// Enabled +0xc, InstanceName +0x10 (the landed 0x00AF7A10 model).
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
// CBaseClass::~CBaseClass @ 0x0099A2E0 (tail).
// Shape: the enum key needs no destruction, so Info is read straight off ecx and `&second` goes to esi; CComponentInstance
// declares no destructor (no vtable store), its counted pointer re-reads Info, the out-of-line root dtor is the tail jmp.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CCPPointerInfo { public:                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> class CCountedPointer { public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class CBaseClass {                                           // vtable 0x01231710
public:
    virtual ~CBaseClass();                                   // 0x0099A2E0
};

namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {               // vtable 0x0129F478, 0x14 bytes
public:
    CCountedPointer<CPSCBase> Component;                     // +0x04
    bool                      Enabled;                       // +0x0c
    unsigned long             InstanceName;                  // +0x10
};
}

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> TComponentSlot;

void DestroyComponentSlot(TComponentSlot* slot)              // emits the implicit destructor; not part of the retail body
{
    slot->TComponentSlot::~TComponentSlot();
}