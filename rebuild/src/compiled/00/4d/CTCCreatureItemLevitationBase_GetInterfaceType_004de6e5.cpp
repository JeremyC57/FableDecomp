// CTCCreatureItemLevitationBase::GetInterfaceType() const -- the component's interface type, TCI_CREATURE_ITEM_LEVITATION (0x61)
// Leaf: ?GetInterfaceType@CTCCreatureItemLevitationBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE6E5, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCCreatureItemLevitationHero's vtable 0x0124167C (the slot holds this body in 3 vtable(s), owner CTCCreatureItemLevitationBase); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCCreatureItemLevitationBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCCreatureItemLevitationBase::vfunc_21 (rebuild/src/compiled/00/4d/CTCCreatureItemLevitationBase_vfunc21_004de6e5.cpp).
// No shared header for CTCCreatureItemLevitationBase: minimal local definition on its RTTI base CTCBase (slots 28 / 30 are
// _purecall in its vtable 0x0124156C).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCCreatureItemLevitationBase : public CTCBase {
public:
    enum ELevitationLeaves { };                                     // W4ELevitationLeaves@CTCCreatureItemLevitationBase@@
    virtual ETCInterfaceType GetInterfaceType() const;              // slot 21
    virtual ELevitationLeaves GetLeafType() const = 0;              // slot 28
protected:
    virtual long GetMaxNumItems() const = 0;                        // slot 30
};
ETCInterfaceType CTCCreatureItemLevitationBase::GetInterfaceType() const
{
    return TCI_CREATURE_ITEM_LEVITATION;
}
