// std::for_each(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, SKillThing f) -- kill every particle thing still alive in a range of intelligent thing pointers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8760, length 58 bytes (padded to 64).
// Leaf: for_each
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. Every caller passes
// CTCParticleAttacher::ParticleList (Ego_r.pdb vector<CIntelligentPointer<CThing>>, retail +0x10/+0x14) and then clears
// it: 0x007F8877 (0x007F8860, the detach path), CTCParticleAttacher::AttachParticles 0x007F8B2A and the attacher's
// slot-27 message handler 0x007F8B84. The functor has no PDB name (Ego_r inlines this for_each); SKillThing is
// descriptive.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice per element), CThing::Kill(bool) @ 0x004C9B80 (false).
// Convention: /Gr `__fastcall` template (GOTCHAS chunk 230): hidden result pointer in ecx, first in edx, last and the
// 4-byte empty functor on the stack (`ret 8`); the result pointer is returned in eax.
// Shape: the functor needs a base class (std::unary_function) so it is non-POD and comes back through the hidden
// pointer (the callers pass `lea ecx,[esp+0Fh]`, a 1-byte slot); `if (p.GetPItem()) p.GetPItem()->Kill(false)` gives
// the two GetPItem calls.
class CThing {
public:
    void Kill(bool immediately);            // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
};
typedef CIntelligentPointer<CThing> PThing;
namespace std {
template <class A, class R> struct unary_function {
    typedef A argument_type;
    typedef R result_type;
};
}
struct SKillThing : std::unary_function<PThing, void> {
    void operator()(PThing& thing) const
    {
        if (thing.GetPItem())
            thing.GetPItem()->Kill(false);
    }
};
namespace std {
template <class It, class F> F __fastcall for_each(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
template SKillThing __fastcall for_each<PThing*, SKillThing>(PThing* first, PThing* last, SKillThing f);
}