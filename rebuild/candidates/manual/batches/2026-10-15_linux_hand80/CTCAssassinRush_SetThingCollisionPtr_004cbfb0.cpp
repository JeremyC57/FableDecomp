// CTCAssassinRush::SetThingCollisionPtr(CThing* thing) -- unless the trail delay has started, CollidedThing is pointed at the thing (through a converting CIntelligentPointer temporary)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBFB0, length 78 bytes.
// Leaf: SetThingCollisionPtr
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50,
// ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv). Ego_r.pdb:
// `?SetThingCollisionPtr@CTCAssassinRush@@QAEXPAVCThing@@@Z`; StartTrailDelay +0x2c, PositionStartedFrom +0x2d (unaligned
// C3DVector), CollidedThing +0x39 (unaligned CIntelligentPointer<CThing>) -- the retail body uses the PDB offsets.
// Shape: `CollidedThing = thing;` -- the converting constructor builds the 8-byte frame temporary (base constructor, its
// vtable, SetPItem(thing)), the inline operator= copies it (`SetPItem(other.GetPItem())`), and the temporary's destructor
// is the base destructor (CIntelligentPointer declares none). `#pragma pack(1)` reproduces the unaligned members.
struct CThing;
struct CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseIntelligentPointer();                 // 0x00A01B10
    ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void SetPItem(CBaseObject* item);          // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)thing); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector { float X, Y, Z; };

struct CTCAssassinRush {                       // CTCBase is flattened here (engine/CTCBase.h layout)
    void*               __vftable;             // +0x00
    CThing*             ParentThing;           // +0x04
    bool                ToBeRemoved;           // +0x08
    bool                IsInGlobalUpdateTCs;   // +0x09
    unsigned char       _pad0a[2];
    unsigned char       PTarget[8];            // +0x0c CIntelligentPointer<CThing>
    void*               PDef;                  // +0x14 CDefPointer<CSpecialAbilitiesAssassinRushDef const>
    unsigned char       AssassinTrail[8];      // +0x18 CIntelligentPointer<CThing>
    unsigned char       AssassinDisplacementTrail[8];   // +0x20 CIntelligentPointer<CThing>
    long                TrailDelay;            // +0x28
    bool                StartTrailDelay;       // +0x2c
    C3DVector           PositionStartedFrom;   // +0x2d
    CIntelligentPointer CollidedThing;         // +0x39
    void SetThingCollisionPtr(CThing* thing);
};
#pragma pack(pop)

void CTCAssassinRush::SetThingCollisionPtr(CThing* thing)
{
    if (!StartTrailDelay)
        CollidedThing = thing;
}
