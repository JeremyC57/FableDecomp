// CTCSoundAtmosphere::~CTCSoundAtmosphere() -- sound-atmosphere component dtor (compiler-generated): release the atmospheres def pointer, then ~CTCBase
// Leaf: ??1CTCSoundAtmosphere@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB47D, 32 bytes, thiscall; size-optimised TU (unaligned, `and [esi+10h],0`), ends `jmp ~CTCBase`.
// Identity (agentFAM06): callers 0x004DCE88 (slot 0, scalar deleting destructor, of the CTCSoundAtmosphereVillage vtable 0x0124111C:
//   the derived class's own destructor adds nothing and expands to this call) and 0x007F5620 (slot 0 of the CTCSoundAtmosphere
//   vtable 0x01271DFC, another TU linking the same COMDAT); Ego_r.pdb / FableWin.pdb ??1CTCSoundAtmosphere@@UAE@XZ; Ego_r.pdb
//   CTCSoundAtmosphere is 0x14 bytes, TimeUntilNextUpdate +0x0C and PDef (CDefPointer<const CSoundAtmospheresDef>) +0x10, the
//   member released here; the tail jump goes to ~CTCBase 0x00686830. Catalog CIVCountedPointer_Release and the landed leaf
//   ?dtor_C3DAnimationBlendState@@... are placeholders.
// Body: retail CTCBase (engine/CTCBase.h) and CDefPointer<T> (engine/CDefPointer.h). No shared header exists for CTCSoundAtmosphere:
//   the class is declared here with Ego_r.pdb's members (NOTES.md "Header debt"). Retail stores no vtable: the compiler-generated
//   destructor. The helper below only makes VC7.1 emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
class CSoundAtmospheresDef;

class CTCSoundAtmosphere : public CTCBase {                  // Ego_r.pdb, 0x14 bytes
public:
    float                                   TimeUntilNextUpdate;     // +0x0C
    CDefPointer<const CSoundAtmospheresDef> PDef;                    // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundAtmosphere) == 0x14);

void EmitSoundAtmosphereDestructor(CTCSoundAtmosphere* p)    // emits the implicit ??1; not a retail function
{
    p->CTCSoundAtmosphere::~CTCSoundAtmosphere();
}
