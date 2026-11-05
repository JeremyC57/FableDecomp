// NScript::CPersonalScript_alextestScript::CBurningPerson::Init() -- set the person's thing on fire through the game interface and reset the speech counter
// Leaf: ?Init@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EED580, length 29 bytes (+ int3 padding).
// Callees: the game interface at +0x04, virtual slot 477 (+0x774) with (&m_me, 1).
// Layout from Ego_r.pdb (CBurningPerson, 32 bytes): SpeakCounter +0x1c; the interface pointer at
// +0x04 and the CScriptThing at +0x08 belong to the script base the PDB leaves unnamed.
// Naming wave 35: identity reg 00eed3a7 "BurningPerson" in 00eed360; Alloc 00eed4d0; vtable 012f5874 slot 2; PDB ?Init@CBurningPerson@CPersonalScript_alextestScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CBurningPerson::Init (wave3/labels_alloc.tsv); PDB ?Init@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CBurningPerson@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.
#define SCRIPT_SLOT(n) virtual void slot##n()
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

struct CGameInterface {                         // only the slot used is typed
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28); SCRIPT_SLOTS10(29);
    SCRIPT_SLOTS10(30); SCRIPT_SLOTS10(31); SCRIPT_SLOTS10(32); SCRIPT_SLOTS10(33); SCRIPT_SLOTS10(34);
    SCRIPT_SLOTS10(35); SCRIPT_SLOTS10(36); SCRIPT_SLOTS10(37); SCRIPT_SLOTS10(38); SCRIPT_SLOTS10(39);
    SCRIPT_SLOTS10(40); SCRIPT_SLOTS10(41); SCRIPT_SLOTS10(42); SCRIPT_SLOTS10(43); SCRIPT_SLOTS10(44);
    SCRIPT_SLOTS10(45); SCRIPT_SLOTS10(46);
    SCRIPT_SLOT(470); SCRIPT_SLOT(471); SCRIPT_SLOT(472); SCRIPT_SLOT(473); SCRIPT_SLOT(474); SCRIPT_SLOT(475); SCRIPT_SLOT(476);
    virtual void SetThingOnFire(const CScriptThing& thing, long on);   // slot 477 (+0x774)
};

namespace NScript {
class CPersonalScript_alextestScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CBurningPerson;
};
}
using NScript::CPersonalScript_alextestScript;
namespace NScript {
class CPersonalScript_alextestScript::CBurningPerson {
public:
        CGameInterface* m_pGame;                    // +0x04
    CScriptThing    m_me;                       // +0x08
    unsigned char   _pad[8];                    // +0x14 ParentClass, +0x18 PMasterData
    long            SpeakCounter;               // +0x1c
private:
    virtual void Init();                          // PDB ?Init@CBurningPerson@CPersonalScript_alextestScript@NScript@@EAEXXZ
};
}

void NScript::CPersonalScript_alextestScript::CBurningPerson::Init()
{
    m_pGame->SetThingOnFire(m_me, 1);
    SpeakCounter = 0;
}