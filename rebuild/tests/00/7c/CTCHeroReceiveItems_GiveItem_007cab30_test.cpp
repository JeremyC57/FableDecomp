// Behaviour test for 0x007CAB30 CTCHeroReceiveItems::GiveItem: the item goes into limbo, is queued with its flag at
// the end of the list, and the display starts at the item's position.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    C3DVector Pos;
    bool InLimbo;
    const C3DVector& GetPosition() const { return Pos; }
    void SetInLimbo(bool inLimbo) { InLimbo = inLimbo; }
};
class CThingObject : public CThing { };

template <class T> class CIntelligentPointer { public:
    T* Item;
    CIntelligentPointer(T* item) : Item(item) { }
    CIntelligentPointer(const CIntelligentPointer& other) : Item(other.Item) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<CIntelligentPointer<CThingObject>, bool> SItemEntry;

struct CItemList {
    SItemEntry* items[8];
    int count;
    void push_back(const SItemEntry& x) { items[count++] = new SItemEntry(x); }
};

class CTCHeroReceiveItems { public:
    CItemList Items;
    C3DVector ShownAt;
    int shows;
    void PrepareToShowItems(const C3DVector& position) { ShownAt = position; ++shows; }
    void GiveItem(CThingObject& item, bool persist)
    {
        item.SetInLimbo(true);
        Items.push_back(SItemEntry(CIntelligentPointer<CThingObject>(&item), persist));
        PrepareToShowItems(item.GetPosition());
    }
};

int main()
{
    CTCHeroReceiveItems tc; tc.Items.count = 0; tc.shows = 0;
    CThingObject a; a.InLimbo = false; a.Pos.X = 1; a.Pos.Y = 2; a.Pos.Z = 3;
    CThingObject b; b.InLimbo = false; b.Pos.X = 4; b.Pos.Y = 5; b.Pos.Z = 6;
    tc.GiveItem(a, true);
    tc.GiveItem(b, false);
    bool ok = a.InLimbo && b.InLimbo && tc.Items.count == 2 && tc.Items.items[0]->first.Item == &a &&
              tc.Items.items[0]->second && tc.Items.items[1]->first.Item == &b && !tc.Items.items[1]->second &&
              tc.shows == 2 && tc.ShownAt.X == 4 && tc.ShownAt.Z == 6;
    delete tc.Items.items[0]; delete tc.Items.items[1];
    if (!ok) { std::printf("007cab30_TEST FAIL\n"); return 1; }
    std::printf("007cab30_TEST PASS\n");
    return 0;
}