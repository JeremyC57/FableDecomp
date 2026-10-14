// CVtable_12c3fa4::CVtable_12c3fa4(CScriptBase* pOwner, long argument) -- two-argument constructor of a script-module
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
struct CScriptBase {
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

struct CVtable_12c3fa4 : CScriptOwnedBase {
    unsigned char _pad[4];                     // +0x48 (uninitialised here)
    SScriptState m_state;                       // +4c
    virtual void v00();
    CVtable_12c3fa4(CScriptBase* pOwner, long argument);
};

CVtable_12c3fa4::CVtable_12c3fa4(CScriptBase* pOwner, long argument)
    : CScriptOwnedBase(pOwner, argument)
    , m_state()
{
}