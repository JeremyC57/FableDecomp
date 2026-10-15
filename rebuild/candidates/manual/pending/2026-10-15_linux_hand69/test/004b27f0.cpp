// Behaviour test for NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript(const CCharString&) @ 0x004B27F0
#include <cstdio>
#include <string.h>

struct CCharStringData {
    const char* m_text;
    bool operator<(const CCharStringData& other) const { return strcmp(m_text, other.m_text) < 0; }
};

struct CCharString {
    CCharStringData* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return *PStringData < *other.PStringData;
    }
};

struct CPersistInfo { CCharString QuestName; CCharString PersistScript; };
struct SPair { CCharString first; CPersistInfo second; };

struct CPersistMap {
    SPair* m_pFirst; SPair* m_pLast; SPair* m_pEnd; long m_sorted;
    SPair* LowerBound(const CCharString& key) { SPair* it = m_pFirst; while (it != m_pLast && it->first < key) ++it; return it; }
    SPair* Find(const CCharString& key)
    {
        SPair* it = LowerBound(key);
        SPair* last = m_pLast;
        return (it == last || key < it->first) ? last : it;
    }
};

struct CRuntimeQuestPersistence {
    CPersistMap PersistScripts;
    CPersistMap MasterDataPersistScripts;
    const CCharString* GetQuestMasterDataPersistScript(const CCharString& quest)
    {
        SPair* it = MasterDataPersistScripts.Find(quest);
        if (it != MasterDataPersistScripts.m_pLast)
            return &it->second.PersistScript;
        return 0;
    }
};

int main()
{
    CCharStringData a = { "Q_A" }, c = { "Q_C" }, b = { "Q_B" }, z = { "Q_Z" };
    SPair pairs[2];
    pairs[0].first.PStringData = &a; pairs[1].first.PStringData = &c;
    CRuntimeQuestPersistence p;
    p.MasterDataPersistScripts.m_pFirst = pairs; p.MasterDataPersistScripts.m_pLast = pairs + 2; p.MasterDataPersistScripts.m_pEnd = pairs + 2;
    CCharStringData keyC = { "Q_C" };
    CCharString kc = { &keyC }, kb = { &b }, kz = { &z }, ka = { &a }, knull = { 0 };
    if (p.GetQuestMasterDataPersistScript(kc) != &pairs[1].second.PersistScript) { std::printf("004b27f0_TEST FAIL\n"); return 1; }
    if (p.GetQuestMasterDataPersistScript(ka) != &pairs[0].second.PersistScript) { std::printf("004b27f0_TEST FAIL\n"); return 1; }
    if (p.GetQuestMasterDataPersistScript(kb) != 0 || p.GetQuestMasterDataPersistScript(kz) != 0 || p.GetQuestMasterDataPersistScript(knull) != 0) { std::printf("004b27f0_TEST FAIL\n"); return 1; }
    std::printf("004b27f0_TEST PASS\n");
    return 0;
}
