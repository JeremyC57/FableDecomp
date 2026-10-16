// CTCTargetingBase::CTCTargetingBase(CThing* thing) -- CTCBase(thing), no script override, four empty thing pointers (nearest interesting, script-override interesting, script-override ranged target, targeted), no def, not looking at the camera
// Leaf: ??0CTCTargetingBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F58E0, length 106 bytes (`ret 4`); stores vtable 0x01271E74 (CTCTargetingBase,
// rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (x4, each followed by the
// inline CIntelligentPointer<CThing> vtable store 0x01238C6C).
// Layout (Ego_r.pdb CTCTargetingBase 0x38 = retail): ScriptOverrideIgnoreEverything +0x0C, PNearestInterestingThing +0x0D,
// PScriptOverrideInterestingThing +0x15, PScriptOverrideRangedTarget +0x1D, PTargetedThing +0x25 (8-byte intelligent pointers at odd
// offsets: `#pragma pack(1)`), PTargetingDef +0x30, LookAtCamera +0x34.
// Shape: init list in declaration order; each pointer member's address is formed in edi (`lea edi,[esi+0Dh]`) for the base ctor and the
// derived vtable store.
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};
struct CTargetingDef;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CTCBase {
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
};
#pragma pack(push, 1)
struct CTCTargetingBase : CTCBase {             // vtable 0x01271E74
    bool ScriptOverrideIgnoreEverything;                    // +0x0c
    CIntelligentPointerThing PNearestInterestingThing;      // +0x0d
    CIntelligentPointerThing PScriptOverrideInterestingThing;   // +0x15
    CIntelligentPointerThing PScriptOverrideRangedTarget;   // +0x1d
    CIntelligentPointerThing PTargetedThing;                // +0x25
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
