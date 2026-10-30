// CTCHeroReceiveItems::~CTCHeroReceiveItems() -- hero receive-items component dtor: leave the global TC update list, then the member dtors in reverse order (the background and item engine primitive handles, the container and current-item pointers, the picked-up-item vector, the item list) and ~CTCBase
// Leaf: ??1CTCHeroReceiveItems@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CA8E0, 249 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126EBFC, RTTI `.?AVCTCHeroReceiveItems@@` (rtti_bases: CTCHeroReceiveItems : CTCBase);
//   its slot 0 is the scalar deleting dtor 0x007CBD80. Ego_r.pdb has `??1CTCHeroReceiveItems@@UAE@XZ`. The PDB member
//   offsets less 8 for the 4-byte STLport list and 4 more for the 12-byte vector (Items +0x0C, PickedUpItems +0x10,
//   PCurrentItem +0x1C, PContainer +0x29, ItemPrimitiveHandle +0x9C, BackgroundPrimitiveHandle +0xA4) are the ones used here.
//   The inlined handle dtors store vtable 0x0122F598 (RTTI `.?AVCEnginePrimitiveHandle@@`).
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (Ego_r `?RemoveFromGlobalTCs@CTCBase@@QAEXXZ`), CBaseClass::~CBaseClass
//   0x0099A2E0 (the end of each inlined ~CEnginePrimitiveHandle), ~CBaseIntelligentPointer 0x00A01C10 (twice), free
//   0x00BFEA14 (the vector block and the list sentinel), std::_List_base<std::pair<CIntelligentPointer<CThingObject>,bool> >
//   ::clear 0x007CBB80 (out of line), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls: each handle's primitive slot 3
//   (NotifyReleased) when it holds the last reference, and slot 1 (Delete) on the primitive's last release.
// Layout: Ego_r.pdb CTCHeroReceiveItems (0xB8, retail 0xAC); CIntelligentPointer and C3DVector 1-aligned; no engine header.
// Shape: user-written dtor whose body is RemoveFromGlobalTCs(); CEnginePrimitiveHandle's inline virtual dtor (Clear():
//   NotifyReleased on the last reference, then Data.Reset(0)), then its CIVCountedPointer member's Release and ~CBaseClass.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                           // 0x007CBB80 (this instantiation)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CBaseObjectPointer;
class CBaseObject;
class CThing { public:                                      // retail flag byte +0x91 (Dead bit 0)
    unsigned char _pad_0x00[0x91];
    bool          Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResource : public CIVCountedPointeeBase { public:
};
class CDefPointeeBase : public CResource { public:
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
};

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)
class C2DBoxF { public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
};
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

class CThingObject;
class CTCHeroReceiveItems : public CTCBase { public:       // vtable 0x0126EBFC
    std::list<std::pair<CIntelligentPointer<CThingObject>, bool> >  Items;                      // +0x0C
    std::vector<long>                                               PickedUpItems;              // +0x10
    CIntelligentPointer<CThingObject>                               PCurrentItem;               // +0x1C
    bool                                                            PersistCurrentItem;         // +0x24
    bool                                                            ReceivingItems;             // +0x25
    bool                                                            DisplayingItems;            // +0x26
    bool                                                            ForceClear;                 // +0x27
    bool                                                            SendMessageW;               // +0x28 (PDB spelling)
    CIntelligentPointer<CThing>                                     PContainer;                 // +0x29
    float                                                           AmountThroughForAreaResize; // +0x34
    long                                                            FullSizeTimer;              // +0x38
    C2DBoxF                                                         InitialAreaItemTook;        // +0x3C
    C2DBoxF                                                         OldAreaItemTaking;          // +0x4C
    C2DBoxF                                                         CurrentAreaItemTaking;      // +0x5C
    CRightHandedSet                                                 OldItemRHSet;               // +0x6C
    CRightHandedSet                                                 CurrentItemRHSet;           // +0x84
    CEnginePrimitiveHandle                                          ItemPrimitiveHandle;        // +0x9C
    CEnginePrimitiveHandle                                          BackgroundPrimitiveHandle;  // +0xA4
    virtual ~CTCHeroReceiveItems();
};

CTCHeroReceiveItems::~CTCHeroReceiveItems()
{
    RemoveFromGlobalTCs();
}