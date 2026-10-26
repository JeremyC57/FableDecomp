// Behaviour test for 006c0f70 CTCBuyableHouse::CreateDressingSignPost: the created sign post (non-main, named by the global def name) ends up in DressingSignPost.
#include <cstdio>

class CThing { public: int id; };
class CCharString { public: const char* PStringData; };

struct CBaseIntelligentPointer
{
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    virtual void v00() {}
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

static const CCharString g_DressingSignPostDefName = { "SIGNPOST_DRESSING" };
static CThing g_created = { 42 };
static const CCharString* g_lastName = 0;
static bool g_lastMain = true;

class CTCBuyableHouse
{ public:
    CIntelligentPointer<CThing> MainSignPost;
    CIntelligentPointer<CThing> DressingSignPost;

    CThing* CreateSignPost(const CCharString& defName, bool mainSign) { g_lastName = &defName; g_lastMain = mainSign; return &g_created; }
    void CreateDressingSignPost()
    {
        DressingSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_DressingSignPostDefName, false));
    }
};

int main()
{
    CTCBuyableHouse house;
    house.CreateDressingSignPost();
    if (house.DressingSignPost.GetPItem() != &g_created || house.MainSignPost.GetPItem() != 0 ||
        g_lastName != &g_DressingSignPostDefName || g_lastMain)
    {
        std::printf("006c0f70_TEST FAIL\n");
        return 1;
    }
    std::printf("006c0f70_TEST PASS\n");
    return 0;
}