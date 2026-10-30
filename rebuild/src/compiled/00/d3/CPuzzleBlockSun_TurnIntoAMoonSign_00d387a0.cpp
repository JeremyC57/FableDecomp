// CPuzzleBlockSun::TurnIntoAMoonSign(bool createEffect) -- flip a Fire Heart puzzle pad from sun to moon: remove the old pad effect (optionally replacing it with the moon pad effect), spawn the moon tile object carrying this pad's data string, uncount the sun, and when no pad shows a sun (and the round is not just beginning) lose the round to evil with the Fire Heart frame effect, the minigame-complete sound and a short pause; finally remove the sun block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D387A0, length 768 bytes, `ret 4`.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
// "PuzzleBlockMoon" 0x012CCE80, "OBJECT_LPDD_TILE_MOON_01" 0x012CD16C, "MK_FH_FIREHEART" 0x012CD0DC,
// "TILE_FRAME_MOON_COMPLETE" 0x012CD150, "SND_MM_MINIGAME_COMPLETE" 0x012CD0A8); ~CScriptThing 0x004AA840 (out of line);
// CCountedPointer::operator= 0x00704580; CQ_FireHeartScript::DecNumberOfSuns 0x00D38AA0; CScriptThing slots 3
// GetDataString, 4 SetDataString (inline on the local), 6 GetPos, 10 GetAngleXY, 75 IsAlive, 77 IsNull; CGameScriptInterface
// slots 72 GetThingWithScriptName, 97 CreateObject, 100 CreateEffect, 108 RemoveThing, 376 Pause, 692 Play2DSound.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, PadNumber +0x1c (Ego_r.pdb).
// Retail CQ_FireHeartScript = Ego_r.pdb less 0x14 (script base) and 4 per STLport container (as the landed
// SetupPuzzleBlocks 0x00D36140): NumberOfPads +0x7c, NumberOfSuns +0x80, PuzzleRound +0x88, RoundEnded +0x94,
// RoundBeginning +0x95, PreventRoundEnd +0x96, EvilRounds +0x9c, Effect +0xa8, PadMoonFX +0xb8, ProphetsKilled +0xc8.
// Shape: the sibling of CPuzzleBlockMoon::TurnIntoASunSign 0x00D38000 with the moon strings and the evil-round counters.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
class C3DVector { public: float X, Y, Z; };
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline void CScriptThing::SetDataString(const CCharString& data) { if (PImp.Data) PImp.Data->SetDataString(data); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_FireHeartScript { public:                              // retail-offset view (see Layout)
    unsigned char  _pad00[0x7c];
    long           NumberOfPads;                         // +0x7c
    long           NumberOfSuns;                         // +0x80
    long           NumberOfVillagers;                    // +0x84
    long           PuzzleRound;                          // +0x88
    unsigned char  _pad8c[0x94 - 0x8c];
    bool           RoundEnded;                           // +0x94
    bool           RoundBeginning;                       // +0x95
    bool           PreventRoundEnd;                      // +0x96
    unsigned char  _pad97[0x9c - 0x97];
    long           EvilRounds;                           // +0x9c
    long           GoodRounds;                           // +0xa0
    float          PadRadius;                            // +0xa4
    CScriptThing   Effect;                               // +0xa8
    CCharString    PadSunFX;                             // +0xb4
    CCharString    PadMoonFX;                            // +0xb8
    unsigned char  _padbc[0xc8 - 0xbc];
    bool*          ProphetsKilled;                       // +0xc8 (vector<bool> _M_start, one byte per element)
    void DecNumberOfSuns();                              // 0x00D38AA0
};
}
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CPuzzleBlockSun : CEntityScriptBase {
    NScript::CQ_FireHeartScript* ParentClass;            // +0x14
    void*                        PMasterData;            // +0x18
    long                         PadNumber;              // +0x1c
    void TurnIntoAMoonSign(bool createEffect);
};

void CPuzzleBlockSun::TurnIntoAMoonSign(bool createEffect)
{
    ParentClass->PreventRoundEnd = true;
    if (ParentClass->RoundEnded) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->PreventRoundEnd = false;
        return;
    }
    if (!ParentClass->Effect.IsNull() && ParentClass->Effect.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(ParentClass->Effect, false, true);
    }
    if (createEffect) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->Effect.PImp = Game->CreateEffect(ParentClass->PadMoonFX, Me.GetPos(), "", 0.0f, false, false).PImp;
    }
    CScriptThing block = Game->CreateObject("OBJECT_LPDD_TILE_MOON_01", Me.GetPos(), Me.GetAngleXY(), "PuzzleBlockMoon");
    block.SetDataString(Me.GetDataString());
    ParentClass->DecNumberOfSuns();
    if (ParentClass->NumberOfSuns == 0 && !ParentClass->RoundBeginning) {
        if (IsActiveThreadTerminating())
            return;
        ++ParentClass->EvilRounds;
        Game->CreateEffect("TILE_FRAME_MOON_COMPLETE", Game->GetThingWithScriptName("MK_FH_FIREHEART").GetPos(), "", 0.0f, false, false);
        Game->Play2DSound("SND_MM_MINIGAME_COMPLETE");
        Game->Pause(0.2f);
        ParentClass->RoundEnded = true;
        ParentClass->ProphetsKilled[ParentClass->PuzzleRound] = true;
    }
    Game->RemoveThing(Me, false, true);
    ParentClass->PreventRoundEnd = false;
}