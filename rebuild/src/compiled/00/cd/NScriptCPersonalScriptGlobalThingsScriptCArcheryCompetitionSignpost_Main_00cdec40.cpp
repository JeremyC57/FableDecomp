// NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Main() -- archery competition signpost: show "come back later", then each frame mirror the master data's competition state onto the sign text (open / come back later / closed) until the script is done
// Leaf: ?Main@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDEC40, length 325 bytes (plain `ret`; vtable slot 1 = Main of the entity
//          script contract).
// Callees: CCharString(const char*, long = -1) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CEntityScriptBase::IsDone @ 0x00F35B30,
//          the game interface's slots 7 NewScriptFrame and 714 SetReadableObjectTextTag (cgsi_slots_aligned.tsv); literals
//          "TEXT_QST_004_SIGNPOST_COME_BACK_LATER" 0x012C327C, "TEXT_QST_004_SIGNPOST_OPEN" 0x012C3260,
//          "TEXT_QST_004_SIGNPOST_CLOSED" 0x012C3240.
// Shape: the tag temporaries are implicit const char* -> CCharString conversions; the state dispatch is an if / else-if chain
//          over a named copy of the shown state, in the order 0, 2, 1 (three temp slots); the frame loop is a rotated `while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) { ...; NewScriptFrame(); }`.
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18; the master data's
//          shown state +0x3c and competition state +0x40 (descriptive names).
// Naming wave 35: identity Anniversary wave 2026-11-04 (high): vtable 0x012C3208 slot 1; Anniversary RTTI NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost (string-literal match, validated 257/258); PDB name set has the Main member of NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost; slot layout from named TLC siblings; function referenced by this vtable only (2026-11-04_anniversary/labels_high.tsv); PDB ?Main@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CArcheryCompetitionSignpost@@UAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
#include "engine/C3DVector.h"

#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
long __fastcall GFCharStringToInt(const CCharString& text);                                                   // 0x0099E7F0
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CQ_ArcheryCompetitionMasterData {
    unsigned char _pad[0x3c];
    long ShownState;                                     // +0x3c
    long CompetitionState;                               // +0x40
};
struct CQ_ArcheryCompetitionScript;

class CEntityScriptBase { public:   // the PDB class the accessor belongs to
    bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)
};
namespace NScript {
class CPersonalScript_GlobalThingsScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CArcheryCompetitionSignpost;
};
}
using NScript::CPersonalScript_GlobalThingsScript;
namespace NScript {
class CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost {
public:
    virtual ~CArcheryCompetitionSignpost();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing Me;                                     // +0x08
    CQ_ArcheryCompetitionScript* ParentClass;            // +0x14
    CQ_ArcheryCompetitionMasterData* PMasterData;        // +0x18
private:
    virtual void Main();                          // PDB ?Main@CArcheryCompetitionSignpost@CPersonalScript_GlobalThingsScript@NScript@@EAEXXZ
};
}

void NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Main()
{
    Game->SetReadableObjectTextTag(Me, "TEXT_QST_004_SIGNPOST_COME_BACK_LATER");
    PMasterData->CompetitionState = 2;
    while (!((const CEntityScriptBase*)this)->IsActiveThreadTerminating()) {
        if (PMasterData->ShownState != PMasterData->CompetitionState) {
            if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
                return;
            PMasterData->ShownState = PMasterData->CompetitionState;
            long state = PMasterData->ShownState;
            if (state == 0) {
                if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
                    return;
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_004_SIGNPOST_OPEN");
            } else if (state == 2) {
                if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
                    return;
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_004_SIGNPOST_COME_BACK_LATER");
            } else if (state == 1) {
                if (((const CEntityScriptBase*)this)->IsActiveThreadTerminating())
                    return;
                Game->SetReadableObjectTextTag(Me, "TEXT_QST_004_SIGNPOST_CLOSED");
            }
        }
        Game->NewScriptFrame();
    }
}