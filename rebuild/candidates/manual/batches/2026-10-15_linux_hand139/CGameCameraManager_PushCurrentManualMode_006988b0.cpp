// CGameCameraManager::PushCurrentManualMode() -- switch to manual camera use (UsingCameraManually set, BlockModeChange cleared); while an observed thing is alive, no script module is pending and the current script module (if any) allows it, prepare the camera-list mode under CameraListIndex as the pending mode and rewind SpecialCameraListIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006988B0, length 147 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on PObservedThing), the current script module's virtual slot 2
// (+8, a bool), CGameCameraManager::PreparePendingMode(CDefPointer<const CCameraModeDef>) @ 0x00698840 (by-value copy built in its
// argument slot), the def's virtual slot 1 (Destroy, tail jump).
// Layout: retail CGameCameraManager (PDB names; retail offsets as the landed SetCameraManagerDef 0x0069A850 and this chunk's
// SetGameScriptedCameraMode / ReleaseScriptedControl): PCurrentScriptModule +0xd4, PPendingScriptModule +0xdc, PObservedThing +0x100,
// CameraListIndex +0x114, SpecialCameraList +0x11c, SpecialCameraListIndex +0x128, UsingCameraManually +0x12c, BlockModeChange +0x12d.
// Shape: the IS_THING_ALIVE idiom on PObservedThing (address kept in edi); `CDefPointer def = *CameraListIndex;` is a named local
// (add-ref `inc [edi+4]`) copied again into the by-value argument (`push ecx ; mov eax,esp ; mov [eax],edi`); its destructor
// releases with `dec [edi+4] ; jne` and tail-jumps the virtual Destroy.
struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_statusFlags;            // +0x91 (bit 0: dead)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                 // slot 1
};
struct CCameraModeDef : CDefVirtual {
    long m_count;                           // +4
};
struct CDefPointer_CCameraModeDef {
    CCameraModeDef* m_p;
    CDefPointer_CCameraModeDef(const CDefPointer_CCameraModeDef& other) : m_p(other.m_p)
    {
        if (m_p)
            ++m_p->m_count;
    }
    ~CDefPointer_CCameraModeDef()
    {
        if (m_p) {
            if (--m_p->m_count == 0)
                m_p->Destroy();
        }
    }
};

namespace NCameraModes {
struct CScriptModuleBase {
    virtual void v00();
    virtual void v01();
    virtual bool AllowsManualControl();     // slot 2 (+8)
};
}
struct SCountBlock;
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
};

struct CGameCameraManager {
    void*         __vftable;                                            // +0x00
    unsigned char _pad04[0xd0];                                         // +0x04
    CCountedPointer<NCameraModes::CScriptModuleBase> PCurrentScriptModule; // +0xd4
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule; // +0xdc
    unsigned char _pade4[0x1c];                                         // +0xe4
    CBaseIntelligentPointer PObservedThing;                             // +0x100
    unsigned char _pad108[0x0c];                                        // +0x108 CameraList
    CDefPointer_CCameraModeDef* CameraListIndex;                        // +0x114
    unsigned char _pad118[4];                                           // +0x118 InitialModeCameraListIndex
    CDefPointer_CCameraModeDef* SpecialCameraList_first;                // +0x11c SpecialCameraList (first / last / end)
    unsigned char _pad120[8];
    CDefPointer_CCameraModeDef* SpecialCameraListIndex;                 // +0x128
    bool          UsingCameraManually;                                  // +0x12c
    bool          BlockModeChange;                                      // +0x12d
    void PreparePendingMode(CDefPointer_CCameraModeDef def);            // 0x00698840
    void PushCurrentManualMode();
};

void CGameCameraManager::PushCurrentManualMode()
{
    BlockModeChange = false;
    UsingCameraManually = true;
    if (PObservedThing.GetPItem() && !(PObservedThing.GetPItem()->m_statusFlags & 1)) {
        if (!PPendingScriptModule.m_pObject) {
            if (!PCurrentScriptModule.m_pObject || PCurrentScriptModule.m_pObject->AllowsManualControl()) {
                CDefPointer_CCameraModeDef def = *CameraListIndex;
                PreparePendingMode(def);
                SpecialCameraListIndex = SpecialCameraList_first;
            }
        }
    }
}
