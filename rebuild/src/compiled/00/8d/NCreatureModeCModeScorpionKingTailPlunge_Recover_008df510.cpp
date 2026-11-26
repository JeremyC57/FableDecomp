// NCreatureMode::CModeScorpionKingTailPlunge::Recover() -- end the tail plunge: kill the live tail emitter, spawn the def's TAIL_DUST_EFFECT special effect at the tail position, kill the creature's dependent things (interface 0xAD) and set a CCreatureAction_ScorpionTailWithdraw towards the target as the creature's current action
// Leaf: ?Recover@CModeScorpionKingTailPlunge@NCreatureMode@@AAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DF510, 304 bytes incl. int3 padding, `ret` (private, thiscall).
// Identity: catalog "CTCLightningOrb_OnUpdate" (_global) is wrong: the PDB name set's
//   `?Recover@CModeScorpionKingTailPlunge@NCreatureMode@@AAEXXZ`, called from the landed FrameUpdate 0x008E0D20 (+0x3AE),
//   which already names 0x008DF510 Recover.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, CDefClassBase::GetSubDef<
//   CSpecialEffectsDef> 0x00669580 (catalog "<CAICreatureWillPowerIndicatorDef>" propagated), CSpecialEffectsDef::
//   GetEffectFromString 0x00669D30 (catalog "CCRC::CalcAndLookup"), CCharString 0x0099EBF0 / ~ 0x0099EAE0, CreateEffectThing
//   0x006E0880 (__fastcall), CVectorMap::LowerBound 0x0040F020 (inline GetTC 0xAD), the 0xAD component's 0x007AEF60
//   (catalog "CTCBoss::ClearEffects"; it kills every live thing of its 8-byte intelligent-pointer vector +0x3C/+0x40:
//   descriptive KillDependentThings), CCreatureAction_ScorpionTailWithdraw ctor 0x008E45B0 (catalog
//   "CCreatureAction_BriarRoseShotFarInto"; it stores vtable 0x01286334 = RTTI ScorpionTailWithdraw),
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900 (catalog "~CTCLightningOrb"); the
//   effects def's virtual slot 1 on the last release.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge (packed, as the landed FrameUpdate / Spike): PCreature +0x84, TailPos +0x95,
//   PTarget +0xA1, PTailEmitter +0xA9; CModeBase ParentThing +4; CThing def +0x70, dead bit 0 of +0x91. Retail
//   CCreatureAction_ScorpionTailWithdraw is 0x120 bytes (CCombatActionBase 0x114 + C3DVector TargetPos; Ego_r 0x138).
// Shape: the landed Spike 0x008DA0F0 model: same-TU out-only stand-ins for GetSubDef / GetEffectFromString (the effects
//   def lives in ebp after the call, its slot reused by the TAIL_DUST_EFFECT temporary), a counted out pointer released at
//   the function end, GetTCIfPresent (`xor ecx,ecx` default) and IS_THING_ALIVE.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCountedDefPointer / CCharString / CDefVirtual / CDefBase / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Lookup_CSpecialEffectsDef_GetEffectFromString / Find_CDefClassBase_GetSubDef callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/C3DVector.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCBoss { public:                                  // interface type 0xAD (descriptive: the dependent-things component)
public:
    void ClearEffects();   // 0x007AEF60
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad54[0x1c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
    FABLE_FORCEINLINE bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE T* GetTCIfPresent(long interfaceType)
    {
        T* component = 0;
        if (HasTC(interfaceType))
            component = GetTC<T>(interfaceType);
        return component;
    }
};
class CCombatActionBase;
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x006644F0 ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z (was ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCombatActionBase@@@Z)
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase const&);   // 0x006644F0
};
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long a0, C3DVector const&a1, bool a2);   // 0x006E0880 (Ego_r.pdb ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_ScorpionTailWithdraw : public CCombatActionBase { public:   // retail sizeof 0x120
    C3DVector TargetPos;                        // +0x114 (retail sizeof 0x120; Ego_r +0x12C of 0x138)
    CCreatureAction_ScorpionTailWithdraw(CThingCreatureBase* creature, CThing* target);   // 0x008E45B0
};

struct CModeBase {
    virtual ~CModeBase();
    CThing* ParentThing;                        // +0x04
};
namespace NCreatureMode {
#pragma pack(push, 1)
class CModeScorpionKingTailPlunge : public CModeBase { public:
    unsigned char       _pad08[0x84 - 0x08];
    CThingCreatureBase* PCreature;              // +0x84
    bool                bRecovering;            // +0x88
    C3DVector           TargetPos;              // +0x89
    C3DVector           TailPos;                // +0x95
    CIntelligentPointer<CThing> PTarget;            // +0xa1
    CIntelligentPointer<CThing> PTailEmitter;       // +0xa9
    CIntelligentPointer<CThing> PDustTrailEmitter;  // +0xb1
    private:
    void Recover();
    public:
};
#pragma pack(pop)

void CModeScorpionKingTailPlunge::Recover()
{
    if (IS_THING_ALIVE(PTailEmitter))
        PTailEmitter.GetPItem()->Kill(true);
    CDefPointer<const CSpecialEffectsDef> effects;   // NF49: the shared handle (was a local CCountedDefPointer)
    long effectType = 0;
    ParentThing->PDef->GetSubDef(&effects);
    if (effects.Get()) {
        if (effects.Get()->GetEffectFromString("TAIL_DUST_EFFECT", effectType))
            CTCDParticleEmitter::Create(effectType, *(&TailPos), false);
    }
    PCreature->GetTCIfPresent<CTCBoss>(0xad)->ClearEffects();
    PCreature->SetCurrentAction(*reinterpret_cast<CCreatureActionBase const*>(&(CCreatureAction_ScorpionTailWithdraw(PCreature, PTarget.GetPItem()))));
}
}