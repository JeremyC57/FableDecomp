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
#include "engine/CCodaObjectBase.h"
class CVtable_012beda4 : public CCodaObjectBase {      // vtable 0x012BEDA4
public:
    virtual ~CVtable_012beda4();                        // 0x00C6E630
};
class CVtable_012bedcc : public CVtable_012beda4 {        // vtable 0x012BEDCC
public:
    virtual ~CVtable_012bedcc();
};
CVtable_012bedcc::~CVtable_012bedcc()
{
}