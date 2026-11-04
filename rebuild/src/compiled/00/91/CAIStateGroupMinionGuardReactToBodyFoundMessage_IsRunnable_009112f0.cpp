// CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable() -- runnable while FindBodyFoundEvent finds a matching message event for the creature
// Leaf: ?IsRunnable@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009112F0, 11 bytes: `call 0x009112A0 ; test eax,eax ; setne al ; ret`.
// Identity (two sources): (1) slot 11 of the CAIStateGroup_MinionGuardReactToBodyFoundMessage vtable 0x012892B8 (RTTI .?AVCAIStateGroup_MinionGuardReactToBodyFoundMessage@@ : CAIStateGroupBase), the base
//   IsRunnable slot (engine/CAIStateGroupBase.h); (2) Ego_r.pdb ?IsRunnable@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@UAE_NXZ (donor 0x00936020),
//   right after the private ?FindBodyFoundEvent@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@AAEPBVCMessageEvent@@XZ (donor 0x00935FD0), the callee at 0x009112A0.
// Shape: `if (Find()) return true; return false;` gives retail's `test eax,eax ; setne al`; `return Find() != 0;` and `!!` give
//   neg/sbb/neg (probed 2026-11-04).
// Callees: ?FindBodyFoundEvent@CAIStateGroup_MinionGuardReactToBodyFoundMessage@@AAEPBVCMessageEvent@@XZ 0x009112A0 (private, const CMessageEvent* -- Ego_r.pdb spelling).
// Header: engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS). No shared header for the derived class: it is declared
//   here with only the members this row uses.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
class CMessageEvent;
class CAIStateGroup_MinionGuardReactToBodyFoundMessage : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();                          // slot 11
private:
    const CMessageEvent* FindBodyFoundEvent();                          // 0x009112A0
};
bool CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable()
{
    if (FindBodyFoundEvent())
        return true;
    return false;
}