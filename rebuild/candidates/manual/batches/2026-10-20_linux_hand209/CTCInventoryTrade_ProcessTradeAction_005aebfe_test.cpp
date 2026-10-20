// Behaviour test for CTCInventoryTrade::ProcessTradeAction @ 0x005AEBFE (self-contained model of the candidate logic).
#include <stdio.h>
#include <string.h>
struct CDef { int refs; int destroyed; };
// Reference-counting copy: VC7.1 has no NRVO, so the by-value getters return through a copy + destroy of the local.
struct CDefPointer {
    CDef* m_p;
    CDefPointer() : m_p(0) {}
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->refs; }
    ~CDefPointer() { if (m_p) { --m_p->refs; if (m_p->refs == 0) m_p->destroyed = 1; } }
};
static CDef g_tattoo = { 5, 0 }, g_hair = { 5, 0 };
static long g_tattooItem = 0, g_hairItem = 0;
static CDefPointer GetTattooCardDef(long item) { CDefPointer p; p.m_p = item == g_tattooItem ? &g_tattoo : 0; if (p.m_p) ++p.m_p->refs; return p; }
static CDefPointer GetHairCardDef(long item)   { CDefPointer p; p.m_p = item == g_hairItem ? &g_hair : 0; if (p.m_p) ++p.m_p->refs; return p; }
struct CCategory { long id; long GetCategoryIdentifier() { return id; } };
struct Model {
    CCategory* selected; bool allowed; long highlighted;
    char log[64];
    bool IsTradeActionAllowed() { return allowed; }
    CCategory* GetSelectedCategory() { return selected; }
    long GetHighlightedItem() { return highlighted; }
    void ApplyTattooCard(long item) { strcat(log, "T"); }
    void ApplyHairCard(const CDef* card) { strcat(log, card == &g_hair ? "H" : "?"); }
    void BuyHighlightedItem() { strcat(log, "B"); }
    void SellHighlightedItem() { strcat(log, "S"); }
    void ProcessTradeAction()
    {
        long category = 0;
        CCategory* sel = GetSelectedCategory();
        if (sel) category = sel->GetCategoryIdentifier();
        if (IsTradeActionAllowed()) {
            switch (category) {
            case 11: {
                long item = GetHighlightedItem();
                if (item > 0) {
                    CDefPointer tattoo = GetTattooCardDef(item);
                    if (tattoo.m_p) ApplyTattooCard(item);
                    else {
                        CDefPointer hair = GetHairCardDef(item);
                        if (hair.m_p) ApplyHairCard(hair.m_p);
                        else BuyHighlightedItem();
                    }
                }
                break; }
            case 12: SellHighlightedItem(); break;
            }
        }
    }
};
int main()
{
    bool ok = true;
    CCategory buy = { 11 }, sell = { 12 }, other = { 3 };
    Model m; memset(&m, 0, sizeof m); m.allowed = true;
    g_tattooItem = 7; g_hairItem = 9;
    m.selected = &buy; m.highlighted = 7; m.ProcessTradeAction();     // tattoo
    m.highlighted = 9; m.ProcessTradeAction();                         // hair
    m.highlighted = 3; m.ProcessTradeAction();                         // plain purchase
    m.highlighted = 0; m.ProcessTradeAction();                         // nothing highlighted
    m.selected = &sell; m.ProcessTradeAction();                        // sell
    m.selected = &other; m.ProcessTradeAction();                       // unknown category: nothing
    m.selected = 0; m.ProcessTradeAction();                            // no category (0): nothing
    m.allowed = false; m.selected = &sell; m.ProcessTradeAction();     // not allowed
    ok = ok && strcmp(m.log, "THBS") == 0;
    ok = ok && g_tattoo.refs == 5 && g_hair.refs == 5 && !g_tattoo.destroyed && !g_hair.destroyed;   // temporaries released
    if (!ok) { printf("005aebfe_TEST FAIL\n"); return 1; }
    printf("005aebfe_TEST PASS\n"); return 0;
}
