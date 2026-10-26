// CScriptGameResourceObjectScriptedThing::IsFollowingThing() -- true when the scripted thing's flag bit 0 (+0x6c) is set and its current AI state-group type is 2 (following)
// Leaf: IsFollowingThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009037E0, length 44 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 twice (on PThing at +0x14, re-fetched for
// each access, no null test), the thing's current state-group type getter @ 0x006D5AE0 (labelled
// "_Cons_val<...>", propagated; it reads the creature brain at +0x1a8 and tail-calls the brain).
// Shape: the intelligent pointer's address kept in esi; `if (a && b) return true; return false;`
// keeps two exits (`mov al,1` / `xor al,al`), a `bool` with no full-register materialisation.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing is CIntelligentPointer<CThing>,
// opaque there, so it is reached through a local view of the intelligent pointer).
#include "engine/CScriptGameResourceObjectScriptedThing.h"

class CThing_Retail { public:
    unsigned char  _pad00[0x6c];
    unsigned char  m_flags;                        // +0x6c
    long GetCurrentStateGroupType();               // 0x006D5AE0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    bool IsFollowingThing();
};

bool CScriptGameResourceObjectScriptedThing_Methods::IsFollowingThing()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if ((thing->GetPItem()->m_flags & 1) && thing->GetPItem()->GetCurrentStateGroupType() == 2)
        return true;
    return false;
}