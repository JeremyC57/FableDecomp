// NCreatureMode::CModeHoverNormal::GetAnimUpdateType() const -- returns (EAnimUpdateMode)1
// Leaf: ?GetAnimUpdateType@CModeHoverNormal@NCreatureMode@@MBE?AW4EAnimUpdateMode@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744100, thiscall, `ret`.
// Identity: slot 13 of NCreatureMode::CModeHoverNormal's vtable 0x012687FC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeHoverNormal or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetAnimUpdateType(void) at this slot; Ego_r/FableWin name set: ?GetAnimUpdateType@CModeHoverNormal@NCreatureMode@@MBE?AW4EAnimUpdateMode@@XZ; `ret` matches the signature.
#include "engine/EAnimUpdateMode.h"
namespace NCreatureMode {
class CModeHoverNormal {
public:
protected:
    virtual EAnimUpdateMode GetAnimUpdateType() const;
};
EAnimUpdateMode CModeHoverNormal::GetAnimUpdateType() const
{
    return (EAnimUpdateMode)1;
}
}
