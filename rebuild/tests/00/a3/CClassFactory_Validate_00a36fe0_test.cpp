// Behaviour test for 0x00A36FE0 CClassFactory::Validate() (NF446): links the row object and calls it by its own symbol. No class body is
// defined here: the classes come from the row's shared headers (its prelude, verbatim). The factory is a raw buffer whose ClassList (+0x18,
// STLport list<CClass*>: one sentinel node pointer; node = next +0, prev +4, value +8) holds three objects; each object's vftable slot 1
// (CBaseObject::Validate) records the object it ran on.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_CLASSLIST
#define FABLE_RETAIL_CCLASSFACTORY_VALIDATE
#include "engine/CClassFactory.h"
#include "engine/CClass.h"
#include <stdio.h>
#include <string.h>
static const void* g_seen[8]; static int g_n = 0;
extern "C" void __fastcall nf446_validate(void* self) { if (g_n < 8) g_seen[g_n] = self; ++g_n; }
static void* g_vt[4];
struct Node { Node* next; Node* prev; void* value; };
static void* g_obj[3][2];
static Node g_nodes[4];
static unsigned char g_factory[0x40];
static int run(int count)
{
    memset(g_factory, 0, sizeof g_factory); g_n = 0;
    Node* head = &g_nodes[0]; head->value = 0;
    Node* last = head;
    for (int i = 0; i < count; ++i) {
        Node* n = &g_nodes[i + 1]; g_obj[i][0] = g_vt; n->value = g_obj[i];
        last->next = n; n->prev = last; last = n;
    }
    last->next = head; head->prev = last;
    *(Node**)(g_factory + 0x18) = head;
    ((CClassFactory*)g_factory)->CClassFactory::Validate();
    if (g_n != count) return 0;
    for (int i = 0; i < count; ++i) if (g_seen[i] != (const void*)g_obj[i]) return 0;
    return 1;
}
int main(void)
{
    g_vt[1] = (void*)&nf446_validate;
    if (!run(0) || !run(1) || !run(3)) { printf("00a36fe0_TEST FAIL\n"); return 1; }
    printf("00a36fe0_TEST PASS\n"); return 0;
}
