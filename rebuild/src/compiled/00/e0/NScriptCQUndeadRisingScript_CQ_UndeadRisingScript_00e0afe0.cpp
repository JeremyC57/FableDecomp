// NScript::CQ_UndeadRisingScript::CQ_UndeadRisingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- two-argument constructor of a script-module
// Leaf: ??0CQ_UndeadRisingScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// class whose vtable 0x012dfd3c has no RTTI locator (the script module), so it is named by its vtable address.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0AFE0, length 54 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated. Callee: CScriptBase::CScriptBase @ 0x00CB8110
// (the script root constructor, see GOTCHAS: "0x00CB8110 is the CScriptBase ctor, not CHeroMorphDef").
//
// Shape: a cousin of the __fastcall factories landed as Alloc_CVtable_* (0x00D67740 ...): root base ctor,
// the second argument stored at +0x40 and the first at +0x44 by an inlined abstract (`__declspec(novtable)`)
// intermediate base constructor -- both stores precede this class's vtable store, which a plain initialiser
// list cannot reproduce (it interleaves the vtable store between them) -- then six dwords at +60.. zeroed from one register by the inline constructor of an embedded
// state block (that is what keeps those stores BELOW the vtable store). `ret 8`: two stack arguments.
// Member names are descriptive.
// Naming wave 35: identity out-of-line copy of the ctor the Alloc 00e0e760 inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable 012dfd3c, ret 8 (two args); PDB ??0CQ_UndeadRisingScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CQ_UndeadRisingScript proven by its Main 00e0b160 entity registrations; was CVtable_012dfd3c::CVtable_012dfd3c (wave3/labels_alloc.tsv); PDB ??0CQ_UndeadRisingScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_12dfd3c@@QAE@PAVCScriptBase@@J@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
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

struct SScriptState {                           // embedded block: six zeroed dwords at +60
    long m_stage;
    long m_frame;
    long m_counter;
    void* m_pThingA;
    void* m_pThingB;
    void* m_pThingC;
    SScriptState() : m_stage(0), m_frame(0), m_counter(0), m_pThingA(0), m_pThingB(0), m_pThingC(0) { }
};

namespace NScript {
class CQ_UndeadRisingScript : public CScriptOwnedBase {
public:
    unsigned char _pad[0x18];                     // +0x48 (uninitialised here)
    SScriptState m_state;                       // +60
    virtual void v00();
    CQ_UndeadRisingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_UndeadRisingScript();    // ??1CQ_UndeadRisingScript@NScript@@UAE@XZ
};
}
using NScript::CQ_UndeadRisingScript;

NScript::CQ_UndeadRisingScript::CQ_UndeadRisingScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptOwnedBase((CScriptBase*)data, (long)&game)
    , m_state()
{
}