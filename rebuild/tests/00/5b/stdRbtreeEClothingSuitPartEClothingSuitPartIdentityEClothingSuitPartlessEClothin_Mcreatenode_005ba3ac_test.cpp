// Behaviour test for 005ba3ac: std::_Rb_tree<EClothingSuitPart,..>::_M_create_node -- a fresh 0x14-byte node holding a copy of the value
#include <cstdio>
#include <cstdlib>
#include <new>
enum EClothingSuitPart { PART_0, PART_1, PART_2, PART_3 };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { EClothingSuitPart value; };
static Node* CreateNode(const EClothingSuitPart& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    new (&tmp->value) EClothingSuitPart(x);
    return tmp;
}
int main()
{
    EClothingSuitPart k = PART_2;
    Node* a = CreateNode(k); k = PART_3; Node* b = CreateNode(k);
    int ok = sizeof(Node) == 0x14 && a != b && a->value == PART_2 && b->value == PART_3 &&
             (char*)&a->value - (char*)a == 0x10;
    std::free(a); std::free(b);
    std::printf(ok ? "005ba3ac_TEST PASS\n" : "005ba3ac_TEST FAIL\n");
    return ok ? 0 : 1;
}
