// Leaf: ?ScalarDeletingDestructor@CTCCreatureTintEffect@@QAEPAXI@Z
// agentNF182 (2026-11-20), in-place respelling of this row's existing local model: the destructor it calls is ??1CTCCreatureTintEffect@@UAE@XZ 0x007CF0B0
//   (Ego_r.pdb + FableWin.pdb; its landed row defines that leaf), declared virtual as in retail and called by qualified name so the
//   call stays direct. The real ??_G is emitted only by a TU that compiles a CTCCreatureTintEffect constructor or destructor (retail TU-mate: the
//   out-of-line constructor, a separate landed row), so the self leaf stays a stand-in (NF182 NOTES).
// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
class CTCCreatureTintEffect { public: virtual ~CTCCreatureTintEffect(); void* ScalarDeletingDestructor(unsigned flags); };
void __cdecl operator delete(void* p);
void* CTCCreatureTintEffect::ScalarDeletingDestructor(unsigned flags) {
    this->CTCCreatureTintEffect::~CTCCreatureTintEffect();
    if (flags & 1) ::operator delete(this);
    return this;
}