// CVtable_012bedcc::~CVtable_012bedcc() -- libpk / GameCODA mixer voice-effect destructor: restore its vtable, then run the voice-effect base destructor
// Leaf: ??1CVtable_012bedcc@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C701D0, 11 bytes: `mov dword [ecx],0012BEDCCh ; jmp 0x00C6E630` (no members with destructors of
//   its own; the base destructor is out of line, so the call is a tail jump).
// Identity: the class is the no-RTTI mixer voice effect named by its vtable 0x012BEDCC (landed convention: its constructor 0x00C70120,
//   UpdateVoices 0x00C70750 slot 1 and the scalar deleting destructor 0x00C70A30 slot 0, which calls this address as the destructor). The
//   base is the landed ??1CVtable_012beda4@@UAE@XZ 0x00C6E630 (vtable 0x012BEDA4) over the GameCODA root CCodaObjectBase. These are the
//   landed spellings (one retail address, one name); the original class name is unknown.
// Callees: ??1CVtable_012beda4@@UAE@XZ 0x00C6E630; data ??_7CVtable_012bedcc@@6B@ 0x012BEDCC.
// Header: engine/CCodaObjectBase.h (the GameCODA root); the voice-effect classes have no shared header (landed rows model them
//   locally), so only the destructors are declared.
// NF444 binder: the base is the shared engine/CSalRender.h (GameCODA SDK class; was the local model CVtable_012beda4). This row keeps
//   its own symbol; only the base destructor it calls is respelled (??1CVtable_012beda4@@UAE@XZ -> ??1CSalRender@@UAE@XZ, unit 0x00C6E630).
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4; destructor 0x00C6E630)
class CVtable_012bedcc : public CSalRender {        // vtable 0x012BEDCC
public:
    virtual ~CVtable_012bedcc();
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDCC slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDCC (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};
CVtable_012bedcc::~CVtable_012bedcc()
{
}