// CPlayer::InitModeClickPastText(EPlayerMode mode, CProcessedInput* input) -- entering the click-past-text mode: attach the click-past-text and cut-scene input processes to the player's interface, then cancel the live player character's current mutex special ability
// Leaf: ?InitModeClickPastText@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634890, 131 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B14C): the Init of
//   mode 14, whose Uninit is 0x006334F0. Ego_r.pdb's donor order (UninitModeControlCreature, UninitModeClickPastText,
//   UninitModeCutScene, InitModeFirstPerson ... InitModeClickPastText, InitModeCutScene, InitModeFreezeControls,
//   InitModeSpecialAbilityFreezeControls) lands on retail 0x00633420, 0x006334F0, 0x00633510, 0x00633530 ... 0x00634890,
//   0x00634920, 0x006349B0, 0x00634A30; the body attaches CPlayer::InputProcessClickPastText (+0x140), and its sibling
//   0x00634920 (InitModeCutScene) tests CPlayer::KeepAbilitiesDuringCutscenes (+0x224).
// Label correction: landed 0x006359C0 / 0x00633670 are named InitModeClickPastText / UninitModeClickPastText, but they attach
//   InputProcessYesNoQuestion (+0x144) and sit where the donor order puts InitModeYesNoQuestion / UninitModeYesNoQuestion
//   (mode 15): they are the YesNoQuestion pair (see NOTES.md).
// Model: the landed J104 row CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 (CThing::GetTC through the
//   STLport bitset reference proxy, three character fetches) with interface 0xA7 = CTCSpecialAbilities.
// Callees: AddToInterface (virtual slot 1) twice, CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three calls),
//   CVectorMap LowerBound 0x0040F020, CTCSpecialAbilities::CancelCurrentMutexAbility 0x0072B420.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)
class CTCSpecialAbilities : public CTCBase {                // partial view
public:
    void CancelCurrentMutexAbility();                     // 0x0072B420
};

class CThingPhysical : public CThing { };                   // RTTI CThingPlayerCreature <- CThingCreatureBase <- CThingGameObject
class CThingGameObject : public CThingPhysical { };         //   <- CThingPhysical <- CThing
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CInputProcessManager : public CBaseClassNonCopyable { };   // RTTI CGamePlayerInterface <- CInputProcessManager
class CGamePlayerInterface : public CInputProcessManager { };
class CAInputProcess : public CBaseClassNonCopyable {
public:
    virtual bool AddToInterface(CInputProcessManager& manager);   // slot 1 (+0x04)
};
class CInputProcessGameBase : public CAInputProcess { };
class CInputProcessClickPastText : public CInputProcessGameBase { };
class CInputProcessCutScene : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
private:
    void InitModeClickPastText(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeClickPastText(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessClickPastText_ptr->AddToInterface(*PlayerInterface);
    InputProcessCutScene_ptr->AddToInterface(*PlayerInterface);
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    if (character.GetPItem() && !character->IsDead()) {
        CTCSpecialAbilities* abilities;
        if (character->GetTC(TCI_SPECIAL_ABILITIES, &abilities))
            abilities->CancelCurrentMutexAbility();
    }
}