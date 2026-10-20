// Behaviour test for 008f1720: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBase::EndRound() -- base EndRound, clears the highlight off the highlighted card's graphic appearance, empties the card list and forgets the selected and highlighted cards
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
// (memmove from <cstring>)

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CTCBase;
struct CCard;
struct CThingObject;

struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    void clear()
    {
        CCard** i = (CCard**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_3 = 3 };
struct CTCGraphicAppearance {
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool c, bool d, EAppearanceMeshEffectPriority priority,
                          void* owner);       // 0x004C04C0
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CCard {
    CThing* Thing;                              // +0x00 CThingObject*
};

struct CTCTavernGame {
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    void EndRound();                            // 0x005E9AA0
};

struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x38];
    CCardVector   Cards;                        // +0x128
    unsigned char _pad_0x134[0x1d4];
    CCard*        SelectedCard;                 // +0x308
    CCard*        HighlightedCard;              // +0x30c
    void EndRound();
};

void CTCTavernGameCardBase::EndRound()
{
    CTCTavernGame::EndRound();
    if (HighlightedCard) {
        CThing* thing = HighlightedCard->Thing;
        NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
        if (thing->HasTC(0x5b))
            appearance = (NGraphicAppearance::CTCGraphicAppearance*)thing->GetTC(0x5b);
        appearance->SetAsHighlighted(0.0f, 0.0f, CRGBColour(0, 0, 0, 0), false, false,
                                     NGraphicAppearance::APPEARANCE_MESH_EFFECT_PRIORITY_3, this);
    }
    Cards.clear();
    SelectedCard = 0;
    HighlightedCard = 0;
}

// ---- stubs and driver ----
void CTCTavernGame::v00() {}
static int g_baseEndRounds;
void CTCTavernGame::EndRound() { ++g_baseEndRounds; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static int g_highlights; static const void* g_owner; static float g_a = 1, g_b = 1; static bool g_c = true, g_d = true; static long g_prio;
static unsigned char g_col[4];
static NGraphicAppearance::CTCGraphicAppearance g_appearance;
void NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float a, float b, const CRGBColour& colour, bool c, bool d,
    EAppearanceMeshEffectPriority priority, void* owner)
{
    if (this == &g_appearance) ++g_highlights;
    g_a = a; g_b = b; g_c = c; g_d = d; g_prio = priority; g_owner = owner;
    g_col[0] = colour.B; g_col[1] = colour.G; g_col[2] = colour.R; g_col[3] = colour.A;
}

int main()
{
    static CTCTavernGameCardBase game;
    static CThing thing;
    static CCard card;
    static CCard* cards[3];
    card.Thing = &thing;
    thing.m_presentBits[0x5b >> 5] = 1UL << (0x5b & 31);
    g_entries[0].type = 0x5b; g_entries[0].component = (CTCBase*)&g_appearance;
    g_entries[1].type = 0x70; g_entries[1].component = 0;
    thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 2;
    game.Cards._M_start = cards; game.Cards._M_finish = cards + 3; game.Cards._M_end_of_storage = cards + 3;
    game.HighlightedCard = &card; game.SelectedCard = &card;
    game.EndRound();
    if (g_baseEndRounds != 1 || g_highlights != 1 || g_a != 0.0f || g_b != 0.0f || g_c || g_d || g_prio != 3 || g_owner != &game
        || g_col[0] || g_col[1] || g_col[2] || g_col[3]) { std::printf("008f1720_TEST FAIL (highlight)\n"); return 1; }
    if (game.Cards._M_finish != cards || game.Cards._M_start != cards || game.SelectedCard || game.HighlightedCard)
        { std::printf("008f1720_TEST FAIL (clear)\n"); return 1; }
    // no highlighted card: no appearance call
    game.Cards._M_finish = cards + 2; game.SelectedCard = &card;
    game.EndRound();
    if (g_baseEndRounds != 2 || g_highlights != 1 || game.Cards._M_finish != cards || game.SelectedCard)
        { std::printf("008f1720_TEST FAIL (no highlight)\n"); return 1; }
    std::printf("008f1720_TEST PASS\n");
    return 0;
}