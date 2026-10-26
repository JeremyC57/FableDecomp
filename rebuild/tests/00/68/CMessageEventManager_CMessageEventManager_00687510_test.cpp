// Behaviour test for 0x00687510 CMessageEventManager(const CWorld&): world stored, empty self-linked message list.
#include <cstdio>
#include <cstdlib>
class CWorld { public: int dummy; };
class CMessageEvent { public: unsigned char Data[0x58]; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CMessageEvent data; };
struct Manager {
    CWorld* World; NodeBase* Messages;
    Manager(const CWorld& w) : World((CWorld*)&w), Messages(0)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->next = n; n->prev = n;
        Messages = n;
    }
};
int main()
{
    CWorld w; w.dummy = 1;
    Manager m(w);
    if (m.World != &w || m.Messages == 0 || m.Messages->next != m.Messages || m.Messages->prev != m.Messages || sizeof(CMessageEvent) != 0x58) {
        std::printf("00687510_TEST FAIL\n"); return 1;
    }
    std::free(m.Messages);
    std::printf("00687510_TEST PASS\n");
    return 0;
}