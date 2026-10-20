// behaviour test for 007d5fb0 CTCTavernGameCardPairs::SelectCard: remembers the first selection, drops face-up or repeated
// selections, flips a face-down card with the flip sound, and on two cards plays correct (and clears both) or incorrect
#include <cstdio>
#include <cstring>
static char g_sounds[8][32];
static int g_soundCount = 0, g_baseCalls = 0;
struct CCharString {
    char m_text[48];
    CCharString(const char* text, long) { std::strcpy(m_text, text); }
    ~CCharString() {}
};
struct CCard {
    void* Thing;
    long  Name;
    long  Value;
    bool  Turning;
    bool  FaceDown;
    void Turn() { Turning = true; FaceDown = !FaceDown; }
};
static CCard* g_nextSelection = 0;
struct CTCTavernGameCardBase {
    virtual void v00() {}
    bool    GameStarted;
    CCard*  SelectedCard;
    bool SelectCard() { ++g_baseCalls; SelectedCard = g_nextSelection; return true; }
    unsigned long PlaySound(const CCharString& name) { std::strcpy(g_sounds[g_soundCount++], name.m_text); return 0; }
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    CCard* AlternateSelectedCard;
    bool SelectCard();
};
bool CTCTavernGameCardPairs::SelectCard()
{
    GameStarted = true;
    if (SelectedCard && AlternateSelectedCard)
        return true;
    if (!AlternateSelectedCard)
        AlternateSelectedCard = SelectedCard;
    CTCTavernGameCardBase::SelectCard();
    if (SelectedCard && (SelectedCard->Turning || !SelectedCard->FaceDown))
        SelectedCard = 0;
    if (SelectedCard == AlternateSelectedCard)
        SelectedCard = 0;
    if (SelectedCard && SelectedCard->FaceDown == true && !SelectedCard->Turning) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP", -1);
            PlaySound(sound);
        }
        SelectedCard->Turn();
    }
    if (SelectedCard && AlternateSelectedCard) {
        if (SelectedCard->Value == AlternateSelectedCard->Value) {
            SelectedCard = 0;
            AlternateSelectedCard = 0;
            CCharString sound("TAVERN_GAME_CORRECT", -1);
            PlaySound(sound);
        } else {
            CCharString sound("TAVERN_GAME_INCORRECT", -1);
            PlaySound(sound);
        }
    }
    return true;
}
int main()
{
    CCard first = { 0, 0, 4, false, false };     // already turned up (the first pick)
    CCard match = { 0, 0, 4, false, true };      // face down, same value
    CCard other = { 0, 0, 9, false, true };
    CTCTavernGameCardPairs game;
    game.GameStarted = false; game.SelectedCard = &first; game.AlternateSelectedCard = 0;
    g_nextSelection = &match;
    game.SelectCard();
    bool ok = game.GameStarted && g_baseCalls == 1 && match.FaceDown == false && match.Turning &&
              game.SelectedCard == 0 && game.AlternateSelectedCard == 0 && g_soundCount == 2 &&
              std::strcmp(g_sounds[0], "TAVERN_GAME_CARD_FLIP") == 0 && std::strcmp(g_sounds[1], "TAVERN_GAME_CORRECT") == 0;
    match.Turning = false; first.Value = 4;
    game.SelectedCard = &first; game.AlternateSelectedCard = 0; g_nextSelection = &other;
    game.SelectCard();
    ok = ok && game.SelectedCard == &other && game.AlternateSelectedCard == &first && g_soundCount == 4 &&
         std::strcmp(g_sounds[3], "TAVERN_GAME_INCORRECT") == 0;
    game.SelectCard();                           // both set: returns at once
    ok = ok && g_baseCalls == 2 && g_soundCount == 4;
    if (!ok) { std::printf("007d5fb0_TEST FAIL\n"); return 1; }
    std::printf("007d5fb0_TEST PASS\n");
    return 0;
}