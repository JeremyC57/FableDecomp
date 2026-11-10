// NPlayerGui::CDrawQuestInfo::~CDrawQuestInfo() -- implicit destructor: the quest-element list, the Text and Name strings, the oracle primitive handles, the oracle-symbol list, then CDrawBase's primitive-handle vector
// Leaf: ??1CDrawQuestInfo
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00443D70, length 147 bytes, plain `ret` (thiscall).
// Callees: list<CCountedPointer<CQuestInfoElement> >::clear() @ 0x00441140 (landed), free @ 0x00BFEA14, CWideString::
// ~CWideString @ 0x0099B510, ~_List_base<long> @ 0x0042AB73 (out of line: list<long>'s clear is inlined into it in its own
// size TU, so this speed TU calls it; manifest "CActiveFile::OnReadFinished" is propagated); the handle destructors run
// through vtable slot 0 (`push 0 ; call [vptr]`).
// Label: the catalog "EventConversation_Destructor" is behavioural. Callers: the `scalar deleting destructor' 0x00444EB0 and
// the CCountedPointer deleter 0x00445370, whose Reset (0x00445470) the CPlayerGui constructor uses for the member it fills
// with `new (0x5C) CDrawQuestInfo` (ctor 0x0064F659, landed).
// Layout (the landed ctor): CDrawBase::PrimitiveHandles +0x1C, GuiMode +0x28, FadeInTimer +0x2C, MiniModeTimer +0x30, MiniMode
// +0x34, FramesUntilMiniMode +0x38, MiniGameElementIndex +0x3C, OracleSymbols +0x40, OraclePrimitiveHandles +0x44, Name +0x50,
// Text +0x54, Elements +0x58.
// Model: the class declares no destructor: VC7.1 emits the compiler-generated one out of line for the retail-style `delete p`
// deleter below and stores no vtable in it. STLport `~_List_base` = `clear(); _M_node.deallocate(node, 1)`; the
// list<long> instantiation's destructor is an explicit specialisation declared here and defined elsewhere (the retail
// call); vector teardown = `_Destroy(start, finish)` then `if (start) free(start)`. Speed TU.
#include <stddef.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CPlayerGui;

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

class CEnginePrimitiveHandle { public:                    // vptr + counted primitive pointer
    virtual ~CEnginePrimitiveHandle();
    void* Data;
};
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> class allocator;
template <class T, class _Alloc> struct _List_base {   // STLport list: the sentinel node pointer (alloc proxy)
    _List_node<T>* _M_node;
    void clear();                                   // out of line: 0x00441140 (list<CCountedPointer<CQuestInfoElement> >)
    ~_List_base()
    {
        clear();
        if (_M_node != 0)                           // _M_node.deallocate(_M_node._M_data, 1)
            free(_M_node);
    }
};
template <> _List_base<long, allocator<long> >::~_List_base();   // 0x0042AB73 (Codex name fix 2026-11-10: STLport's two-parameter base)
template <class T, class _Alloc = allocator<T> > struct list : _List_base<T, _Alloc> { };
}
struct CQuestInfoElement;
#include "engine/CCountedPointer.h"
class CWideString { public:
    void* m_pData;
    ~CWideString();                                 // 0x0099B510
};

namespace NPlayerGui {
class CDrawBase { public:
    virtual void v00();
    unsigned char                        _body[0x1c - 4];   // +0x04 .. +0x1c
    std::vector<CEnginePrimitiveHandle>  PrimitiveHandles;  // +0x1c
};

class CDrawQuestInfo : public CDrawBase { public:                // vtable 0x0125A688
    virtual void v00();
    long                                           GuiMode;                // +0x28 EPlayerGuiMode
    float                                          FadeInTimer;            // +0x2c
    float                                          MiniModeTimer;          // +0x30
    bool                                           MiniMode;               // +0x34
    long                                           FramesUntilMiniMode;    // +0x38
    long                                           MiniGameElementIndex;   // +0x3c
    std::list<long>                                OracleSymbols;          // +0x40
    std::vector<CEnginePrimitiveHandle>            OraclePrimitiveHandles; // +0x44
    CWideString                                    Name;                   // +0x50
    CWideString                                    Text;                   // +0x54
    std::list<CCountedPointer<CQuestInfoElement> > Elements;               // +0x58
};
}

void __fastcall DestroyDrawQuestInfo(NPlayerGui::CDrawQuestInfo* p)   // the CCountedPointer deleter shape (0x00445370)
{
    delete p;
}