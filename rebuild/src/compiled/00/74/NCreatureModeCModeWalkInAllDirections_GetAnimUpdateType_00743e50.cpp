// NCreatureMode::CModeWalkInAllDirections::GetAnimUpdateType() const -- returns (EAnimUpdateMode)2
// Leaf: ?GetAnimUpdateType@CModeWalkInAllDirections@NCreatureMode@@MBE?AW4EAnimUpdateMode@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743E50, thiscall, `ret`.
// Identity: slot 13 of NCreatureMode::CModeWalkInAllDirections's vtable 0x012687AC (defined here: the slot holds this body in 1 vtable(s), all of NCreatureMode::CModeWalkInAllDirections or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetAnimUpdateType(void) at this slot; Ego_r/FableWin name set: ?GetAnimUpdateType@CModeWalkInAllDirections@NCreatureMode@@MBE?AW4EAnimUpdateMode@@XZ; `ret` matches the signature.
#include "engine/EAnimUpdateMode.h"
namespace NCreatureMode {
class CModeWalkInAllDirections {
public:
protected:
    virtual EAnimUpdateMode GetAnimUpdateType() const;
};
EAnimUpdateMode CModeWalkInAllDirections::GetAnimUpdateType() const
{
    return (EAnimUpdateMode)2;
}
}
