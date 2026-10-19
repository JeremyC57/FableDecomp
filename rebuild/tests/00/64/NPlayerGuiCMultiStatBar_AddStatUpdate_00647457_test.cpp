// behaviour copy of NPlayerGui::CMultiStatBar::AddStatUpdate: merge into an update of the same type or queue a new one,
// seeding the colour / graphic / effect for that type when unset; mode 1 becomes 2
#include <cstdio>
struct CRGBColour { unsigned char B, G, R, A; };
struct CStatUpdate { int Type; long Change; CRGBColour Colour; };
struct CUpdateNode { CUpdateNode* _M_next; CUpdateNode* _M_prev; CStatUpdate _M_data; };
static CUpdateNode g_pool[8]; static int g_used = 0;
struct CUpdateList {
    CUpdateNode* _M_node;
    void push_back(const CStatUpdate& u)
    {
        CUpdateNode* n = &g_pool[g_used++]; n->_M_data = u;
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev; _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
};
struct COwner { CRGBColour UpdateColour; CRGBColour TypeColours[4]; long TypeGraphics[4]; long TypeEffects[4]; };
struct CMultiStatBar {
    COwner* POwner; long DisplayMode; CUpdateList Updates; CRGBColour CurrentColour; long CurrentGraphic; long CurrentEffect;
    void AddStatUpdate(int type, long change)
    {
        for (CUpdateNode* it = Updates._M_node->_M_next; it != Updates._M_node; it = it->_M_next) {
            if (it->_M_data.Type == type) {
                it->_M_data.Colour = POwner->UpdateColour;
                it->_M_data.Change += change;
                goto done;
            }
        }
        {
            CStatUpdate update;
            update.Change = change;
            update.Colour = POwner->UpdateColour;
            update.Type = type;
            Updates.push_back(update);
        }
        if (CurrentColour.R == 0 && CurrentColour.G == 0 && CurrentColour.B == 0 && CurrentColour.A == 0xff)
            CurrentColour = POwner->TypeColours[type];
        if (CurrentGraphic == 0)
            CurrentGraphic = POwner->TypeGraphics[type];
        if (CurrentEffect == 0)
            CurrentEffect = POwner->TypeEffects[type];
    done:
        if (DisplayMode == 1)
            DisplayMode = 2;
    }
};
int main()
{
    CUpdateNode sentinel; sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    COwner o; CRGBColour c1 = { 1, 2, 3, 4 }; o.UpdateColour = c1;
    for (int i = 0; i < 4; ++i) { CRGBColour c = { 0, 0, (unsigned char)(10 + i), 0xff }; o.TypeColours[i] = c; o.TypeGraphics[i] = 100 + i; o.TypeEffects[i] = 200 + i; }
    CMultiStatBar b; b.POwner = &o; b.DisplayMode = 1; b.Updates._M_node = &sentinel;
    CRGBColour black = { 0, 0, 0, 0xff }; b.CurrentColour = black; b.CurrentGraphic = 0; b.CurrentEffect = 7;
    b.AddStatUpdate(2, 5);
    if (g_used != 1 || sentinel._M_next != &g_pool[0] || g_pool[0]._M_data.Type != 2 || g_pool[0]._M_data.Change != 5
        || g_pool[0]._M_data.Colour.A != 4 || b.CurrentColour.R != 12 || b.CurrentGraphic != 102 || b.CurrentEffect != 7 || b.DisplayMode != 2)
    { std::printf("00647457_TEST FAIL\n"); return 1; }
    CRGBColour c2 = { 9, 9, 9, 9 }; o.UpdateColour = c2; b.DisplayMode = 1; b.CurrentGraphic = 0;
    b.AddStatUpdate(2, 3);
    if (g_used != 1 || g_pool[0]._M_data.Change != 8 || g_pool[0]._M_data.Colour.B != 9 || b.CurrentGraphic != 0 || b.DisplayMode != 2)
    { std::printf("00647457_TEST FAIL\n"); return 1; }
    std::printf("00647457_TEST PASS\n");
    return 0;
}