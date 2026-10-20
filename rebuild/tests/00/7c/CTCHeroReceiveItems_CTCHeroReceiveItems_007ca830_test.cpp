// Behaviour test for CTCHeroReceiveItems(CThing*) (0x007CA830): owner set, a self-linked list sentinel, an empty vector, two
// constructed intelligent pointers and two null primitive handles, cleared flags, FullSizeTimer 1, registered for updates.
#include <cstdio>
#include <cstdlib>
class CThing;
class CThingObject;
static int g_registered = 0, g_pointers = 0, g_baseClasses = 0;
struct CTCBase {
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    void AddToGlobalUpdateTCs() { IsInGlobalUpdateTCs = true; ++g_registered; }
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer() : m_pItem(0) { ++g_pointers; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtables 0x0125A858 / 0x01238C6C
    virtual void v00() { }
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
struct C2DBoxF { float X0, Y0, X1, Y1; C2DBoxF() { } };
struct CRightHandedSet { float M[6]; CRightHandedSet() { } };
class CBaseClass {                                      // vtable 0x01231710
public:
    virtual void v000() { }
    CBaseClass() { ++g_baseClasses; }
};
struct CEnginePersistentPrimitiveBase;
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
};
class CEnginePrimitiveHandle : public CBaseClass {      // vtable 0x0122F598
public:
    CIVCountedPointer Data;                             // +0x04
    virtual void v001() { }
    CEnginePrimitiveHandle() { }
};
struct CTCHeroReceiveItems : CTCBase {
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
    virtual void v00() { }
    CTCHeroReceiveItems(CThing* thing);
};

CTCHeroReceiveItems::CTCHeroReceiveItems(CThing* thing)
    : CTCBase(thing), PersistCurrentItem(false), ReceivingItems(false), DisplayingItems(false), ForceClear(false),
      SendMessage(false), AmountThroughForAreaResize(0), FullSizeTimer(1)
{
    AddToGlobalUpdateTCs();
}

int main()
{
    int bad = 0;
    CThing* owner = (CThing*)0x4321;
    CTCHeroReceiveItems* r = new CTCHeroReceiveItems(owner);
    if (r->ParentThing != owner || g_registered != 1 || !r->IsInGlobalUpdateTCs) bad |= 1;
    SItemNode* n = r->Items._M_node.m_data;
    if (!n || n->_M_next != n || n->_M_prev != n) bad |= 2;
    if (r->PickedUpItems._M_start || r->PickedUpItems._M_finish || r->PickedUpItems._M_end_of_storage.m_data) bad |= 4;
    if (g_pointers != 2 || g_baseClasses != 2 || r->ItemPrimitiveHandle.Data.Object || r->BackgroundPrimitiveHandle.Data.Object) bad |= 8;
    if (r->PersistCurrentItem || r->ReceivingItems || r->DisplayingItems || r->ForceClear || r->SendMessage) bad |= 16;
    if (r->AmountThroughForAreaResize != 0.0f || r->FullSizeTimer != 1) bad |= 32;
    if (bad) { std::printf("007ca830_TEST FAIL\n"); return 1; }
    std::printf("007ca830_TEST PASS\n");
    return 0;
}