// CTCInterestingToVillagers::CTCInterestingToVillagers(CThing& thing) -- CTCBase(thing), then this class's vtable and a null PDef
// Leaf: ??0CTCInterestingToVillagers@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB0CA, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012402F4.
// Identity (proven): CTCInterestingToVillagers::CTCInterestingToVillagers(CThing&); evidence: it stores vtable 0x012402F4 whose RTTI is .?AVCTCInterestingToVillagers@@ : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb holds ??0CTCInterestingToVillagers@@QAE@AAVCThing@@@Z; its only caller is the TC type factory 0x004DC821 (`new CTCInterestingToVillagers(thing)`).
// Label: the manifest's "CTCCameraCollision" is propagated (family J079: five byte-identical TC constructors; retail has no
//   identical-code folding).
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (agent951 G056's spelling).
// Layout: Ego_r.pdb CTCInterestingToVillagers::PDef (CDefPointer<const CInterestingToVillagersDef>) at +0x0C.
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
class CInterestingToVillagersDef;
class CTCInterestingToVillagers : public CTCBase {
public:
    CTCInterestingToVillagers(CThing& thing);
    CDefPointer<const CInterestingToVillagersDef> PDef;                    // +0x0c
};
CTCInterestingToVillagers::CTCInterestingToVillagers(CThing& thing)
    : CTCBase(thing)
{
}