// behaviour test for 0x0051ea30 CThingManager::FillRenderListsWithAllEntities: for thing types 1..15 whose ClassTypeInfo entry is
// Used, call SetAsRenderedThisFrame(0xF) on every thing of the type's list (STLport list<CClass*>, circular with a header node);
// type 0, type 16 and unused types are skipped.
#include <cstdio>
#include <cstring>
struct Node { Node* next; Node* prev; void* data; };
struct CThing { int id; int calls; unsigned long lastFlags; void SetAsRenderedThisFrame(unsigned long f) { ++calls; lastFlags = f; } };
struct CClassTypeInfo { Node* TypeList; unsigned long NoAllocated; bool Used; bool Local; void* AllocFunc; void* Name; };
struct CThingManager {
    void* vptr; CClassTypeInfo* start; CClassTypeInfo* finish; CClassTypeInfo* eos;
    bool IsClassTypeRegistered(unsigned long t) const { return start[t].Used; }
    Node*& GetTypeList(unsigned char t) { return start[t].TypeList; }
    void FillRenderListsWithAllEntities();
};
// The row (the matched source's statements on the model).
void CThingManager::FillRenderListsWithAllEntities()
{
    for (long c1 = 1; c1 < 16; ++c1)
        if (IsClassTypeRegistered(c1)) {
            Node* list = GetTypeList((unsigned char)c1);
            for (Node* it = list->next; it != list; it = it->next) {
                CThing* pthing = (CThing*)it->data;
                pthing->SetAsRenderedThisFrame(0xF);
            }
        }
}
static Node heads[17]; static Node nodes[17][3]; static CThing things[17][3]; static CClassTypeInfo info[17];
static void Build(int t, int count)
{
    Node* h = &heads[t]; h->next = h->prev = h;
    for (int i = 0; i < count; ++i) {
        Node* n = &nodes[t][i]; n->data = &things[t][i];
        n->prev = h->prev; n->next = h; h->prev->next = n; h->prev = n;
    }
    info[t].TypeList = h;
}
int main()
{
    int fails = 0;
    memset(things, 0, sizeof(things)); memset(info, 0, sizeof(info));
    for (int t = 0; t < 17; ++t) { Build(t, (t % 3) + 1); info[t].Used = true; }
    info[0].Used = true;          // type 0 is never walked
    info[16].Used = true;         // past the end: never walked
    info[5].Used = false;         // unused type in range: skipped
    info[9].Used = false; Build(9, 3);
    Build(7, 0);                  // used but empty: nothing to do
    CThingManager m; m.vptr = 0; m.start = info; m.finish = m.eos = info + 17;
    m.FillRenderListsWithAllEntities();
    for (int t = 0; t < 17; ++t) {
        int count = (t == 7) ? 0 : (t % 3) + 1;
        bool walked = t >= 1 && t <= 15 && t != 5 && t != 9;
        for (int i = 0; i < 3; ++i) {
            int expect = (walked && i < count) ? 1 : 0;
            if (things[t][i].calls != expect) fails |= 1;
            if (expect && things[t][i].lastFlags != 0xF) fails |= 2;
        }
    }
    if (things[1][0].calls != 1 || things[15][0].calls != 1 || things[14][2].calls != 1) fails |= 4;
    if (things[0][0].calls != 0 || things[16][0].calls != 0 || things[5][0].calls != 0) fails |= 8;
    if (fails) { printf("0051ea30_TEST FAIL %d\n", fails); return 1; }
    printf("0051ea30_TEST PASS\n");
    return 0;
}