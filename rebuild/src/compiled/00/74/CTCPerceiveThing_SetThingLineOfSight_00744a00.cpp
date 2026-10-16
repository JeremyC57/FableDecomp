// CTCPerceiveThing::SetThingLineOfSight(bool lineOfSight) -- on a line-of-sight change: drop the parent from the GUI perceiver drawing if it had sight, store the flag, re-add or remove it by whether a target exists; while in sight reset FramesSinceDirectlyPerceivedThing
// Leaf: SetThingLineOfSight
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744A00, 102 bytes. Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inline UpdateDrawPerceivers).
// Shape: the trailing test re-reads LineOfSight from memory (the GUI calls may alias it).
// Model (local; engine/CTCPerceiveThing.h flattens the CIntelligentPointer member to a byte array): PerceivedThing +0x0c
//   (CIntelligentPointer<CThing const>), FramesSinceDirectlyPerceivedThing +0x1c, the flag bytes +0x2c..+0x33.
//   g_pPlayerGui @ 0x013B8790: CPlayerGui::AddPerceivingThing @ 0x004378F0 / RemovePerceivingThing @ 0x00437900 are
//   `mov ecx,[ecx+1B8h] ; jmp` forwarders to CDrawPerceivers (0x0064A2DB AddPerceivingThing / 0x0064639C); the catalog's
//   "_Cons_val<...>" labels on them are propagated.
// The add-or-remove tail is the inline UpdateDrawPerceivers(): the perceiving thing is drawn while its target exists.
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing const>, vtable 0x01237F1C
    CIntelligentPointer(const CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CPlayerGui {                             // the global g_pPlayerGui @ 0x013B8790
    void AddPerceivingThing(CThing* thing);     // 0x004378F0 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
    void RemovePerceivingThing(CThing* thing);  // 0x00437900 (forwards to CDrawPerceivers @ +0x1b8; catalog "_Cons_val<...>")
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
struct CDefPointeeBase;
struct C3DVector { float X, Y, Z; };
struct CTCPerceiveThing {
    void*     __vftable;                        // +0x00 (CTCBase)
    CThing*   ParentThing;                      // +0x04
    bool      ToBeRemoved;                      // +0x08
    bool      IsInGlobalUpdateTCs;              // +0x09
    CIntelligentPointer PerceivedThing;         // +0x0c
    float     DistanceFromPerceivedThingSquared;// +0x14
    float     AwarenessTimer;                   // +0x18
    long      FramesSinceDirectlyPerceivedThing;// +0x1c
    C3DVector LastPerceivedPos;                 // +0x20
    bool      Perceiving;                       // +0x2c
    bool      LineOfSight;                      // +0x2d
    bool      ExtendedLineOfSight;              // +0x2e
    bool      IgnoreFOVLineOfSight;             // +0x2f
    bool      DirectAwareness;                  // +0x30
    bool      Smelt;                            // +0x31
    bool      Heard;                            // +0x32
    bool      SearchingForThingToPerceive;      // +0x33
    CDefPointeeBase* PDef;                      // +0x34 CDefPointer<CPerceivedThingDef const>::Object
    long      UpdateOffset;                     // +0x38
    float     SoundRadius;                      // +0x3c
    float     SightRadius;                      // +0x40
    float     ExtendedSightRadius;              // +0x44
    float     SmellRadius;                      // +0x48
    float     GiveUpChaseRadius;                // +0x4c
    bool      PerceptionCombat;                 // +0x50
    long      FrameHitByPerceivedThing;         // +0x54
    void UpdateDrawPerceivers()
    {
        if (PerceivedThing.GetPItem())
            g_pPlayerGui->AddPerceivingThing(ParentThing);
        else
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
    }
    void SetPerceivedThing(const CThing* thing);
    void SetThingLineOfSight(bool lineOfSight);
    void SetThingExtendedLineOfSight(bool lineOfSight);
};

void CTCPerceiveThing::SetThingLineOfSight(bool lineOfSight)
{
    if (LineOfSight != lineOfSight) {
        if (LineOfSight)
            g_pPlayerGui->RemovePerceivingThing(ParentThing);
        LineOfSight = lineOfSight;
        if (lineOfSight)
            UpdateDrawPerceivers();
    }
    if (LineOfSight)
        FramesSinceDirectlyPerceivedThing = 0;
}