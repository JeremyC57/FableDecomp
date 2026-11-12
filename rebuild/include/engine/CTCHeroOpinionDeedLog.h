#ifndef FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
#define FABLE_ENGINE_CTCHEROOPINIONDEEDLOG_H
// New header (chunk 794). Retail opt-in only: FABLE_RETAIL_CTCHEROOPINIONDEEDLOG. No default path exists (no repo header before).
// Base: Fable.exe RTTI .?AVCTCHeroOpinionDeedLog@@ : CTCBase. Interface TCI_HERO_OPINION_DEED_LOG (0x51; OnHit 0x007039A0 tests
// [thing+0x28] & 0x20000 and looks the component up with key 0x51). Methods: Ego_r.pdb / FableWin.pdb spellings
//   ?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@_N5PBVCFaction@@5J@Z (0x006C6990)
//   ?InformRelevantThingsOfDeed@CTCHeroOpinionDeedLog@@AAE_NJPAVCThingAICreature@@W4EOpinionPostDeedType@@PBVCFaction@@_N@Z (0x006C6040)
// Layout (Ego_r.pdb struct_layouts_egor.tsv, retail STLport map/list shifts): kept opaque past CTCBase here.
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#include "engine/CTCBase.h"
#include "engine/EOpinionDeedType.h"
class C3DVector;
class CThing;
class CThingAICreature;
class CFaction;
class COpinionDeed;
class COpinionDeedEffectsDef;
enum EOpinionPostDeedType;
class CTCHeroOpinionDeedLog : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_OPINION_DEED_LOG; }
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);   // 0x006C6990
private:
    bool InformRelevantThingsOfDeed(long handle, CThingAICreature* creature, EOpinionPostDeedType postType,
                                    const CFaction* faction, bool b1);                                          // 0x006C6040
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
    // Nested opt-in (chunk 794, row 0x006C6040 InformRelevantThingsOfDeed): Ego_r.pdb OpinionDeedMap (std::map<long, COpinionDeed*>,
    // retail +0x0C: `lea esi,[edi+0Ch]` then the out-of-line find 0x006C7830) and
    // ?PeekPDeedEffectsDef@CTCHeroOpinionDeedLog@@QBE?AV?$CDefPointer@$$CBVCOpinionDeedEffectsDef@@@@W4EOpinionDeedType@@@Z (out of line,
    // 0x006C5630: hidden result + the type). Needs stlport/fable_map.h and engine/CDefPointer.h. The members after the map stay unmodelled.
public:
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
    // ?GetDeedFromHandle@CTCHeroOpinionDeedLog@@QBEPBVCOpinionDeed@@J@Z (Ego_r.pdb + FableWin.pdb), out of line at 0x006C58F0 (find
    // 0x006C7830, end -> NULL, else the mapped deed) and auto-inlined by its own TU (0x006C6040 copies the by-value handle first).
    const COpinionDeed* GetDeedFromHandle(long handle) const;                                     // 0x006C58F0
private:
    std::map<long, COpinionDeed*> OpinionDeedMap;                        // +0x0c
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
    // Retail opt-in (Codex pass18): Ego_r TPI19ce2 member order. Native004d3f51 allocates34a8;
    // dtor006c5d70 proves list+18, intelligent pointer+20 and92 reaction records of90 bytes.
    unsigned char OpinionDeedsSearchingForWitnesses[4]; // std::list<CWitnessesSearchingDeed>,+18
    long NextHandle; // +1c
    CIntelligentPointer<CThing> VillageForCrimes; // +20
    long WeaponOutDeedHandle; // +28
    bool WeaponOutCrimeEnabled; // +2c
    bool GuardsIgnoreCrimes; // +2d
    unsigned short DeedEffectsDefIndices[92]; // +2e
    unsigned char DeedReactions[13248]; // COpinionDeedReaction[92],+e8; each has18 CCharString then18 ulong.
#endif
#endif
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x006C8D20
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x006C8D30
public:
#endif
};
#ifdef FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
FABLE_STATIC_ASSERT(sizeof(CTCHeroOpinionDeedLog)==0x34a8);
#endif
#endif
#endif
