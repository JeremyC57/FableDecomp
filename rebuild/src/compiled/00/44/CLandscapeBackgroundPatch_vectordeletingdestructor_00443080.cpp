class CLandscapeBackgroundPatch { public:
// Leaf: ?CLandscapeBackgroundPatch_vector_deleting_destructor@CLandscapeBackgroundPatch@@QAEPAXI@Z
    void* vftable;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

// Naming (NF450 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3; r449c: FableWin.pdb name, registered as a one-source best guess):
//   0x00442190 ??1CDrawSpiritTargetingCursor@NPlayerGui@@QAE@XZ (was @CLandscapeBackgroundPatch_scalar_dtor@4)
namespace NPlayerGui { class CDrawSpiritTargetingCursor { public: ~CDrawSpiritTargetingCursor(); }; }
void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    reinterpret_cast<NPlayerGui::CDrawSpiritTargetingCursor*>(this)->NPlayerGui::CDrawSpiritTargetingCursor::~CDrawSpiritTargetingCursor();
    if (flags & 1)
        operator delete((void*)this);
    return (void*)this;
}