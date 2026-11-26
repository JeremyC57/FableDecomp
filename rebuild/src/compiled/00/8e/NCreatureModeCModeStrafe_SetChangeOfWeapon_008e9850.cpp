// NCreatureMode::CModeStrafe::SetChangeOfWeapon(CThing* weapon) -- take the new weapon's id and animation group into the strafe filter ("STAND" and -1 with no weapon thing), rebuild the combination anims, recompute the aiming frame count / centre from the projectile-aim animation's length, then clear the strafing combination anims
// Leaf: ?SetChangeOfWeapon@CModeStrafe@NCreatureMode@@QAEXPAVCThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E9850, 251 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (const instantiation; inlined HasTC + GetTC for
//   CTCWeapon 0x48), CTCWeapon::GetAnimationGroupName 0x005DABE0, CAnimFilterOfGroup::SetCurrentGroupName(const CCharString&)
//   0x0073DFF0 (out of line at the first site, inlined at the second), CCharString(const char*, long) 0x0099EBF0 /
//   operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, CCRC::Calc 0x004014A0 (`__fastcall`), CModeStrafe::
//   SetUpCombinationAnims 0x008E9190, the mode's global-component getter 0x0073A680 (reads [0x013B8A1C]+0x24; unused
//   `this`), the animation bank lookup 0x00A242C0 (index -> record, length in seconds at +0x10), __ftol2 0x00BFEA70,
//   CModeStrafe::ClearStrafingCombinationAnims(bool) 0x008E9710.
// Globals: the frames-per-second long 0x01375550.
// Layout: Ego_r.pdb CModeStrafe (0x70, matches retail; as the landed sibling UpdateWeaponState 0x008E9950): Filter +0x2c
//   (Group +0x30, GroupCRC +0x34), ActiveWeapon +0x38, AimingNumFrames +0x3c, AimingCenterFrame +0x40,
//   PAnimProjectileAim +0x50 (CCountedPointer<CAnimEntryLite>, the entry's first word is its animation index).
// Shape: HasTC + one-ternary GetTC (key homed in the dead `weapon` slot); the aim entry is a named local held across
//   the getter call (edi), the components a named local and the bank an inline accessor (`mov eax,[eax+3Ch] ; push ;
//   mov ecx,eax`); the centre re-reads AimingNumFrames.
// Naming (NF126, 2026-11-20): the local CModeBase stand-in replaced by the shared nested CTCCreatureModeManager::CModeBase
//   (engine/CTCCreatureModeManager.h, FABLE_RETAIL_CMODEBASE_VIRTUALS); the stand-in CGlobalComponents (GetAnimBank) is the world: engine/CWorld.h with the NF126 overlay opt-in FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK (FableWin ?GetMeshBank@CWorld@@QAEPAVCMeshDataBank@@XZ).
#include "fable_compiler.h"
class CTCBase;

// Naming (NF205, 2026-11-20, q11692 / q12295): the local CCharString / CCRC / CAnimFilterOfGroup models replaced by fable_string.h and
//   engine/CAnimFilterOfGroup.h with the NF128 inline FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE (FableWin.pdb
//   ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z, 0x01DD3450): the first site calls the out-of-line copy 0x0073DFF0,
//   the second expands it; the CRC reads CCharString::NullChar for an empty string, as the owner 0x0073DFF0 does.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE
#include "fable_string.h"
#include "engine/CAnimFilterOfGroup.h"

#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];           // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;               // +0x44
    unsigned char  _pad54[0x38];
    unsigned short m_weaponId;                 // +0x8c (DefGlobalIndex)
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE const T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        const CInterfaceMap::value_type* sel = (it == map->end() || interfaceType < it->first) ? map->end() : it;
        return (const T*)sel->second;
    }
};

class CTCWeapon { public:                             // interface type 0x48
    const CCharString& GetAnimationGroupName() const;   // 0x005DABE0
};

class C3DAnimationInfo { public:
    unsigned char _pad00[0x10];
    float         Seconds;                     // +0x10
};
class CMeshDataBank { public:
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long) const;   // 0x00A242C0
};

namespace NAnimationEntryBodge {
class CAnimEntryLite { public:
    long AnimIndex;                            // +0
};
}
#include "engine/CCountedPointer.h"

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#define FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK
#include "engine/CWorld.h"
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {

struct CModeStrafe : CTCCreatureModeManager::CModeBase {               // vtable 0x01268588
    float              AnimMovementDistance;   // +0x1c
    void*              StrafeAnimName;         // +0x20 (CCharString)
    long               AnimIndex;              // +0x24
    bool               LastUsable;             // +0x28
    CAnimFilterOfGroup Filter;                 // +0x2c
    long               ActiveWeapon;           // +0x38
    long               AimingNumFrames;        // +0x3c
    long               AimingCenterFrame;      // +0x40
    float              RumbleParam;            // +0x44
    float              PullBackAmount;         // +0x48
    bool               ForceAnimNoBlendIn;     // +0x4c
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite> PAnimProjectileAim;   // +0x50
    private:
    void SetUpCombinationAnims();
    public:
              // 0x008E9190
    private: void ClearStrafingCombinationAnims(bool force); public:   // 0x008E9710
    void SetChangeOfWeapon(CThing* weapon);
};

void CModeStrafe::SetChangeOfWeapon(CThing* weapon)
{
    if (weapon && weapon->HasTC(0x48)) {
        const CTCWeapon* tc = weapon->GetTC<CTCWeapon>(0x48);
        ActiveWeapon = weapon->m_weaponId;
        Filter.SetCurrentGroupName(tc->GetAnimationGroupName());
    } else {
        ActiveWeapon = -1;
        Filter.SetCurrentGroupName("STAND");
    }
    SetUpCombinationAnims();
    NAnimationEntryBodge::CAnimEntryLite* aim = PAnimProjectileAim.Data;
    AimingNumFrames = 0;
    if (aim) {
        CWorld& world = GetWorld();
        AimingNumFrames = (long)(CMainGameComponent::GetConstantFPS() * world.GetMeshBank()->PeekAnimation(aim->AnimIndex)->Seconds);
    }
    AimingCenterFrame = AimingNumFrames / 2;
    ClearStrafingCombinationAnims(false);
}

}