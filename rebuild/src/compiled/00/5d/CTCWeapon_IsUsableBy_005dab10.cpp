// CTCWeapon::IsUsableBy(const CThing& thing) const -- only creatures may use a weapon; class-1 things, a suppressed game, things without hero experience and weapon classes 0/1/3 always may; class 2 needs berserk or the experience requirements
// Leaf: ?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DAB10, 179 bytes, `ret 4`, then a 4-entry jump table at 0x005DABC4.
// Ego_r.pdb `?IsUsableBy@CTCWeapon@@QBE_NABVCThing@@@Z`.
// Callees: CTCBase::GetGameSubsystem() @ 0x00686D60 (`[0x013B8A1C]+0x1c`, unidentified; its virtual slot 9 (+0x24)
//          IsSuppressed() returns a bool -- landed spelling of 0x007B6A80 / 0x007E1F70); the creature getter @ 0x006A98D0
//          (returns the word at +0x1e8, object unidentified; manifest "CWorld::DrawGetEnvironment" is propagated);
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (interface 0x68 = CTCHeroExperience, bit 8
//          of the bitset dword at +0x2c); CTCWeapon::GetWeaponClass() const @ 0x005DA7F0 (manifest
//          "CCategory::GetCategoryIdentifier" is a byte-twin label); the requirement check @ 0x00851B50 on the getter
//          result's +0x13c sub-object with the hero-experience component (labelled "CAbility_CheckRequirements").
// Layout: CThing class byte +0x10, interface bitset +0x20 (CTCBerserk 0x84 = bit 4 of +0x30), components +0x44,
//          ThingTypeFlags +0x6c (bit 3: creature); CTCWeapon from engine/CTCWeapon.h (only GetWeaponClass is used).
// Shape: the creature test wraps the body and its single `return false` is shared with the switch's missing default
//          (that shared tail keeps all four pushes in the prologue; separate `return false`s sink ebp/esi past the
//          first exits); the getter is called before the interface test; the out-parameter GetInterface (its type
//          temporary reuses the dead argument slot); HasTC is __forceinline (plain inline is not expanded inside the
//          `||`); the class-2 arm returns an `||` (`mov eax,1` / `xor eax,eax` tails) while the other trues share `mov al,1`.
// Self respelled (NF177, PRIORITY q11929): Leaf was the stand-in ?IsUsableBy@CTCWeapon_Getters@@QBE_NABVCThing@@@Z; now the Ego_r.pdb 0x0063A950 /
//   FableWin.pdb 0x0239AF10 spelling the shared header's retail branch already declares (defined on CTCWeapon itself).
// Callee respelled (NF154, PRIORITY q4964): 0x00686D60 is ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ (Ego_r.pdb 0x006B2150 /
//   FableWin.pdb 0x02191560; engine/CTCBase.h FABLE_RETAIL_CTCBASE_COMPONENT), and its slot 9 is CMainGameComponent::IsEditorActive
//   (FableWin.pdb ?IsEditorActive@CMainGameComponent@@UBE_NXZ, 0x004197B0; engine/CMainGameComponent.h FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR);
//   was the stand-in ?GetGameSubsystem@CTCWeapon_Getters@@QAEPAUCGameSubsystem@@XZ with a local CGameSubsystem view (dropped). CTCWeapon is
//   now the header's polymorphic retail branch (FABLE_RETAIL_CTCWEAPON: CTCWeapon : CTCBase), so the protected accessor is reachable.
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "engine/CTCWeapon.h"
#include "engine/CMainGameComponent.h"

class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCHeroExperience;                       // interface 0x68
struct CExperienceRequirements {                // the +0x13c sub-object of the creature's +0x1e8 record
};
struct CCreatureRecord1E8 {                     // unidentified
    unsigned char           _pad00[0x13c];
    CExperienceRequirements Requirements;       // +0x13c
};
class CThingCreatureBase : public CThing { public:
    const CCreatureRecord1E8* GetRecord1E8() const;   // 0x006A98D0
};

enum EWeaponClass { WEAPON_CLASS_NONE = 0, WEAPON_CLASS_1 = 1, WEAPON_CLASS_2 = 2, WEAPON_CLASS_3 = 3 };

// 0x005DA7F0 keeps its landed stand-in spelling here (it is bound under three spellings by landed rows; PDB
// ?GetWeaponClass@CTCWeapon@@QBE?AW4EWeaponClass@@XZ is proposed in the NF177 hand-back NAME_FIXES.tsv), reached by a downcast.
struct CTCWeapon_Getters : CTCWeapon {
    EWeaponClass GetWeaponClass() const;        // 0x005DA7F0
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00851B50 ?IsValid@CHeroAbilityStatPackage@@QBE_NABVCTCHeroExperience@@@Z (was ?IsMetBy@CExperienceRequirements@@QBE_NPBVCTCHeroExperience@@@Z)
class CHeroAbilityStatPackage { public: bool IsValid(CTCHeroExperience const&) const; };
bool CTCWeapon::IsUsableBy(const CThing& thing) const
{
    if (thing.GetTypeFlags() & 8) {
        if (thing.GetType() == 1 || PeekComponent().IsEditorActive())
            return true;
        const CCreatureRecord1E8* record = static_cast<const CThingCreatureBase&>(thing).GetRecord1E8();
        const const CTCHeroExperience* experience;
        if (!thing.PeekTC(TCI_HERO_EXPERIENCE, &experience))
            return true;
        switch (static_cast<const CTCWeapon_Getters*>(this)->GetWeaponClass()) {
        case WEAPON_CLASS_NONE:
        case WEAPON_CLASS_1:
        case WEAPON_CLASS_3:
            return true;
        case WEAPON_CLASS_2:
            return thing.HasTC(TCI_BERSERK) || reinterpret_cast<const CHeroAbilityStatPackage*>(&(record->Requirements))->IsValid(*(experience));
        }
    }
    return false;
}