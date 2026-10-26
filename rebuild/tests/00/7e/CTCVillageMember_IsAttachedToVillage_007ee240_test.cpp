#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 TUID;
#else
typedef unsigned long long TUID;
#endif
class CThing { public: int id; };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CTCVillageMember { public:
    CBaseIntelligentPointer PVillage; TUID VillageUID;
    bool IsAttachedToVillage() const { if (PVillage.GetPItem()) return true; return VillageUID != 0; }
};
int main()
{
    CThing v; CTCVillageMember m;
    m.PVillage.m_pItem = 0; m.VillageUID = 0;
    if (m.IsAttachedToVillage()) { std::printf("007ee240_TEST FAIL\n"); return 1; }
    m.VillageUID = 1; m.VillageUID <<= 40;          // only the high word set
    if (!m.IsAttachedToVillage()) { std::printf("007ee240_TEST FAIL\n"); return 1; }
    m.VillageUID = 0; m.PVillage.m_pItem = &v;
    if (!m.IsAttachedToVillage()) { std::printf("007ee240_TEST FAIL\n"); return 1; }
    std::printf("007ee240_TEST PASS\n");
    return 0;
}