#include <cstdio>
#include <stddef.h>
#include <stdlib.h>
#include <new>
static int g_names = 0;
struct CClass_Calls;
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_names; } };
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() { head = 0; node* s = (node*)malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
};
typedef CClass_Calls* (__cdecl* ClassAllocFunc)();
struct CClassTypeInfo {
    CRetailList<CClass_Calls*> TypeList; unsigned long NoAllocated; bool Used; bool Local; ClassAllocFunc AllocFunc; CCharString Name;
    CClassTypeInfo() : TypeList(), Name() { Used = false; NoAllocated = 0; Local = false; AllocFunc = 0; }
};
int main()
{
    unsigned char raw[sizeof(CClassTypeInfo)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CClassTypeInfo* t = new (raw) CClassTypeInfo;
    if (g_names != 1 || !t->TypeList.head || t->TypeList.head->next != t->TypeList.head || t->TypeList.head->prev != t->TypeList.head
        || t->NoAllocated != 0 || t->Used || t->Local || t->AllocFunc != 0 || sizeof(*t) != 20 || offsetof(CClassTypeInfo, Name) != 0x10
        || offsetof(CClassTypeInfo, AllocFunc) != 0xc || offsetof(CClassTypeInfo, Used) != 8) { std::printf("00a37880_TEST FAIL\n"); return 1; }
    free(t->TypeList.head);
    std::printf("00a37880_TEST PASS\n"); return 0;
}