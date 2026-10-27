// Test for 0x009F8860 list<CGraphicBankManager::CGraphicDataBankInfo>::_M_create_node: allocates one 0x40-byte node and
// copy-constructs the value into it (links left for the caller).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
static unsigned int g_lastAlloc = 0;
struct CGraphicDataBankInfo { int Id; char Name[48]; int Flags; };
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CGraphicDataBankInfo data; };
static Node* create_node(const CGraphicDataBankInfo& x)
{
    g_lastAlloc = sizeof(Node);
    Node* p = (Node*)malloc(sizeof(Node));
    new ((void*)&p->data) CGraphicDataBankInfo(x);
    return p;
}
int main()
{
    CGraphicDataBankInfo v; v.Id = 17; strcpy(v.Name, "textures.big"); v.Flags = 3;
    Node* n = create_node(v);
    bool ok = n != 0 && n->data.Id == 17 && strcmp(n->data.Name, "textures.big") == 0 && n->data.Flags == 3 &&
              g_lastAlloc == sizeof(Node);
    free(n);
    puts(ok ? "009f8860_TEST PASS" : "009f8860_TEST FAIL");
    return ok ? 0 : 1;
}