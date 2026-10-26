// CPlayerGui::ClearGui() -- clear the health, stamina, ability-button, quick-access and melee-effect displays, stop every illustrator drawing and clear it, then clear the subtitles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438230, length 133 bytes.
// Callees: the five draw classes' clear methods (0x00644644, 0x0064464C, 0x0064528A, 0x00645605, 0x00645821: ecx-adjusting
// thunks, manifest labels are STL noise), for_each<CDrawBase**, mem_fun_t> @ 0x0043F310 (`__fastcall`, functor result in
// the frame word), the CDrawBase::Clear vcall thunk @ 0x0043E420, CSubtitleRenderer::Clear(bool) @ 0x00492BAB.
// Layout: see the CPlayerGui model below (retail offsets derived from engine/CPlayerGui.h).
// Shape: the Drawing reset is a loop over [begin, end) with end cached and the false byte hoisted into dl; the
// second pass is `for_each(begin, end, mem_fun(&CDrawBase::Clear))` with end read first.
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