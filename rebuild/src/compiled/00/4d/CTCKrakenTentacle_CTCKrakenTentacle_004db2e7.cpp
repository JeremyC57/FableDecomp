// CTCKrakenTentacle::CTCKrakenTentacle(CThing* thing) -- CTCBase(thing), own vtable, null def pointer and thing pointers, strike state cleared, empty DummyName, DoneDeathStuff false
// Leaf: ??0CTCKrakenTentacle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB2E7, length 102 bytes (`ret 4`). Vtable 0x012407BC = .?AVCTCKrakenTentacle@@
// (RTTI: CTCBase, CBaseClassNonCopyable, CBaseClass); only caller 0x004DCD04 (the component factory).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// CIntelligentPointer<const CThing>::CIntelligentPointer(const CThing*) @ 0x004A96F0 (out of line, called with 0),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x2, inline CIntelligentPointer<CThing> ctors storing vtable
// 0x01238C6C), CCharString::CCharString() @ 0x0099E4B0.
// Layout: PDB CTCKrakenTentacle (0x50) matches retail: PTentacleDef +0x0C, PKraken +0x10, PObstruction +0x18, PRippleEffect +0x20,
// CanStrike/Tracking/IsStriking +0x28..+0x2A, QueuedStrikeTick +0x2C, LastStrikeTick +0x30, TickToRise +0x34, DesiredPos +0x38 and
// FacingAngle +0x44 (no init), DummyName +0x48, DoneDeathStuff +0x4C.
// Size TU (odd start address): `#pragma optimize("s", on)` gives the `push [esp+14h]` argument and the vtable kept in ebp.
// Shape: all zero stores share ebx; the CDefPointer's inline ctor zero is scheduled into the PKraken argument push.
#pragma optimize("s", on)
struct CThing;
struct CCharString {
    void* m_pData;
    CCharString();                                  // 0x0099E4B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item);   // 0x004A96F0
};
struct CKrakenTentacleDef;
template <class T> struct CDefPointer {
    const T* Object;
    CDefPointer() : Object(0) { }
};
struct C3DVector { float X, Y, Z; };
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
};
struct CTCKrakenTentacle : CTCBase {                // vtable 0x012407BC
    CDefPointer<CKrakenTentacleDef> PTentacleDef;   // +0x0c
    CIntelligentPointerConstThing   PKraken;        // +0x10
    CIntelligentPointerThing        PObstruction;   // +0x18
    CIntelligentPointerThing        PRippleEffect;  // +0x20
    bool      CanStrike;                            // +0x28
    bool      Tracking;                             // +0x29
    bool      IsStriking;                           // +0x2a
    long      QueuedStrikeTick;                     // +0x2c
    long      LastStrikeTick;                       // +0x30
    long      TickToRise;                           // +0x34
    C3DVector DesiredPos;                           // +0x38
    float     FacingAngle;                          // +0x44
    CCharString DummyName;                          // +0x48
    bool      DoneDeathStuff;                       // +0x4c
    virtual void v00();
    CTCKrakenTentacle(CThing* thing);
};

CTCKrakenTentacle::CTCKrakenTentacle(CThing* thing)
    : CTCBase(thing)
    , PKraken(0)
    , CanStrike(false)
    , Tracking(false)
    , IsStriking(false)
    , QueuedStrikeTick(0)
    , LastStrikeTick(0)
    , TickToRise(0)
    , DoneDeathStuff(false)
{
}