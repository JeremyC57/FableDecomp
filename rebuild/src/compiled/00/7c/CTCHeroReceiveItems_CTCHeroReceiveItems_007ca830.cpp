// CTCHeroReceiveItems::CTCHeroReceiveItems(CThing* thing) -- CTCBase(thing), an empty item list and picked-up vector, cleared pointers and flags, a full-size timer of 1, null primitive handles; then joins the global update list
// Leaf: ??0CTCHeroReceiveItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CA830, 167 bytes (`ret 4`); vtable 0x0126EBFC CTCHeroReceiveItems.
// Label: the catalog says CTCShotDefinitionBase::CTCShotDefinitionBase (propagated); the vtable store is CTCHeroReceiveItems'
// (rtti-vtables.tsv) and the members are its Ego_r.pdb members.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (the list's 0x14-byte sentinel node),
// CBaseIntelligentPointer ctor @ 0x00A01B10 (vtables 0x0125A858 CIntelligentPointer<CThingObject>, 0x01238C6C
// CIntelligentPointer<CThing>), CBaseClass ctor @ 0x0099A2D0 (CEnginePrimitiveHandle vtable 0x0122F598),
// CTCBase::AddToGlobalTCs @ 0x00686ED0.
// Layout (retail; the PDB's 12-byte list is the 4-byte STLport node pointer and the vector 12 bytes): Items +0x0c,
// PickedUpItems +0x10, PCurrentItem +0x1c, PersistCurrentItem..SendMessage +0x24..+0x28, PContainer +0x29 (odd offset in the
// PDB too: the intelligent-pointer class is 1-aligned), AmountThroughForAreaResize +0x34, FullSizeTimer +0x38, three C2DBoxF
// +0x3c, two CRightHandedSet +0x6c, ItemPrimitiveHandle +0x9c, BackgroundPrimitiveHandle +0xa4 (0xac bytes).
// Shape: the STLport list base zeroes its node then links a malloc'd sentinel to itself; the vector is the real
// _Vector_base + _STLP_alloc_proxy (its end-of-storage zero after the malloc's `add esp,4`); everything else in declaration
// order through the initialiser list; the global-update registration is the constructor body.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);          // 0x00BFEA0E
class CThing;
class CThingObject;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtables 0x0125A858 / 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
#pragma pack(pop)
template <class T> struct CAllocator {                  // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct SItemNode {                                      // _List_node<pair<CIntelligentPointer<CThingObject>, bool> >
    SItemNode* _M_next;
    SItemNode* _M_prev;
    unsigned char _M_data[0xc];
};
struct CItemList {                                      // STLport list (retail: the node pointer only)
    CAllocProxy<SItemNode> _M_node;
    CItemList() : _M_node(CAllocator<SItemNode>(), 0)
    {
        SItemNode* node = (SItemNode*)malloc(sizeof(SItemNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node.m_data = node;
    }
};
class C2DBoxF { public: float X0, Y0, X1, Y1; C2DBoxF() { } };
class CRightHandedSet { public: float M[6]; CRightHandedSet() { } };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CEnginePersistentPrimitiveBase;
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598
public:
    CIVCountedPointer Data;                             // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
};
class CTCHeroReceiveItems : public CTCBase { public:
    CItemList Items;                                    // +0x0c
    CRetailVector<long> PickedUpItems;                  // +0x10
    CIntelligentPointer<CThingObject> PCurrentItem;     // +0x1c
    bool PersistCurrentItem;                            // +0x24
    bool ReceivingItems;                                // +0x25
    bool DisplayingItems;                               // +0x26
    bool ForceClear;                                    // +0x27
    bool SendMessage;                                   // +0x28 (PDB "SendMessageW": the Win32 macro)
    CIntelligentPointer<CThing> PContainer;             // +0x29
    float AmountThroughForAreaResize;                   // +0x34
    long FullSizeTimer;                                 // +0x38
    C2DBoxF InitialAreaItemTook;                        // +0x3c
    C2DBoxF OldAreaItemTaking;                          // +0x4c
    C2DBoxF CurrentAreaItemTaking;                      // +0x5c
    CRightHandedSet OldItemRHSet;                       // +0x6c
    CRightHandedSet CurrentItemRHSet;                   // +0x84
    CEnginePrimitiveHandle ItemPrimitiveHandle;         // +0x9c
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;   // +0xa4
    virtual void v00();
    CTCHeroReceiveItems(CThing* thing);
};

CTCHeroReceiveItems::CTCHeroReceiveItems(CThing* thing)
    : CTCBase(thing), PersistCurrentItem(false), ReceivingItems(false), DisplayingItems(false), ForceClear(false),
      SendMessage(false), AmountThroughForAreaResize(0), FullSizeTimer(1)
{
    AddToGlobalTCs();
}