// CTCBase::CTCBase(CThing& parent_thing) -- CBaseClassNonCopyable base, ParentThing at +4, ToBeRemoved / IsInGlobalUpdateTCs cleared, this class's vtable
// Leaf: ??0CTCBase@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686800, length 33 bytes.
// Signature (FINDINGS chunk 391 callee "CTCBase::CTCBase(CThing&)"): Ego_r.pdb `??0CTCBase@@QAE@AAVCThing@@@Z`; members
// ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9 (struct_layouts_egor, 12 bytes); the callers pass the owning
// thing by reference (e.g. the CTCSpecialAbilities constructor 0x0072C6E0 of chunk 391). engine/CTCBase.h is the
// flattened PDB record without the constructor, so the class is modelled here (header debt).
// Base: CBaseClassNonCopyable (the 0x0099A2F0 constructor stores vtable 0x0129A7C4, RTTI CBaseClassNonCopyable in
// rtti-vtables.tsv, whose slot 0 0x0099A470 is the virtual scalar deleting destructor; the landed source called it
// `CBase`). Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x0125BE0C = CTCBase (rtti-vtables.tsv). Shape: the ParentThing store and
// the two byte stores (`xor al,al`) precede the vtable store; `this` is returned. The chunk label
// "CEngineSubPrimitive2DClockSprite" is propagated.
class CThing;

class CBaseClassNonCopyable {                     // vtable 0x0129A7C4
public:
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();              // slot 0 (the scalar deleting destructor 0x0099A470)
};

class CTCBase : public CBaseClassNonCopyable {     // vtable 0x0125BE0C
public:
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing& parent_thing);
    virtual ~CTCBase();
};

CTCBase::CTCBase(CThing& parent_thing)
    : CBaseClassNonCopyable()
    , ParentThing(&parent_thing)
    , ToBeRemoved(false), IsInGlobalUpdateTCs(false)
{
}
