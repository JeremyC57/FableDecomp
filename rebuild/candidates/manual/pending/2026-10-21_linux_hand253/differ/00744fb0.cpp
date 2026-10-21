// Grade: DIFFER(355v355) -- frame layout only: retail passes the empty comparator at the Bind2's own address (frame 0x24, lookup key and radius at frame +4); ours gives the comparator its own byte (frame 0x28), shifting the key/radius slots
// CTCPerceiveThing::UpdateHearing() -- (skipped while the owner's village is in a quiet period) look for the most recent noise made by the perceived thing that the owner can hear within its sound radius; hearing it either refreshes direct awareness (when already directly aware with time left) or records where the noise came from and restarts the awareness timer; then update the awareness state
// Leaf: UpdateHearing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744FB0, 355 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (village member 0x23); CTCVillageMember::GetPVillage
//          0x007EE540 (manifest "IS_THING_ALIVE<...>" is propagated); CTCBase::GetWorld 0x00686A70; CWorld::GetWorldFrame
//          0x0049D870; CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); CThing::GetPosition 0x004C73D0 (x2);
//          CTCBase::GetThingSearchTools 0x00686D80; CThingSearchTools::GetPBestLeafThing<CFilter_Bind2<
//          CThingFilter_NoiseCameFromThing, CThingFilter_NoiseCanBeHeardFromPosition>, CThingCompare_GetMostRecentlyCreatedThing,
//          CThingNoise> 0x007461C0; CTCPerceiveThing::SetLastAwareInfo 0x00744730; CTCPerceiveThing::UpdateAwareness 0x00744D00
//          (name descriptive; manifest label is propagated).
// Layout: rebuild/include/engine/CTCPerceiveThing.h (retail offsets): ParentThing +0x04, PerceivedThing +0x0c, AwarenessTimer
//          +0x18, FramesSinceDirectlyPerceivedThing +0x1c, LastPerceivedPos +0x20, DirectAwareness +0x30, Heard +0x32, PDef
//          +0x34, SoundRadius +0x3c; the perceived-thing def's hearing awareness time +0x44 (descriptive); retail CThing
//          interface bitset +0x20 (0xb4 = word +0x34 bit 20), map +0x44, dead flag +0x91 bit 0; CTCVillage quiet-until frame
//          +0xa8 (descriptive).
// Shape: the two filters, the Bind2 and the empty comparator are temporaries (the comparator's address is the Bind2's); the
//          sound radius is read into a named local first.
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingNoise : CThing { };
struct CTCVillage {
    unsigned char _pad00[0xa8];
    long QuietUntilFrame;                       // +0xa8 (descriptive)
};
struct CTCVillageMember {
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CThingFilter_NoiseCameFromThing {
    const CThing* Thing;
    CThingFilter_NoiseCameFromThing(const CThing* thing) : Thing(thing) { }
};
struct CThingFilter_NoiseCanBeHeardFromPosition {
    C3DVector Pos;
    float Radius;
    CThingFilter_NoiseCanBeHeardFromPosition(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
};
template <class A, class B> struct CFilter_Bind2 {
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& a, const B& b) : F1(&a), F2(&b) { }
};
struct CThingCompare_GetMostRecentlyCreatedThing {
    CThingCompare_GetMostRecentlyCreatedThing() { }
};
struct CThingSearchTools {
    bool GetPBestLeafThing(const CFilter_Bind2<CThingFilter_NoiseCameFromThing, CThingFilter_NoiseCanBeHeardFromPosition>& filter,
                           const CThingCompare_GetMostRecentlyCreatedThing& compare, CThingNoise** out);   // 0x007461C0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CPerceivedThingDef {
    unsigned char _pad00[0x44];
    float HearingAwarenessTime;                 // +0x44 (descriptive)
};
struct CTCPerceiveThing {
    void*            __vftable;                 // +0x00
    CThing*          ParentThing;               // +0x04 (CTCBase)
    unsigned char    _pad_0x08[0x4];
    CBaseIntelligentPointer PerceivedThing;     // +0x0c CIntelligentPointer<CThing const>
    float            DistanceFromPerceivedThingSquared;   // +0x14
    float            AwarenessTimer;            // +0x18
    long             FramesSinceDirectlyPerceivedThing;   // +0x1c
    C3DVector        LastPerceivedPos;          // +0x20
    bool             Perceiving;                // +0x2c
    bool             LineOfSight;               // +0x2d
    bool             ExtendedLineOfSight;       // +0x2e
    bool             IgnoreFOVLineOfSight;      // +0x2f
    bool             DirectAwareness;           // +0x30
    bool             Smelt;                     // +0x31
    bool             Heard;                     // +0x32
    bool             SearchingForThingToPerceive;   // +0x33
    CPerceivedThingDef* PDef;                   // +0x34 CDefPointer<CPerceivedThingDef const>
    long             UpdateOffset;              // +0x38
    float            SoundRadius;               // +0x3c
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    CThingSearchTools* GetThingSearchTools();   // 0x00686D80 (CTCBase)
    void SetLastAwareInfo();                    // 0x00744730
    void UpdateAwareness();                     // 0x00744D00
    void UpdateHearing();
};

void CTCPerceiveThing::UpdateHearing()
{
    Heard = false;
    CThing* owner = ParentThing;
    if (owner->HasTC(0xb4) && owner->HasTC(0x23)) {
        CTCVillage* village = ((CTCVillageMember*)owner->GetTC(0x23))->GetPVillage();
        if (village) {
            CWorld* world = GetWorld();
            long quietUntil = village->QuietUntilFrame;
            if (world->GetWorldFrame() < quietUntil)
                return;
        }
    }
    CThing* perceived = PerceivedThing.GetPItem();
    CThingNoise* noise = 0;
    float radius = SoundRadius;
    CThingFilter_NoiseCanBeHeardFromPosition canHear(ParentThing->GetPosition(), radius);
    CThingFilter_NoiseCameFromThing cameFrom(perceived);
    CFilter_Bind2<CThingFilter_NoiseCameFromThing, CThingFilter_NoiseCanBeHeardFromPosition> filter(cameFrom, canHear);
    GetThingSearchTools()->GetPBestLeafThing(filter, CThingCompare_GetMostRecentlyCreatedThing(), &noise);
    if (noise && !(noise->m_flags91 & 1)) {
        Heard = true;
        if (AwarenessTimer != 0.0f && DirectAwareness) {
            if (PerceivedThing.GetPItem()) {
                DirectAwareness = true;
                FramesSinceDirectlyPerceivedThing = 0;
                SetLastAwareInfo();
            }
        } else {
            LastPerceivedPos = noise->GetPosition();
            AwarenessTimer = PDef->HearingAwarenessTime;
        }
    }
    UpdateAwareness();
}
