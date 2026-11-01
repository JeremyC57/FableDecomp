// CTCVillage::SetGuardFollowingPlayer(CThingAICreature* guard) -- point the village's GuardFollowingPlayer intelligent pointer (+0xbd) at the guard through a converting CIntelligentPointer<CThingAICreature> temporary
// Leaf: ?SetGuardFollowingPlayer@CTCVillage@@QAEXPAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090C780, 74 bytes, thiscall, `ret 4`: sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ;
//   call CBaseIntelligentPointer() ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov [esp+8],0x0123E7E0 ; call SetPItem ;
//   lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0BDh] ; call SetPItem ; lea ecx,[esp+4] ;
//   call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4.
// Identity (proven): the only caller is CAIStateGroup_MinionGuardFollowPlayer::Update 0x0090C540 (vtable 0x01288DCC slot 12):
//   at 0x0090C5EC..0x0090C5F5 it calls CTCVillageMember::GetPVillage() 0x007EE540 (landed, FableWin
//   ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ) and calls this on the returned CTCVillage with the state group's
//   creature, right after reading GetPItem of the same village's +0xBD pointer (0x0090C5CD). Ego_r.pdb CTCVillage has
//   GuardFollowingPlayer (CIntelligentPointer<CThingAICreature>) at +0x149 = retail +0xBD (17 STLport lists 12 -> 4, one set
//   16 -> 12: -0x8C); Ego_r.pdb and FableWin.pdb name ?SetGuardFollowingPlayer@CTCVillage@@QAEXPAVCThingAICreature@@@Z
//   (beside ?GetGuardFollowingPlayer@CTCVillage@@QAEPAVCThingAICreature@@XZ). The temporary's vtable 0x0123E7E0 is
//   CIntelligentPointer<CThingAICreature> (rtti-vtables.tsv CIntelligentPointer; reloc_bindings). The manifest label
//   "CVillageTaskCarryCrate::AssignCreatureToTask" is wrong (no CVillageTaskCarryCrate member reaches +0xBD).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, SetPItem 0x00A01B90 (x2), GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10 (the temporary's implicit ~CIntelligentPointer<CThingAICreature> expands to it).
// Layout: engine/CTCVillage.h (generated, retail offsets) gives TeacherTeaching +0xBC and GuardFollowingPlayer +0xBD as an
//   opaque byte run and flattens the vptr, so CTCVillage is modelled locally over the header's offsets. The odd +0xBD in an
//   otherwise naturally aligned class (bools before longs are padded, Ego_r and retail alike) means the intelligent-pointer
//   classes themselves pack to 1, so only they sit inside #pragma pack(1). engine/CTCBase.h flattens the vptr into a data
//   member, so the base is the polymorphic local model of the landed CTC rows (CTCBase::~CTCBase 0x00686830).
// Shape: `GuardFollowingPlayer = guard;` -- the implicit converting constructor builds the temporary (base ctor, then its
//   vtable store scheduled after the argument push, then SetPItem), operator= copies GetPItem into the member's SetPItem,
//   and the temporary dies (landed twin 0x006F5720 CCombatAction_GenericStrikeResponseBase::SetHitter, same body).
#include <stddef.h>
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingAICreature;
class CDefPointeeBase;

#pragma pack(push, 1)                                      // genuine: GuardFollowingPlayer sits at the odd offset +0xBD
class CBaseIntelligentPointer {                            // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject* item);                      // 0x00A01B90
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>: vtable 0x0123E7E0
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

class CTCBase {                                            // CTCBase : CBaseClassNonCopyable : CBaseClass (0xC); engine/CTCBase.h flattens the vptr
public:
    virtual ~CTCBase();                                    // 0x00686830
    CThing* ParentThing;                                   // +0x04
    bool    ToBeRemoved;                                   // +0x08
    bool    IsInGlobalUpdateTCs;                           // +0x09
};

class CTCVillage : public CTCBase {
public:
    void SetGuardFollowingPlayer(CThingAICreature* guard);
    unsigned char _members0c[0xbc - 0x0c];                 // TorchTasksUpdateFrame +0x0C .. InitialSecuritySweepRadius +0xB8
    bool          TeacherTeaching;                         // +0xBC
    CIntelligentPointer<CThingAICreature> GuardFollowingPlayer;   // +0xBD
    long          NextFrameToFollowPlayer;                 // +0xC8
    bool          LightActiveStateToChange;                // +0xCC
    long          MaxFrameToAssignTask;                    // +0xD0
    long          HeroEnemyOfGuardsUntil;                  // +0xD4
    long          LastFrameApologised;                     // +0xD8
    CIntelligentPointer<CThingAICreature> GuardWarningHero;   // +0xDC
    bool          CurrentIsHeroCriminal;                   // +0xE4
};

typedef char CTCVillageLayoutCheck[(offsetof(CTCVillage, GuardFollowingPlayer) == 0xbd && offsetof(CTCVillage, NextFrameToFollowPlayer) == 0xc8 && offsetof(CTCVillage, GuardWarningHero) == 0xdc && sizeof(CTCVillage) == 0xe8) ? 1 : -1];

void CTCVillage::SetGuardFollowingPlayer(CThingAICreature* guard)
{
    GuardFollowingPlayer = guard;
}