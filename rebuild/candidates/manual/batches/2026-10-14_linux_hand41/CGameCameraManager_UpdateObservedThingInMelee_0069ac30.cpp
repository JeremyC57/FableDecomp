// CGameCameraManager::UpdateObservedThingInMelee() -- when the current mode is interruptable (or a mode is pending), toggles the melee camera mode to follow the observed thing's melee state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069AC30, length 96 bytes.
// Callees: IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase>) @ 0x00697A50 (a static, callee-cleaned:
// `__stdcall`, the by-value copy built in its stack slot with the inlined copy constructor), IsObservedThingInMelee
// @ 0x006994F0, AddMeleeCameraMode @ 0x0069A8B0 (manifest label "CTCCombat::AddMeleeFlourishAbility" is propagated),
// RemoveCombatCameraMode @ 0x0069A920.
// Shape: `if (!interruptable && PendingMode == 0) return;` then the melee predicate with ObservedThingInMelee (+0xa9) read
// once into al before the branch; the two tail calls are sibling `jmp`s.
struct CWorld;
struct CThing;

namespace NCameraModes {
struct CBase {
    virtual void v00();
    virtual void v01();
    virtual long GetMode();                         // slot 2: NCameraModes::EMode
    unsigned char _body[0x50];                      // +0x04 .. +0x54
    unsigned char m_camera[0x38];                   // +0x54 CCamera (the mode's own camera)
};
}

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4 per-type deleter, object in ecx
    void* m_pObject;                                // +8
};

template <class T>
struct CCountedPointer {
    T* m_pObject;                                   // +0
    SCountBlock* m_pCount;                          // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;                    // operator delete @ 0x00BFE9BC
            }
        }
    }
};

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1: runs when the last reference goes
    long m_refCount;                                // +4
};

struct CDefPointer {
    CDefPointeeBase* m_pObject;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                if (--m_pObject->m_refCount == 0) m_pObject->Destroy();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject) ++m_pObject->m_refCount;
        }
        return *this;
    }
};

struct CTools {                                     // retail CTools is 0x20 bytes (the PDB's is 0x24: 16-byte vector)
    unsigned char _body[0x20];
    bool IsLineOfSightFiltered(const void* camera, const void* pos, void* hitPos, void* hitNormal, long* hitIndex,
                               const CThing*& hitThing, long filter, bool flag);    // 0x00877C40
};

// Retail CGameCameraManager layout: 0x18 bytes narrower than the PDB (engine/CGameCameraManager.h) from CamTools (+0x78)
// onwards -- retail CTools is 0x20, retail std::vector is 12 bytes and the STLport list 4 bytes -- so the header's offsets
// (CurrentMode +0x9c, UsingCameraManually +0x144, PDef +0x14c) do not match the bytes (+0x98, +0x12c, +0x134); the member
// names are the PDB's.
struct CGameCameraManager {
    void* __vftable;                                // +0x00
    unsigned char Camera[0x38];                     // +0x04 CCamera
    unsigned char OldCamera[0x38];                  // +0x3c CCamera
    CWorld* World;                                  // +0x74
    CTools CamTools;                                // +0x78
    CCountedPointer<NCameraModes::CBase> CurrentMode;   // +0x98
    CCountedPointer<NCameraModes::CBase> PendingMode;   // +0xa0
    bool NewAutoMode;                               // +0xa8
    bool ObservedThingInMelee;                      // +0xa9
    bool ObservedThingInBuilding;                   // +0xaa
    unsigned char _pad_0xab[0x81];                  // +0xab .. +0x12c
    bool UsingCameraManually;                       // +0x12c
    bool BlockModeChange;                           // +0x12d
    unsigned char _pad_0x12e[2];                    // +0x12e
    CDefPointer PCameraManagerSet;                  // +0x130 CDefPointer<const CCameraManagerSetDef>
    CDefPointer PDef;                               // +0x134 CDefPointer<const CCameraManagerDef>
    static bool __stdcall IsCameraModeInterruptable(CCountedPointer<NCameraModes::CBase> mode);   // 0x00697A50
    bool IsObservedThingInMelee();                  // 0x006994F0
    void AddMeleeCameraMode();                      // 0x0069A8B0
    void RemoveCombatCameraMode();                  // 0x0069A920
    void UpdateObservedThingInMelee();
};

void CGameCameraManager::UpdateObservedThingInMelee()
{
    if (!IsCameraModeInterruptable(CurrentMode) && PendingMode.m_pObject == 0)
        return;
    if (IsObservedThingInMelee()) {
        if (!ObservedThingInMelee)
            AddMeleeCameraMode();
    } else {
        if (ObservedThingInMelee)
            RemoveCombatCameraMode();
    }
}
