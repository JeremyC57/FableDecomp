// CGameCameraManager::RemoveBuildingCameraMode() -- unless the main player is in mode 0x2f, restores the main camera set def, adds the melee mode when the observed thing is in melee, and clears ObservedThingInBuilding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069B610, length 64 bytes.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (`GetPlayer(MainPlayer)`; manifest label "Catch@0040d959" is wrong),
// CPlayer::HasMode(EPlayerMode) @ 0x00634160, SetUpMainCameraManagerSetDef @ 0x0069ADC0, IsObservedThingInMelee
// @ 0x006994F0, AddMeleeCameraMode @ 0x0069A8B0 (manifest "CTCCombat::AddMeleeFlourishAbility" is propagated).
// Globals: the engine singleton @ 0x013B86A0 whose player manager sits at +0x1c (retail CEngine layout).
// Shape: `push 2Fh` precedes the GetMainPlayer call because VC evaluates the HasMode argument before the object expression.
class CPlayer { public:
    bool HasMode(long mode);                        // 0x00634160
};

class CPlayerManager { public:
    CPlayer* GetMainPlayer();                       // 0x00449970
};

class CEngine { public:
    unsigned char _pad[0x1c];
    CPlayerManager* m_pPlayerManager;               // +0x1c
};

extern CEngine* g_pEngine;                          // 0x013B86A0

class CWorld;
class CThing;

namespace NCameraModes {
class CBase { public:
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
class CCountedPointer { public:
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

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1: runs when the last reference goes
    long m_refCount;                                // +4
};

struct CDefPointer {
    CDefPointeeBase* m_pObject;
    void Release()
    {
        CDefPointeeBase* old = m_pObject;           // a local copy: retail decrements through the register (`dec [ecx+4] ; jne`)
        if (old) {
            if (--old->m_refCount == 0) old->Destroy();
            m_pObject = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_pObject != other.m_pObject) {
            Release();
            m_pObject = other.m_pObject;            // re-read after Destroy (`mov eax,[edi]`)
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
class CGameCameraManager { public:
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
    void SetUpMainCameraManagerSetDef();            // 0x0069ADC0
    bool IsObservedThingInMelee();                  // 0x006994F0
    void AddMeleeCameraMode();                      // 0x0069A8B0
    void RemoveBuildingCameraMode();
};

void CGameCameraManager::RemoveBuildingCameraMode()
{
    if (!g_pEngine->m_pPlayerManager->GetMainPlayer()->HasMode(0x2f)) {
        SetUpMainCameraManagerSetDef();
        if (IsObservedThingInMelee())
            AddMeleeCameraMode();
        ObservedThingInBuilding = false;
    }
}