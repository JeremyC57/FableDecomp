// CTCActionUseBase::CTCActionUseBase(CThing*) -- CTCBase(owner), own vtable, null action-use def, usable
// Leaf: ??0CTCActionUseBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DA939, length 32 bytes (a /Os module: unaligned entry,
// `push dword ptr [esp+8]` pass-through, `and dword ptr [..],0` zero store).
// Callee: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name). Own vtable 0x0123F09C = CTCActionUseBase (rtti-vtables.tsv); the landed
// CTCActionUse* constructors (0x004DA987, 0x004DAB6C, ...) call this one.
//
// Layout = Ego_r.pdb CTCActionUseBase (0x14 bytes): CTCBase 0xc, CDefPointer<CActionUseDef> PActionUseDef
// +0x0c, Usable +0x10. Shape: base constructor, vtable, the def pointer's inline constructor (one zero
// store, `and` under /Os), then Usable = true. engine/CTCBase.h holds the layout only, so the base is
// modelled as `CTCBase_Calls` (as the landed CTCRockTrollShield).
#pragma optimize("s", on)

class CThing;
class CActionUseDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCActionUseBase : public CTCBase_Calls { public:                   // vtable 0x0123F09C
    CDefPointer<CActionUseDef> PActionUseDef;               // +0x0c
    bool                       Usable;                      // +0x10
    virtual void v00();
    CTCActionUseBase(CThing* owner);
};

CTCActionUseBase::CTCActionUseBase(CThing* owner)
    : CTCBase_Calls(owner)
    , Usable(true)
{
}