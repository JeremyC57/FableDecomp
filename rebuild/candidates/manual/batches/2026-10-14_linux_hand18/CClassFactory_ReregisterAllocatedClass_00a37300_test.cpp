#include <cstdio>
#include <stddef.h>
#include <stdlib.h>
struct CClassFactory_Retail;
static int g_frees = 0; static int g_ondelete = 0; static int g_register = 0; static unsigned long g_regtype = 0; static int g_reglocal = -1;
template<class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    void erase(node* n) { node* next = n->next; node* prev = n->prev; prev->next = next; next->prev = prev; free(n); ++g_frees; }
    CRetailList() { head = (node*)malloc(sizeof(node)); head->next = head; head->prev = head; }
    node* push_back(T v) { node* n = (node*)malloc(sizeof(node)); n->value = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; return n; }
    int count() const { int c = 0; for (node* n = head->next; n != head; n = n->next) ++c; return c; }
};
struct CClass_Retail {
    virtual void OnCreated() { }
    void* PData; CRetailList<CClass_Retail*>::node* ClassListPos; CRetailList<CClass_Retail*>::node* TypeListPos; unsigned char Type;
    void UnregisterFromFactory(CClassFactory_Retail* factory);
};
struct CClassTypeInfo_Retail { CRetailList<CClass_Retail*> TypeList; unsigned long NoAllocated; bool Used; bool Local; void* AllocFunc; void* Name; };
struct CClassFactory_Retail {
    virtual void v00() { } virtual void AddNewClassType() { } virtual void OnCreateClass(CClass_Retail*) { }
    virtual void OnDeleteClass(CClass_Retail* cls) { ++g_ondelete; }
    CClassTypeInfo_Retail* ClassTypeInfo_first; CClassTypeInfo_Retail* ClassTypeInfo_last; CClassTypeInfo_Retail* ClassTypeInfo_end;
    unsigned long MaxClassTypeID; unsigned long NoAllocatedClasses; CRetailList<CClass_Retail*> ClassList;
    void RegisterAllocatedClass(CClass_Retail* cls, unsigned long type, bool local) { ++g_register; g_regtype = type; g_reglocal = local ? 1 : 0; }
    void ReregisterAllocatedClass(CClass_Retail* cls, unsigned long type, bool local) { UnregisterAllocatedClass(cls); RegisterAllocatedClass(cls, type, local); }
    void UnregisterAllocatedClass(CClass_Retail* cls)
    {
        CClassTypeInfo_Retail* info = &ClassTypeInfo_first[cls->Type];
        --NoAllocatedClasses; --info->NoAllocated; info->TypeList.erase(cls->TypeListPos); ClassList.erase(cls->ClassListPos);
    }
};
void CClass_Retail::UnregisterFromFactory(CClassFactory_Retail* factory) { factory->OnDeleteClass(this); factory->UnregisterAllocatedClass(this); }
static CClassTypeInfo_Retail g_types[3];
static CClassFactory_Retail g_factory;
static CClass_Retail g_a, g_b;
static void Setup()
{
    g_factory.ClassTypeInfo_first = g_types; g_factory.ClassTypeInfo_last = g_types + 3; g_factory.ClassTypeInfo_end = g_types + 3;
    g_factory.NoAllocatedClasses = 2; g_types[2].NoAllocated = 2; g_types[2].Used = true;
    g_a.Type = 2; g_a.ClassListPos = g_factory.ClassList.push_back(&g_a); g_a.TypeListPos = g_types[2].TypeList.push_back(&g_a);
    g_b.Type = 2; g_b.ClassListPos = g_factory.ClassList.push_back(&g_b); g_b.TypeListPos = g_types[2].TypeList.push_back(&g_b);
}
static bool AfterUnregisterA()
{
    return g_frees == 2 && g_factory.NoAllocatedClasses == 1 && g_types[2].NoAllocated == 1 && g_factory.ClassList.count() == 1 && g_types[2].TypeList.count() == 1
        && g_factory.ClassList.head->next->value == &g_b && g_types[2].TypeList.head->next->value == &g_b && sizeof(CClassTypeInfo_Retail) == 20
        && offsetof(CClassFactory_Retail, NoAllocatedClasses) == 0x14 && offsetof(CClassFactory_Retail, ClassList) == 0x18 && offsetof(CClass_Retail, Type) == 0x10;
}
int main()
{
    Setup();
    g_factory.ReregisterAllocatedClass(&g_a, 1, true);
    if (!AfterUnregisterA() || g_ondelete != 0 || g_register != 1 || g_regtype != 1 || g_reglocal != 1) { std::printf("00a37300_TEST FAIL\n"); return 1; }
    std::printf("00a37300_TEST PASS\n"); return 0;
}
