// CClassFactory::ReregisterAllocatedClass(CClass*, unsigned long, bool) -- the inlined UnregisterAllocatedClass(cls) (counts dropped, both list nodes unlinked and freed), then RegisterAllocatedClass(cls, type, local)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37300, length 91 bytes.
// Callees: free @ 0x00BFEA14 (twice, one deferred `add esp,8`), CClassFactory::RegisterAllocatedClass
// (CClass*, unsigned long, bool) @ 0x00A371F0 (auto-named "CClass_AddAllocation"). Called from
// 0x004CA02B as `g_pClassFactory->(cls, cls->Type, flag)`, i.e. the class is moved to a new type /
// locality: the manifest's "UnregisterAllocatedClass" label covers only the inlined first half.
// Retail CClassFactory = {vptr, vector<CClassTypeInfo> (12 bytes) +4, MaxClassTypeID +0x10,
// NoAllocatedClasses +0x14, list<CClass*> head +0x18} (engine header is the Ego_r layout, hence
// `_Retail`); the 20-byte type-info element keeps its NoAllocated count at +4, and retail lists
// carry no size, so erase is unlink + free. The class's current Type (+0x10) indexes the type table.
#include <stdlib.h>

struct CClassFactory_Retail;

template<class T> struct CRetailList {                      // retail std::list: the sentinel pointer only
    struct node { node* next; node* prev; T value; };
    node* head;
    void erase(node* n)                                     // unlink and free; no size field in retail
    {
        node* next = n->next;
        node* prev = n->prev;
        prev->next = next;
        next->prev = prev;
        free(n);
    }
};

struct CClass_Retail {                                      // engine/CClass.h layout (0x11 bytes)
    virtual void OnCreated();                               // slot 0
    void*         PData;                                    // +0x04 CBaseIntelligentPointer
    CRetailList<CClass_Retail*>::node* ClassListPos;        // +0x08
    CRetailList<CClass_Retail*>::node* TypeListPos;         // +0x0c
    unsigned char Type;                                     // +0x10
    void UnregisterFromFactory(CClassFactory_Retail* factory);   // 0x00A37160
};

struct CClassTypeInfo_Retail {                              // 20-byte element (retail list is 4 bytes)
    CRetailList<CClass_Retail*> TypeList;                   // +0x00
    unsigned long NoAllocated;                              // +0x04
    bool          Used;                                     // +0x08
    bool          Local;                                    // +0x09
    void*         AllocFunc;                                // +0x0c
    void*         Name;                                     // +0x10 CCharString
};

struct CClassFactory_Retail {                               // retail 0x1c bytes (engine/CClassFactory.h is 4+8 larger)
    virtual void v00();
    virtual void AddNewClassType();                         // slot 1
    virtual void OnCreateClass(CClass_Retail* cls);         // slot 2 @ 0x0051DA20
    virtual void OnDeleteClass(CClass_Retail* cls);         // slot 3 @ 0x0051DA30
    CClassTypeInfo_Retail* ClassTypeInfo_first;             // +0x04 vector<CClassTypeInfo> {first, last, end}
    CClassTypeInfo_Retail* ClassTypeInfo_last;              // +0x08
    CClassTypeInfo_Retail* ClassTypeInfo_end;               // +0x0c
    unsigned long MaxClassTypeID;                           // +0x10
    unsigned long NoAllocatedClasses;                       // +0x14
    CRetailList<CClass_Retail*> ClassList;                  // +0x18
    void RegisterAllocatedClass(CClass_Retail* cls, unsigned long type, bool local);   // 0x00A371F0
    void ReregisterAllocatedClass(CClass_Retail* cls, unsigned long type, bool local); // 0x00A37300
    // Inlined into both 0x00A37160 and 0x00A37300: drop the factory-wide and per-type counts, then
    // unlink the class from its type list and from the global class list.
    void UnregisterAllocatedClass(CClass_Retail* cls)
    {
        CClassTypeInfo_Retail* info = &ClassTypeInfo_first[cls->Type];
        --NoAllocatedClasses;
        --info->NoAllocated;
        info->TypeList.erase(cls->TypeListPos);
        ClassList.erase(cls->ClassListPos);
    }
};

void CClassFactory_Retail::ReregisterAllocatedClass(CClass_Retail* cls, unsigned long type, bool local)
{
    UnregisterAllocatedClass(cls);
    RegisterAllocatedClass(cls, type, local);
}