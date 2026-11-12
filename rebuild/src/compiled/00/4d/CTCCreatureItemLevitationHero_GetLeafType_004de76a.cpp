// CTCCreatureItemLevitationHero::GetLeafType() const -- the hero's levitation leaf type (0)
// Leaf: ?GetLeafType@CTCCreatureItemLevitationHero@@UBE?AW4ELevitationLeaves@CTCCreatureItemLevitationBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE76A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCCreatureItemLevitationHero's vtable 0x0124167C (the slot holds this body in 1 vtable(s), owner CTCCreatureItemLevitationHero); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetLeafType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetLeafType@CTCCreatureItemLevitationHero@@UBE?AW4ELevitationLeaves@CTCCreatureItemLevitationBase@@XZ; `ret` matches the signature.
// Was: CTCCreatureItemLevitationHero::vfunc_28 (rebuild/src/compiled/00/4d/gapscan_sub004de76a_004de76a.cpp).
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
class CTCCreatureItemLevitationHero : public CTCCreatureItemLevitationBase {
public:
    virtual ELevitationLeaves GetLeafType() const;
protected:
    virtual long GetMaxNumItems() const;
};
CTCCreatureItemLevitationBase::ELevitationLeaves CTCCreatureItemLevitationHero::GetLeafType() const
{
    return (ELevitationLeaves)0;
}
