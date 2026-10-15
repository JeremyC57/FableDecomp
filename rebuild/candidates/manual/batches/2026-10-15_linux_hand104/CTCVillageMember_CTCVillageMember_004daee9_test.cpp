#include <cstdio>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 TUID;
#else
typedef unsigned long long TUID;
#endif
struct CThing { int id; };
struct CTCBase { CThing* Thing; CTCBase(CThing* t) : Thing(t) { } virtual ~CTCBase() { } };
struct CVillageMemberDef;
struct CDefPointerVillageMemberDef { const CVillageMemberDef* Object; CDefPointerVillageMemberDef() : Object(0) { } };
struct CIntelligentPointer { CThing* m_pItem; CIntelligentPointer(CThing* t) : m_pItem(t) { } };
struct CTCVillageMember : CTCBase {
    CDefPointerVillageMemberDef PDef; CIntelligentPointer PVillage; TUID VillageUID;
    CTCVillageMember(CThing* thing) : CTCBase(thing), PDef(), PVillage(0), VillageUID(0) { }
};
int main()
{
    CThing owner;
    unsigned char raw[sizeof(CTCVillageMember)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0x5A;
    CTCVillageMember* m = new (raw) CTCVillageMember(&owner);
    if (m->Thing != &owner || m->PDef.Object != 0 || m->PVillage.m_pItem != 0 || m->VillageUID != 0) { std::printf("004daee9_TEST FAIL\n"); return 1; }
    std::printf("004daee9_TEST PASS\n");
    return 0;
}
