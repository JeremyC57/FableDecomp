// behaviour test for 00e57530 CBS_Teacher::LookForBook (the same search and reply selection over mocked state)
#include <cstdio>
#include <cstring>
struct Teacher {
    long booksInGame, lastRequested; bool carried[8], donated[8], owned[8];
    long BooksWanted, BooksAccepted, BooksComment;
    char said[64]; long asked; float health;
    bool IsActiveThreadTerminating() { return false; }
    void Speak(const char* t) { std::strcpy(said, t); }
    void LookForBook();
};
void Teacher::LookForBook()
{
    long found = -1; bool newlyOwned = false; char buf[64];
    for (long i = 0; i < booksInGame; ++i) {
        if ((i == BooksWanted || i == BooksAccepted || i == BooksComment) && found >= 0) break;
        if (carried[i]) {
            if (i != lastRequested && !donated[i]) {
                if (!newlyOwned) found = i;
                if (!owned[i]) { owned[i] = true; newlyOwned = true; }
            } else if (found == -1) found = -2;
        }
    }
    if (found == -2 || found >= BooksComment) { if (health > 0.0f) Speak("TEXT_QST_B16_BOOK_NOT_FOUND"); }
    else if (found == -1) { if (health > 0.0f) Speak("TEXT_QST_B16_BOOK_NONE_FOUND"); }
    else if (found >= BooksAccepted) { std::sprintf(buf, "TEXT_QST_B16_BOOK_COMMENT_%d", (int)(found - BooksAccepted)); if (health > 0.0f) Speak(buf); }
    else { std::sprintf(buf, "TEXT_QST_B16_BOOK_REQUEST_%d", (int)found); if (health > 0.0f) Speak(buf); asked = found; }
}
static void init(Teacher& t) { std::memset(&t, 0, sizeof(t)); t.booksInGame = 8; t.lastRequested = -1; t.BooksWanted = 3; t.BooksAccepted = 5; t.BooksComment = 7; t.asked = -9; t.health = 1.0f; }
int main()
{
    int ok = 1; Teacher t;
    init(t); t.LookForBook(); if (std::strcmp(t.said, "TEXT_QST_B16_BOOK_NONE_FOUND") != 0) ok = 0;
    init(t); t.carried[1] = true; t.LookForBook(); if (std::strcmp(t.said, "TEXT_QST_B16_BOOK_REQUEST_1") != 0 || t.asked != 1 || !t.owned[1]) ok = 0;
    init(t); t.carried[6] = true; t.LookForBook(); if (std::strcmp(t.said, "TEXT_QST_B16_BOOK_COMMENT_1") != 0) ok = 0;
    init(t); t.carried[2] = true; t.donated[2] = true; t.LookForBook(); if (std::strcmp(t.said, "TEXT_QST_B16_BOOK_NOT_FOUND") != 0) ok = 0;
    if (!ok) { std::printf("00e57530_TEST FAIL\n"); return 1; }
    std::printf("00e57530_TEST PASS\n");
    return 0;
}
