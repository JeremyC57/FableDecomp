// Behaviour test for CTCCreatureModeManager::GetCurrentMode (0x0073A6E0): the object of the first counted pointer in the mode list.
#include <cstdio>
class CModeBase { public: int Id; };
struct CCountedPointer { CModeBase* Data; void* Info; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CCountedPointer data; };
class CTCCreatureModeManager { public:
    Node* PModes;
    CModeBase& GetCurrentMode() const { return *((Node*)PModes->next)->data.Data; }
};
int main()
{
    CModeBase m1, m2; m1.Id = 1; m2.Id = 2;
    Node head, a, b;
    head.next = &a; a.next = &b; b.next = &head;
    a.data.Data = &m1; b.data.Data = &m2;
    CTCCreatureModeManager mm; mm.PModes = &head;
    if (&mm.GetCurrentMode() != &m1) { std::printf("0073a6e0_TEST FAIL\n"); return 1; }
    head.next = &b;
    if (mm.GetCurrentMode().Id != 2) { std::printf("0073a6e0_TEST FAIL\n"); return 1; }
    std::printf("0073a6e0_TEST PASS\n");
    return 0;
}