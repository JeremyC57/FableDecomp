// CPuzzleBlockMoon::TurnIntoASunSign(bool createEffect) -- flip a Fire Heart puzzle pad from moon to sun: remove the old pad effect (optionally replacing it with the sun pad effect), spawn the sun tile object carrying this pad's data string, count the sun, and when every pad shows a sun (and the round is not just beginning) win the round with the Fire Heart frame effect, the minigame-complete sound and a short pause; finally remove the moon block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38000, length 770 bytes, `ret 4`.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
// "PuzzleBlockSun" 0x012CCE70, "OBJECT_LPDD_TILE_SUN_01" 0x012CD0EC, "MK_FH_FIREHEART" 0x012CD0DC,
// "TILE_FRAME_SUN_COMPLETE" 0x012CD0C4, "SND_MM_MINIGAME_COMPLETE" 0x012CD0A8); ~CScriptThing 0x004AA840 (out of line);
// CCountedPointer::operator= 0x00704580; CQ_FireHeartScript::IncNumberOfSuns 0x00D38310; CScriptThing slots 3
// GetDataString, 4 SetDataString (inline on the local), 6 GetPos, 10 GetAngleXY, 75 IsAlive, 77 IsNull; CGameScriptInterface
// slots 72 GetThingWithScriptName, 97 CreateObject, 100 CreateEffect, 108 RemoveThing, 376 Pause, 692 Play2DSound.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, PadNumber +0x1c (Ego_r.pdb).
// Retail CQ_FireHeartScript = Ego_r.pdb less 0x14 (script base) and 4 per STLport container (as the landed
// SetupPuzzleBlocks 0x00D36140): NumberOfPads +0x7c, NumberOfSuns +0x80, PuzzleRound +0x88, RoundEnded +0x94,
// RoundBeginning +0x95, PreventRoundEnd +0x96, GoodRounds +0xa0, Effect +0xa8, PadSunFX +0xb4, ProphetsReleased +0xd4.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
};
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
    unsigned char  _pad97[0xa0 - 0x97];
    long           GoodRounds;                           // +0xa0
    float          PadRadius;                            // +0xa4
    CScriptThing   Effect;                               // +0xa8
    CCharString    PadSunFX;                             // +0xb4
    CCharString    PadMoonFX;                            // +0xb8
    unsigned char  _padbc[0xd4 - 0xbc];
    bool*          ProphetsReleased;                     // +0xd4 (vector<bool> _M_start, one byte per element)
    void IncNumberOfSuns();                              // 0x00D38310
};
}
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
struct CPuzzleBlockMoon : CEntityScriptBase {
    NScript::CQ_FireHeartScript* ParentClass;            // +0x14
    void*                        PMasterData;            // +0x18
    long                         PadNumber;              // +0x1c
    void TurnIntoASunSign(bool createEffect);
};

void CPuzzleBlockMoon::TurnIntoASunSign(bool createEffect)
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
        ParentClass->Effect.PImp = Game->CreateEffect(ParentClass->PadSunFX, Me.GetPos(), "", 0.0f, false, false).PImp;
    }
    CScriptThing block = Game->CreateObject("OBJECT_LPDD_TILE_SUN_01", Me.GetPos(), Me.GetAngleXY(), "PuzzleBlockSun");
    block.SetDataString(Me.GetDataString());
    ParentClass->IncNumberOfSuns();
    if (ParentClass->NumberOfSuns == ParentClass->NumberOfPads && !ParentClass->RoundBeginning) {
        if (IsActiveThreadTerminating())
            return;
        ++ParentClass->GoodRounds;
        Game->CreateEffect("TILE_FRAME_SUN_COMPLETE", Game->GetThingWithScriptName("MK_FH_FIREHEART").GetPos(), "", 0.0f, false, false);
        Game->Play2DSound("SND_MM_MINIGAME_COMPLETE");
        Game->Pause(0.2f);
        ParentClass->RoundEnded = true;
        ParentClass->ProphetsReleased[ParentClass->PuzzleRound] = true;
    }
    Game->RemoveThing(Me, false, true);
    ParentClass->PreventRoundEnd = false;
}