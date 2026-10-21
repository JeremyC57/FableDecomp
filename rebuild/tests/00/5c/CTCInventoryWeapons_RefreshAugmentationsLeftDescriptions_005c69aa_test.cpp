#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions (0x005C69AA): same body as src/, with the
// item gather, the child lookup, the description builder and the component text stubbed and recorded.
struct CCharString { const char* s; };
static CCharString g_AugmentationsLeftDescriptionM = { "Augmentations_Left_Description_M" };
static CCharString g_AugmentationsLeftDescriptionR = { "Augmentations_Left_Description_R" };
static int g_strings = 0;
struct CWideString { int v; CWideString() : v(0) { ++g_strings; } ~CWideString() { --g_strings; } };

template <class T1, class T2> struct pair { T1 first; T2 second; };
typedef pair<long, long> CItem;
static int g_frees = 0;
struct CItemVector {
    CItem* _M_start; CItem* _M_finish; CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector() { if (_M_start) { ++g_frees; std::free(_M_start); } }
    unsigned long size() const { return _M_finish - _M_start; }
    CItem& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(const CItem& x)
    {
        unsigned long n = size();
        CItem* p = (CItem*)std::malloc((n + 1) * sizeof(CItem));
        for (unsigned long i = 0; i < n; ++i) p[i] = _M_start[i];
        p[n] = x;
        if (_M_start) std::free(_M_start);
        _M_start = p; _M_finish = p + n + 1; _M_end_of_storage = _M_finish;
    }
};
struct CTCInventoryItem { static bool IsAugmentation(long d) { return d >= 500; } };
namespace NUISystem { struct CComponent; }
struct SComponentNode { SComponentNode* _M_next; SComponentNode* _M_prev; NUISystem::CComponent* _M_data; };
static int g_listDtors = 0;
struct CComponentList {
    SComponentNode* _M_node;
    SComponentNode head; SComponentNode pool[8]; int used;
    CComponentList() : used(0) { _M_node = &head; head._M_next = head._M_prev = &head; }
    ~CComponentList() { ++g_listDtors; }
    void clear() { head._M_next = head._M_prev = &head; }
    void push_back(NUISystem::CComponent* c)
    {
        SComponentNode* n = &pool[used++]; n->_M_data = c;
        n->_M_next = &head; n->_M_prev = head._M_prev; head._M_prev->_M_next = n; head._M_prev = n;
    }
};
namespace NUISystem {
struct CComponent {
    int textValue; int sets; bool a, b;
    CComponent* kidsM[2]; CComponent* kidsR[2];
    void FindChildren(CCharString name, CComponentList& out)
    {
        out.clear();
        CComponent** k = std::strcmp(name.s, "Augmentations_Left_Description_M") == 0 ? kidsM : kidsR;
        for (int i = 0; i < 2; ++i) if (k[i]) out.push_back(k[i]);
    }
    void SetText(const CWideString& text, bool x, bool y) { textValue = text.v; ++sets; a = x; b = y; }
};
}
namespace NInventory {
struct CTCInventoryBase {
    CItem stock[4];
    void GetAllItems(CItemVector& items) const { for (int i = 0; i < 4; ++i) items.push_back(stock[i]); }
};
}
static int g_builds = 0;
struct CTCInventoryWeapons : NInventory::CTCInventoryBase {
    NUISystem::CComponent* WeaponsList;
    void ConstructAugmentationsLeftDescription(CWideString& text, CItemVector& augmentations, bool melee)
    {
        ++g_builds;
        long total = 0;
        for (unsigned long i = 0; i < augmentations.size(); ++i) total += augmentations[i].second;
        text.v = (melee ? 1000 : 2000) + total * 10 + (long)augmentations.size();
    }
    void RefreshAugmentationsLeftDescriptions();
};

void CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions()
{
    if (WeaponsList) {
        CItemVector items;
        CItemVector augmentations;
        GetAllItems(items);
        for (unsigned long i = 0; i < items.size(); ++i)
            if (CTCInventoryItem::IsAugmentation(items[i].first))
                augmentations.push_back(items[i]);
        CComponentList components;
        WeaponsList->FindChildren(g_AugmentationsLeftDescriptionM, components);
        for (SComponentNode* n = components._M_node->_M_next, *end = components._M_node; n != end; n = n->_M_next) {
            NUISystem::CComponent* component = n->_M_data;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, true);
            component->SetText(text, true, true);
        }
        WeaponsList->FindChildren(g_AugmentationsLeftDescriptionR, components);
        for (SComponentNode* n = components._M_node->_M_next, *end = components._M_node; n != end; n = n->_M_next) {
            NUISystem::CComponent* component = n->_M_data;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, false);
            component->SetText(text, true, true);
        }
    }
}

int main()
{
    int fails = 0;
    NUISystem::CComponent list, m1, m2, r1;
    std::memset(&list, 0, sizeof list); std::memset(&m1, 0, sizeof m1); std::memset(&m2, 0, sizeof m2); std::memset(&r1, 0, sizeof r1);
    list.kidsM[0] = &m1; list.kidsM[1] = &m2; list.kidsR[0] = &r1;
    CTCInventoryWeapons tc;
    tc.stock[0].first = 501; tc.stock[0].second = 2;
    tc.stock[1].first = 10;  tc.stock[1].second = 7;
    tc.stock[2].first = 600; tc.stock[2].second = 3;
    tc.stock[3].first = 499; tc.stock[3].second = 9;
    tc.WeaponsList = 0;
    tc.RefreshAugmentationsLeftDescriptions();
    if (g_builds != 0 || g_listDtors != 0 || g_frees != 0) ++fails;            // no list: nothing at all
    tc.WeaponsList = &list;
    tc.RefreshAugmentationsLeftDescriptions();
    if (g_builds != 3 || g_listDtors != 1 || g_frees != 2 || g_strings != 0) ++fails;
    if (m1.textValue != 1052 || m2.textValue != 1052 || r1.textValue != 2052) ++fails;   // 2 augmentations, 5 total
    if (m1.sets != 1 || r1.sets != 1 || !m1.a || !m1.b || !r1.a || !r1.b) ++fails;
    if (fails) { std::printf("005c69aa_TEST FAIL %d\n", fails); return 1; }
    std::printf("005c69aa_TEST PASS\n");
    return 0;
}