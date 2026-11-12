// CTCSoundAtmosphere::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTCSoundAtmosphere 0x004DB47D, then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCSoundAtmosphere@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F5620, 30 bytes, thiscall, `ret 4` (`add esp,4` after the delete: a speed TU); slot 0 of the
//   CTCSoundAtmosphere vtable 0x01271DFC (rtti-vtables.tsv; RTTI .?AVCTCSoundAtmosphere@@ : CTCBase).
// Names (agentNF20, 2026-11-11; was the stand-in CTCSoundAtmosphere::ScalarDeletingDestructor calling CTCSoundAtmosphere::Dtor): the
//   destructor is FableWin.pdb ??1CTCSoundAtmosphere@@UAE@XZ 0x004DB47D (user-declared: it is called, not expanded), operator delete 0x00BFE9BC.
// Shape: VC7.1 emits the deleting destructor with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction
//   (the inline constructor is the emission vehicle). Local model: no engine header; the base is the shared engine/CTCBase.h.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSoundAtmosphere : public CTCBase {
public:
    CTCSoundAtmosphere(CThing& thing) : CTCBase(thing) { }
    virtual ~CTCSoundAtmosphere();                                       // 0x004DB47D
    virtual ETCInterfaceType GetInterfaceType() const;                   // CTCBase slot 21 (declared: the vtable is emitted here)
    virtual CCharString GetClassTypeName() const;                        // CTCBase slot 22
};

void* MakeOne(CThing& thing)
{
    return new CTCSoundAtmosphere(thing);
}
