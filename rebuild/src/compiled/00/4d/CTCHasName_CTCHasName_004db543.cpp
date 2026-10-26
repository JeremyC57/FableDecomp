// CTCHasName::CTCHasName(CThing& thing) -- CTCBase(thing), then this class's vtable and a null PDef
// Leaf: ??0CTCHasName@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB543, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x01240AA4.
// Identity (proven): CTCHasName::CTCHasName(CThing&); evidence: it stores vtable 0x01240AA4 whose RTTI is .?AVCTCHasName@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb holds ??0CTCHasName@@QAE@AAVCThing@@@Z; its only caller is the TC type factory 0x004DCF15 (`new CTCHasName(thing)`).
// Label: the manifest's "CTCCameraCollision" is propagated (family J079: five byte-identical TC constructors; retail has no
//   identical-code folding).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (agent951 G056's spelling).
// Layout: Ego_r.pdb CTCHasName::PDef (CDefPointer<const CHasNameDef>) at +0x0C.
// Shape: agent951 G056 (0x004D369B) with a CDefPointer member: its inline default constructor nulls Object (`and dword ptr [esi+0Ch],0`
//   under /Os).
#pragma optimize("s", on)
class CThing;
class CTCBase {                                            // retail 0x0C bytes
public:
    CTCBase(CThing& thing);                                // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CDefPointeeBase;
template <class T> class CDefPointer {                     // Ego_r.pdb 4 bytes
public:
    CDefPointer() : Object(0) { }
    CDefPointeeBase* Object;                               // +0x00
};
class CHasNameDef;
class CTCHasName : public CTCBase {
public:
    CTCHasName(CThing& thing);
    CDefPointer<const CHasNameDef> PDef;                    // +0x0c
};
CTCHasName::CTCHasName(CThing& thing)
    : CTCBase(thing)
{
}