// CTCSoundAtmosphereVillage::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CTCSoundAtmosphereVillage (it only runs ~CTCSoundAtmosphere 0x004DB47D), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCSoundAtmosphereVillage@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCE88, 28 bytes, thiscall, `ret 4`; slot 0 of the CTCSoundAtmosphereVillage vtable
//   (rtti-vtables.tsv; RTTI .?AVCTCSoundAtmosphereVillage@@ : CTCSoundAtmosphere : CTCBase).
// Names (agentNF20, 2026-11-11; was the free stand-in CTCSoundAtmosphereVillage_ScalarDeletingDestructor calling
//   CTCSoundAtmosphereVillage_Destruct): the destructor it calls is FableWin.pdb ??1CTCSoundAtmosphere@@UAE@XZ 0x004DB47D, operator delete 0x00BFE9BC.
// Shape: VC7.1 emits the deleting destructor with the vtable, i.e. in a TU that constructs the class: MakeOne is that construction.
//   Local models: no engine header (the base CTCSoundAtmosphere as in the landed ??_G 0x007F5620); CTCBase is the shared header.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCSoundAtmosphere : public CTCBase {
public:
    CTCSoundAtmosphere(CThing& thing);                                   // Ego_r.pdb ??0CTCSoundAtmosphere@@QAE@AAVCThing@@@Z
    virtual ~CTCSoundAtmosphere();                                       // 0x004DB47D
};

class CTCSoundAtmosphereVillage : public CTCSoundAtmosphere {
public:
    CTCSoundAtmosphereVillage(CThing& thing) : CTCSoundAtmosphere(thing) { }
    virtual ETCInterfaceType GetInterfaceType() const;                   // CTCBase slot 21 (declared: the vtable is emitted here)
    virtual CCharString GetClassTypeName() const;                        // CTCBase slot 22
};

void* MakeOne(CThing& thing)
{
    return new CTCSoundAtmosphereVillage(thing);
}
