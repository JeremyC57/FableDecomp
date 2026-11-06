// Behaviour test for CBS_Teacher::AskForBook (0x00E55CE0): yes -> the book is taken, marked donated, counted (and counted as good
// when it is a wanted book, with the morality reward), the book's cut-scene plays, the good hat comes once GoodBooksDonated reaches
// GoodBooksForHat, the evil hat once the bad count reaches BadBooksForHat, the key (after a hat) when BooksDonated == BooksAccepted,
// and the result is true; no -> a wanted book is remembered as LastBookRequested with the given refusal line, else never mind;
// result false.
#include <cstdio>
#include <string>
#include <vector>

struct Teacher {
    long badForHat, goodForHat, wanted, accepted, morality;
    long donatedCount, goodCount, lastRequested; bool hat, key, reading; std::vector<bool> donated;
    std::vector<std::string> cutscenes; std::string line, prize; float moralityGiven; std::vector<long> reactions;
    Teacher() : badForHat(0), goodForHat(0), wanted(0), accepted(0), morality(0), donatedCount(0), goodCount(0), lastRequested(0),
        hat(false), key(false), reading(false), moralityGiven(0.0f) { }   // VC7.1: no value-init
    bool Ask(long book, const std::string& refusal, bool yes)
    {
        bool result = false;
        if (yes) {
            reading = true; donated[book] = true; ++donatedCount; result = true;
            if (book < wanted) { ++goodCount; line = "TEXT_QST_B16_BOOK_ACCEPTED"; moralityGiven += (float)morality; }
            else line = "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY";
            char n[16]; std::sprintf(n, "%ld", book); cutscenes.push_back(std::string("CS_SCHOOLBOOK_") + n);
            if (!hat) {
                if (goodCount >= goodForHat) { prize = "OBJECT_HERO_HAT_WIZARD_GOOD"; cutscenes.push_back("CS_SCHOOLBOOK_TEACHER_GIVES_HAT"); hat = true; }
                else if (donatedCount - goodCount >= badForHat) { prize = "OBJECT_HERO_HAT_WIZARD_EVIL"; cutscenes.push_back("CS_SCHOOLBOOK_TEACHER_GIVES_HAT"); hat = true; }
            } else if (!key) {
                if (donatedCount == accepted) { prize = "OBJECT_SILVER_KEY"; cutscenes.push_back("CS_SCHOOLBOOK_TEACHER_GIVES_KEY"); key = true; }
            }
            reading = false; reactions.push_back(book);
        } else if (book < wanted) { lastRequested = book; line = refusal; }
        else line = "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND";
        return result;
    }
};

int main()
{
    bool ok = true;
    Teacher t = Teacher(); t.badForHat = 2; t.goodForHat = 2; t.wanted = 3; t.accepted = 3; t.morality = 5; t.donated.resize(8); t.lastRequested = -1;
    ok = ok && t.Ask(0, "R", true) && t.goodCount == 1 && !t.hat && t.cutscenes.back() == "CS_SCHOOLBOOK_0" && t.moralityGiven == 5.0f;
    ok = ok && t.Ask(1, "R", true) && t.hat && t.prize == "OBJECT_HERO_HAT_WIZARD_GOOD";
    ok = ok && t.Ask(5, "R", true) && t.key && t.prize == "OBJECT_SILVER_KEY" && t.line == "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY";
    ok = ok && t.donated[5] && t.reactions.size() == 3 && !t.reading;
    ok = ok && !t.Ask(2, "TEXT_QST_B16_BOOK_REFUSED", false) && t.lastRequested == 2 && t.line == "TEXT_QST_B16_BOOK_REFUSED";
    ok = ok && !t.Ask(6, "X", false) && t.line == "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND" && t.lastRequested == 2;
    Teacher e = Teacher(); e.badForHat = 2; e.goodForHat = 9; e.wanted = 1; e.accepted = 9; e.donated.resize(8);
    e.Ask(4, "R", true); e.Ask(5, "R", true); ok = ok && e.hat && e.prize == "OBJECT_HERO_HAT_WIZARD_EVIL";
    if (!ok) { std::printf("00e55ce0_TEST FAIL\n"); return 1; }
    std::printf("00e55ce0_TEST PASS\n");
    return 0;
}