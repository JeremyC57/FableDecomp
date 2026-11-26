// CInputProcessSpecialAbilities::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- with abilities enabled and a live hero holding the abilities component: an assignable special-move button triggers the assigned ability, game action 0x39 cycles the assigned abilities (mouse-wheel steps of 120 or a held button) and action 0x5e cycles forward; then hero state 0xb queues game event 0x40 (outside mode 0x28) and state 0x10 with a named state queues game event 0x3b (outside modes 0x23 and 2)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690030, 805 bytes, `ret 8`, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CPlayer::GetPControlledCreature 0x00487DC0 / PeekPControlledCreature 0x00487DD0 / GetCurrentMode 0x00633BE0, LowerBound
//   0x0040F020 (non-const, interface 0x6f) and 0x004365B0 (const, interface 0xa7), CInputProcessGameBase 0x00687A90/0x00687BD0/
//   0x00687C70/0x00687CF0/0x00687D40, GetAssignableSpecialMoveIndexForEvent 0x00445FA0, OnSpecialAbilityButtonPressed 0x005EED4C,
//   CycleAssignedAbilities 0x005F64DD, CTCSpecialAbilities::IsChargingUpAbility 0x0072A860 / GetCurrentAbilityStrafeAnim 0x0072B870,
//   CCharString operator!= 0x0099E960 / dtor
//   0x0099EAE0, AddGameEvent 0x0041649C on CMainGameComponent::Get() 0x013B86A0, SetAsPerformedEvent 0x00A0D390, _ftol 0x00BFEA70.
// Layout: engine/CInputProcessGameBase.h fields (MyPlayer +0x18, PGamePlayerInterface +0x24); CThing interface map +0x44,
//   presence bits +0x20, Dead bit +0x91 (retail CThing); the other members are descriptive offsets read from retail.
// Shape: SetAsPerformedEvent written in both arms of the scroll-loop if/else (VC tail-merges the calls, and the second use gives
//   processed esi over the countdown); the current-ability test is a switch with case 0x10 first; the strafe-anim result is a named local.
// Leaf: ProcessInput
// NF33 (2026-11-16): IsEventGameAction / GetControlMovementFromGameActionEvent respelled to the Ego_r.pdb + FableWin.pdb
//   CInputProcessGameBase spellings (EGameAction, const; the movement vector by reference).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /GS
#include "fable_compiler.h"
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
inline float Abs(float x) { return (float)fabs(x); }
#define FABLE_RETAIL_CCHARSTRING                    // NF59: fable_string.h's CCharString (~CCharString 0x0099EAE0, operator!=(const char*) 0x0099E960)
#include "fable_string.h"
class CInputEvent {
public:
    unsigned char _pad00[0x28];
    long          Type;                                 // +0x28 (descriptive; read inline)
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
#include "engine/EPlayerMode.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad54[0x3d];
    unsigned char Dead : 1;                        // +0x91 bit 0
    template<class T> FABLE_FORCEINLINE T* GetInterfaceUnchecked(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->end() || interfaceType < it->first ? map->end() : it)->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
// NF59 (round 7, queue q10499): the abilities component is the retail engine/CTCInventoryAbilities.h (opt-ins _ASSIGN:
//   CycleAssignedAbilities 0x005F64DD; _ONSPECIALABILITYBUTTONPRESSED: Ego_r.pdb 0x0057DBC3 + FableWin.pdb
//   ?OnSpecialAbilityButtonPressed@CTCInventoryAbilities@@QAEXJ@Z 0x005EED4C, the landed self row's leaf); the local model named the
//   0x005EED4C call TriggerSpecialAbility (a label no PDB has).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ONSPECIALABILITYBUTTONPRESSED
#include "engine/CTCInventoryAbilities.h"
// NF118 (PRIORITY q6369 / q6388 / q6389): interface 0xa7 is TCI_SPECIAL_ABILITIES (167): the descriptive CTCHeroState stand-in is the
//   retail engine/CTCSpecialAbilities.h. Its calls are Ego_r.pdb / FableWin.pdb ?IsChargingUpAbility@CTCSpecialAbilities@@QBE_NXZ 0x0072A860
//   (was CheckTermination) and ?GetCurrentAbilityStrafeAnim@CTCSpecialAbilities@@QBE?AVCCharString@@XZ 0x0072B870 (was GetName), and the
//   +0x10 read is the inline FableWin ?GetCurrentAbilityBeingUsed@CTCSpecialAbilities@@QBE?AW4EHeroAbility@@XZ (Ego_d.pdb carves its
//   COMDAT under input_process_special_abilities.obj, this TU).
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_ISCHARGINGUPABILITY
#define FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYSTRAFEANIM
#define FABLE_RETAIL_CTCSPECIALABILITIES_GETCURRENTABILITYBEINGUSED
#include "engine/CTCSpecialAbilities.h"
class CPlayer {
public:
    unsigned char _pad00[0x28];
    signed char   PlayerNumber;                    // +0x28 (descriptive)
    unsigned char _pad29[0x218 - 0x29];
    bool          AbilitiesEnabled;                // +0x218 (descriptive)
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;               // 0x00487DD0
    EPlayerMode GetCurrentMode() const;   /* ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ */                   // 0x00633BE0
};
class CGamePlayerInterface { public:
    long GetAssignableSpecialMoveIndexForEvent(const CInputEvent& event) const;   // 0x00445FA0
};
class CWorld; class CDefinitionManager;
#include "engine/EGameAction.h"
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CDefinitionManager*     DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;              // 0x00687A90; NF236: protected const (Ego_r / FableWin IBE)
    bool IsEventButtonReleased(const CInputEvent& event) const;                 // 0x00687BD0; NF236: protected const (Ego_r / FableWin IBE)
public:
    protected:
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event) const;
    public:
   // 0x00687C70
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;    // 0x00687CF0
    float GetMouseWheelMovement(const CInputEvent& event) const;             // 0x00687D40 (Ego_r + FableWin QBE; NF292, was GetEventScrollAmount)
};
class CInputProcessSpecialAbilities : public CInputProcessGameBase {
public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessSpecialAbilities::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (MyPlayer->AbilitiesEnabled && hero && !hero->Dead && (hero->m_presentBits[3] & 0x8000)) {
        CTCInventoryAbilities* abilities = hero->GetInterfaceUnchecked<CTCInventoryAbilities>(0x6f);
        CTCSpecialAbilities* state = 0;
        hero->GetInterface(&state, 0xa7);
        if (IsEventAssignableSpecialMoveButton(event)) {
            if (IsEventButtonPressed(event)) {
                abilities->OnSpecialAbilityButtonPressed(PGamePlayerInterface->GetAssignableSpecialMoveIndexForEvent(event));
                processed.SetAsPerformedEvent();
            } else if (IsEventButtonReleased(event)) {
                processed.SetAsPerformedEvent();
            }
        } else if (IsEventGameAction((EGameAction)(0x39), event)) {
            if (state && !state->IsChargingUpAbility()) {
                if (event.Type == 0xe) {
                    float amount = GetMouseWheelMovement(event);
                    float size = Abs(amount);
                    if (size > 0.0001f) {
                        long steps = (long)size / 120;
                        for (long i = 0; i < steps; ++i) {
                            if (amount > 0.0f) {
                                abilities->CycleAssignedAbilities(true);
                                processed.SetAsPerformedEvent();
                            } else {
                                abilities->CycleAssignedAbilities(false);
                                processed.SetAsPerformedEvent();
                            }
                        }
                    }
                } else if (IsEventButtonReleased(event)) {
                    abilities->CycleAssignedAbilities(true);
                    processed.SetAsPerformedEvent();
                }
            }
        } else if (IsEventGameAction((EGameAction)(0x5e), event) && state && !state->IsChargingUpAbility() && IsEventButtonPressed(event)) {
            abilities->CycleAssignedAbilities(true);
            processed.SetAsPerformedEvent();
        }
    }
    const CThing* creature = MyPlayer->PeekPControlledCreature();
    CTCSpecialAbilities* heroState;
    if (creature->GetInterface(&heroState, 0xa7)) {
        switch (heroState->GetCurrentAbilityBeingUsed()) {
        case 0x10: {
            long mode = MyPlayer->GetCurrentMode();
            if (mode != 0x23 && mode != 2) {
                CCharString name = heroState->GetCurrentAbilityStrafeAnim();
                if (name != "") {
                    CGameEvent gameEvent(0x3b);
                    gameEvent.Player = MyPlayer->PlayerNumber;
                    CMainGameComponent::Get()->AddGameEvent(gameEvent);
                }
            }
            break;
        }
        case 0xb:
            if (MyPlayer->GetCurrentMode() != 0x28) {
                CGameEvent gameEvent(0x40);
                gameEvent.Player = MyPlayer->PlayerNumber;
                CMainGameComponent::Get()->AddGameEvent(gameEvent);
            }
            break;
        }
    }
}