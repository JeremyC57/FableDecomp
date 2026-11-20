// Leaf: ?CTCAIScratchpad_scalar_deleting_destructor@@YIPAXPAVCTCAIScratchpad@@HI@Z
// agentNF182 (2026-11-20), in-place respelling of this row's existing local model: the destructor it calls is ??1CTCAIScratchpad@@UAE@XZ 0x007ADAA0
//   (Ego_r.pdb + FableWin.pdb; its landed row defines that leaf), declared virtual as in retail and called by qualified name so the
//   call stays direct. The real ??_G is emitted only by a TU that compiles a CTCAIScratchpad constructor or destructor (retail TU-mate: the
//   out-of-line constructor, a separate landed row), so the self leaf stays a stand-in (NF182 NOTES).
class CTCAIScratchpad { public:
    virtual ~CTCAIScratchpad();      // 0x007ADAA0
};

void operator delete(void*);

void* __fastcall CTCAIScratchpad_scalar_deleting_destructor(CTCAIScratchpad* scratchpad, int edx_unused, unsigned int flags)
{
    scratchpad->CTCAIScratchpad::~CTCAIScratchpad();
    if (flags & 1)
        operator delete(scratchpad);
    return scratchpad;
}