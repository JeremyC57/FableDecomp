// Leaf: ?vector_deleting_destructor@CLandscapeBackgroundPatch@@QAEPAXI@Z

class CLandscapeBackgroundPatch { public:
    void* vftable;
    void* vector_deleting_destructor(unsigned int flags);
};

// Naming (NF450 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3; r449c: FableWin.pdb name, registered as a one-source best guess):
//   0x004420D0 ??1CDrawMeleeActionButtonEffects@NPlayerGui@@QAE@XZ (was ?inner_dtor@CLandscapeBackgroundPatch@@QAEXXZ)
namespace NPlayerGui { class CDrawMeleeActionButtonEffects { public: ~CDrawMeleeActionButtonEffects(); }; }
void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    reinterpret_cast<NPlayerGui::CDrawMeleeActionButtonEffects*>(this)->NPlayerGui::CDrawMeleeActionButtonEffects::~CDrawMeleeActionButtonEffects();
    if (flags & 1)
        operator delete(this);
    return this;
}