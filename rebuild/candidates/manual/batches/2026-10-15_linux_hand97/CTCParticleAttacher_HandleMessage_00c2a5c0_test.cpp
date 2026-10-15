// Behaviour test for 0x00C2A5C0: the entry is destroyed; its owner too when the owner's list reports 0.
#include <cstdio>
static int g_entryDtors = 0, g_ownerDtors = 0;
struct CEntryList { long inUse; virtual long IsInUse() { return inUse; } virtual ~CEntryList() {} };
struct COwner { CEntryList m_entries; ~COwner() { ++g_ownerDtors; } };
struct CEntry { COwner* m_pOwner; ~CEntry() { ++g_entryDtors; } };
struct CTCParticleAttacher {
    void HandleMessage(CEntry* entry)
    {
        COwner* owner = entry->m_pOwner;
        delete entry;
        if (owner->m_entries.IsInUse() == 0) delete owner;
    }
};
int main()
{
    CTCParticleAttacher h;
    COwner* owner = new COwner; owner->m_entries.inUse = 1;
    CEntry* e1 = new CEntry; e1->m_pOwner = owner;
    h.HandleMessage(e1);
    if (g_entryDtors != 1 || g_ownerDtors != 0) { std::printf("00c2a5c0_TEST FAIL\n"); return 1; }
    owner->m_entries.inUse = 0;
    CEntry* e2 = new CEntry; e2->m_pOwner = owner;
    h.HandleMessage(e2);
    if (g_entryDtors != 2 || g_ownerDtors != 1) { std::printf("00c2a5c0_TEST FAIL\n"); return 1; }
    std::printf("00c2a5c0_TEST PASS\n");
    return 0;
}
