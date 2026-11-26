// CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase&) -- combat action 0x91 with no sounds and Level 0; animation ST_MULTI_ARROW_CAST; load SPECIAL_ABILITIES_MULTI_ARROW_DEF, play its cast sound through the creature's sound component and spawn its CAST_EFFECT particle effect at the creature, attached to the creature's left hand
// Leaf: ??0CCreatureAction_CastSkillMultiArrow
// Naming (NF457, r454b): the constructor takes `CThingCreatureBase&`, ??0CCreatureAction_CastSkillMultiArrow@@QAE@AAVCThingCreatureBase@@@Z as both PDB name sets (Ego_r + FableWin) spell it (was ??0CCreatureAction_CastSkillMultiArrow@@QAE@PAVCThingCreatureBase@@@Z); a reference passes the same pointer, so the object is unchanged.
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE6F0, length 522 bytes (+ int3 padding), `ret 4`, no EH frame.
// Callees: CCombatActionBase ctor 0x00858030, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0, CGameDefinitionManager::Get 0x0044C6B0 (__fastcall),
//   GetDefIndexFromName 0x009AD390, GetDef<> 0x007CC8E0, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CDefStringTable::GetString 0x009D49B0 (GDefStringTable
//   0x013CA828), the sound component's PlaySound 0x0066BD00, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CreateParticleEffect 0x0072AB70 (__fastcall),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0 (bone 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME 0x013BAC34).
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18): Level +0x114 (PDB +0x12c); the multi-arrow def's cast sound
//   (CDefString) at +0x60; CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
// Shape: the landed one-argument-longer sibling 0x008CE900 model (same-TU GetDef / GetSubDef / GetEffectFromString /
//   LowerBound stand-ins so every out slot lands in the dead argument slots). The new tail is a single && chain
//   `effect && !IsDead() && GetInterface(&emitter, 0x4b)`: a separate HasTC test before GetInterface tests the bit twice.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BAC34 SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME (global, const; initializer 0x0121E620), was g_LeftHandAttachmentName.
#include "fable_compiler.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefVirtual / CDefBase / CDefPointer / CSpecialEffectsDef / CDefClassBase models with same-TU stand-in bodies over made-up Lookup_CSpecialEffectsDef_GetEffectFromString / Find_CDefClassBase_GetSubDef callees): the call at
// 0x00669580 is the member template ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z
// (Ego_r.pdb / FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10
// idiom), and CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body
// (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY, the 0x007941D0 idiom), so the caller's handles stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY   // NF49: before the first CDefinitionManager.h include (see the manager note below)
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CTCBase;
#include "engine/C3DVector.h"
class CDefString { public: long Index; };
class CDefStringTable { public:
    CCharString GetString(long index) const;           // 0x009D49B0 Ego_r/FableWin ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (agentNF16)
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)              // 0x013CA828
class CSpecialAbilitiesMultiArrowDef : public CDefClassBase { public:
    unsigned char _pad25[0x60 - 0x25];   // (the shared CDefClassBase is 0x25 bytes, packed)
    CDefString    CastSound;                          // +0x60
};
// NF49: the manager calls come from engine/CGameDefinitionManager.h (were local CDefinitionManager / CGameDefinitionManager
// models with a same-TU stand-in GetDef body over a made-up FindDef_CGameDefinitionManager_GetDef callee): GetDef 0x007CC8E0 is the
// member template ??$GetDef@VCSpecialAbilitiesMultiArrowDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSpecialAbilitiesMultiArrowDef@@@@@Z
// (FableWin.pdb), its body visible and kept out of line (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY);
// FABLE_RETAIL_CGAMEDEFINITIONMANAGER drops the default-path GetDef that would hide it.
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CGameDefinitionManager.h"
class CTCSoundPlayer { public:
    unsigned long PlayAttachedCriteria(CCharString const&a0, bool a1, bool a2, bool a3, long a4);   // 0x0066BD00 (Ego_r.pdb ?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z)
};
#pragma auto_inline(on)
struct CTCSound {                                     // the interface-0x10 component
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;             // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    bool HasTC(long interfaceType) const { return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0; }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {};
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long a, float b);   // 0x006E0BE0
};
extern const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME;          // 0x013BAC34
// NF49 (round 7): CreateParticleEffect 0x0072AB70 is the public static __fastcall
// ?CreateParticleEffect@CTCSpecialAbilities@@SIPAVCThing@@JABVC3DVector@@@Z (Ego_r.pdb / FableWin.pdb), declared by
// engine/CTCSpecialAbilities.h; was a free stand-in taking `const C3DVector*` (same register passing: ecx = effect, edx = &pos).
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#include "engine/CTCSpecialAbilities.h"
class CSoundPair { public:
    long First, Second;
    CSoundPair() : First(0), Second(0) { }
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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase {
public:
    // NF123 (2026-11-20, q6404): creature by reference, Ego_r.pdb ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (0x00858030; was PAV).
    CCombatActionBase(CThingCreatureBase& creature, long type, const CSoundPair& sounds);   // 0x00858030
    virtual void v00();
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_CastSkillMultiArrow : public CCombatActionBase {
public:
    long Level;                                       // +0x114 (PDB +0x12c)
    CCreatureAction_CastSkillMultiArrow(CThingCreatureBase& creature);
    virtual void v00();
};

CCreatureAction_CastSkillMultiArrow::CCreatureAction_CastSkillMultiArrow(CThingCreatureBase& creature)
    : CCombatActionBase(creature, 0x91, CSoundPair()), Level(0)
{
    SetCombatAnimation("ST_MULTI_ARROW_CAST");
    CDefPointer<const CSpecialAbilitiesMultiArrowDef> def;
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_MULTI_ARROW_DEF");
        index = ((CDefinitionManager*)&(CGameDefinitionManager::Get()))->GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &def);
    CTCSound* sound;
    if (GetCreature()->GetInterface(&sound, 0x10)) {
        CCharString soundName = GDefStringTable.GetString(def->CastSound.Index);
        ((CTCSoundPlayer*)sound)->PlayAttachedCriteria(soundName, 1, 0, 0, -1);
    }
    CDefPointer<const CSpecialEffectsDef> effects;
    long effectType = 0;
    if (def->GetSubDef(&effects)) {
        if (effects->GetEffectFromString("CAST_EFFECT", effectType)) {
            CThing* effect = CTCSpecialAbilities::CreateParticleEffect(effectType, GetCreature()->GetPos());
            CTCDParticleEmitter* emitter;
            if (effect && !effect->IsDead() && effect->GetInterface(&emitter, 0x4b))
                emitter->AttachToThing(*GetCreature(), 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0);
        }
    }
}