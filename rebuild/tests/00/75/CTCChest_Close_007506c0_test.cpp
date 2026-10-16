// behaviour test for 0x007506C0 CTCChest::Close
#include <cstdio>
struct Flagged { bool m_flag; };
struct CThing {
    unsigned int bits[9]; Flagged* comp; int forced;
    template<class T> bool GetInterface(T** out, long type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = comp; return true; } return false; }
    void SetForceFrameUpdate(bool f) { if (f) ++forced; }
};
struct Helper { bool Opened, Closed, Opening, Closing; void Close() { Opened = false; Closed = false; Opening = false; Closing = true; } };
struct Chest {
    CThing* ParentThing; Helper ChestOpener; bool ChestOpen;
    void Close()
    {
        if (ChestOpen) {
            ChestOpen = false; ChestOpener.Close(); ParentThing->SetForceFrameUpdate(true);
            Flagged* f; if (ParentThing->GetInterface(&f, 9)) f->m_flag = true;
        }
    }
};
int main()
{
    Flagged fl = { false };
    CThing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.comp = &fl; t.forced = 0;
    Chest c; c.ParentThing = &t; c.ChestOpen = false; c.ChestOpener.Opened = true; c.ChestOpener.Closing = false;
    c.Close();
    bool ok = t.forced == 0 && c.ChestOpener.Opened;
    c.ChestOpen = true; c.Close();
    ok = ok && !c.ChestOpen && !c.ChestOpener.Opened && c.ChestOpener.Closing && t.forced == 1 && !fl.m_flag;
    t.bits[0] = 0x200; c.ChestOpen = true; c.Close();
    ok = ok && fl.m_flag && t.forced == 2;
    if (!ok) { std::printf("007506c0_TEST FAIL\n"); return 1; }
    std::printf("007506c0_TEST PASS\n");
    return 0;
}