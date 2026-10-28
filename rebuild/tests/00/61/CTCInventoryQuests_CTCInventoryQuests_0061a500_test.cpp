// Behaviour test for 0x0061A500 CTCInventoryQuests::~CTCInventoryQuests() (user body: delete every component by index, then clear)
#include <cstdio>
#include <cstring>

static int g_deleted = 0;
struct CComponent { virtual ~CComponent() { ++g_deleted; } };
struct CComponentVector {
    CComponent* Items[8]; CComponent** Finish;
    CComponent** begin() { return Items; }
    CComponent** end() { return Finish; }
    unsigned long size() const { return (unsigned long)(Finish - Items); }
    CComponent*& operator[](unsigned long n) { return Items[n]; }
    void clear()
    {
        CComponent** first = begin(); CComponent** last = end();
        CComponent** i = (last == Finish) ? first : (CComponent**)((char*)std::memmove(first, last, (char*)Finish - (char*)last) + ((char*)Finish - (char*)last));
        Finish = i;
    }
};
struct CQuestsView {
    CComponentVector Components;
    void Destroy()
    {
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
    }
};

int main()
{
    CQuestsView v;
    v.Components.Items[0] = new CComponent; v.Components.Items[1] = 0; v.Components.Items[2] = new CComponent;
    v.Components.Finish = v.Components.Items + 3;
    v.Destroy();
    if (g_deleted != 2 || v.Components.size() != 0) { std::printf("0061a500_TEST FAIL\n"); return 1; }
    v.Destroy();                                                            // empty: nothing deleted
    if (g_deleted != 2) { std::printf("0061a500_TEST FAIL\n"); return 1; }
    std::printf("0061a500_TEST PASS\n");
    return 0;
}