// CPlayerGui::AddScreenMessage(const CWideString& text) -- word-wrap a message into the visible screen area (left edge pushed in to the GUI def's message margin) with the def's message font, and queue it on the screen-message list with its display time (frames-per-second x the def's seconds) and reveal speed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438AA0, length 528 bytes, `ret 4`.
// Callees: CDisplayManager::GetVVisibleTelevisionScreenArea 0x009BECE0 (g_pDisplayManager 0x013B8390, hidden result),
// the lazily loaded "PLAYER_GUI_PC" def (CCharString 0x0099EBF0, CGameDefinitionManager::Get 0x0044C6B0, GetDef<T>
// 0x0043FF30, ~CCharString 0x0099EAE0; global 0x013B878C), GFVXToAX 0x009E1DA0 (__stdcall float; virtual-to-absolute X `K * x / width`, the inverse of the landed GFAXToVX 0x009E1E10, Ego_r.pdb ?GFVXToAX@@YIMM@Z pairs with ?GFVYToAY at 0x009E1DD0), the font bank's
// GetFont 0x009E2C80 (bank 0x013B838C, CCountedPointer by hidden pointer; count-block destroy +4 and operator delete
// 0x00BFE9BC), CWideString() 0x0099AED0 / (const CWideString&) 0x0099B720 / ~CWideString 0x0099B510,
// CPlayerGui::FormatStringForArea 0x00437D10 (static __fastcall), __ftol2 0x00BFEA70, NPlayerGui::CScreenMessage ctor
// 0x00438020, malloc 0x00BFEA0E (the MSVCR71.DLL import thunk; the inlined STLport list push_back allocates its node with malloc). Global: frames per second 0x01375550.
// Layout: retail CPlayerGui ScreenMessages (STLport list, node pointer) at +0x328; CScreenMessage is 0x10 bytes (Text,
// FramesToShowFor, NumberOfCharactersShowing, LinesInText) as the landed CScreenMessage::Update 0x00438770; the GUI
// def's message margin +0x8e0, font name +0x8f8, display seconds +0x908 and characters per frame +0x90c (descriptive).
// Shape: the message is a temporary passed straight to the inline push_back (retail keeps the ctor's eax as the source);
// the insert reads the old tail into a local before linking; the font counted pointer is a temporary whose dtor does not
// clear its words; the margin clamp is `if (area.TLX < margin) area.TLX = margin;`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class CDisplayManager { public:
    C2DBoxF GetVVisibleTelevisionScreenArea() const;        // 0x009BECE0
};
extern CDisplayManager* g_pDisplayManager;                  // 0x013B8390
float __stdcall GFVXToAX(float x);                          // 0x009E1DA0
extern long g_framesPerSecond;                              // 0x01375550

class CPlayerGuiDef { public:                                      // the "PLAYER_GUI_PC" definition (descriptive)
    unsigned char _pad000[0x8e0];
    float         MessageLeftMargin;                        // +0x8e0
    unsigned char _pad8e4[0x8f8 - 0x8e4];
    CCharString   MessageFont;                              // +0x8f8
    unsigned char _pad8fc[0x908 - 0x8fc];
    float         MessageDisplaySeconds;                    // +0x908
    long          MessageCharactersPerFrame;                // +0x90c
};
struct CPlayerGuiDefPointer { CPlayerGuiDef* Object; };
extern CPlayerGuiDefPointer g_PlayerGuiDef;                 // 0x013B878C
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CPlayerGuiDefPointer* out);   // 0x0043FF30
};
class CGameDefinitionManager { public:
    static CDefinitionManager& __fastcall Get();            // 0x0044C6B0
};
__forceinline const CPlayerGuiDef* GetPlayerGuiDef()
{
    if (!g_PlayerGuiDef.Object) {
        CCharString name("PLAYER_GUI_PC", -1);
        CGameDefinitionManager::Get().GetDef(name, &g_PlayerGuiDef);
    }
    return g_PlayerGuiDef.Object;
}

class CCPPointerInfo { public:
    long References;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
};
class CFontBank;
struct CFontBankManager {
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
extern CFontBankManager* g_pFontBankManager;                // 0x013B838C

namespace NPlayerGui {
class CScreenMessage { public:
    CWideString Text;
    long        FramesToShowFor;
    long        NumberOfCharactersShowing;
    long        LinesInText;
    CScreenMessage(const CWideString& text, long frames, long charsPerFrame);   // 0x00438020
};
}
struct SScreenMessageNode {
    SScreenMessageNode*        _M_next;
    SScreenMessageNode*        _M_prev;
    NPlayerGui::CScreenMessage _M_data;
};
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (MSVCR71.DLL import thunk)
void* __cdecl operator new(unsigned int, void* where);
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
struct CScreenMessageList {                                 // STLport list<CScreenMessage>
    SScreenMessageNode* _M_node;
    void push_back(const NPlayerGui::CScreenMessage& value)
    {
        SScreenMessageNode* pos = _M_node;
        SScreenMessageNode* node = (SScreenMessageNode*)malloc(sizeof(SScreenMessageNode));
        new (&node->_M_data) NPlayerGui::CScreenMessage(value);
        SScreenMessageNode* prev = pos->_M_prev;
        node->_M_next = pos;
        node->_M_prev = prev;
        prev->_M_next = node;
        pos->_M_prev = node;
    }
};

class CPlayerGui { public:
    unsigned char      _pad000[0x328];
    CScreenMessageList ScreenMessages;                      // +0x328
    static void __fastcall FormatStringForArea(const C2DBoxF& area, const CFontBank& font, const CWideString& text, CWideString& out);   // 0x00437D10
    void AddScreenMessage(const CWideString& text);
};

void CPlayerGui::AddScreenMessage(const CWideString& text)
{
    C2DBoxF area = g_pDisplayManager->GetVVisibleTelevisionScreenArea();
    float margin = GFVXToAX(GetPlayerGuiDef()->MessageLeftMargin);
    if (area.TLX < margin)
        area.TLX = margin;
    CFontBank* font = g_pFontBankManager->GetFont(GetPlayerGuiDef()->MessageFont).Data;
    CWideString formatted;
    FormatStringForArea(area, *font, text, formatted);
    ScreenMessages.push_back(NPlayerGui::CScreenMessage(formatted, (long)(g_framesPerSecond * GetPlayerGuiDef()->MessageDisplaySeconds), GetPlayerGuiDef()->MessageCharactersPerFrame));
}