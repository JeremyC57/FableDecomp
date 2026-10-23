// CSubtitleRenderer::IsInInteractiveState() -- whether the subtitle box is waiting on the player: forced/click flags, a text-complete click-past outside cutscenes, an active casting/fishing-style hero component (0xFC) or a quest-card/gui state
// Retail: Fable.exe (TLC, VC7.1) @ 0x00490A22, 446 bytes, plain `ret`; /Os frame-pointer TU like the landed siblings 0x00490927/0x0048DB46.
// Callees: CSubtitleRenderer::IsTextFullyShown? 0x0048E072 (name unconfirmed), IsWaitingForClickPast 0x0048DB46, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetControlledCreature 0x00487DD0 / GetHeroCreature 0x00487DC0, IS_THING_ALIVE<CThing*> 0x0043DBF0, bitset::test 0x00410DE0,
//   LowerBound const 0x004365B0 / non-const 0x0040F020, front-end predicate 0x00434D60 on g_pMainGameComponent+0x28, CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0.
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: IsInInteractiveState
class CWideString { public: void* PStringData; };

struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank;
class CGraphicDataBank;

struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0C
    bool       Opening;                            // +0x0D
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
};

struct CRetailVector { void* m_pFirst; void* m_pLast; void* m_pEnd; };   // VC6 vector: 12 bytes

class CSubtitleRenderer {
public:
    void*              __vftable;                  // +0x00
    C2DBoxF            VWindow;                    // +0x04
    C2DBoxF            VTelevisionWindow;          // +0x14
    long               TextCharactersShowing;      // +0x24
    bool               TextGroupHasEntriesRemaining; // +0x28
    long               TextGroupIndex;             // +0x2C
    CWideString        TextGroupTag;               // +0x30
    unsigned char      TextInfoQueue[0x28];        // +0x34 (VC6 deque<CSetTextInfo>)
    CWideString        Text;                       // +0x5C
    CWideString        NameText;                   // +0x60
    CWideString        Button1Text;                // +0x64
    CWideString        Button2Text;                // +0x68
    CWideString        Button3Text;                // +0x6C
    CWideString        Button1OverrideText;        // +0x70
    CWideString        Button2OverrideText;        // +0x74
    unsigned char      Button1Alpha;               // +0x78
    unsigned char      Button2Alpha;               // +0x79
    unsigned long      DisplayFlags;               // +0x7C
    CFontBank*         PFont;                      // +0x80
    CGraphicDataBank*  Graphics;                   // +0x84
    float              MaxDisplayTime;             // +0x88
    float              TextTimer;                  // +0x8C
    long               PlayerModeAdded;            // +0x90
    CRetailVector      PrimitiveHandles;           // +0x94
    CRetailVector      ButtonPrimitiveHandles;     // +0xA0
    CRetailVector      EmbeddedPrimitiveHandles;   // +0xAC
    CRetailVector      EmbeddedGraphics;           // +0xB8
    CRetailVector      EmbeddedHighlights;         // +0xC4
    CRetailVector      EmbeddedHighlightActionNames; // +0xD0
    bool               PausedGame;                 // +0xDC
    bool               AllCharactersDisplayed;     // +0xDD
    bool               InCutscene;                 // +0xDE
    bool               CutsceneSkipped;            // +0xDF
    bool               CutsceneSkippable;          // +0xE0
    bool               CutsceneSkippableWhilePaused; // +0xE1
    bool               CutsceneActionMode;         // +0xE2
    bool               Button1TextOverridden;      // +0xE3
    bool               Button2TextOverridden;      // +0xE4
    bool               DisplayingText;             // +0xE5
    bool               UpdateWhilePaused;          // +0xE6
    bool               UpdateTimer;                // +0xE7
    CFadeInFadeOutBase SkipCutsceneTimer;          // +0xE8
    CFadeInFadeOutBase ButtonPulseTimer;           // +0x104
    bool IsWaitingForClickPast() const;            // 0x0048DB46
    bool IsTextFullyShown() const;                 // 0x0048E072
    bool IsInInteractiveState();
};


struct CTCBase;
struct CPresenceBits { bool test(long bit); };     // 0x00410DE0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;    // 0x004365B0
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline void GetInterface(long type, T** out)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
struct CTCA { unsigned char _pad[0xc]; unsigned char m_c; unsigned char m_d; unsigned char _p2[0xe]; unsigned char m_1c; unsigned char _p3[0xa3]; unsigned char m_c0; };
struct CTCB { unsigned char _pad[0xc]; unsigned char m_c; };
struct CThingCreatureBase : CThing {};
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();   // 0x00487DD0
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CFrontEnd { bool IsActive(); };             // 0x00434D60
struct CMainGameComponent {
    unsigned char _pad0[0x1c];
    CPlayerManager* PPlayerManager;                // +0x1C
    unsigned char _pad1[0x8];
    CFrontEnd* m_pFrontEnd;                        // +0x28
};
struct CHeroInfoScreensManager { unsigned char _pad[0x34]; unsigned char m_open; };
struct CPlayerGui { CHeroInfoScreensManager* GetPHeroInfoScreensManager() const; };   // 0x00437CD0
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

bool CSubtitleRenderer::IsInInteractiveState()
{
    unsigned long flags = DisplayFlags;
    if (!(flags & 0x4c)) {
        if (InCutscene)
            return false;
        if (DisplayingText) {
            if (!((flags & 0x20) && (flags & 2) && IsTextFullyShown() && AllCharactersDisplayed)) {
                if (!(IsWaitingForClickPast() && IsTextFullyShown() && AllCharactersDisplayed))
                    return false;
            }
        } else {
            CThingCreatureBase* hero = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetControlledCreature();
            bool busy = false;
            if (IS_THING_ALIVE<CThing*>(hero) && hero->HasInterface(0xfc)) {
                CTCA* c = hero->GetInterfaceConst<CTCA>(0xfc);
                if (c->m_c && !c->m_1c && !c->m_c0)
                    busy = true;
            }
            if (!busy) {
                CTCA* a = 0;
                CThingCreatureBase* h2 = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
                if (h2->HasInterface(0xfc))
                    a = h2->GetTC<CTCA>(0xfc);
                CTCB* b = 0;
                CThingCreatureBase* h3 = g_pMainGameComponent->PPlayerManager->GetMainPlayer()->GetHeroCreature();
                if (h3->HasInterface(0xc6))
                    b = h3->GetTC<CTCB>(0xc6);
                return g_pMainGameComponent->m_pFrontEnd->IsActive() || (a && a->m_d) || (b && b->m_c)
                    || g_pPlayerGui->GetPHeroInfoScreensManager()->m_open;
            }
        }
    }
    return true;
}