// Behaviour test for NScript::CV_BookCollectingScript::CBS_Teacher::Main (0x00E54E90): the gossip stage from the books donated
// (<4 and stage 0 -> 1, >3 and stage 1 -> 2, >9 and stage <3 -> 3); the information marker only between 845 and 1615; when talked
// to: the intro once, then in school hours ask again for the last requested book (a hand-over clears it) or look for books.
#include <cstdio>

struct Teacher {
    long Books, Gossip, LastBook; bool Info, DoneIntro;
    void SetGossip()
    {
        if (Books < 4 && Gossip == 0) Gossip = 1;
        else if (Books > 3 && Gossip == 1) Gossip = 2;
        else if (Books > 9 && Gossip < 3) Gossip = 3;
    }
    void Marker(long t)
    {
        if (Info && (t > 1615 || t < 845)) Info = false;
        else if (!Info && t > 845 && t < 1615) Info = true;
    }
    int Talk(long t, bool has_book, bool hands_over)
    {
        if (!DoneIntro) { DoneIntro = true; return 1; }
        if (t >= 845 && t <= 1615) {
            if (LastBook >= 0) { if (has_book && hands_over) { LastBook = -1; return 2; } return 3; }
            LastBook = 4; return 4;
        }
        return 5;
    }
};

int main()
{
    Teacher b = { 0, 0, -1, false, false };
    bool ok = true;
    b.SetGossip(); ok = ok && b.Gossip == 1;
    b.Books = 5; b.SetGossip(); ok = ok && b.Gossip == 2;
    b.Books = 10; b.SetGossip(); ok = ok && b.Gossip == 3;
    b.Marker(900); ok = ok && b.Info; b.Marker(1700); ok = ok && !b.Info;
    ok = ok && b.Talk(900, false, false) == 1 && b.Talk(900, false, false) == 4 && b.Talk(900, true, false) == 3;
    ok = ok && b.Talk(900, true, true) == 2 && b.LastBook == -1 && b.Talk(2000, false, false) == 5;
    if (!ok) { std::printf("00e54e90_TEST FAIL\n"); return 1; }
    std::printf("00e54e90_TEST PASS\n");
    return 0;
}
