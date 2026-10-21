#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCardSorting::SelectCard (0x007D5890): the body of src/ over stand-ins -- every face-up
// card lower than the selected one is highlighted as a mistake (ending the round, and losing the game in type 1);
// otherwise a correct highlighted card plays the "correct" sound.
static int g_baseCalls = 0, g_highlights = 0, g_sounds = 0, g_releases = 0;
static char g_lastSound[32];
static float g_lastTime = 0.0f;
struct CRGBColour { unsigned char B, G, R, A; };
struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) {}
};
namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_3 = 3 };
struct CTCGraphicAppearance {
    int highlighted;
    void SetAsHighlighted(float a, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority priority, void*)
    { ++highlighted; ++g_highlights; g_lastTime = a; (void)priority; }
};
}
struct CThing {
    NGraphicAppearance::CTCGraphicAppearance appearance;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    { if (interfaceType != 0x5b) return false; *ppOut = &appearance; return true; }
};
struct CCard {
    CThing* Thing;
    long    Name;
    long    Value;
    bool    Turning;
    bool    FaceDown;
};
struct CCardVector { CCard** _M_start; CCard** _M_finish; CCard** _M_end_of_storage; };
struct CTavernGameCardBaseDef {
    long       m_count;
    float      HighlightTime;
    CRGBColour HighlightColour;
    void ReleaseTemporary() { --m_count; ++g_releases; }
};
static CTavernGameCardBaseDef g_def = { 1, 2.5f, { 1, 2, 3, 4 } };
struct CCardDefPointer {
    CTavernGameCardBaseDef* Object;
    CCardDefPointer() : Object(0) {}
    ~CCardDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};
struct CTCTavernGameCardBase {
    bool        GameWon;
    bool        GameFinished;
    CCardVector Cards;
    bool        RoundFinished;
    long        GameType;
    CCard*      SelectedCard;
    CCard*      HighlightedCard;
    bool SelectCard() { ++g_baseCalls; return true; }
    void GetCardDef(CCardDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
    unsigned long PlaySound(const CCharString& name) { ++g_sounds; strcpy(g_lastSound, name.text); return 0; }
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    bool SelectCard();
};

bool CTCTavernGameCardSorting::SelectCard()
{
    CTCTavernGameCardBase::SelectCard();
    if (!SelectedCard)
        return true;
    bool mistake = false;
    CCardDefPointer def;
    GetCardDef(&def);
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it) {
        CCard* card = *it;
        if (!card->FaceDown && card->Value < SelectedCard->Value) {
            card->Thing->GetInterface(&appearance, 0x5b);
            appearance->SetAsHighlighted(def->HighlightTime, 0.0f, def->HighlightColour, false, false,
                                         NGraphicAppearance::APPEARANCE_MESH_EFFECT_PRIORITY_3, this);
            mistake = true;
        }
    }
    if (mistake) {
        switch (GameType) {
        case 1:
            GameWon = false;
            break;
        }
        RoundFinished = true;
        GameFinished = true;
        return false;
    }
    if (HighlightedCard && !HighlightedCard->FaceDown && !HighlightedCard->Turning) {
        CCharString sound("TAVERN_GAME_CORRECT", -1);
        PlaySound(sound);
    }
    return true;
}

static int fail(int line) { printf("007d5890_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing things[3];
    for (int i = 0; i < 3; ++i) things[i].appearance.highlighted = 0;
    CCard cards[3];
    for (int i = 0; i < 3; ++i) { cards[i].Thing = &things[i]; cards[i].Name = i; cards[i].Turning = false; }
    cards[0].Value = 1; cards[0].FaceDown = true;     // lower but face down: not a mistake
    cards[1].Value = 5; cards[1].FaceDown = false;
    cards[2].Value = 9; cards[2].FaceDown = false;
    CCard* list[3] = { &cards[0], &cards[1], &cards[2] };
    CTCTavernGameCardSorting game;
    game.Cards._M_start = list; game.Cards._M_finish = list + 3; game.Cards._M_end_of_storage = list + 3;
    game.GameWon = true; game.GameFinished = false; game.RoundFinished = false; game.GameType = 1;
    game.HighlightedCard = &cards[1];

    game.SelectedCard = 0;                             // nothing selected
    CHECK(game.SelectCard() == true && g_baseCalls == 1 && g_releases == 0);

    game.SelectedCard = &cards[1];                     // lowest face-up card: correct
    CHECK(game.SelectCard() == true);
    CHECK(g_highlights == 0 && g_sounds == 1 && strcmp(g_lastSound, "TAVERN_GAME_CORRECT") == 0);
    CHECK(g_releases == 1 && g_def.m_count == 1);

    cards[1].Turning = true;                           // highlighted card still turning: no sound
    CHECK(game.SelectCard() == true && g_sounds == 1);

    game.SelectedCard = &cards[2];                     // a face-up lower card exists: mistake
    CHECK(game.SelectCard() == false);
    CHECK(g_highlights == 1 && things[1].appearance.highlighted == 1 && g_lastTime == 2.5f);
    CHECK(!game.GameWon && game.RoundFinished && game.GameFinished);

    game.GameWon = true; game.GameType = 2;            // other game types keep GameWon
    CHECK(game.SelectCard() == false && game.GameWon);
    printf("007d5890_TEST PASS\n");
    return 0;
}
