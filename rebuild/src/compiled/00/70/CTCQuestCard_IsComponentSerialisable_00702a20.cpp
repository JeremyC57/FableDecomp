// CTCQuestCard::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCQuestCard@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702A20, thiscall, `ret`.
// Identity: slot 23 of CTCQuestCard's vtable 0x01264C44 (defined here: the slot holds this body in 1 vtable(s), all of CTCQuestCard or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCQuestCard@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCQuestCard.h (opt-ins as in the landed row CTCQuestCard_CTCQuestCard_00702a50.cpp).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCARD
#include "engine/CTCQuestCard.h"
bool CTCQuestCard::IsComponentSerialisable() const
{
    return true;
}
