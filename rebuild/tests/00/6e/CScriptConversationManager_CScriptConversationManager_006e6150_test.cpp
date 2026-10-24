// Behaviour test for 0x006E6150 (CScriptConversationManager ctor: empty STLport tree header + index 1 + AlwaysShowText off).
#include <cstdio>
#include <cstdlib>

struct Node { bool Color; Node* Parent; Node* Left; Node* Right; unsigned char Value[0x30]; };
struct Manager {
    Node* Header; unsigned int NodeCount; unsigned int Compare; long NextConversationIndex; bool AlwaysShowText;
    Manager() : Header(0)
    {
        Header = (Node*)std::malloc(sizeof(Node));
        NodeCount = 0;
        Header->Color = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header;
        NextConversationIndex = 1; AlwaysShowText = false;
    }
};
#define FAIL { std::printf("006e6150_TEST FAIL\n"); return 1; }
int main()
{
    Manager m;
    if (!m.Header || m.NodeCount != 0 || m.Header->Color || m.Header->Parent != 0) FAIL;
    if (m.Header->Left != m.Header || m.Header->Right != m.Header) FAIL;
    if (m.NextConversationIndex != 1 || m.AlwaysShowText) FAIL;
    std::free(m.Header);
    std::printf("006e6150_TEST PASS\n");
    return 0;
}