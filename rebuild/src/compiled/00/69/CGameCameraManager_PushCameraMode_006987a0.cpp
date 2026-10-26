// CGameCameraManager::PushCameraMode(CCountedPointer<NCameraModes::CBase> mode) -- switches to the mode (by-value copy) and marks the camera as manually driven
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006987A0, length 70 bytes.
// Callees: CGameCameraManager::SetCameraMode(CCountedPointer<NCameraModes::CBase>) @ 0x00697E20, operator delete @ 0x00BFE9BC.
// Shape: the by-value argument (data at [esp+4], count block at [esp+8]) is copied into the callee's stack slot through the
// user copy constructor (`sub esp,8 ; mov eax,esp ; mov [eax],ecx ; mov [eax+4],esi ; inc [esi]`), then UsingCameraManually
// (+0x12c) is set, and the function destroys its own by-value parameter (`dec [esi] ; jne ; call [esi+4] ; delete`), the flag
// store being scheduled between the null test and the decrement. The count block is {references, deleter, object}.
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
    void SetCameraMode(CCountedPointer<NCameraModes::CBase> mode);       // 0x00697E20
    void PushCameraMode(CCountedPointer<NCameraModes::CBase> mode);
};

void CGameCameraManager::PushCameraMode(CCountedPointer<NCameraModes::CBase> mode)
{
    SetCameraMode(mode);
    UsingCameraManually = true;
}