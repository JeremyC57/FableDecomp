#include <cstdio>
#include <cstring>
// Behaviour model: CCharString shares its data block, the CRC stub is a plain FNV-style hash.
static unsigned int Calc(unsigned int seed, const void* data, unsigned int length)
{
    const unsigned char* p = (const unsigned char*)data;
    unsigned int h = seed ^ 2166136261u;
    for (unsigned int i = 0; i < length; ++i) h = (h ^ p[i]) * 16777619u;
    return h;
}
struct CCharStringData { const char* m_pData; unsigned int m_length; };
static CCharStringData g_blocks[8];
static int g_blockCount = 0;
struct CCharString {
    CCharStringData* m_pData;
    CCharString() : m_pData(0) { }
    CCharString(const char* text, long length = -1)
    {
        m_pData = &g_blocks[g_blockCount++ & 7];
        m_pData->m_pData = text;
        m_pData->m_length = (unsigned int)(length < 0 ? std::strlen(text) : length);
    }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
};
struct CAnimFilterOfGroup {
    CCharString  Group;
    unsigned int GroupCRC;
    CAnimFilterOfGroup() : GroupCRC(0) { }
    CAnimFilterOfGroup(const CCharString& group) : Group()
    {
        Group = group;
        GroupCRC = Calc(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);
    }
    void SetGroup(const CCharString& group)
    {
        Group = group;
        GroupCRC = Calc(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);
    }
};
struct CThing { int id; };
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(CThing* item) : m_pItem(0) { m_pItem = item; }
};
struct CModeStandBase {
    CThing* m_pThing;
    CModeStandBase(CThing* thing) : m_pThing(thing) { }
};
static unsigned int CrcOf(const char* s) { return Calc(0, s, (unsigned int)std::strlen(s)); }

struct CModeDragonHover : CModeStandBase {
    CAnimFilterOfGroup Filter;
    CIntelligentPointer PTarget;
    bool UseHeadTurning;
    CModeDragonHover(CThing* thing, CThing* target);
};
CModeDragonHover::CModeDragonHover(CThing* thing, CThing* target)
    : CModeStandBase(thing)
    , Filter("HOVER")
    , PTarget(target)
    , UseHeadTurning(true)
{
}

int main()
{
    CThing thing; thing.id = 1;
    CThing other; other.id = 2;
    CModeDragonHover m(&thing, &other);
    if (!(m.m_pThing == &thing && m.Filter.GroupCRC == CrcOf("HOVER") && m.PTarget.m_pItem == &other && m.UseHeadTurning)) { std::printf("008d8580_TEST FAIL\n"); return 1; }
    std::printf("008d8580_TEST PASS\n");
    return 0;
}
