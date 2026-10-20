// behaviour test for 006cfc90 CTCWife::SetAttractionIcon: shown -> married icon (priority 3) once the marriage frame has
// passed, else the ring / no-ring icon (priority 4); hidden -> icon 0 at 1.0 scale, alpha 255; GUI refresh on change only
#include <cstdio>
class CThing;
struct CTCBase;
static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
static long g_priority = -1; static float g_scale = 0; static int g_alpha = -1; static float g_time = 0;
struct CTCEmoteIcon {
    long EmoteIcon;
    void SetEmoteIcon(long priority, long icon, float activeTime, float scale, unsigned char alpha)
    { g_priority = priority; EmoteIcon = icon; g_time = activeTime; g_scale = scale; g_alpha = alpha; }
};
struct CWifeIconDefs { long WifeAttractionIcon; long WifeAttractionIconWithRing; long WifeAttractionIconMarried; };
struct CDefinitionHolder { CWifeIconDefs* Defs; };
static CWifeIconDefs g_icons; static CDefinitionHolder g_holder;
static int g_refreshes = 0;
struct CPlayerGui { void Refresh(long flag) { if (flag == 0) ++g_refreshes; } };
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
struct CTCWife {
    virtual void v00() {}
    CThing*       ParentThing;
    long          FrameGotMarriedToThePlayer;
    bool          ReceivedWeddingRing;
    CDefinitionHolder* GetDefinitions() { return &g_holder; }
    bool IsMarriageFrameReached() const { return FrameGotMarriedToThePlayer <= GFGetCurrentFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);
};
void CTCWife::SetAttractionIcon(bool show, float scale, unsigned char alpha)
{
    CThing* parent = ParentThing;
    if (!parent->HasTC(0x45))
        return;
    CTCEmoteIcon* icon = (CTCEmoteIcon*)parent->GetTC(0x45);
    bool wasShowing = icon->EmoteIcon != 0;
    if (show) {
        if (IsMarriageFrameReached()) {
            CDefinitionHolder* definitions = GetDefinitions();
            icon->SetEmoteIcon(3, definitions->Defs->WifeAttractionIconMarried, -1.0f, scale, alpha);
        } else
            icon->SetEmoteIcon(4, ReceivedWeddingRing ? GetDefinitions()->Defs->WifeAttractionIconWithRing
                                                      : GetDefinitions()->Defs->WifeAttractionIcon,
                               -1.0f, scale, alpha);
    } else {
        bool married = IsMarriageFrameReached();
        icon->SetEmoteIcon(married ? 3 : 4, 0, -1.0f, 1.0f, 0xff);
    }
    if (show != wasShowing)
        g_pPlayerGui->Refresh(0);
}
int main()
{
    g_holder.Defs = &g_icons; g_icons.WifeAttractionIcon = 11; g_icons.WifeAttractionIconWithRing = 12; g_icons.WifeAttractionIconMarried = 13;
    CTCEmoteIcon icon; icon.EmoteIcon = 0;
    CInterfaceMap::SEntry entries[1] = { { 0x45, (CTCBase*)&icon } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_presentBits[2] = 1UL << 5;
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCWife wife; wife.ParentThing = &thing; wife.FrameGotMarriedToThePlayer = 200; wife.ReceivedWeddingRing = true;
    wife.SetAttractionIcon(true, 0.5f, 7);
    bool ok = g_priority == 4 && icon.EmoteIcon == 12 && g_scale == 0.5f && g_alpha == 7 && g_time == -1.0f && g_refreshes == 1;
    wife.ReceivedWeddingRing = false; wife.SetAttractionIcon(true, 0.5f, 7);
    ok = ok && icon.EmoteIcon == 11 && g_refreshes == 1;
    g_frame = 300; wife.SetAttractionIcon(true, 0.75f, 9);
    ok = ok && g_priority == 3 && icon.EmoteIcon == 13 && g_scale == 0.75f && g_refreshes == 1;
    wife.SetAttractionIcon(false, 0.5f, 7);
    ok = ok && g_priority == 3 && icon.EmoteIcon == 0 && g_scale == 1.0f && g_alpha == 255 && g_refreshes == 2;
    g_frame = 100; wife.SetAttractionIcon(false, 0.5f, 7);
    ok = ok && g_priority == 4 && g_refreshes == 2;
    thing.m_presentBits[2] = 0; g_priority = -1; wife.SetAttractionIcon(true, 0.5f, 7);
    ok = ok && g_priority == -1;
    if (!ok) { std::printf("006cfc90_TEST FAIL\n"); return 1; }
    std::printf("006cfc90_TEST PASS\n");
    return 0;
}