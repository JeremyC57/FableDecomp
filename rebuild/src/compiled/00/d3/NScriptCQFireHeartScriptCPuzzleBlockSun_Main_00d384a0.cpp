// NScript::CQ_FireHeartScript::CPuzzleBlockSun::Main() -- the Fireheart sun pad block: wait until Me's data string is set and take it as the pad number, wait until the hero is 1.5 pad radii away, then until the mission succeeds: when the parent resets the blocks, turn into a moon sign if the starting pattern does not mark this pad, count the reset, wait for the reset to end and for the hero to step away again; otherwise, when the hero steps onto the pad between rounds, play the multiplier-lock sound and turn into a moon sign
// Leaf: ?Main@CPuzzleBlockSun@CQ_FireHeartScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D384A0, 753 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the entity-script vtable 0x012CCEF4, which the landed
//   NScript::CQ_FireHeartScript::CPuzzleBlockSun::Alloc 0x00D36470 stores (slot 0 is the scalar deleting dtor); Ego_r.pdb 0x00D46B20
//   ?Main@CPuzzleBlockSun@CQ_FireHeartScript@NScript@@EAEXXZ (after TurnIntoAMoonSign 0x00D46740, the private member this
//   row calls twice). Catalog label: sub_00d384a0 (gap scan).
// Callees: CScriptThing::GetDataString (slot 3), CCharString::operator!=(const char*) 0x0099E960, ~CCharString 0x0099EAE0,
//   CCharString(const char*, long) 0x0099EBF0, GFCharStringToInt 0x0099E7F0, IsDistanceBetweenThingsOver 0x00CBE3EA /
//   IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30,
//   CPuzzleBlockSun::TurnIntoAMoonSign 0x00D387A0; CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero, 690 PlaySoundOnThing
//   (ghidra_out/cgsi_slots_aligned.tsv). Every spelling is the Ego_r.pdb one.
// Literals: "NULL" 0x012393E4, "SND_COMBAT_MULITIPLIERLOCK_04" 0x012CD130; the 1.5 double 0x012925E0.
// Layout: Ego_r.pdb CPuzzleBlockSun (0x20, no STL members, no shift): CEntityScriptBase {vptr, Game +4, Me +8},
//   ParentClass +0x14, PMasterData +0x18, PadNumber +0x1c. CQ_FireHeartScript at retail offsets (Ego_r.pdb - 0x14 up to the two
//   maps, - 0x1c after them; the landed SetupPuzzleBlocks 0x00D36140 uses the same): MissionSucceeded +0x49, ResetBlocks +0x8c,
//   NumBlocksReset +0x90, RoundEnded +0x94, RoundBeginning +0x95, PadRadius +0xa4, StartingPattern +0xbc (retail vector<bool>
//   stores one byte per element).
// Shape: the rotated script wait idiom (`while (!cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` then a
//   termination test); `PadRadius * 1.5` is a double product narrowed to the float argument; the trailing termination test
//   compiles to a bare call.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail STLport vector, 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](unsigned int n) { return *(_M_start + n); }
};
}
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_FireHeartScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    CQ_SunnyvaleMasterData* MasterScriptData;           // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool MissionSucceeded;                              // +0x49
    bool MissionFailed;                                 // +0x4a
    unsigned char _maps04c[0x18];                       // +0x4c CutsceneFlags, +0x58 csargs (STLport maps, 12 bytes each)
    long FreedProphets;                                 // +0x64
    long KilledProphets;                                // +0x68
    long StartNumberOfSuns;                             // +0x6c
    long NumberOfRounds;                                // +0x70
    long NumberOfPatterns;                              // +0x74
    long NumberOfProphets;                              // +0x78
    long NumberOfPads;                                  // +0x7c
    long NumberOfSuns;                                  // +0x80
    long NumberOfVillagers;                             // +0x84
    long PuzzleRound;                                   // +0x88
    bool ResetBlocks;                                   // +0x8c
    long NumBlocksReset;                                // +0x90
    bool RoundEnded;                                    // +0x94
    bool RoundBeginning;                                // +0x95
    bool PreventRoundEnd;                               // +0x96
    bool HasHeroHitVillagersGroupOne;                   // +0x97
    bool HasHeroHitVillagersGroupTwo;                   // +0x98
    bool InfoDisplayed;                                 // +0x99
    long EvilRounds;                                    // +0x9c
    long GoodRounds;                                    // +0xa0
    float PadRadius;                                    // +0xa4
    CScriptThing Effect;                                // +0xa8
    CCharString PadSunFX;                               // +0xb4
    CCharString PadMoonFX;                              // +0xb8
    std::vector<bool> StartingPattern;                  // +0xbc
    class CPuzzleBlockSun : public CEntityScriptBase {
    public:
        CQ_FireHeartScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
        long PadNumber;                                 // +0x1c
    private:
        virtual void Main();                            // slot 1
        void TurnIntoAMoonSign(bool);                                 // 0x00D387A0
    };
};

void CQ_FireHeartScript::CPuzzleBlockSun::Main()
{
    while (!(Me.GetDataString() != "NULL")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PadNumber = GFCharStringToInt(Me.GetDataString());
    while (!IsDistanceBetweenThingsOver(Me, Game->GetHero(), ParentClass->PadRadius * 1.5)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass->MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->ResetBlocks) {
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass->StartingPattern[PadNumber]) {
                if (IsActiveThreadTerminating())
                    return;
                TurnIntoAMoonSign(false);
            }
            ParentClass->NumBlocksReset++;
            while (ParentClass->ResetBlocks) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!IsDistanceBetweenThingsOver(Me, Game->GetHero(), ParentClass->PadRadius * 1.5)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), ParentClass->PadRadius)
                 && !ParentClass->RoundEnded && !ParentClass->RoundBeginning) {
            if (IsActiveThreadTerminating())
                return;
            Game->PlaySoundOnThing(Me, "SND_COMBAT_MULITIPLIERLOCK_04");
            TurnIntoAMoonSign(true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}   // namespace NScript