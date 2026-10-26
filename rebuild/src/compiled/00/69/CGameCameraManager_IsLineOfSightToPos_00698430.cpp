// CGameCameraManager::IsLineOfSightToPos(const C3DVector& pos) -- filtered line-of-sight test from the current mode's camera to the position through the camera tools
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698430, length 64 bytes.
// Callees: CTools::IsLineOfSightFiltered @ 0x00877C40 (8 arguments, `ret 20h`).
// Shape: 0x1c bytes of locals (two C3DVector out-records at [esp+10h]/[esp+4], a long at [esp]) plus a pointer out-record
// that VC homes in the dead argument slot (`lea edx,[esp+28h]` = &pos after its value was loaded); the first argument is the
// current mode's camera (`add eax,54h`; the mode pointer is a named local so its load stays at the top), `this` for the call is
// &CamTools (`add ecx,78h`); the callee's bool comes back in al.
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
    bool IsLineOfSightToPos(const void* pos);
};

bool CGameCameraManager::IsLineOfSightToPos(const void* pos)
{
    NCameraModes::CBase* mode = CurrentMode.m_pObject;   // read first (`mov eax,[ecx+98h]` before the frame is built)
    unsigned char hitPos[0xc];                      // C3DVector
    unsigned char hitNormal[0xc];                   // C3DVector
    long hitIndex;
    const CThing* hitThing;
    return CamTools.IsLineOfSightFiltered(mode->m_camera, pos, hitPos, hitNormal, &hitIndex, hitThing, 7, true);
}