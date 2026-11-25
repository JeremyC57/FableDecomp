// CVtable_012bede0::~CVtable_012bede0() -- libpk / GameCODA mixer voice-effect destructor: restore its vtable, then run the voice-effect base destructor
// Leaf: ??1CVtable_012bede0@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C70BB0, 11 bytes: `mov dword [ecx],0012BEDE0h ; jmp 0x00C6E630` (no members with destructors of
//   its own; the base destructor is out of line, so the call is a tail jump).
// Identity: the class is the no-RTTI mixer voice effect named by its vtable 0x012BEDE0 (landed convention: its constructor 0x00C70B00,
//   UpdateVoices 0x00C71120 slot 1 and the scalar deleting destructor 0x00C71400 slot 0, which calls this address as the destructor). The
//   base is the landed ??1CVtable_012beda4@@UAE@XZ 0x00C6E630 (vtable 0x012BEDA4) over the GameCODA root CCodaObjectBase. These are the
//   landed spellings (one retail address, one name); the original class name is unknown.
// Callees: ??1CVtable_012beda4@@UAE@XZ 0x00C6E630; data ??_7CVtable_012bede0@@6B@ 0x012BEDE0.
// Header: engine/CCodaObjectBase.h (the GameCODA root); the voice-effect classes have no shared header (landed rows model them
//   locally), so only the destructors are declared.
// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base destructor it calls is respelled (??1CVtable_012beda4@@UAE@XZ -> ??1CSalRender@@UAE@XZ, unit 0x00C6E630).
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4; destructor 0x00C6E630)
class CVtable_012bede0 : public CSalRender {        // vtable 0x012BEDE0
public:
    virtual ~CVtable_012bede0();
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDE0 slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDE0 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};
CVtable_012bede0::~CVtable_012bede0()
{
}