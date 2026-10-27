// CTCSmashableBase::CTCSmashableBase(CThing& thing) -- CTCBase(thing), then this class's vtable and a null Definition
// Leaf: ??0CTCSmashableBase@@IAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EEBE0, 32 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x0127185C (RTTI .?AVCTCSmashableBase@@ : CTCBase); FableWin.pdb ??0CTCSmashableBase@@IAE@AAVCThing@@@Z (protected); callers the CTCSmashableBarrier 0x004DB024 / CTCSmashableWindow 0x004DC6BA ctors (landed); Ego_r.pdb Definition (CDefPointer<const CSmashableDef>) at +0x14.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family K017, representative 0x00702AA0): the base call, the vtable store, then the one zero store at 0x14
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
class CSmashableDef;
class CDefPointeeBase;
template <class T> class CDefPointer {                    // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    CDefPointer() : Object(0) { }
};
class CTCSmashableBase : public CTCBase {
protected:
    CTCSmashableBase(CThing& thing);
public:
    long LastWorldFrameHit;                              // +0x0C (not initialised here)
    bool Smashed;                                        // +0x10
    CDefPointer<const CSmashableDef> Definition;         // +0x14
};
CTCSmashableBase::CTCSmashableBase(CThing& thing)
    : CTCBase(thing)
{
}