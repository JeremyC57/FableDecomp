// CTCExpression::CTCExpression(CThing& thing) -- CTCBase(thing), then this class's vtable and a null PDef
// Leaf: ??0CTCExpression@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB050, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012401F4.
// Identity (proven): CTCExpression::CTCExpression(CThing&); evidence: it stores vtable 0x012401F4 whose RTTI is .?AVCTCExpression@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb holds ??0CTCExpression@@QAE@AAVCThing@@@Z; its only caller is the TC type factory 0x004DC78F (`new CTCExpression(thing)`).
// Label: the manifest's "CTCCameraCollision" is propagated (family J079: five byte-identical TC constructors; retail has no
//   identical-code folding).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (agent951 G056's spelling).
// Layout: Ego_r.pdb CTCExpression::PDef (CDefPointer<const CExpressionDef>) at +0x0C.
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
class CExpressionDef;
class CTCExpression : public CTCBase {
public:
    CTCExpression(CThing& thing);
    CDefPointer<const CExpressionDef> PDef;                    // +0x0c
};
CTCExpression::CTCExpression(CThing& thing)
    : CTCBase(thing)
{
}