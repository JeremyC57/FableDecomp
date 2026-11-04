// NPlayerGui::CDrawBase::vcall{0}(NPlayerGui::CDrawBase* this) -- compiler-generated __fastcall vcall thunk `[thunk]: __fastcall 'vcall'{0,{flat}}' for a member pointer to a slot-0 virtual: jump through [this->vtbl+0]
// Leaf: ??_9@$BA@AE
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E420, 4 bytes: `mov eax,[ecx] ; jmp [eax]` (then int3 padding).
// Identity (two sources): (1) CPlayerGui::ClearGui 0x00438230 (landed) pushes 0x0043E420 as the pointer-to-member of
//   `for_each(..., mem_fun(&NPlayerGui::CDrawBase::Clear))` (CDrawBase::Clear is slot 0), and CPlayerGui::DrawPreparePrimitivesForEngine
//   0x00439070 does the same; (2) the landed callers already bind `??_9@$BA@AE` -> 0x0043E420 (reloc_bindings.tsv: CPlayerGui::ClearGui
//   and the libpk comparators 0x00C0A010.. / 0x00C24D10.., which call it directly with a constant member-pointer template argument).
//   VC7.1 names __fastcall vcall thunks without the class (`??_9@$B<offset>@AE`, GOTCHAS chunk 753 / family pass 6), so every TU that
//   takes a pointer to a slot-0 virtual emits this one COMDAT and the linker keeps one copy: 0x0043E420 is THE vcall{0} thunk. The
//   catalog's "NPlayerGui::CDrawBase vcall{0}" names its first user.
// TU: CPlayerGui::ClearGui's translation unit; the body below is the landed ClearGui 0x00438230 source (graded there, not here).
//   Only the thunk is this row.
namespace NPlayerGui {
class CDrawBase { public:                                          // PDB NPlayerGui::CDrawBase
    virtual void Clear();                                   // slot 0 (the vcall thunk 0x0043E420 jumps through [vtbl+0])
    virtual void PrepareForLoad();                          // slot 1 (the vcall thunk 0x0043E430)
    void*         PlayerGui;                                // +0x04
    void*         PlayerManager;                            // +0x08
    void*         World;                                    // +0x0c
    void*         Component;                                // +0x10
    void*         Bank;                                     // +0x14
    bool          Drawing;                                  // +0x18
};
class CDrawHealth            { public: void ClearDisplay(); };     // 0x00644644
class CDrawStamina           { public: void ClearDisplay(); };     // 0x0064464C
class CDrawAbilityButtons    { public: void ClearDisplay(); };     // 0x0064528A
class CDrawQuickAccessIcons  { public: void ClearDisplay(); };     // 0x00645605
class CDrawMeleeActionButtonEffects { public: void ClearDisplay(); };   // 0x00645821
class CDrawMiniMap           { public: void SetAsActive(bool active); };   // 0x00643E2E
class CDrawQuestInfo         { public: void Clear(); };            // 0x00647319
class CDrawGuildSeal         { public: void PrepareForLoad(); };   // 0x0064AE66 (manifest: CQuestInfoElement::Clear)
}

template <class R, class T> class mem_fun_t {               // SGI functor holding a 4-byte pointer-to-member
public:
    explicit mem_fun_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T* object) const { return (object->*m_pfn)(); }
private:
    R (T::*m_pfn)();
};
template <class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*pfn)()) { return mem_fun_t<R, T>(pfn); }
template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x0043F310 (SGI, /Gr)

class CSubtitleRenderer { public:
    void Clear(bool flag);                                  // 0x00492BAB
    void PrepareForLoad();                                  // 0x004933E6 (manifest label propagated)
};

struct CIllustratorVector {                                 // STLport vector<NPlayerGui::CDrawBase*> (12 bytes)
    NPlayerGui::CDrawBase** _M_start;
    NPlayerGui::CDrawBase** _M_finish;
    NPlayerGui::CDrawBase** _M_end_of_storage;
};

// CPlayerGui in retail layout: the counted draw pointers sit 8 below the Ego_r.pdb offsets (engine/CPlayerGui.h),
// the three ability vectors are 12 bytes, so the flags from DrawGui on sit 0x14 lower, and 0x10 lower from
// HeroCentreUID (8-aligned) on.
class CPlayerGui { public:
    void*                       __vftable;                  // +0x000
    unsigned char               _pad004[0x18 - 0x04];       // World, PlayerManager, InputInterface, Component, Graphics
    CSubtitleRenderer           SubtitleRenderer;           // +0x018
    unsigned char               _pad019[0x140 - 0x19];
    NPlayerGui::CDrawHealth*    PDrawHealth;                // +0x140 (PDB +0x148)
    void*                       PDrawHealth_Info;
    NPlayerGui::CDrawStamina*   PDrawStamina;               // +0x148
    unsigned char               _pad14c[0x160 - 0x14c];
    NPlayerGui::CDrawMiniMap*   PDrawMiniMap;               // +0x160
    unsigned char               _pad164[0x1c8 - 0x164];
    NPlayerGui::CDrawQuestInfo* PDrawQuestInfo;             // +0x1c8
    void*                       PDrawQuestInfo_Info;
    NPlayerGui::CDrawGuildSeal* PDrawGuildSeal;             // +0x1d0
    void*                       PDrawGuildSeal_Info;
    NPlayerGui::CDrawQuickAccessIcons* PDrawQuickAccessIcons;   // +0x1d8
    unsigned char               _pad1dc[0x200 - 0x1dc];
    NPlayerGui::CDrawAbilityButtons* PDrawAbilityButtons;   // +0x200
    unsigned char               _pad204[0x220 - 0x204];
    NPlayerGui::CDrawMeleeActionButtonEffects* PDrawMeleeActionButtonEffects;   // +0x220
    unsigned char               _pad224[0x29c - 0x224];
    bool                        ForcedClosed;               // +0x29c
    bool                        ShowTime;                   // +0x29d
    bool                        ShowMoneyBag;               // +0x29e
    bool                        ShowStatBars;               // +0x29f
    float                       Alpha;                      // +0x2a0
    unsigned char               _pad2a4[4];
    unsigned __int64            HeroCentreUID;              // +0x2a8
    bool                        DrawingSafeAreas;           // +0x2b0
    bool                        DrawingDebugTGA;            // +0x2b1
    unsigned char               _pad2b2[0x2bc - 0x2b2];     // DebugTGATexture
    bool                        ScreenMessagingEnabled;     // +0x2bc
    bool                        ForceAllowSubtitleToRender; // +0x2bd
    bool                        LiveGuiMode;                // +0x2be
    CIllustratorVector          Illustrators;               // +0x2c0
    unsigned char               _pad2cc[0x330 - 0x2cc];     // stat bars, primitive handles, lists, info screens
    bool                        DisplayBottomHalfGUI;       // +0x330
    void ToggleLiveGui();                                   // 0x00438310
    void ClearGui();
    void PrepareForLoad();
};

void CPlayerGui::ClearGui()
{
    PDrawHealth->ClearDisplay();
    PDrawStamina->ClearDisplay();
    PDrawAbilityButtons->ClearDisplay();
    PDrawQuickAccessIcons->ClearDisplay();
    PDrawMeleeActionButtonEffects->ClearDisplay();
    NPlayerGui::CDrawBase** last = Illustrators._M_finish;
    for (NPlayerGui::CDrawBase** it = Illustrators._M_start; it != last; ++it)
        (*it)->Drawing = false;
    for_each(Illustrators._M_start, Illustrators._M_finish, mem_fun(&NPlayerGui::CDrawBase::Clear));
    SubtitleRenderer.Clear(true);
}