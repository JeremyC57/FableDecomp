// Behaviour test for 007fb610: body copied from src/007fb610.cpp (CTCKraken::RaiseSunkenTentacles), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
template<class T> struct CListNode { CListNode* Next; CListNode* Prev; T Data; };
template<class T> struct CList { CListNode<T>* Node; };
long g_FramesPerSecond = 30;
static int g_frameCalls = 0;
long GetWorldFrame() { ++g_frameCalls; return 1000; }
struct CKrakenDef { unsigned char _pad00[0x78]; float TentacleRiseDelay; };
class CTCKrakenTentacle { public: unsigned char _pad00[0x34]; long TickToRise; };
class CTCBase {
public:
    virtual ~CTCBase() {}
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
class CTCKraken : public CTCBase {
public:
    CKrakenDef* PKrakenDef;
    unsigned char _pad10[0x44 - 0x10];
    CList<CBaseIntelligentPointer> Tentacles;
    void RaiseSunkenTentacles();
};
void CTCKraken::RaiseSunkenTentacles()
{
    CTCKrakenTentacle* tentacle;
    for (CListNode<CBaseIntelligentPointer>* it = Tentacles.Node->Next; it != Tentacles.Node; it = it->Next) {
        it->Data.GetPItem()->GetInterface(&tentacle, 0xe3);
        long fps = g_FramesPerSecond;
        CKrakenDef* def = PKrakenDef;
        tentacle->TickToRise = GetWorldFrame() + (long)(def->TentacleRiseDelay * fps);
    }
}
int main()
{
    static CThing things[2]; static CInterfaceMap::SEntry entries[2][2]; static CTCKrakenTentacle tcs[2]; static CKrakenDef def;
    memset(things, 0, sizeof(things)); memset(tcs, 0, sizeof(tcs));
    for (int i = 0; i < 2; ++i) {
        entries[i][0].type = 0x10; entries[i][1].type = 0xe3; entries[i][1].component = (CTCBase*)&tcs[i];
        things[i].m_interfaces.first = entries[i]; things[i].m_interfaces.last = entries[i] + 2;
        things[i].m_presentBits[0xe3 >> 5] |= 1UL << (0xe3 & 31);
        tcs[i].TickToRise = -1;
    }
    def.TentacleRiseDelay = 2.5f;
    CListNode<CBaseIntelligentPointer> head, n[2];
    head.Next = &n[0]; n[0].Next = &n[1]; n[1].Next = &head;
    n[0].Data.m_pItem = &things[0]; n[1].Data.m_pItem = &things[1];
    CTCKraken kraken; kraken.PKrakenDef = &def; kraken.Tentacles.Node = &head;
    kraken.RaiseSunkenTentacles();
    CHECK(g_frameCalls == 2); CHECK(tcs[0].TickToRise == 1075); CHECK(tcs[1].TickToRise == 1075);
    // empty list
    g_frameCalls = 0; head.Next = &head; kraken.RaiseSunkenTentacles(); CHECK(g_frameCalls == 0);
    if (fails == 0) printf("007fb610_TEST PASS\n"); else printf("007fb610_TEST FAIL\n");
    return fails;
}