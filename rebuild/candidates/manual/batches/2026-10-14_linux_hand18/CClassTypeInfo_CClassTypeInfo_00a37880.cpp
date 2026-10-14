// CClassTypeInfo::CClassTypeInfo() -- the TypeList sentinel node allocated and self-linked, the Name string constructed, then NoAllocated/Used/Local/AllocFunc zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37880, length 50 bytes.
// Callees: malloc @ 0x00BFEA0E (12-byte list node), CCharString::CCharString @ 0x0099E4B0.
// Retail layout (20 bytes; Ego_r's is 28 because its std::list is 12 bytes): TypeList (retail
// std::list = the sentinel pointer only) +0, NoAllocated +4, Used +8, Local +9, AllocFunc +0xc,
// Name (CCharString) +0x10. This is the element type of CClassFactory::ClassTypeInfo (20-byte stride
// in 0x00A37160/0x00A37300). The list constructor nulls the head, builds the sentinel in a local
// (next = prev = itself) and stores it; the four scalars are assigned in the constructor body, which
// is why their zero stores follow the Name constructor call (in the body's order: Used, NoAllocated,
// Local, AllocFunc -- the stores keep the source order).
#include <stdlib.h>

struct CClass_Calls;

struct CCharString {
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
};

template<class T> struct CRetailList {                      // retail std::list: the sentinel pointer only
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList()
    {
        head = 0;
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

typedef CClass_Calls* (__cdecl* ClassAllocFunc)();

struct CClassTypeInfo {
    CRetailList<CClass_Calls*> TypeList;                    // +0x00
    unsigned long  NoAllocated;                             // +0x04
    bool           Used;                                    // +0x08
    bool           Local;                                   // +0x09
    ClassAllocFunc AllocFunc;                               // +0x0c
    CCharString    Name;                                    // +0x10
    CClassTypeInfo();
};

CClassTypeInfo::CClassTypeInfo()
    : TypeList()
    , Name()
{
    Used = false;
    NoAllocated = 0;
    Local = false;
    AllocFunc = 0;
}
