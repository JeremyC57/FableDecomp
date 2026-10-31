// Behaviour test for CTCBuyableHouse::CreateMainSignPost (0x006C0F00): a copy of the model; CreateSignPost records the def name
// it was asked for and returns a distinct thing.
#include <cstdio>

class CBaseObject { public: int id; };
class CThing : public CBaseObject { };
class CCharString { public: const char* PStringData; };

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
private:
    CBaseObject* PData;
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

const CCharString g_MainSignPostDefName  = { "OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST" };
const CCharString g_OwnedSignPostDefName = { "OBJECT_BUYABLE_HOUSE_OWNED_SIGNPOST" };
const CCharString g_RentSignPostDefName  = { "OBJECT_BUYABLE_HOUSE_RENT_SIGNPOST" };

CThing g_things[3];
int g_created = 0;
const CCharString* g_lastName = 0;
bool g_lastMain = false;

class CTCBuyableHouse {
public:
    CTCBuyableHouse() : OwnedByPlayer(false), Rented(false), MainSignPost(0) {}
    bool OwnedByPlayer;
    bool Rented;
    CIntelligentPointer<CThing> MainSignPost;
    CThing* CreateSignPost(const CCharString& defName, bool mainSign)
    {
        g_lastName = &defName; g_lastMain = mainSign;
        return &g_things[g_created++ % 3];
    }
    void CreateMainSignPost()
    {
        if (OwnedByPlayer) {
            if (Rented)
                MainSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_RentSignPostDefName, true));
            else
                MainSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_OwnedSignPostDefName, true));
        }
        else
            MainSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_MainSignPostDefName, true));
    }
};

int main()
{
    CTCBuyableHouse house;
    house.CreateMainSignPost();
    if (g_lastName != &g_MainSignPostDefName || !g_lastMain || house.MainSignPost.GetPItem() != &g_things[0]) {
        std::printf("006c0f00_TEST FAIL\n"); return 1;
    }
    house.OwnedByPlayer = true;
    house.CreateMainSignPost();
    if (g_lastName != &g_OwnedSignPostDefName || !g_lastMain || house.MainSignPost.GetPItem() != &g_things[1]) {
        std::printf("006c0f00_TEST FAIL\n"); return 1;
    }
    house.Rented = true;
    house.CreateMainSignPost();
    if (g_lastName != &g_RentSignPostDefName || !g_lastMain || house.MainSignPost.GetPItem() != &g_things[2]) {
        std::printf("006c0f00_TEST FAIL\n"); return 1;
    }
    house.OwnedByPlayer = false;                             // rented but not owned: still the main for-sale sign
    house.CreateMainSignPost();
    if (g_lastName != &g_MainSignPostDefName || house.MainSignPost.GetPItem() != &g_things[0]) {
        std::printf("006c0f00_TEST FAIL\n"); return 1;
    }
    std::printf("006c0f00_TEST PASS\n");
    return 0;
}