// CClassFactory::CClassFactory(unsigned char maxClassTypeID) -- construct a class factory: empty class-type table and class list, no allocated classes, then size the type table to the maximum class type id
// Leaf: ??0CClassFactory@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A373B0, 80 bytes, `ret 4`.
// Name: FableWin.pdb ??0CClassFactory@@IAE@E@Z (protected). The catalog label NHeroInformationScreens::CBase::CBase is a
// propagated base-ctor label: the body stores vtable 0x0129CF70 = RTTI CClassFactory (rtti-vtables.tsv), and the PDB
// argument is an `unsigned char` (the `movzx`) -- see NOTES label corrections.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, malloc @ 0x00BFEA0E (the 12-byte list header node),
// vector<CClassTypeInfo>::resize(unsigned long) @ 0x00A38150 (builds the default CClassTypeInfo and resizes; catalog label
// CTCInventoryMap_InitMiniMapMarkers is a fold).
// Layout: engine/CClassFactory.h carries the PDB's 16-byte vector and 12-byte list, so the class is modelled locally with
// its PDB member names at the retail offsets: ClassTypeInfo +0x04 (12-byte STLport vector), MaxClassTypeID +0x10,
// NoAllocatedClasses +0x14, ClassList +0x18 (4-byte STLport list: header-node pointer).
// Shape: the vector is the STLport `_Vector_base` model (ebx zero words); the list is `_List_base`: the proxy nulls the
// node word, the header is malloc'd and linked to itself; NoAllocatedClasses is cleared in the body, before the resize.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBaseClass { virtual ~CBaseClass(); };
struct CBaseClassNonCopyable : CBaseClass {
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

template <class T> struct CAllocator {                      // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {     // STLport _STLP_alloc_proxy
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                     // STLport _Vector_base
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {        // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
    void resize(unsigned long size);
};

struct SListNode {                                          // _List_node<CClass*>, 12 bytes
    SListNode* _M_next;
    SListNode* _M_prev;
    void*      _M_data;
};
struct CListBase {                                          // STLport _List_base (4 bytes)
    CAllocProxy<SListNode> _M_node;
    CListBase(const CAllocator<SListNode>& alloc) : _M_node(alloc, 0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node.m_data = node;
    }
};
struct CList : CListBase {                                  // list<CClass*>
    CList() : CListBase(CAllocator<SListNode>()) { }
};

struct CClassTypeInfo { unsigned char _body[0x20]; };

class CClassFactory : public CBaseClassNonCopyable {
protected:
    CClassFactory(unsigned char maxClassTypeID);
    virtual ~CClassFactory();
    CVector<CClassTypeInfo> ClassTypeInfo;                  // +0x04
    unsigned long           MaxClassTypeID;                 // +0x10
    unsigned long           NoAllocatedClasses;             // +0x14
    CList                   ClassList;                      // +0x18
};

CClassFactory::CClassFactory(unsigned char maxClassTypeID)
    : MaxClassTypeID(maxClassTypeID)
{
    NoAllocatedClasses = 0;
    ClassTypeInfo.resize(MaxClassTypeID);
}