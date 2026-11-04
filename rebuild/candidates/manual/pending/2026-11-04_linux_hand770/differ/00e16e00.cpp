// NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Main() -- White Balverine guard 1: take scripted control of the guard, walk it to its post, then until the White Balverine quest is finished keep it running to the hero whenever the White Balverine is within 30 m; finally remove it
// Grade: DIFFER(1120v1064) -- best masked dl=22: the CIsThingConscious predicate destructor at entry expands its member ~CScriptThing inline (retail: inline predicate dtor, member ~CScriptThing called out of line 0x004AA840; the landed same-family rows reached it only with the inline-depth-0 pragma, now forbidden), and two loop-alignment pads shift. Tried: scoped vs temporary predicate, header CIsThingConscious (virtual dtor -> whole dtor called out of line), __forceinline virtual dtor, late inline ~CScriptThing definition, out-of-line ~CScriptThing (dl=150), implicit (compiler-generated) predicate dtor (a10, dl=52). Next idea: the retail mechanism for a nested call inside an expanded inline dtor (INLINE_DEPTH_REVISIT.md family).
// Leaf: ?Main@CWB_Guard1@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E16E00, 1064 bytes (int3 padded to 1072), plain `ret`.
// Identity (two sources): slot 1 (Main) of the vtable 0x012E0B60 that CWB_Guard1::Alloc 0x00E16D10 (landed) stores, between the
//   landed CWB_Guard1 ??_G 0x00E16DE0 / Init 0x00E16DC0 / GetParentScript 0x00E16DB0 slots; Ego_r.pdb / FableWin.pdb
//   ?Main@CWB_Guard1@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ; string "WB_Guard1Pos". Its byte-identical twin 0x00E17320 is
//   CWB_Guard2::Main (vtable 0x012E0B7C of CWB_Guard2::Alloc 0x00E17230, string "WB_Guard2Pos").
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (and inline), SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject() 0x0099A380 / ~ 0x0099A430, CScriptGameResourceObjectScriptedThingBase IsValid
//   0x00CD23B9 / Release 0x00CD2770 / MoveToPosition 0x007E72F0 / GetScriptThing 0x007E7490 / IsPerformingScriptTask 0x007E7450 /
//   ~ 0x007E74D0, IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceFromThingToPositionOver 0x00CBE45C, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, operator delete 0x00BFE9BC; Game slots 7, 8, 70, 72, 108, 512.
// Layout: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18 (CQ_SunnyvaleMasterData,
//   WhiteBalverineFinished +0xe1: Ego_r.pdb, no containers before it).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
class C3DVector { public: float X, Y, Z; };
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
namespace NScript {
class CPredicateBase
{
public:
    virtual bool operator()() const;                     // slot 0 (0x00CDDAB0)
};
}
struct CIsThingConscious : public NScript::CPredicateBase {    // NScript predicate, vtable 0x012C2FE8
    CScriptThing Thing;                                  // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) {}
    ~CIsThingConscious() {}
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterface.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);          // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);  // 0x00CBE45C

namespace NScript { class CQ_WhiteBalverineKnotholeGladeScript; } using NScript::CQ_WhiteBalverineKnotholeGladeScript;
struct CQ_SunnyvaleMasterData {
    unsigned char _pad00[0xe1];
    bool          WhiteBalverineFinished;                // +0xe1
};

class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript {
public:
    class CWB_Guard1;
};
class CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1 : public CEntityScriptBase {
public:
    CQ_WhiteBalverineKnotholeGladeScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*               PMasterData;   // +0x18
private:
    virtual void Main();
};
}

void NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_Guard1::Main()
{
    {
        CIsThingConscious predicate(Me);
        SetPredicate(predicate);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsValid())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, (EScriptAIPriority)3)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetCutsceneBehaviour(Me, (ECutsceneBehaviour)1);
    CScriptThing post = Game->GetThingWithScriptName("WB_Guard1Pos");
    handle.MoveToPosition(post.GetPos(), 3.0f, (EScriptEntityMoveType)1, false, true);
    while (!IsDistanceBetweenThingsUnder(Me, post, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!PMasterData->WhiteBalverineFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        CScriptThing balverine = Game->GetThingWithScriptName("WB_WhiteBalverine");
        if (IsDistanceBetweenThingsUnder(Me, balverine, 30.0f)) {
            if (IsActiveThreadTerminating())
                return;
            C3DVector heroPos = Game->GetHero().GetPos();
            while (IsDistanceFromThingToPositionOver(handle.GetScriptThing(), heroPos, 2.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
                handle.MoveToPosition(heroPos, 0.0f, (EScriptEntityMoveType)1, false, true);
                while (handle.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, false, true);
}
