// Behaviour test for CTCOwnedEntity::vfunc_19 @ 0x007E8440: true only for a set, live owner.
#include <cstdio>
struct CThing { unsigned char m_flags91; };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CTCOwnedEntity {
    CBaseIntelligentPointer POwner;
    bool vfunc_19() const
    {
        CThing* owner = POwner.GetPItem();
        if (owner && !(owner->m_flags91 & 1)) return true;
        return false;
    }
};
int main()
{
    CThing alive = { 2 }, dead = { 3 };
    CTCOwnedEntity e; e.POwner.m_pItem = 0;
    if (e.vfunc_19()) { std::printf("007e8440_TEST FAIL\n"); return 1; }
    e.POwner.m_pItem = &dead;
    if (e.vfunc_19()) { std::printf("007e8440_TEST FAIL\n"); return 1; }
    e.POwner.m_pItem = &alive;
    if (!e.vfunc_19()) { std::printf("007e8440_TEST FAIL\n"); return 1; }
    std::printf("007e8440_TEST PASS\n");
    return 0;
}