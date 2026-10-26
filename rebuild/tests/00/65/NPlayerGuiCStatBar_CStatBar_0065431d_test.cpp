#include <cstdio>
// Behaviour test: NPlayerGui::CStatBar::CStatBar -- the CStatBase part from the def, six zeroed indices, the stat-bar font
// fetched by the def's font name (the counted-pointer temporary released), an empty change-text list, five primitive
// handles and one effect slot.
class CCharString { public: int Id; };
class CFontBank { public: int Id; };
class CPlayerGuiDef { public:
    CCharString StatBarsFontName;
    const CCharString& GetStatBarsFontName() const { return StatBarsFontName; }
};
static int g_fontPointers = 0;
struct CFontPointer {
    CFontBank* m_pObject; void* m_pInfo;
    CFontPointer(CFontBank* p) : m_pObject(p), m_pInfo(0) { ++g_fontPointers; }
    CFontPointer(const CFontPointer& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo) { ++g_fontPointers; }
    ~CFontPointer() { --g_fontPointers; }
};
static CFontBank g_fonts[3] = { { 0 }, { 1 }, { 2 } };
struct CFontBankManager {
    int LastName;
    CFontPointer GetFont(const CCharString& name) { LastName = name.Id; return CFontPointer(&g_fonts[name.Id]); }
};
static CFontBankManager g_mgr;
static CFontBankManager* g_pFontBankManager = &g_mgr;
namespace NPlayerGui {
struct CRetailHandleVector {
    void* m_pFirst; void* m_pLast; void* m_pEnd; long Count;
    CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), Count(-1) {}
    void Resize(long count) { Count = count; }
};
enum EPlayerGuiMode { PLAYER_GUI_MODE_DEFAULT = 1 };
class CStatBase {
public:
    CStatBase(const CPlayerGuiDef& def) : GuiDef(&def), ValueInitialised(false), CurrentValue(0.0f), GuiMode(PLAYER_GUI_MODE_DEFAULT) {}
    virtual ~CStatBase() {}
    const CPlayerGuiDef* GuiDef; bool ValueInitialised; float CurrentValue; EPlayerGuiMode GuiMode;
    CRetailHandleVector PrimitiveHandles;
    CRetailHandleVector Effects;
};
class CStatBar : public CStatBase {
public:
    class CChangeText { public: int Unused; };
    class CChangeTextList {
    public:
        void* _M_node;
        CChangeTextList() : _M_node(this) {}
        bool empty() const { return _M_node == this; }
    };
    CStatBar(const CPlayerGuiDef& def)
        : CStatBase(def), GraphicFrame(0), OldGraphicFrame(0), GraphicIndex(0), BorderGraphicIndex(0), EffectIndex(0),
          OldEffectIndex(0), FontBank(g_pFontBankManager->GetFont(def.GetStatBarsFontName()).m_pObject), ChangeTexts()
    {
        PrimitiveHandles.Resize(5);
        Effects.Resize(1);
    }
    unsigned long GraphicFrame, OldGraphicFrame; long GraphicIndex, BorderGraphicIndex, EffectIndex, OldEffectIndex;
    CFontBank* FontBank;
    CChangeTextList ChangeTexts;
};
}
int main()
{
    CPlayerGuiDef def; def.StatBarsFontName.Id = 2;
    NPlayerGui::CStatBar bar(def);
    if (bar.GuiDef != &def || bar.GuiMode != NPlayerGui::PLAYER_GUI_MODE_DEFAULT || bar.ValueInitialised) { std::printf("0065431d_TEST FAIL\n"); return 1; }
    if (bar.GraphicFrame || bar.OldGraphicFrame || bar.GraphicIndex || bar.BorderGraphicIndex || bar.EffectIndex || bar.OldEffectIndex) { std::printf("0065431d_TEST FAIL\n"); return 1; }
    if (bar.FontBank != &g_fonts[2] || g_mgr.LastName != 2 || g_fontPointers != 0) { std::printf("0065431d_TEST FAIL\n"); return 1; }
    if (!bar.ChangeTexts.empty() || bar.PrimitiveHandles.Count != 5 || bar.Effects.Count != 1) { std::printf("0065431d_TEST FAIL\n"); return 1; }
    std::printf("0065431d_TEST PASS\n");
    return 0;
}