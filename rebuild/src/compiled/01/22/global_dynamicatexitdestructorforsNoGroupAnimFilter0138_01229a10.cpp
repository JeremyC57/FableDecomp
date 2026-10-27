// `dynamic atexit destructor for 's_NoGroupAnimFilter_01382438''() -- destroy the file-static CAnimFilterNoGroup at exit (its inlined destructor leaves the CAnimFilterBase vtable)
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x01229a10, 11 bytes, in the .text$yd group (compiler atexit destructors), no arguments.
// Identity (probable): the destructor thunk of a 4-byte CAnimFilterNoGroup static at 0x01382438: the object's .data word is the
//   CAnimFilterNoGroup vtable 0x0122FFDC (RTTI), so VC7.1 folded its constructor into static data and kept only the atexit
//   pair -- the registrant $E1 0x0121ED80 (`push 0x01229A10; call atexit 0x004012BC`, CRT table entry 0x0137458C) and this
//   $E2, whose ~CAnimFilterNoGroup / ~CAnimFilterBase inline down to the CAnimFilterBase vtable 0x0122FFCC store. The only
//   other reference is 0x00739E50 `mov eax, 0x01382438; ret`, a GetAnimFilter override (Ego_r.pdb
//   CTCCreatureModeManager::CModeBase::GetAnimFilter() const returns const CAnimFilterBase&). Static name unrecovered.
// Shape: VC7.1 numbers the thunks per TU: $E1 the registrant (.text$yc + .CRT$XCU), $E2 the destructor (.text$yd).
class CAnimationEntry;
class CAnimFilterBase {
public:
    virtual ~CAnimFilterBase() {}
    virtual bool operator()(const CAnimationEntry& entry) const = 0;
    virtual CAnimFilterBase* Clone() const = 0;
};
class CAnimFilterNoGroup : public CAnimFilterBase {
public:
    virtual ~CAnimFilterNoGroup() {}
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};
static CAnimFilterNoGroup s_NoGroupAnimFilter_01382438;               // 0x01382438