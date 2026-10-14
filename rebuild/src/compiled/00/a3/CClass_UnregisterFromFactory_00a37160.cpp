// CClass::UnregisterFromFactory(CClassFactory*) -- factory->OnDeleteClass(this), then the inlined CClassFactory::UnregisterAllocatedClass(this): counts dropped, both list nodes unlinked and freed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37160, length 81 bytes.
// Callees: CClassFactory vtable slot 3 OnDeleteClass(CClass*) (+0xc, @ 0x0051DA30), free @ 0x00BFEA14 (twice;
// both cdecl cleanups are deferred to one `add esp,8`).
// `this` is the CClass (its Type at +0x10 and list positions at +8/+0xc are read), the argument the
// factory: called from the CThing (0x004C8411) and CNavigatorBase (0x00A4718E) destructors with the
// global factory, so the manifest's "CClassFactory::UnregisterAllocatedClass" label names the inlined
// callee, not this method. Retail CClassFactory = {vptr, vector<CClassTypeInfo> (12 bytes) +4,
// MaxClassTypeID +0x10, NoAllocatedClasses +0x14, list<CClass*> head +0x18} (the engine header has
// the Ego_r 16-byte vector / 12-byte list, hence `_Retail`); the type-info stride is 20 bytes and its
// NoAllocated count sits at +4. Retail lists carry no size, so erase is unlink + free.
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

void CClass_Retail::UnregisterFromFactory(CClassFactory_Retail* factory)
{
    factory->OnDeleteClass(this);
    factory->UnregisterAllocatedClass(this);
}