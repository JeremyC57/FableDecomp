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
class CThing;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
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