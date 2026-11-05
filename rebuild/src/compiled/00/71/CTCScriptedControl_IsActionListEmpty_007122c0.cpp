// CTCScriptedControl::IsActionListEmpty() const -- true when no scripted action is queued
// Leaf: ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x007122C0, 15 bytes: `mov eax,[ecx+14h] ; mov edx,[eax] ; xor ecx,ecx ; cmp edx,eax ; sete cl ;
// mov al,cl ; ret`.
// Identity (proven): Ego_r.pdb names ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ and lays PActionList
// (std::list<CCountedPointer<CActionBase> >) at +0x14; the many callers (0x004C70F7, 0x006A85AE, 0x00747EF9 ...) take the
// scripted-control component from the thing's interface map; landed callers already bind this name. The manifest label
// "CScriptTask_IsRunning" is propagated.
// Shape: STLport 4.6 list::empty() (`_M_node->_M_next == _M_node`) returned as a plain `bool` goes through the full ecx
// register (`xor ecx,ecx ; sete cl ; mov al,cl`).
namespace std {
template<class _Tp> class allocator { public: };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class _Tp, class _Alloc = allocator<_Tp> > class list { public:      // STLport 4.6 (4 bytes)
    _List_node_base* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};
}
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
class CScriptGameResourceObjectScriptedThing;
class CTCBase { public:                             // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};
class CTCScriptedControl : public CTCBase { public:
    class CActionBase;
    CIntelligentPointer<CScriptGameResourceObjectScriptedThing> PScriptHandleImp;   // +0x0c
    std::list<CCountedPointer<CActionBase> > PActionList;                         // +0x14
    bool IsActionListEmpty() const;
};

bool CTCScriptedControl::IsActionListEmpty() const
{
    return PActionList.empty();
}