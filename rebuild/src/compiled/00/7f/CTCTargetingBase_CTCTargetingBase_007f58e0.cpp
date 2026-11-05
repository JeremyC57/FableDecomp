// CTCTargetingBase::CTCTargetingBase(CThing* thing) -- CTCBase(thing), no script override, four empty thing pointers (nearest interesting, script-override interesting, script-override ranged target, targeted), no def, not looking at the camera
// Leaf: ??0CTCTargetingBase
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F58E0, length 106 bytes (`ret 4`); stores vtable 0x01271E74 (CTCTargetingBase,
// rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (x4, each followed by the
// inline CIntelligentPointer<CThing> vtable store 0x01238C6C).
// Layout (Ego_r.pdb CTCTargetingBase 0x38 = retail): ScriptOverrideIgnoreEverything +0x0C, PNearestInterestingThing +0x0D,
// PScriptOverrideInterestingThing +0x15, PScriptOverrideRangedTarget +0x1D, PTargetedThing +0x25 (8-byte intelligent pointers at odd
// offsets: `#pragma pack(1)`), PTargetingDef +0x30, LookAtCamera +0x34.
// Shape: init list in declaration order; each pointer member's address is formed in edi (`lea edi,[esi+0Dh]`) for the base ctor and the
// derived vtable store.
class CThing;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CTargetingDef;
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
};
#pragma pack(push, 1)
class CTCTargetingBase : public CTCBase { public:             // vtable 0x01271E74
    bool ScriptOverrideIgnoreEverything;                    // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;      // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;   // +0x15
    CIntelligentPointer<CThing> PScriptOverrideRangedTarget;   // +0x1d
    CIntelligentPointer<CThing> PTargetedThing;                // +0x25
    unsigned char _pad2d[3];
    CDefPointer<const CTargetingDef> PTargetingDef;         // +0x30
    bool LookAtCamera;                                      // +0x34
    CTCTargetingBase(CThing* thing);
    virtual ~CTCTargetingBase();
};
#pragma pack(pop)

CTCTargetingBase::CTCTargetingBase(CThing* thing)
    : CTCBase(thing)
    , ScriptOverrideIgnoreEverything(false)
    , LookAtCamera(false)
{
}