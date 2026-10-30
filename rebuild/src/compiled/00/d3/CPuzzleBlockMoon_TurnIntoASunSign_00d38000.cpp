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
struct CScriptThingImp {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03();
    virtual void SetDataString(const CCharString& data);  // slot 4 (+0x10)
};
class CScriptThing { public:                                    // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                             // 0x004AA840
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;           // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data) { if (PImp.Data) PImp.Data->SetDataString(data); }   // slot 4 (+0x10)
    virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28)
    virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const;
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
    virtual bool IsDead() const;                         // slot 76 (+0x130)
    virtual bool IsNull() const;                         // slot 77 (+0x134)
};
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
class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
};
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