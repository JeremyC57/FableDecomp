// CTCGold::CTCGold(CThing& thing) -- CTCBase(thing), then this class's vtable and a null Gold
// Leaf: ??0CTCGold@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CBDA0, 32 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x0126ED04 (RTTI .?AVCTCGold@@ : CTCBase); FableWin.pdb ??0CTCGold@@QAE@AAVCThing@@@Z; only caller CTCGold::Alloc 0x004D580D (landed); Ego_r.pdb Gold (long) at +0x0C.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family K017, representative 0x00702AA0): the base call, the vtable store, then the one zero store at 0x0C
//   (an init-list zero or an embedded CDefPointer's inline default ctor) -- agent952 H073's TC ctor shape in a speed TU.
//   CTCBase is modelled locally as the landed G056/H073 rows do (engine/CTCBase.h has no constructor).
class CThing;
class CTCBase {                                          // retail 0x0C bytes (Ego_r.pdb)
public:
    CTCBase(CThing& thing);                              // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCGold : public CTCBase {
public:
    CTCGold(CThing& thing);
public:
    long Gold;                                           // +0x0C
};
CTCGold::CTCGold(CThing& thing)
    : CTCBase(thing), Gold(0)
{
}