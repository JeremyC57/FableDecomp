// NPlayerGui::CStatBar::CStatBar(const CPlayerGuiDef& def) -- stat bar constructor: the CStatBase part, zeroed frame/graphic/effect indices, the stat-bar font from the def, an empty change-text list, five primitive handles and one effect slot
// Leaf: ??0CStatBar@NPlayerGui@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065431D, 117 bytes, `ret 4`. Size-optimised NPlayerGui TU (frame pointer,
// `leave`). Vtable 0x0125A738 = NPlayerGui::CStatBar (rtti-vtables: slot 0 0x00646762, 1 ClearGraphics 0x00645F8D, 2
// DrawStatBox 0x00645CE9).
// Identity: Ego_r.pdb `??0CStatBar@NPlayerGui@@QAE@ABVCPlayerGuiDef@@@Z`. The catalog label "NPlayerGui_CStatBase_CStatBase"
// (and the landed 0x0064CC11 note calling this "the sibling copy") is wrong: 0x0064CC11 is CStatBase's constructor and
// this body calls it, then stores CStatBar's vtable.
// Callees: NPlayerGui::CStatBase::CStatBase(const CPlayerGuiDef&) 0x0064CC11, CFontBankManager::GetFont(const CCharString&)
// 0x009E2C80 (manager 0x013B838C; CCountedPointer<CFontBank> by value, destroyed by 0x004057A0 as in the landed
// CDrawScriptScreenText::Draw 0x0064560D), list<CChangeText>(const allocator&) 0x00655B69 (out of line; the allocator
// temporary in the high byte of the dead `def` slot), vector<CEnginePrimitiveHandle>::Resize 0x00443F90 (5),
// vector<CIntelligentPointer<CParticleEmitter>>::resize 0x00657037 (1).
// Layout: Ego_r.pdb CStatBar (0x78) over CStatBase (0x50, retail 0x48: PrimitiveHandles +0x30, Effects +0x3C, the landed
// 0x0064CC11 model): GraphicFrame +0x48, OldGraphicFrame +0x4C, GraphicIndex +0x50, BorderGraphicIndex +0x54, EffectIndex
// +0x58, OldEffectIndex +0x5C, FontBank +0x60, ChangeTexts +0x64 (4-byte STLport list). CPlayerGuiDef StatBarsFontName at
// +0x4E0 (Ego_r.pdb +0x510).
// Shape: everything in the initialiser list (the six zeros share eax; the vtable store is scheduled after them), the font
// is the counted-pointer temporary's object pointer; the two resizes are the body, handles first. Retail keeps `def` in
// edx across the CStatBase call (caller-saved): that needs the landed CStatBase constructor body as a same-TU noinline
// stand-in (GOTCHAS: same-TU callee register knowledge).
#pragma optimize("s", on)
class CCharString;
class CFontBank;
class CPlayerGuiDef { public:
    unsigned char _pad[0x4e0];
    unsigned char StatBarsFontName[4];                        // +0x4E0 CCharString (retail)
    const CCharString& GetStatBarsFontName() const { return *(const CCharString*)StatBarsFontName; }
};

struct CFontPointer {                                         // CCountedPointer<CFontBank>
    CFontBank* m_pObject;
    void*      m_pInfo;
    ~CFontPointer();                                          // 0x004057A0
};
struct CFontBankManager {
    CFontPointer GetFont(const CCharString& name);            // 0x009E2C80
};
extern CFontBankManager* g_pFontBankManager;                  // 0x013B838C

namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
}

namespace NPlayerGui {

struct CRetailHandleVector {                                  // vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    void Resize(long count);                                  // 0x00443F90
};

struct CRetailEmitterVector {                                 // vector<CIntelligentPointer<CParticleEmitter>>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
    void Resize(long count);                                  // 0x00657037
};

enum EPlayerGuiMode { PLAYER_GUI_MODE_DEFAULT = 1 };

class CStatBase {
public:
    CStatBase(const CPlayerGuiDef& def);                      // 0x0064CC11
    virtual ~CStatBase();
    const CPlayerGuiDef* GuiDef;                              // +0x04
    bool                 ValueInitialised;                    // +0x08
    float                CurrentValue;                        // +0x0C
    float                OldValue;                            // +0x10
    float                LastOldValue;                        // +0x14
    float                RecentMaxValue;                      // +0x18
    float                OldRecentMaxValue;                   // +0x1C
    unsigned long        FrameCurrentValueLastMoved;          // +0x20
    unsigned long        FrameFullyOpenedOn;                  // +0x24
    EPlayerGuiMode       GuiMode;                             // +0x28
    float                FadeInTimer;                         // +0x2C
    CRetailHandleVector  PrimitiveHandles;                    // +0x30
    CRetailEmitterVector Effects;                             // +0x3C
};

// Same-TU stand-in: the landed CStatBase constructor body (0x0064CC11), out of line, so VC7.1 sees that it leaves edx
// alone and keeps `def` in edx across the call as retail does.
#pragma auto_inline(off)
__declspec(noinline) CStatBase::CStatBase(const CPlayerGuiDef& def)
    : GuiDef(&def),
      ValueInitialised(false),
      CurrentValue(0.0f),
      OldValue(0.0f),
      LastOldValue(0.0f),
      RecentMaxValue(0.0f),
      OldRecentMaxValue(0.0f),
      FrameCurrentValueLastMoved(0),
      FrameFullyOpenedOn(0),
      GuiMode(PLAYER_GUI_MODE_DEFAULT),
      FadeInTimer(0.0f)
{
}
#pragma auto_inline(on)

class CStatBar : public CStatBase {
public:
    class CChangeText;
    class CChangeTextList {                                   // STLport list<CChangeText>: the node pointer
    public:
        void* _M_node;
        explicit CChangeTextList(const std::allocator<CChangeText>& a = std::allocator<CChangeText>());   // 0x00655B69
    };

    CStatBar(const CPlayerGuiDef& def);
    virtual ~CStatBar();
    virtual void ClearGraphics();                             // slot 1 0x00645F8D

    unsigned long   GraphicFrame;                             // +0x48
    unsigned long   OldGraphicFrame;                          // +0x4C
    long            GraphicIndex;                             // +0x50
    long            BorderGraphicIndex;                       // +0x54
    long            EffectIndex;                              // +0x58
    long            OldEffectIndex;                           // +0x5C
    CFontBank*      FontBank;                                 // +0x60
    CChangeTextList ChangeTexts;                              // +0x64
};

CStatBar::CStatBar(const CPlayerGuiDef& def)
    : CStatBase(def),
      GraphicFrame(0),
      OldGraphicFrame(0),
      GraphicIndex(0),
      BorderGraphicIndex(0),
      EffectIndex(0),
      OldEffectIndex(0),
      FontBank(g_pFontBankManager->GetFont(def.GetStatBarsFontName()).m_pObject),
      ChangeTexts()
{
    PrimitiveHandles.Resize(5);
    Effects.Resize(1);
}

}