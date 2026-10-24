// NPlayerGui::CDrawPerceivers::~CDrawPerceivers() -- implicit destructor: clear and free the perceived-things list, then CDrawBase's primitive-handle vector (virtual element destructors, then free the storage)
// Leaf: ??1CDrawPerceivers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00441C20, length 75 bytes, plain `ret` (thiscall).
// Callees: list<CIntelligentPointer<const CThing> >::clear() @ 0x0043F230 (out of line), free @ 0x00BFEA14; the handle
// destructors run through vtable slot 0 (`push 0 ; call [vptr]`).
// Label: the catalog "TreePane_DestructorHelper" is behavioural. Callers: the `scalar deleting destructor' 0x00442D30 and the
// CCountedPointer deleter 0x00443820, whose Reset (0x00444670) the CPlayerGui constructor uses for the member it fills with
// `new (0x40) CDrawPerceivers` (ctor 0x0064ED06, landed).
// Layout (PDB CDrawPerceivers, retail 4 lower): CDrawBase::PrimitiveHandles +0x1C, State +0x28, Allegiance +0x2C,
// FrameInterp +0x30, ThrobScale +0x34, ThingCount +0x38, Things +0x3C (STLport list: one sentinel-node pointer).
// Model: the class declares no destructor: VC7.1 emits the compiler-generated one out of line for the retail-style `delete p`
// deleter below and stores no vtable in it. STLport `~_List_base` = `clear(); _M_node.deallocate(node, 1)` (null-tested
// free); vector teardown = `_Destroy(start, finish)` then `if (start) free(start)`. Speed TU.
#include <stddef.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
struct CPlayerGui;

namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) free(_M_start); }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

struct CEnginePrimitiveHandle {                    // vptr + counted primitive pointer
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
template <class T> struct CIntelligentPointer {    // vptr + item
    virtual ~CIntelligentPointer();
    void* m_pItem;
};

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_base {             // STLport list: the sentinel node pointer (alloc proxy)
    _List_node<T>* _M_node;
    void clear();                                   // out of line: 0x0043F230 (list<CIntelligentPointer<const CThing> >)
    ~_List_base()
    {
        clear();
        if (_M_node != 0)                           // _M_node.deallocate(_M_node._M_data, 1)
            free(_M_node);
    }
};
template <class T> struct list : _List_base<T> { };
}

struct CThing;

namespace NPlayerGui {
struct CDrawBase {
    virtual void v00();
    unsigned char                        _body[0x1c - 4];   // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle>  PrimitiveHandles;  // +0x1c
};

struct CDrawPerceivers : CDrawBase {               // vtable 0x0125A670
    virtual void v00();
    long                                           State;        // +0x28 EPerceivedState
    long                                           Allegiance;   // +0x2c EPerceivedAllegiance
    float                                          FrameInterp;  // +0x30
    float                                          ThrobScale;   // +0x34
    long                                           ThingCount;   // +0x38
    std::list<CIntelligentPointer<const CThing> >  Things;       // +0x3c
};
}

void __fastcall DestroyDrawPerceivers(NPlayerGui::CDrawPerceivers* p)   // the CCountedPointer deleter shape (0x00443820)
{
    delete p;
}