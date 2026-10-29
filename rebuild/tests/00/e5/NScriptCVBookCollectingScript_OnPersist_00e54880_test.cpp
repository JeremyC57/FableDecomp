// Behaviour test for 0x00E54880 NScript::CV_BookCollectingScript::OnPersist: eight scalar members (bool / long, zero
// defaults) and the two book vectors are transferred in retail's order under their member names.
#include <cstdio>
#include <cstring>

struct CBoolVector { int Size; };
class CPersistContext { public:
    char Log[512];
    void Transfer(const char* name, bool& value, const bool& def) { std::strcat(Log, name); std::strcat(Log, def ? ":b1;" : ":b0;"); value = true; }
    void Transfer(const char* name, long& value, const long& def) { char t[16]; std::sprintf(t, ":l%ld;", def); std::strcat(Log, name); std::strcat(Log, t); value = 5; }
    void TransferVector(const char* name, CBoolVector& v) { std::strcat(Log, name); std::strcat(Log, ":v;"); v.Size = 3; }
};
struct CV_BookCollectingScript {
    bool ReadingBook; bool DoneIntro, HatRewarded, KeyRewarded; long GossipState; bool HasInformation;
    long BooksDonated, GoodBooksDonated, LastBookRequested, BooksInGame; CBoolVector BookDonated, BookOwned;
    void OnPersist(CPersistContext& context)
    {
        context.Transfer("DoneIntro", DoneIntro, false);
        context.Transfer("HatRewarded", HatRewarded, false);
        context.Transfer("KeyRewarded", KeyRewarded, false);
        context.Transfer("GossipState", GossipState, 0L);
        context.Transfer("HasInformation", HasInformation, false);
        context.Transfer("BooksDonated", BooksDonated, 0L);
        context.Transfer("GoodBooksDonated", GoodBooksDonated, 0L);
        context.Transfer("LastBookRequested", LastBookRequested, 0L);
        context.TransferVector("BookDonated", BookDonated);
        context.TransferVector("BookOwned", BookOwned);
    }
};

int main()
{
    CV_BookCollectingScript s; std::memset(&s, 0, sizeof(s));
    CPersistContext c; c.Log[0] = 0;
    s.OnPersist(c);
    int ok = !std::strcmp(c.Log, "DoneIntro:b0;HatRewarded:b0;KeyRewarded:b0;GossipState:l0;HasInformation:b0;BooksDonated:l0;"
                                 "GoodBooksDonated:l0;LastBookRequested:l0;BookDonated:v;BookOwned:v;")
             && s.DoneIntro && s.HatRewarded && s.KeyRewarded && s.GossipState == 5 && s.HasInformation && s.BooksDonated == 5
             && s.GoodBooksDonated == 5 && s.LastBookRequested == 5 && s.BookDonated.Size == 3 && s.BookOwned.Size == 3
             && !s.ReadingBook && s.BooksInGame == 0;
    if (!ok) { std::printf("00e54880_TEST FAIL\n"); return 1; }
    std::printf("00e54880_TEST PASS\n");
    return 0;
}