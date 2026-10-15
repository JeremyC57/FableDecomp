// CThingDeadCreature::CThingDeadCreature() -- construct the physical thing as type 13, the game-object layer (its standing-on intelligent pointer) and the dead creature's own members (null animation filter, null killer, no body reorienter, decay not started)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835C20, length 94 bytes (+ int3 padding).
// Callees: CThingPhysical::CThingPhysical(long type) @ 0x005296B0 (out of line, called with 13; manifest label
// "CTCDPhysicalObstruction" is propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice).
// Vtables: CThingGameObject 0x01244F7C, CIntelligentPointer<CThing> 0x01238C6C, CThingDeadCreature 0x01272B04,
// CIntelligentPointer<const CThingCreatureBase> 0x0125AAEC (rtti-vtables.tsv).
// Layout: Ego_r.pdb CThingGameObject (0xe8) puts PThingStandingOn at +0xd8 and its bitfield byte at +0xe0, CThingDeadCreature
// (0x108) PAnimFilter +0xe8, PKiller +0xf0, BodyDieTimer +0xf8, BodyReorienter +0xfc, StartedDecay +0x100; retail is
// 0x18 bytes narrower ahead of them (PThingStandingOn +0xc0, the bitfield byte +0xc8, then +0xd0 / +0xd8 / +0xe0 / +0xe4 /
// +0xe8), so a local model is used (engine/CThingGameObject.h keeps only the PDB-named members and disagrees).
// Shape: the CThingGameObject constructor is inline (its vtable store right after the base call, then its member
// pointer: base constructor + vtable store); the derived vtable store follows, then the init list in declaration
// order: the counted pointer's two zero words (scheduled before the killer's constructor call), the killer pointer, the
// scoped_ptr (a member object with an inline constructor: its zero store lands before `pop edi`, where a plain pointer
// initialiser sinks below it) and the bool; BodyDieTimer is left uninitialised.
// Leaf: ??0CThingDeadCreature
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

struct CThingPointer : CBaseIntelligentPointer {            // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer() { }
};

struct CCreaturePointer : CBaseIntelligentPointer {         // CIntelligentPointer<const CThingCreatureBase>, vtable 0x0125AAEC
    virtual void v00();
    CCreaturePointer() { }
};

struct CAnimFilterBase;
struct CCPPointerInfo;
struct CAnimFilterPointer {                     // CCountedPointer<CAnimFilterBase>
    CAnimFilterBase* Data;
    CCPPointerInfo*  Info;
    CAnimFilterPointer() : Data(0), Info(0) { }
};

struct CThingBodyReorienter;
struct CReorienterPointer {                     // scoped_ptr<CThingBodyReorienter>
    CThingBodyReorienter* ptr;
    explicit CReorienterPointer(CThingBodyReorienter* p = 0) : ptr(p) { }
};

struct CThingPhysical {
    virtual void v00();
    unsigned char _pad04[0xbc];
    CThingPhysical(long type);                  // 0x005296B0
};

struct CThingGameObject : CThingPhysical {      // vtable 0x01244F7C
    CThingPointer PThingStandingOn;             // +0xc0
    unsigned char m_gameObjectFlags;            // +0xc8 (bitfield group, not initialised here)
    unsigned char _padc9[7];                    // +0xc9 tail padding: the PDB size 0xe8 is the 0xe1 data rounded to 8
    virtual void v00();
    CThingGameObject(long type) : CThingPhysical(type) { }
};

struct CThingDeadCreature : CThingGameObject {  // vtable 0x01272B04
    CAnimFilterPointer    PAnimFilter;          // +0xd0
    CCreaturePointer      PKiller;              // +0xd8
    long                  BodyDieTimer;         // +0xe0
    CReorienterPointer    BodyReorienter;       // +0xe4 scoped_ptr<CThingBodyReorienter>
    bool                  StartedDecay;         // +0xe8
    virtual void v00();
    CThingDeadCreature();
};

CThingDeadCreature::CThingDeadCreature()
    : CThingGameObject(13)
    , StartedDecay(false)
{
}