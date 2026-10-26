// Behaviour test for CTCOwnedEntity::OnForceRecreateUID @ 0x007E8370: copy the UID of a live owner only.
#include <cstdio>
class CThing { public: unsigned long uidLow, uidHigh; unsigned char m_flags91; };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
class CTCOwnedEntity { public:
    CBaseIntelligentPointer POwner;
    unsigned long OwnerUIDLow, OwnerUIDHigh;
    void OnForceRecreateUID()
    {
        if (IS_THING_ALIVE(POwner)) { OwnerUIDLow = POwner.GetPItem()->uidLow; OwnerUIDHigh = POwner.GetPItem()->uidHigh; }
    }
};
int main()
{
    CThing dead = { 5, 6, 1 }, alive = { 7, 8, 0 };
    CTCOwnedEntity e; e.OwnerUIDLow = 1; e.OwnerUIDHigh = 2; e.POwner.m_pItem = 0;
    e.OnForceRecreateUID();
    e.POwner.m_pItem = &dead; e.OnForceRecreateUID();
    if (e.OwnerUIDLow != 1 || e.OwnerUIDHigh != 2) { std::printf("007e8370_TEST FAIL\n"); return 1; }
    e.POwner.m_pItem = &alive; e.OnForceRecreateUID();
    if (e.OwnerUIDLow != 7 || e.OwnerUIDHigh != 8) { std::printf("007e8370_TEST FAIL\n"); return 1; }
    std::printf("007e8370_TEST PASS\n");
    return 0;
}