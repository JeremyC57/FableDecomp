// NTCScriptedControl::CActionTalkToThing::~CActionTalkToThing() -- implicit destructor: destroy QuestionButton3, QuestionButton2, QuestionButton1 (+0x9c, +0x98, +0x94), then CActionTalkBase::~CActionTalkBase (tail jump)
// Leaf: ??1CActionTalkToThing@NTCScriptedControl@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A570, 44 bytes: push esi ; mov esi,ecx ; lea ecx,[esi+9Ch] ; call 0x0099EAE0 ;
//   lea ecx,[esi+98h] ; call 0x0099EAE0 ; lea ecx,[esi+94h] ; call 0x0099EAE0 ; mov ecx,esi ; pop esi ; jmp 0x0071A430.
// Identity (proven): the only caller is 0x0071A550 (call at 0x0071A553), slot 0 (scalar deleting destructor) of vtable
//   0x012659C8, whose RTTI is .?AVCActionTalkToThing@NTCScriptedControl@@ (rtti-vtables.tsv); RTTI base
//   CActionTalkToThing : CActionTalkBase; the tail target 0x0071A430 is the landed ??1CActionTalkBase@NTCScriptedControl@@UAE@XZ,
//   whose own header comment already names this row as the CActionTalkToThing destructor. FableWin.pdb has the generated
//   ??1CActionTalkToThing@NTCScriptedControl@@UAE@XZ; Ego_r.pdb has only ??_E (no vtable store here: compiler-generated).
//   The manifest label "_Dest_val<std::allocator<CMacro>,CMacro>" is propagated from the twin family.
// Callees: CCharString::~CCharString 0x0099EAE0 (three times, reverse declaration order),
//   NTCScriptedControl::CActionTalkBase::~CActionTalkBase 0x0071A430 (tail jump).
// Layout: Ego_r.pdb CActionTalkToThing (168 bytes) : CActionTalkBase (152): AddedDuringCutscene +152, AskingHeroQuestion +153,
//   DisplayOverScreenFade +154, QuestionButton1/2/3 +156/+160/+164; retail CActionTalkBase is 0x90 (CArray = 12-byte
//   STLport vector; the landed 0x0071A430 model), so the bools sit at +0x90..+0x92 and the strings at +0x94/+0x98/+0x9C.
//   CActionTalkBase's members are not touched here: its chain (CTCScriptedControl::CActionBase : CBaseClassNonCopyable,
//   PTCScript / Activated / Terminated) is kept and the rest is one opaque block (full member list in 0x0071A430).
// Shape: the class declares no destructor; the implicit one is emitted out of line through a qualified explicit
//   destructor call in a one-line helper (EmitImplicitDestructor, not a retail function; as landed 0x0071A430).
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData;
class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};

class CTCScriptedControl {
public:
    class CActionBase : public CBaseClassNonCopyable {
    public:
        CTCScriptedControl* PTCScript;                      // +0x04
        bool                Activated;                      // +0x08
        bool                Terminated;                     // +0x09
    };
};

namespace NTCScriptedControl {

class CActionTalkBase : public CTCScriptedControl::CActionBase {
public:
    virtual ~CActionTalkBase();                             // 0x0071A430
private:
    unsigned char _members_0x0c[0x90 - 0x0c];               // PTargetThing +0x0C .. SkippedCurrentLine +0x8F (0x0071A430)
};

class CActionTalkToThing : public CActionTalkBase {         // vtable 0x012659C8
public:
    bool        AddedDuringCutscene;                        // +0x90
    bool        AskingHeroQuestion;                         // +0x91
    bool        DisplayOverScreenFade;                      // +0x92
    CCharString QuestionButton1;                            // +0x94
    CCharString QuestionButton2;                            // +0x98
    CCharString QuestionButton3;                            // +0x9C
};

}

void EmitImplicitDestructor(NTCScriptedControl::CActionTalkToThing* p) { p->NTCScriptedControl::CActionTalkToThing::~CActionTalkToThing(); }