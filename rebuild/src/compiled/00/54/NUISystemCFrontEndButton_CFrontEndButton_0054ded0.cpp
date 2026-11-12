// NUISystem::CFrontEndButton::~CFrontEndButton() -- store the button's three subobject vtables, destroy the action-param list, destroy and free the two CCountedPointer<CAction> vectors, then the base control destructor
// Leaf: ??1CFrontEndButton
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Count->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054DED0, length 127 bytes (+ int3 padding).
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller
// is the scalar deleting destructor @ 0x0054E1C0, slot 0 of NUISystem::CFrontEndButton's vtable 0x01249554 (stored at
// +0; 0x01249530 and 0x01249528 are the same class's vtables of its subobjects at +4 and +0x18).
// Callees: list<CActionParam*> destructor @ 0x0042AB1C (catalogued as the propagated "CActiveFile::OnReadFinished"),
// _Destroy_range<CCountedPointer<CAction>> @ 0x0054A1C0 (landed; `__fastcall` first/last, the iterator tag by reference at
// `lea [esp+7]`), free @ 0x00BFEA14 (x2), the base control destructor @ 0x0055B760 (catalogued "CHoverable_destructor").
// Layout: Ego_r.pdb NUISystem::CFrontEndButton (PDB 500 bytes; retail members sit 0x30 lower): m_fLastInputTime +0x194,
// Actions +0x198, ActionsOnDelete +0x1a4 (12-byte STLport vectors), UIActionParams +0x1b0.
// Shape: the three vtable stores (lea of the list member scheduled first), members destroyed in reverse order (the list
// out of line, each vector's inline destructor: range destroy, free), then the base destructor as a plain call; `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

namespace NUISystem {
class CAction;
class CActionParam;
}
#include "engine/CCountedPointer.h"

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x0054A1C0 (std::__destroy_aux<T*>)
}

struct CActionVector {                             // vector<CCountedPointer<NUISystem::CAction>>
    CCountedPointer<NUISystem::CAction>* _M_start;
    CCountedPointer<NUISystem::CAction>* _M_finish;
    CCountedPointer<NUISystem::CAction>* _M_end_of_storage;
    ~CActionVector()
    {
        std::__destroy_aux(_M_start, _M_finish, std::__false_type());
        if (_M_start)
            free(_M_start);
    }
};
struct CActionParamList {                          // list<NUISystem::CActionParam*>
    void* _M_node;
    ~CActionParamList();                           // 0x0042AB1C
};

struct SControlBase0 { virtual void v00(); };                                       // +0x00
struct SControlBase1 { virtual void v10(); unsigned char _pad[0x10]; };             // +0x04
struct SControlBase2 { virtual void v20(); };                                       // +0x18
struct CFrontEndControl_0055b760 : SControlBase0, SControlBase1, SControlBase2 {    // 0x194 bytes
    unsigned char _pad[0x194 - 0x1c];
    ~CFrontEndControl_0055b760();                  // 0x0055B760
};

namespace NUISystem {
class CFrontEndButton : public CFrontEndControl_0055b760 { public:   // vtables 0x01249554 / 0x01249530 / 0x01249528
    float            m_fLastInputTime;             // +0x194
    CActionVector    Actions;                      // +0x198
    CActionVector    ActionsOnDelete;              // +0x1a4
    CActionParamList UIActionParams;               // +0x1b0
    virtual void v00();
    virtual void v10();
    virtual void v20();
    ~CFrontEndButton();
};

CFrontEndButton::~CFrontEndButton()
{
}
}