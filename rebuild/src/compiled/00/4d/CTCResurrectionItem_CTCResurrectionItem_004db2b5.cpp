// CTCResurrectionItem::CTCResurrectionItem(CThing& thing) -- CTCBase(thing), then this class's vtable and a null PDef
// Leaf: ??0CTCResurrectionItem@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB2B5, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x01240744.
// Identity (proven): CTCResurrectionItem::CTCResurrectionItem(CThing&); evidence: it stores vtable 0x01240744 whose RTTI is .?AVCTCResurrectionItem@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb holds ??0CTCResurrectionItem@@QAE@AAVCThing@@@Z; its only caller is the TC type factory 0x004DCC99 (`new CTCResurrectionItem(thing)`).
// Label: the manifest's "CTCCameraCollision" is propagated (family J079: five byte-identical TC constructors; retail has no
//   identical-code folding).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (agent951 G056's spelling).
// Layout: Ego_r.pdb CTCResurrectionItem::PDef (CDefPointer<const CResurrectionItemDef>) at +0x0C.
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
class CResurrectionItemDef;
class CTCResurrectionItem : public CTCBase {
public:
    CTCResurrectionItem(CThing& thing);
    CDefPointer<const CResurrectionItemDef> PDef;                    // +0x0c
};
CTCResurrectionItem::CTCResurrectionItem(CThing& thing)
    : CTCBase(thing)
{
}