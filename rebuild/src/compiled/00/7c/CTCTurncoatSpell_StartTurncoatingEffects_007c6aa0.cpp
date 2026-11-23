// CTCTurncoatSpell::StartTurncoatingEffects() -- while the target lives: start the looping turncoat sound, then look up the hero and enemy turncoating effects in the spell's effect table and attach a fresh particle effect of each to the caster and to the target
// Leaf: ?StartTurncoatingEffects@CTCTurncoatSpell@@AAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C6AA0, 762 bytes (+ int3 padding), plain `ret`.
// Callees: CBaseIntelligentPointer() 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10; LowerBound
//   0x0040F020 (interfaces 0x10 sound player, 0x4B particle emitter); CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00; the spell def's
//   effect-table lookup 0x00669580 (manifest label "GetSubDef<CAICreatureWillPowerIndicatorDef>" is propagated) and the
//   table's named lookup 0x00669D30 (manifest label "CCRC::CalcAndLookup"); CThing::GetPos() const 0x004C73D0;
//   CreateParticleEffect 0x0072AB70 (`__fastcall`); CTCDParticleEmitter::AttachToThing 0x006E0BE0; CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0. CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout (retail CTCTurncoatSpell = Ego_r.pdb less 4 after the timer): PDef +0x40, PTarget +0x44, LoopingSound +0x50,
//   TurncoatingEffectOnMe +0x74, TurncoatingEffectOnTarget +0x7c.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CEffectTableDef / CEffectTablePointer / CSpellDefPointer models with same-TU stand-in bodies over made-up GetSubDefIndex / GetEffectTableFromGlobalIndex callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/C3DVector.h"
class CTCBase;
class CBaseObject;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in taking `const C3DVector*` (same register passing: ecx = effect, edx = &pos).
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
FABLE_FORCEINLINE bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}
class CTCDParticleEmitter { public:                            // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
class CTCSoundPlayer {
public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
private:    // 0x0066BD00
};

#include "engine/CSpecialAbilitiesTurncoatSpellDef.h"   // NF49: the shared def class (was a local stand-in with a GetSubDef member)

class CTCTurncoatSpell { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x40 - 0x08];
    CDefPointer<const CSpecialAbilitiesTurncoatSpellDef> PDef;   // +0x40
    CIntelligentPointer<CThing> PTarget;                // +0x44
    long LevelOfSpell;                                  // +0x4c
    unsigned long LoopingSound;                         // +0x50
    unsigned char _pad54[0x74 - 0x54];
    CIntelligentPointer<CThing> TurncoatingEffectOnMe;      // +0x74
    CIntelligentPointer<CThing> TurncoatingEffectOnTarget;  // +0x7c
    private:
    void StartTurncoatingEffects();
    public:
};

void CTCTurncoatSpell::StartTurncoatingEffects()
{
    if (!IS_THING_ALIVE(PTarget))
        return;
    CTCSoundPlayer* sound;
    if (ParentThing->GetTC(TCI_SOUND_PLAYER, &sound))
        LoopingSound = ((CTCSoundPlayer*)sound)->PlayAttachedCriteria("HERO_ABILITY_TURNCOAT_CONTINUOUS", 0, 0, 0, -1);
    {   // own scope: the table/effect slots are then packed after the sound name's (retail F+0 table, F+4 effect/name)
        CDefPointer<const CSpecialEffectsDef> table;
        long effect = 0;
        if (PDef->GetSubDef(&table)) {
            if (table->GetEffectFromString("HERO_TURNCOATING_EFFECT", effect)) {
                TurncoatingEffectOnMe = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, ParentThing->GetPos()));
                if (IS_THING_ALIVE(TurncoatingEffectOnMe)) {
                    CTCDParticleEmitter* emitter;
                    ((CThing*)TurncoatingEffectOnMe.GetPItem())->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                    emitter->AttachToThing(*ParentThing, 8, "", 0, 0.0f);
                }
            }
            if (table->GetEffectFromString("ENEMY_TURNCOATING_EFFECT", effect)) {
                TurncoatingEffectOnTarget = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect, ((CThing*)PTarget.GetPItem())->GetPos()));
                if (IS_THING_ALIVE(TurncoatingEffectOnTarget)) {
                    CTCDParticleEmitter* emitter;
                    ((CThing*)TurncoatingEffectOnTarget.GetPItem())->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                    emitter->AttachToThing(*(((CThing*)PTarget.GetPItem())), 8, "", 0, 0.0f);
                }
            }
        }
    }
}