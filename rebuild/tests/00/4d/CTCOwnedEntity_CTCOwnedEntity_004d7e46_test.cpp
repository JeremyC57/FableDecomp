// Behaviour test for CTCOwnedEntity::CTCOwnedEntity @ 0x004D7E46: owner forwarded, flag false, empty pointer, UID 0.
#include <cstdio>
struct CThing { long id; };
struct CBaseIntelligentPointer { CThing* m_pItem; CBaseIntelligentPointer() : m_pItem(0) {} };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* owner) : ParentThing(owner) {} virtual ~CTCBase() {} };
struct CTCOwnedEntity : CTCBase {
    bool SwitchableNavigationTCAdded;
    CBaseIntelligentPointer POwner;
    unsigned long OwnerUIDLow, OwnerUIDHigh;
    CTCOwnedEntity(CThing* owner) : CTCBase(owner), SwitchableNavigationTCAdded(false), OwnerUIDLow(0), OwnerUIDHigh(0) {}
};
int main()
{
    CThing t; t.id = 1;
    CTCOwnedEntity e(&t);
    if (e.ParentThing != &t || e.SwitchableNavigationTCAdded || e.POwner.m_pItem != 0 || e.OwnerUIDLow != 0 || e.OwnerUIDHigh != 0)
    { std::printf("004d7e46_TEST FAIL\n"); return 1; }
    std::printf("004d7e46_TEST PASS\n");
    return 0;
}