// CScriptRequest::CScriptRequest() -- out-of-line copy of the request constructor: the head base, then the part at +0x10 built from (this, 1)
// Leaf: ??0CScriptRequest
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1DF50, 23 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "CAIStateGroup_SitForMeal_constructor" is propagated. No code xrefs; the same constructor is inlined in
//   the landed 0x00C1DF70 (InitializeAndDispatch), whose model this reuses: head constructor 0x00C1DD50 (0x10 bytes,
//   vtable 0x012B332C), part constructor 0x00C1D9D0 (owner, flag).
// Shape: the base constructor on `this`, then `push 1 ; push esi ; lea ecx,[esi+10h]` for the member; no own vtable.
struct CRequestHead {                       // 0x10 bytes: vtable 0x012B332C, a flag byte and two words
    unsigned char _body[0x10];
    CRequestHead();                         // 0x00C1DD50
};

struct CRequestPart {                       // 0x20 bytes
    unsigned char _body[0x20];
    CRequestPart(CRequestHead* owner, long flag);   // 0x00C1D9D0
};

struct CScriptRequest : CRequestHead {
    CRequestPart m_part;                    // +0x10
    CScriptRequest();
};

CScriptRequest::CScriptRequest() : CRequestHead(), m_part(this, 1)
{
}