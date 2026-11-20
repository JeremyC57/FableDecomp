// CJoystick::~CJoystick() -- base joystick destructor: reset the vtable, destroy the maintained-event list (clear it, free its sentinel node) and then the 64 stored input events, last to first
// Leaf: ??1CJoystick
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4420, length 65 bytes.
// Label correction: the catalog's "JoystickGui_Destructor" is CJoystick's destructor: it stores CJoystick's vtable 0x0129C0EC
//   (the landed constructor 0x009E43B0 stores the same one) and is called by the CJoystick deleting destructor 0x009E4980 and by
//   the CJoystickDX destructor path (0x00AB6AC2 / 0x00AB7981).
// Callees: std::list<CMaintainedEvent>::clear @ 0x009E4710 (out of line), free @ 0x00BFEA14 (the list sentinel),
//   CInputEvent::~CInputEvent @ 0x00A03B30 (an empty out-of-line body, ICF-folded `ret`, called once per array element).
// Model: same local layout as the landed constructor 0x009E43B0 -- engine/CJoystick.h carries the PDB's 12-byte
//   list (+0xd18 counters) where retail's STLport list is one node pointer (+0xd10), so the header cannot be used here.
// Shape: an empty user destructor: the vtable store, then the members in reverse order -- the list's inline destructor
//   (`clear(); if (node) free(node);`) and the inlined `dec edi ; jne` array-destructor loop walking back from +0xd04
//   in 0x34-byte steps (CInputEvent's destructor is declared, not inline, so each element is a call).
// NF136 (2026-11-20, q3887): the local CInputEvent model and the CMaintainedEventList / SMaintainedNode stand-ins (whose clear() bound
//   0x009E4710 as ?clear@CMaintainedEventList@@QAEXXZ) replaced by engine/CInputEvent.h (??0 / ??1CInputEvent@@QAE@XZ, unchanged spellings)
//   and the retail member type std::list<CMaintainedEvent> from stlport/fable_containers.h with FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE:
//   the inline STLport ~_List_base (clear(); deallocate the sentinel) binds ?clear@?$_List_base@VCMaintainedEvent@@V?$allocator@VCMaintainedEvent@@@std@@@std@@QAEXXZ.
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CInputEvent.h"
#include "engine/CMaintainedEvent.h"
#include "stlport/fable_containers.h"
class CInputManager;
class CJoystick {
public:
    virtual void v00();
    CInputEvent          EventStore[64];           // +0x04
    CInputManager*       InputManager;             // +0xd04
    bool                 ExclusiveMode;            // +0xd08
    std::list<CMaintainedEvent> MaintainedEventsList;     // +0xd0c
    long                 NoStoredEvents;           // +0xd10
    long                 NoEventScansRunning;      // +0xd14
    ~CJoystick();
};

CJoystick::~CJoystick()
{
}