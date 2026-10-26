// CTCDRumble::CTCDRumble(CThing& thing) -- CTCDriverBase(thing), then this class's vtable and a null PDef
// Leaf: ??0CTCDRumble@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB399, 28 bytes (size-optimised TU, odd entry), thiscall, `ret 4`. Vtable 0x012408BC.
// Identity (proven): CTCDRumble::CTCDRumble(CThing&); evidence: it stores vtable 0x012408BC whose RTTI is .?AVCTCDRumble@@ : CTCDriverBase : CTCBase : CBaseClassNonCopyable : CBaseClass;
//   FableWin.pdb holds ??0CTCDRumble@@QAE@AAVCThing@@@Z; its only caller is the TC type factory 0x004DCDC4 (`new CTCDRumble(thing)`).
// Label: the manifest's "CTCCameraCollision" is propagated (family J079: five byte-identical TC constructors; retail has no
//   identical-code folding).
// Callees: CTCDriverBase::CTCDriverBase(CThing&) 0x00771200 (FableWin.pdb spelling).
// Layout: Ego_r.pdb CTCDRumble::PDef (CDefPointer<const CRumbleDef>) at +0x0C; the other members (Strength, Length, GlobalRumble, LifeTimer) are left uninitialised by the constructor, as retail shows.
// Shape: agent951 G056 (0x004D369B) with a CDefPointer member: its inline default constructor nulls Object (`and dword ptr [esi+0Ch],0`
//   under /Os); the base is CTCDriverBase, whose constructor 0x00771200 is out of line.
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
class CTCDriverBase : public CTCBase {                   // Ego_r.pdb 0x0C bytes, no members of its own
public:
    CTCDriverBase(CThing& thing);                          // 0x00771200 (FableWin.pdb ??0CTCDriverBase@@QAE@AAVCThing@@@Z)
};
class CDefPointeeBase;
template <class T> class CDefPointer {                     // Ego_r.pdb 4 bytes
public:
    CDefPointer() : Object(0) { }
    CDefPointeeBase* Object;                               // +0x00
};
class CRumbleDef;
class CTCDRumble : public CTCDriverBase {
public:
    CTCDRumble(CThing& thing);
    CDefPointer<const CRumbleDef> PDef;                    // +0x0c
    float Strength;                                    // +0x10
    float Length;                                      // +0x14
    bool GlobalRumble;                                 // +0x18
    long LifeTimer;                                    // +0x1C
};
CTCDRumble::CTCDRumble(CThing& thing)
    : CTCDriverBase(thing)
{
}