// CTCCreatureItemLevitationNymph::GetLeafType() const -- the nymph's levitation leaf type (1)
// Leaf: ?GetLeafType@CTCCreatureItemLevitationNymph@@UBE?AW4ELevitationLeaves@CTCCreatureItemLevitationBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE73A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCreatureItemLevitationNymph's vtable 0x012415F4 (the slot holds this body in 1 vtable(s), owner CTCCreatureItemLevitationNymph); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetLeafType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLeafType@CTCCreatureItemLevitationNymph@@UBE?AW4ELevitationLeaves@CTCCreatureItemLevitationBase@@XZ; `ret` matches the signature.
// Was: CTCCreatureItemLevitationNymph::vfunc_28 (rebuild/src/compiled/00/4d/CTCCreatureItemLevitationNymph_vfunc28_004de73a.cpp).
// No shared header for the levitation classes: minimal local definitions on the RTTI bases (CTCBase <- CTCCreatureItemLevitationBase).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCCreatureItemLevitationBase : public CTCBase {
public:
    enum ELevitationLeaves { };                                     // W4ELevitationLeaves@CTCCreatureItemLevitationBase@@
    virtual ELevitationLeaves GetLeafType() const = 0;              // slot 28
protected:
    virtual long GetMaxNumItems() const = 0;                        // slot 30
};
class CTCCreatureItemLevitationNymph : public CTCCreatureItemLevitationBase {
public:
    virtual ELevitationLeaves GetLeafType() const;
};
CTCCreatureItemLevitationBase::ELevitationLeaves CTCCreatureItemLevitationNymph::GetLeafType() const
{
    return (ELevitationLeaves)1;
}
