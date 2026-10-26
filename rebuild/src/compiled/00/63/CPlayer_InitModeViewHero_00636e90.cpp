// CPlayer::InitModeViewHero(EPlayerMode, CProcessedInput*) -- attach the view-hero input processes, reset the GUI's spirit health display and hide its bottom half, then push a CMoveOnCylinder camera mode built from the CAMERA_MODE_PC_VIEW_HERO definition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00636E90, length 299 bytes, `ret 8` (a CPlayer::PlayerModeInfo InitFunc).
// Callees: CAInputProcess::AddToInterface(CGamePlayerInterface*) (virtual slot 1, x3), CPlayerGui forwarder 0x004378E0
// (PDrawSpiritsHealth +0x160 -> 0x00643EA2; propagated labels) on g_pPlayerGui 0x013B8790, CCharString(const char*,
// long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0 (static `__fastcall`),
// CDefinitionManager::GetDef<CCameraModeDef> 0x00637650, the world's +0x34 object's slot 9 GetCameraManager (+0x24),
// operator new 0x00BFEA1A (the 0xd0-byte mode and the 12-byte count block), NCameraModes::CBase(CGameCameraManager*,
// const CDefPointer<const CCameraModeDef>&) 0x0084E420 (CMoveOnCylinder vtable 0x0125A064 stored after it), the
// counted pointer's destroy function 0x0056C0F0, CGameCameraManager::PushCameraMode(CCountedPointer<CBase>)
// 0x006987A0, the def's virtual slot 1 (the long release). Literal "CAMERA_MODE_PC_VIEW_HERO" 0x0125A0C4.
// Layout: engine/CPlayer.h (PlayerInterface +0x0C, World +0x1C, the scoped_ptr<CInputProcess*> members) through the
// CPlayer_Methods carrier; retail CPlayerGui DisplayBottomHalfGUI +0x330 (PDB +0x35c less the 12/4-byte containers).
// Shape: the landed SetSnapToFirstCamPointMode (0x0069E5D0) by-value CCountedPointer built in the argument area and
// the landed DecRef/GetRefCount pointee Release (0x00BBDCB0) for the CDefPointer destructor; the camera manager out
// pointer is not initialised; the mode's constructor is __forceinline inside the new-expression.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
class CProcessedInput;
void* operator new(unsigned int size);                 // 0x00BFEA1A
void __fastcall Destroy_0056c0f0(void* object);        // 0x0056C0F0

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

class CPlayerGui { public:
    unsigned char _pad000[0x330];
    bool          DisplayBottomHalfGUI;                // +0x330
    void ResetDrawSpiritsHealth();                     // 0x004378E0
};
extern CPlayerGui* g_pPlayerGui;                       // 0x013B8790

struct CDefPointee {                                   // a counted definition
    virtual void v00();
    virtual void DeleteThis() const;                   // slot 1 (+0x04)
    mutable long RefCount;                             // +0x04
    void DecRef() const { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            DeleteThis();
    }
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) { }
    ~CDefPointer()
    {
        if (Data)
            Data->Release();
    }
};
class CCameraModeDef : public CDefPointee { public: };

class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();  // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<const CCameraModeDef>& def);   // 0x00637650
};

struct SCountBlock {
    long References;                                   // +0
    void (__fastcall* Destroy)(void*);                 // +4
    void* Object;                                      // +8
    SCountBlock(void* object) : References(1), Destroy(Destroy_0056c0f0), Object(object) { }
};
template <class T> class CCountedPointer { public:
    T* Data;
    SCountBlock* Info;
    CCountedPointer(T* p) : Data(p)
    {
        if (Data)
            Info = new SCountBlock(Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
};

class CGameCameraManager;
namespace NCameraModes {
    class CBase { public:
        CBase(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def);   // 0x0084E420
        virtual ~CBase();
        unsigned char _body[0xd0 - 4];
    };
    struct CMoveOnCylinder : CBase {                   // vtable 0x0125A064
        __forceinline CMoveOnCylinder(CGameCameraManager* manager, const CDefPointer<const CCameraModeDef>& def) : CBase(manager, def) { }
        virtual ~CMoveOnCylinder();
    };
}
class CGameCameraManager {
public:
    void PushCameraMode(CCountedPointer<NCameraModes::CBase> mode);   // 0x006987A0
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                // +0x34
};

struct CPlayer_Methods : CPlayer {
    void InitModeViewHero(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeViewHero(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBButtonExitMode_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessToggleViewHeroMode_ptr)->AddToInterface(PlayerInterface);
    g_pPlayerGui->ResetDrawSpiritsHealth();
    g_pPlayerGui->DisplayBottomHalfGUI = false;
    CDefPointer<const CCameraModeDef> def;
    CGameDefinitionManager::Get().GetDef("CAMERA_MODE_PC_VIEW_HERO", def);
    CGameCameraManager* camera;
    ((CWorld*)World)->CameraSource->GetCameraManager(&camera);
    camera->PushCameraMode(CCountedPointer<NCameraModes::CBase>(new NCameraModes::CMoveOnCylinder(camera, def)));
}