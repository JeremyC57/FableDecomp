#include <cstdio>
// Behaviour test for CTCTextureDecal::ChangeDecal (0x0070A3E0).
struct SDecalData { long m_words[4]; };
struct CDecalEntry { long m_allocEntryID; long m_unknown; SDecalData m_data; };
struct SDecalNode { SDecalNode* Next; SDecalNode* Prev; CDecalEntry Entry; };
struct CTCTextureDecal {
    SDecalNode* Decals;
    long CurrentAllocEntryID;
    bool Dirty;
    void ChangeDecal(long id, const SDecalData& data);
};
void CTCTextureDecal::ChangeDecal(long id, const SDecalData& data)
{
    for (SDecalNode* node = Decals->Next; node != Decals; node = node->Next) {
        if (node->Entry.m_allocEntryID == id) {
            node->Entry.m_data = data;
            Dirty = true;
        }
    }
}
int main()
{
    SDecalNode s, n[3];
    s.Next = &n[0]; n[0].Next = &n[1]; n[1].Next = &n[2]; n[2].Next = &s;
    s.Prev = &n[2]; n[0].Prev = &s; n[1].Prev = &n[0]; n[2].Prev = &n[1];
    long ids[3] = { 5, 7, 5 };
    for (int i = 0; i < 3; ++i) { n[i].Entry.m_allocEntryID = ids[i]; for (int k = 0; k < 4; ++k) n[i].Entry.m_data.m_words[k] = 0; }
    CTCTextureDecal d; d.Decals = &s; d.Dirty = false;
    SDecalData data = { { 1, 2, 3, 4 } };
    d.ChangeDecal(9, data);
    if (d.Dirty) { std::printf("0070a3e0_TEST FAIL\n"); return 1; }
    d.ChangeDecal(5, data);
    if (!d.Dirty || n[0].Entry.m_data.m_words[3] != 4 || n[2].Entry.m_data.m_words[0] != 1 || n[1].Entry.m_data.m_words[1] != 0)
    { std::printf("0070a3e0_TEST FAIL\n"); return 1; }
    std::printf("0070a3e0_TEST PASS\n");
    return 0;
}