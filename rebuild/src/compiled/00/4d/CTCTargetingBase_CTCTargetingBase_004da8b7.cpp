// CTCTargetingBase::~CTCTargetingBase() -- releases the targeting def, destroys the four thing pointers in reverse order, then ~CTCBase (tail jump)
#pragma optimize("s", on)
// Leaf: ??1CTCTargetingBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA8B7, length 64 bytes. Called from the deleting destructors of
// CTCTargetingBase's derived components (0x004E009E, 0x004E1030, 0x004E3809, 0x007F5D03).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x4), CTCBase::~CTCBase @ 0x00686830, and the
// def's virtual slot 1 (destroy) when its reference count drops to zero.
// Ego_r.pdb CTCTargetingBase (0x38): ScriptOverrideIgnoreEverything +0x0c, PNearestInterestingThing +0x0d,
// PScriptOverrideInterestingThing +0x15, PScriptOverrideRangedTarget +0x1d, PTargetedThing +0x25 (all
// CIntelligentPointer<CThing>, UNALIGNED: byte-packed), PTargetingDef (CDefPointer<const CTargetingDef>) +0x30, LookAtCamera +0x34.
// Shape: no vtable store at all -- the abstract class is `__declspec(novtable)`; the def pointer's inline release keeps the
// pointer in ecx and clears the member inside the non-null arm with `and dword ptr [..],0` (a size TU:
// `#pragma optimize("s",on)`, the release `__forceinline`); the intelligent pointers have no destructor of their own, so
// the base destructor is called directly at each member address, last member first.
class CThing;
class CBaseObject;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    ~CTCBase();                              // 0x00686830
};

struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+4)
    long m_refCount;                         // +0x04
};

struct CDefPointer {                         // CDefPointer<const CTargetingDef>
    CDefPointee* Object;
    __forceinline ~CDefPointer()
    {
        CDefPointee* def = Object;
        if (def) {
            if (--def->m_refCount == 0)
                def->Destroy();
            Object = 0;
        }
    }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    ~CBaseIntelligentPointer();              // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
};

struct __declspec(novtable) CTCTargetingBase : CTCBase {
    bool                ScriptOverrideIgnoreEverything;    // +0x0c
    CIntelligentPointer PNearestInterestingThing;          // +0x0d
    CIntelligentPointer PScriptOverrideInterestingThing;   // +0x15
    CIntelligentPointer PScriptOverrideRangedTarget;       // +0x1d
    CIntelligentPointer PTargetedThing;                    // +0x25
    unsigned char       _pad_0x2d[3];
    CDefPointer         PTargetingDef;                     // +0x30
    bool                LookAtCamera;                      // +0x34
    ~CTCTargetingBase();
};
#pragma pack(pop)

CTCTargetingBase::~CTCTargetingBase()
{
}