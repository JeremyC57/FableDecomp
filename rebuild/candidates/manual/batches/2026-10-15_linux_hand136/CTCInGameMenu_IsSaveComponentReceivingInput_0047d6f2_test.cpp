// Behaviour test for 0x0047D6F2 (see src/0047d6f2.cpp): the "COMPONENT_SAVE" input query over mock components and child lists.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0047d6f2_TEST FAIL\n"); return 1; } } while (0)

struct CComponent;
struct SNode { SNode* next; SNode* prev; CComponent* data; };
struct CComponentList {
    SNode* node;
    CComponentList() { node = new SNode; node->next = node; node->prev = node; node->data = 0; }
    ~CComponentList() { SNode* n = node->next; while (n != node) { SNode* d = n; n = n->next; delete d; } delete node; }
    void push_back(CComponent* c) { SNode* n = new SNode; n->data = c; n->next = node; n->prev = node->prev; node->prev->next = n; node->prev = n; }
};
struct CComponent {
    int state; const char* name; CComponent** kids; int kidCount;
    void FindChildrenByName(const char* wanted, CComponentList& found)
    {
        for (int i = 0; i < kidCount; ++i) if (kids[i] == 0 || std::strcmp(kids[i]->name, wanted) == 0) found.push_back(kids[i]);
    }
    int GetState() { return state; }
};
struct CTCInGameMenu {
    CComponent** first; CComponent** last;
    unsigned int size() const { return (unsigned int)(last - first); }
    bool IsSaveComponentReceivingInput()
    {
        for (unsigned int i = 0; i < size(); ++i) {
            CComponentList found;
            first[i]->FindChildrenByName("COMPONENT_SAVE", found);
            for (SNode* n = found.node->next; n != found.node; n = n->next) {
                CComponent* child = n->data;
                if (child && child->GetState() == 3) return true;
            }
        }
        return false;
    }
};
int main()
{
    CComponent saveIdle = { 1, "COMPONENT_SAVE", 0, 0 }, other = { 3, "OTHER", 0, 0 }, saveActive = { 3, "COMPONENT_SAVE", 0, 0 };
    CComponent* kidsA[3] = { &saveIdle, &other, 0 };
    CComponent* kidsB[1] = { &saveActive };
    CComponent a = { 0, "A", kidsA, 3 }, b = { 0, "B", kidsB, 1 };
    CComponent* comps[2] = { &a, &b };
    CTCInGameMenu menu; menu.first = comps; menu.last = comps + 1;
    CHECK(!menu.IsSaveComponentReceivingInput());
    menu.last = comps + 2;
    CHECK(menu.IsSaveComponentReceivingInput());
    menu.last = comps;
    CHECK(!menu.IsSaveComponentReceivingInput());
    std::printf("0047d6f2_TEST PASS\n");
    return 0;
}
