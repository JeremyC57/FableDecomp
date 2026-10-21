// CPlayer::UninitModeFirstPersonTargeting(EPlayerMode, CProcessedInput*) -- detach the first-person targeting input processes and, for a living hero, take its creature mode manager out of mode 25 and push the camera's current manual mode; then drop player mode 22
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635B30, length 192 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?UninitModeFirstPersonTargeting@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a
// CPlayer::PlayerModeInfo UninitFunc; InitModeFirstPersonTargeting is 0x00635A50).
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (four processes), CPlayer::GetHeroThing 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface, type 0x31 = the creature mode
// manager, bit 17 of +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, the world's +0x34 object's
// slot 9 (GetCameraManager out-parameter, +0x24), CGameCameraManager::PushCurrentManualMode 0x006988B0,
// CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C) through the lane's
// CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44, dead bit +0x91 & 1.
// Shape: the mode manager is zero-initialised and filled by the out-parameter GetInterface (`xor ecx,ecx` before the
// bitset test, the component loaded straight into ecx for its one use); the camera manager out pointer is
// zero-initialised in its frame word and the getter's bool guards the push; RemoveMode(22) runs on every path.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;
struct CTCBase;

struct CAInputProcess {
    void RemoveFromInterface();                         // 0x00A0D2D0
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CThing {
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
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CTCCreatureModeManager {                         // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};

struct CGameCameraManager {
    void PushCurrentManualMode();                       // 0x006988B0
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                  // 0x00634190
    void UninitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::UninitModeFirstPersonTargeting(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessFirstPersonTargeting_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->RemoveFromInterface();
    CThing* hero = GetHeroThing();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetInterface(&modes, 0x31);
        modes->RemoveMode(25);
        CGameCameraManager* camera = 0;
        if (World->CameraSource->GetCameraManager(&camera))
            camera->PushCurrentManualMode();
    }
    RemoveMode(22);
}