// CVtable_012bede0::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C70BB0), then the library's CCodaObjectBase::operator delete (0x00C3C370) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCVtable_012bede0@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71400, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "engine_api CLandscapeBackgroundPatch 0xC70A30/0xC71400/0xC70120/0xC70B00
//   are the voice effects' dtors/ctors"):
//   - slot 0 of the mixer voice-effect vtable 0x012BEDE0 (agent444's CVtable_012bede0: the landed 0x00C71120 UpdateVoices is
//     slot 1 of the same table), stored by the landed constructor 0x00C70B00 (called once, from 0x00C55393);
//   - it calls the destructor 0x00C70BB0 (the vtable store, then a tail jump to the CVtable_012beda4 destructor
//     0x00C6E630) and frees through 0x00C3C370, the libpk root's class operator delete: CVtable_012beda4 derives from
//     the CCodaObjectBase root (vtable 0x012B5868).
//   The CLandscapeBackgroundPatch vector deleting destructor (catalog engine_api.tsv) is a propagated label over a
//   `T::VecDel` placeholder; the class is named after its vtable (no-RTTI module), as the landed siblings are.
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
// NF444 binder: the base is the shared engine/CSalRender.h (was a local CCodaObjectBase + CVtable_012beda4 model); this row keeps its own
//   symbol and no longer references ??0CVtable_012beda4@@QAE@XZ. Fidelity: scaffolding, not retail: the TU-local reference class below
//   compiles the inline constructor (CSalRender is abstract, so nothing is instantiated), which emits the vtable and this ??_G.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRender.h"   // CSalRender (vtable 0x012BEDA4; constructor 0x00C6E6A0, destructor 0x00C6E630)

class CVtable_012bede0 : public CSalRender {     // vtable 0x012BEDE0
public:
    CVtable_012bede0() { }                             // out of line at 0x00C70B00 in retail
    virtual ~CVtable_012bede0();                       // 0x00C70BB0
    virtual void ClearFilterHistory();                // slot 1: own override (retail 0x012BEDE0 slot 1)
    virtual void Process(const short*, int*(&)[4]);   // slots 2-4: own overrides in retail 0x012BEDE0 (SDK signatures;
    virtual void SetGain(float, const float*, bool);  //   the landed method rows still carry their stand-in spellings)
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
};

namespace {
class CVtable_012bede0Reference : public CVtable_012bede0 {
public:
    CVtable_012bede0Reference();
};
// Out of class, so VC7.1 compiles it although nothing calls it.
CVtable_012bede0Reference::CVtable_012bede0Reference()
{
}
}
