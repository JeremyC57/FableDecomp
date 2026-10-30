// CAS_Ambusher::Main() -- ambush scam ambusher: ally with the trickster (if it lives), target the hero, wait until the hero is within the ambush distance, play a random ambush sound on itself, then idle until the script is done
// Leaf: ?Main@CAS_Ambusher@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2FD30, length 360 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract; the landed CAS_Ambusher::Init is 0x00E2BC10).
// Callees: CV_AmbushScamScript::GetTrickster @ 0x00E2EC30 (CScriptThing by value through the hidden pointer, on ParentClass),
//          CScriptThing::IsAlive inline (slot 75 on the game-side thing), CEntityScriptBase::IsDone @ 0x00F35B30 (manifest
//          "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), the game interface's slots 599
//          EntitySetThingAsAllyOfThing, 70 GetHero, 451 GiveThingBestEnemyTarget, 7 NewScriptFrame, 690 PlaySoundOnThing
//          (cgsi_slots_aligned.tsv), IsDistanceBetweenThingsUnder @ 0x00CBE2FF (__fastcall: the two things in ecx/edx, the
//          float on the stack), rand @ 0x00BFEB16 (MSVCR71), CDefString::operator CCharString @ 0x00415D70, ~CCharString
//          @ 0x0099EAE0, ~CScriptThing @ 0x004AA840; data: pGlobalScriptDef @ 0x0143E90C (sound-name vector +0x2c0/+0x2c4,
//          ambush distance +0x2cc).
// Shape: the trickster is a frame local destroyed at both exits; the early `if (IsDone()) return;` sits before the rest of
//          the body, which lives in a __forceinline helper so VC7.1 pushes/pops ebx (the address of Me) inside that region
//          only; the two wait loops are rotated whiles (`while (!near) { frame; if done return; }`, then
//          `do frame while (!done)`); the sound index is `rand() % size()` with the vector size as a pointer difference.
// Layout: Ego_r.pdb CAS_Ambusher (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18. CScriptThing per the landed script model (CBaseClass + CCountedPointer, 12 bytes).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
class CDefString { public:                                      // 4 bytes: a def-side string
    void* PStringData;
    operator CCharString() const;                        // 0x00415D70
};
extern "C" int __cdecl rand();                           // 0x00BFEB16 (MSVCR71)

class CCPPointerInfo;                                   // CCountedPointer's shared block
template <class T> class CCountedPointer { public:               // destroyed by the out-of-line ~CScriptThing here
    T*              Data;
    CCPPointerInfo* Info;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    virtual ~CScriptThing();                                     // 0x004AA840 (both exits call it)
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x2c0];
    CDefString* AS_AmbushSoundsBegin;                    // +0x2c0 vector<CDefString> (retail STLport, 12 bytes)
    CDefString* AS_AmbushSoundsEnd;                      // +0x2c4
    CDefString* AS_AmbushSoundsEndOfStorage;             // +0x2c8
    float       AS_AmbushDistance;                       // +0x2cc
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CV_AmbushScamScript {
    CScriptThing GetTrickster();                         // 0x00E2EC30
};
struct CQ_SunnyvaleMasterData;

struct CAS_Ambusher {
    virtual ~CAS_Ambusher();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CV_AmbushScamScript* ParentClass;                    // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
    bool IsDone();                                       // 0x00F35B30
    __forceinline void RunAmbush();
};

// Target the hero, wait for it to come near, shout, then idle until the script ends.
__forceinline void CAS_Ambusher::RunAmbush()
{
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->AS_AmbushDistance)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    unsigned long count = pGlobalScriptDef->AS_AmbushSoundsEnd - pGlobalScriptDef->AS_AmbushSoundsBegin;
    unsigned long index = rand() % count;
    {
        const CCharString& sound = pGlobalScriptDef->AS_AmbushSoundsBegin[index];
        Game->PlaySoundOnThing(Me, sound);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}

void CAS_Ambusher::Main()
{
    CScriptThing trickster = ParentClass->GetTrickster();
    if (trickster.IsAlive()) {
        if (IsDone())
            return;
        Game->EntitySetThingAsAllyOfThing(Me, trickster);
        Game->EntitySetThingAsAllyOfThing(trickster, Me);
    }
    RunAmbush();
}