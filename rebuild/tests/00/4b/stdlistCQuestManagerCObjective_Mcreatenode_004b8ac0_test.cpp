// Behaviour test for 004b8ac0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
#include <new>
static int g_Copies = 0;
class CWideString { public: const wchar_t* P; CWideString(const wchar_t* p) : P(p) { } CWideString(const CWideString& o) : P(o.P) { ++g_Copies; } };
struct CObjective { CWideString Name; unsigned int Index; int CompletionStatus; CObjective() : Name(L"q"), Index(0), CompletionStatus(0) { } };
struct NodeBase { NodeBase* Next; NodeBase* Prev; };
struct Node : NodeBase { CObjective Data; };
static Node* CreateNode(const CObjective& x) { Node* n = (Node*)std::malloc(sizeof(Node)); new (&n->Data) CObjective(x); return n; }
int main() {
    CObjective o; o.Index = 5; o.CompletionStatus = 2;
    Node* n = CreateNode(o);
    if (n->Data.Index != 5 || n->Data.CompletionStatus != 2 || n->Data.Name.P != o.Name.P || g_Copies != 1) { std::printf("004b8ac0_TEST FAIL\n"); return 1; }
    std::free(n);
    { std::printf("004b8ac0_TEST PASS\n"); return 0; }
}