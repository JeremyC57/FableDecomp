// CTCDegradableBase::CTCDegradableBase(CThing& thing) -- CTCBase(thing), then this class's vtable and a null PDefinition
// Leaf: ??0CTCDegradableBase@@IAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EC850, 32 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x01287F2C (RTTI .?AVCTCDegradableBase@@ : CTCBase); FableWin.pdb ??0CTCDegradableBase@@IAE@AAVCThing@@@Z (protected); caller the CTCDegradableBarrier ctor 0x007710D0 (landed); Ego_r.pdb PDefinition (CDefPointer<const CDegradableDef>) at +0x10.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800.
// Shape (family K017, representative 0x00702AA0): the base call, the vtable store, then the one zero store at 0x10
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
class CDegradableDef;
class CDegradableInfo;
class CThingPhysical;
class CDefPointeeBase;
template <class T> class CDefPointer {                    // Ego_r.pdb: CDefPointeeBase* Object
public:
    CDefPointeeBase* Object;                             // +0x00
    CDefPointer() : Object(0) { }
};
class CTCDegradableBase : public CTCBase {
protected:
    CTCDegradableBase(CThing& thing);
public:
    long LastWorldFrameHit;                              // +0x0C (not initialised here)
    CDefPointer<const CDegradableDef> PDefinition;       // +0x10
    CDegradableInfo* PCurrentDegradableInfo;             // +0x14
    CThingPhysical* PParentThingPhysical;                // +0x18
};
CTCDegradableBase::CTCDegradableBase(CThing& thing)
    : CTCBase(thing)
{
}