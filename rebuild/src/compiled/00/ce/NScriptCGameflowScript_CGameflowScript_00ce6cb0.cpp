// NScript::CGameflowScript::CGameflowScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- two-argument constructor of a script-module
// Leaf: ??0CGameflowScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// class whose vtable 0x012c3fa4 has no RTTI locator (the script module), so it is named by its vtable address.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE6CB0, length 54 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated. Callee: CScriptBase::CScriptBase @ 0x00CB8110
// (the script root constructor, see GOTCHAS: "0x00CB8110 is the CScriptBase ctor, not CHeroMorphDef").
//
// Shape: a cousin of the __fastcall factories landed as Alloc_CVtable_* (0x00D67740 ...): root base ctor,
// the second argument stored at +0x40 and the first at +0x44 by an inlined abstract (`__declspec(novtable)`)
// intermediate base constructor -- both stores precede this class's vtable store, which a plain initialiser
// list cannot reproduce (it interleaves the vtable store between them) -- then six dwords at +4c.. zeroed from one register by the inline constructor of an embedded
// state block (that is what keeps those stores BELOW the vtable store). `ret 8`: two stack arguments.
// Member names are descriptive.
// Naming wave 35: identity CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc 00cef950 at 00cd55aa with name "Gameflow"; Alloc news + stores script vtable 012c3fa4; PDB ?Alloc@CGameflowScript@NScript@@SIPAVCScriptBase@@; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret 8 = PDB arity; PDB ??0CGameflowScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z; was CVtable_012c3fa4::CVtable_012c3fa4 (wave3/labels_alloc.tsv); PDB ??0CGameflowScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_12c3fa4@@QAE@PAVCScriptBase@@J@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptBase { public:
    virtual void v00();
    CScriptBase();                              // 0x00CB8110
};

struct __declspec(novtable) CScriptOwnedBase : CScriptBase {   // abstract intermediate base: stores no vtable of its own
    unsigned char _scriptBody[0x3c];            // +0x04 CScriptBase members, untouched here
    long m_argument;                            // +0x40 (second argument)
    CScriptBase* m_pOwner;                      // +0x44 (first argument)
    CScriptOwnedBase(CScriptBase* pOwner, long argument) : CScriptBase(), m_argument(argument), m_pOwner(pOwner) { }
};

struct SScriptState {                           // embedded block: six zeroed dwords at +4c
    long m_stage;
    long m_frame;
    long m_counter;
    void* m_pThingA;
    void* m_pThingB;
    void* m_pThingC;
    SScriptState() : m_stage(0), m_frame(0), m_counter(0), m_pThingA(0), m_pThingB(0), m_pThingC(0) { }
};

namespace NScript {
class CGameflowScript : public CScriptOwnedBase {
public:
    unsigned char _pad[4];                     // +0x48 (uninitialised here)
    SScriptState m_state;                       // +4c
    virtual void v00();
    CGameflowScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CGameflowScript();    // ??1CGameflowScript@NScript@@UAE@XZ
};
}
using NScript::CGameflowScript;

NScript::CGameflowScript::CGameflowScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptOwnedBase((CScriptBase*)data, (long)&game)
    , m_state()
{
}