// CSoundAtmospheresDef::Transfer(CPersistContext& context) -- persist the def's members in declaration order
// Leaf: ?Transfer@CSoundAtmospheresDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EC153, 16 bytes, virtual `ret 4` (RTTI vtable slot 18; RTTI base CSubDefClassBase): TransferVector<CCharString> 0x004331F9.
// Port (round 4, NF40): was a pad-struct stand-in (no Leaf line) on a local CPersistContext model with stand-in per-type transfer members
//   over opaque fields; now engine/CPersistContext.h (Transfer<T> / TransferVector<T>) and the retail def chain
//   engine/CSubDefClassBase.h. No shared header exists for CSoundAtmospheresDef, so the row keeps its own model of the class, with the
//   Ego_r.pdb members (SoundAtmospheres +0x28; the retail offsets are the Ego_r ones) and the virtual override
//   ?Transfer@CSoundAtmospheresDef@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb / FableWin.pdb).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"

class CSoundAtmospheresDef : public CSubDefClassBase {
public:
    virtual void Transfer(CPersistContext& context);     // slot 18
    std::vector<CCharString>   SoundAtmospheres;              // +0x28
};

void CSoundAtmospheresDef::Transfer(CPersistContext& context)
{
    context.TransferVector(SoundAtmospheres);
}
