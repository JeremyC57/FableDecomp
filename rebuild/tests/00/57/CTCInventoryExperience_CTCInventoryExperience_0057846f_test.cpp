// Behaviour test for 0x0057846F CTCInventoryExperience::~CTCInventoryExperience() (user body: delete + erase every component, drop three pointers)
#include <cstdio>

static int g_deleted = 0, g_erases = 0;
struct CComponent { virtual ~CComponent() { ++g_deleted; } };
struct CComponentVector {
    CComponent* Items[8]; int Count;
    CComponent** begin() { return Items; }
    CComponent** end() { return Items + Count; }
    CComponent** erase(CComponent** pos)
    {
        ++g_erases;
        for (CComponent** p = pos; p + 1 < end(); ++p) *p = p[1];
        --Count;
        return pos;
    }
};
struct CExperienceView {
    CComponentVector Components;
    void* pYesNo; void* pViewport; void* pTextViewport;
    void Destroy()
    {
        for (CComponent** it = Components.begin(); it != Components.end(); ) {
            delete *it;
            it = Components.erase(it);
        }
        pYesNo = 0;
        pViewport = 0;
        pTextViewport = 0;
    }
};

int main()
{
    CExperienceView v; int dummy;
    v.Components.Count = 4;
    v.Components.Items[0] = new CComponent; v.Components.Items[1] = 0;     // a null slot is erased without a delete
    v.Components.Items[2] = new CComponent; v.Components.Items[3] = new CComponent;
    v.pYesNo = &dummy; v.pViewport = &dummy; v.pTextViewport = &dummy;
    v.Destroy();
    if (g_deleted != 3 || g_erases != 4 || v.Components.Count != 0) { std::printf("0057846f_TEST FAIL\n"); return 1; }
    if (v.pYesNo || v.pViewport || v.pTextViewport) { std::printf("0057846f_TEST FAIL\n"); return 1; }
    std::printf("0057846f_TEST PASS\n");
    return 0;
}