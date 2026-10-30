// CBanditCronies::Main() -- bandit crony: register itself with the parent script, wait a frame, then wait until the hero is within 13m, shout a random crony sound, target the hero and idle until the script is done
// Leaf: ?Main@CBanditCronies@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E03810, length 339 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CScriptThing copy ctor @ 0x004ABE90 and ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter's member; the
//          filter vtable 0x012C32F0 is stored inline), CEntityScriptBase::SetPredicate @ 0x00F35B10 (manifest
//          "CWorldMap::AllocateMapThingUID" is propagated; landed CHTEPrisoner_Main / CSUMMONEDCREATURE_Main model),
//          CEntityScriptBase::IsDone @ 0x00F35B30, the game interface's slots 7 NewScriptFrame, 70 GetHero,
//          451 GiveThingBestEnemyTarget, 690 PlaySoundOnThing (cgsi_slots_aligned.tsv), IsDistanceBetweenThingsUnder
//          @ 0x00CBE2FF (__fastcall), rand @ 0x00BFEB16, CDefString::operator CCharString @ 0x00415D70, CCharString copy
//          ctor @ 0x0099EC30, ~CCharString @ 0x0099EAE0; data: pGlobalScriptDef @ 0x0143E90C (sound-name vector +0x3ac/+0x3b0).
// Shape: the alive filter is a scoped 16-byte frame object (landed CHTEPrisoner::Main model); `NewScriptFrame; if (IsDone())
//          return;` precedes the rotated `while (!near) { frame; if done return; }`; the sound is a CCharString LOCAL (operator
//          CCharString temp + copy ctor), destroyed after the final `do frame while (!done)` loop; ebx = &Me for the whole body.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; CScriptThing per the landed script model (12 bytes).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
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
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    virtual ~CScriptThing();                                     // 0x004AA840
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
    unsigned char _pad[0x3ac];
    CDefString* BC_ShoutSoundsBegin;                     // +0x3ac vector<CDefString> (retail STLport, 12 bytes)
    CDefString* BC_ShoutSoundsEnd;                       // +0x3b0
    CDefString* BC_ShoutSoundsEndOfStorage;              // +0x3b4
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
class CIsThingAlive { public:                                   // vtable 0x012C32F0: the entity script's alive-filter over Me
    virtual void v00();
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

struct CBanditCronies {
    virtual ~CBanditCronies();
    virtual void Main();                                 // slot 1
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    void RegisterFilter(CIsThingAlive& filter);          // 0x00F35B10 (CEntityScriptBase::SetPredicate)
    bool IsDone();                                       // 0x00F35B30
};

void CBanditCronies::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 13.0f)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    unsigned long count = pGlobalScriptDef->BC_ShoutSoundsEnd - pGlobalScriptDef->BC_ShoutSoundsBegin;
    unsigned long index = rand() % count;
    CCharString sound = pGlobalScriptDef->BC_ShoutSoundsBegin[index];
    Game->PlaySoundOnThing(Me, sound);
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    do {
        Game->NewScriptFrame();
    } while (!IsDone());
}