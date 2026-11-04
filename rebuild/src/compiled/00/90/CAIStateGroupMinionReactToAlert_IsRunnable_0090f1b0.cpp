// CAIStateGroup_MinionReactToAlert::IsRunnable() -- runnable while FindAlertEvent finds a matching message event for the creature
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToAlert@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090F1B0, 11 bytes: `call 0x0090F110 ; test eax,eax ; setne al ; ret`.
// Identity (two sources): (1) slot 11 of the CAIStateGroup_MinionReactToAlert vtable 0x01289070 (RTTI .?AVCAIStateGroup_MinionReactToAlert@@ : CAIStateGroupBase), the base
//   IsRunnable slot (engine/CAIStateGroupBase.h); (2) Ego_r.pdb ?IsRunnable@CAIStateGroup_MinionReactToAlert@@UAE_NXZ (donor 0x00933FF0),
//   right after the private ?FindAlertEvent@CAIStateGroup_MinionReactToAlert@@AAEPBVCMessageEvent@@XZ (donor 0x00933F50), the callee at 0x0090F110.
// Shape: `if (Find()) return true; return false;` gives retail's `test eax,eax ; setne al`; `return Find() != 0;` and `!!` give
//   neg/sbb/neg (probed 2026-11-04).
// Callees: ?FindAlertEvent@CAIStateGroup_MinionReactToAlert@@AAEPBVCMessageEvent@@XZ 0x0090F110 (private, const CMessageEvent* -- Ego_r.pdb spelling).
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS). No shared header for the derived class: it is declared
//   here with only the members this row uses.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
class CMessageEvent;
class CAIStateGroup_MinionReactToAlert : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();                          // slot 11
private:
    const CMessageEvent* FindAlertEvent();                          // 0x0090F110
};
bool CAIStateGroup_MinionReactToAlert::IsRunnable()
{
    if (FindAlertEvent())
        return true;
    return false;
}