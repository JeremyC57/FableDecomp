// CPlayer::InitModeFirstPersonTargeting(EPlayerMode, CProcessedInput*) -- attach the first-person targeting input processes and, for a living hero, put its creature mode manager into mode 25 (unless already there) and push the special camera mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635A50, length 224 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?InitModeFirstPersonTargeting@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a
// CPlayer::PlayerModeInfo InitFunc; UninitModeFirstPersonTargeting is 0x00635B30).
// Callees: CAInputProcess::AddToInterface(CGamePlayerInterface*) (virtual slot 1, four processes), CPlayer::GetHeroThing
// 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface, type 0x31 = the
// creature mode manager, bit 17 of +0x24), CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (the mode's type is
// virtual slot 1), CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10, the world's +0x34 object's slot 9
// (GetCameraManager out-parameter, +0x24), CGameCameraManager::PushSpecialCameraMode 0x00698F50.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C) through the lane's
// CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44, dead bit +0x91 & 1.
// Shape: the mode manager is zero-initialised and filled by the out-parameter GetInterface (`xor edi,edi` before the
// bitset test, the component left in edi); the camera manager out pointer is zero-initialised in its frame word and
// the getter's bool guards the push.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
class CProcessedInput;
class CTCBase;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CMode {
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
};
class CTCCreatureModeManager { public:                         // interface 0x31
    CMode* GetCurrentMode();                            // 0x0073A6E0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};

class CGameCameraManager { public:
    void PushSpecialCameraMode();                       // 0x00698F50
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void InitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessFirstPersonTargeting_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->AddToInterface(PlayerInterface);
    CThing* hero = GetHeroThing();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetInterface(&modes, 0x31);
        if (modes->GetCurrentMode()->GetModeType() != 25)
            modes->AddMode(25);
        CGameCameraManager* camera = 0;
        if (World->CameraSource->GetCameraManager(&camera))
            camera->PushSpecialCameraMode();
    }
}