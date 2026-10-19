// CTCHeroAttachableAppearanceModifiers::CTCHeroAttachableAppearanceModifiers(CThing*) -- thing-component and appearance-component bases, change flags cleared, empty primitive list, hidden-material set and graphics list, three empty appearance-modifier lists, then registration with the owner's graphic appearance
// Leaf: ??0CTCHeroAttachableAppearanceModifiers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00706D10, 226 bytes (+ int3 padding), `ret 4`.
// Label correction: the catalog's "CEngineSubPrimitive2DClockSprite::Ctor" is propagated; the vtables stored are 0x01264F0C
//   (primary) / 0x01264EF4 (component base) = CTCHeroAttachableAppearanceModifiers (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, malloc 0x00BFEA0E (the set's 0x14-byte header node, the 3-list block),
//   vector<CModifierEntry>::vector(const vector&) 0x00708830 (each modifier list copied from the empty default),
//   CTCGraphicAppearance::RegisterComponent 0x004BD390 (owner +0x64). Vtable 0x0125A170 = CAGraphicAppearanceComponent.
// Layout (retail; engine/CTCHeroAttachableAppearanceModifiers.h carries the PDB's 16-byte containers, retail's are 12):
//   CTCBase +0, CAGraphicAppearanceComponent +0xc, FrameLastChanged +0x10, Changed/RecreatePrimitives/
//   AppearanceModifiersLoaded bits 0-2 of +0x14, PrimHandles +0x18, HiddenMaterialNames +0x24 (set: header, count +0x28),
//   AppearanceModifiers +0x30 (vector of three 12-byte lists), Graphics +0x3c (0x48 bytes).
// Shape: the landed CTCGraphicAppearanceReplaceableMesh (0x007A66A0) bases; STLport containers as member objects (the
//   _Vector_base zeroes, the tree header allocation; the modifier vector's end pointer is an _STLP_alloc_proxy member, which
//   places the allocation's `push 24h` between its zero stores); AppearanceModifiers is the STLport `vector(n, value)` constructor
//   `explicit vector(n)` (allocate 3, then uninitialized_fill_n of a T() temporary built after the allocation, through the
//   element copy ctor with a count-down in the dead `owner` slot; the empty temporary has no destructor call in retail, so
//   the 12-byte element is modelled without one).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CThing;
namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent;
struct CPrimitive;
}
struct CTCGraphicAppearance {
    void RegisterComponent(NGraphicAppearance::CAGraphicAppearanceComponent* component);   // 0x004BD390
};
struct CThing {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;        // +0x64 (retail)
};
struct CTCBase {                                       // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                               // +0x04
    bool    ToBeRemoved;                               // +0x08
    bool    IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* owner);                            // 0x00686800
};
namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent {                  // vtable 0x0125A170
    virtual void v00();
    CAGraphicAppearanceComponent() { }
};
}

template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct SNameNode {                                     // _Rb_tree_node<CCharString> (0x14 bytes)
    unsigned char _M_color;
    SNameNode*    _M_parent;
    SNameNode*    _M_left;
    SNameNode*    _M_right;
    void*         _M_value;
};
struct CNameTreeBase {
    SNameNode* _M_header;
    CNameTreeBase() : _M_header(0)
    {
        _M_header = (SNameNode*)malloc(sizeof(SNameNode));
    }
};
struct CNameSet : CNameTreeBase {                      // set<CCharString> (retail 12 bytes)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CNameSet() : _M_node_count(0)
    {
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CModifierEntry { unsigned char _body[8]; };
struct CModifierEntryList {                            // vector<CModifierEntry> (12 bytes)
    CModifierEntry* _M_start;
    CModifierEntry* _M_finish;
    CModifierEntry* _M_end_of_storage;
    CModifierEntryList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CModifierEntryList(const CModifierEntryList& other);   // 0x00708830
};
struct SModifierListAllocProxy {                       // STLport _STLP_alloc_proxy: the end-of-storage pointer
    CModifierEntryList* _M_data;
    SModifierListAllocProxy(CModifierEntryList* data) : _M_data(data) { }
};
struct CModifierListVector {                           // vector<vector<CModifierEntry>> (12 bytes)
    CModifierEntryList*     _M_start;
    CModifierEntryList*     _M_finish;
    SModifierListAllocProxy _M_end_of_storage;
    static __forceinline CModifierEntryList* uninitialized_fill_n(CModifierEntryList* first, unsigned long n, const CModifierEntryList& value)
    {
        CModifierEntryList* cur = first;
        for (; n > 0; --n, ++cur)
            new (cur) CModifierEntryList(value);
        return cur;
    }
    __forceinline explicit CModifierListVector(unsigned long n)
        : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = (CModifierEntryList*)malloc(n * sizeof(CModifierEntryList));
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
        _M_finish = uninitialized_fill_n(_M_start, n, CModifierEntryList());
    }
};

struct CModifierGraphic { unsigned char _body[0x10]; };

struct CTCHeroAttachableAppearanceModifiers : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {
    unsigned long                                FrameLastChanged;         // +0x10
    bool                                         Changed : 1;              // +0x14
    bool                                         RecreatePrimitives : 1;
    bool                                         AppearanceModifiersLoaded : 1;
    CRetailVector<NGraphicAppearance::CPrimitive*> PrimHandles;            // +0x18
    CNameSet                                     HiddenMaterialNames;      // +0x24
    CModifierListVector                          AppearanceModifiers;      // +0x30
    CRetailVector<CModifierGraphic>              Graphics;                 // +0x3c
    virtual void v00();
    CTCHeroAttachableAppearanceModifiers(CThing* owner);
};

CTCHeroAttachableAppearanceModifiers::CTCHeroAttachableAppearanceModifiers(CThing* owner)
    : CTCBase(owner), CAGraphicAppearanceComponent(), FrameLastChanged(0), Changed(false), RecreatePrimitives(false),
      AppearanceModifiersLoaded(false), AppearanceModifiers(3)
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
}