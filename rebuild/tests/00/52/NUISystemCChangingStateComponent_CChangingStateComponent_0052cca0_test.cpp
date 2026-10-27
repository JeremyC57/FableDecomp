// Behaviour test for 0052cca0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
static int g_BaseCopies = 0;
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct List { NodeBase* Node; List() { Node = new NodeBase; Node->Next = Node; Node->Prev = Node; } ~List() { delete Node; } };
class CComponent { public: int Id; CComponent() : Id(0) { } CComponent(const CComponent& o) : Id(o.Id) { ++g_BaseCopies; } virtual ~CComponent() { } };
class CChangingStateComponent : public CComponent { public:
    int StatesBeingDone; List StatesToDo;
    CChangingStateComponent() : StatesBeingDone(4) { }
    CChangingStateComponent(const CChangingStateComponent& o) : CComponent(o) { } };
int main() {
    CChangingStateComponent a; a.Id = 31;
    CChangingStateComponent b(a);
    if (b.Id != 31 || g_BaseCopies != 1) { std::printf("0052cca0_TEST FAIL\n"); return 1; }
    if (b.StatesToDo.Node == a.StatesToDo.Node || b.StatesToDo.Node->Next != b.StatesToDo.Node || b.StatesToDo.Node->Prev != b.StatesToDo.Node) { std::printf("0052cca0_TEST FAIL\n"); return 1; }
    { std::printf("0052cca0_TEST PASS\n"); return 0; }
}