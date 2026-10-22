// CCreatureAction_JackScream::PerformAction() -- find the "SwordDropMarker" thing within 20 of the Jack; the sword's drop target is that marker's position (or, without a live marker, the ground below the point 2 units back along -X/-Y) raised by 1.4; then take the sword out of the primary slot, place it at the creature's WEAPON_FOCUS_01 dummy with that orientation and hand the orientation and target to the scream mode
// Leaf: ?PerformAction@CCreatureAction_JackScream
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C2240, length 587 bytes, plain `ret` (virtual).
// Callees: CCharString @ 0x0099EBF0 / copy @ 0x0099EC30 / ~ @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CCreatureActionBase::GetThingSearchTools @ 0x00692990,
//          CThingSearchTools::GetPFirstThingInRoughArea<HasTrackName> @ 0x007FD110, CCreatureActionBase::GetWorldMap
//          @ 0x00692C10, CWorldMap::GetHeightZBelowPos @ 0x0063E480, CVectorMap::LowerBound @ 0x0040F020,
//          CTCCarrying::GetThingInPrimarySlot @ 0x006A1440 / RemoveThingFromPrimarySlot(bool) @ 0x006A1E00,
//          CTCGraphicAppearance::GetDummyObject @ 0x004C08C0, the sword's physics slots 30 (+0x78) / 38 (+0x98) / 75 (+0x12c).
// Layout: retail CCombatActionBase is 0x114 bytes; this class's PMode (CCountedPointer<CModeJackOfBladesScream>) object
//   pointer at +0x114 (the landed Initialise 0x008C0D10). The mode's fields +0x7c (orientation), +0x94 (flag) and +0xa4
//   (target) are named descriptively. Model of the area search: the landed CTCKraken::GetCentreMarker 0x007FBC90.
// Shape: the track-name filter is a named local built before the position (the box is an argument temporary); the
//   marker and the target computation sit in a block of their own (so the 0x46 lookup key and the dummy name reuse the
//   marker's frame word); the marker branch loads target.Z for the shared `fadd 1.4`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    CCharString(const CCharString& other);                   // 0x0099EC30
    ~CCharString();                                          // 0x0099EAE0
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2)
    virtual void SetPosition(const C3DVector* position);     // slot 30 (+0x78)
    VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37)
    virtual void SetFixed(bool fixed);                       // slot 38 (+0x98)
    VF(39) VF10(4) VF10(5) VF10(6) VF(70) VF(71) VF(72) VF(73) VF(74)
    virtual void SetOrientation(const CRightHandedSet* orientation);   // slot 75 (+0x12c)
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CPhysicsBody*  PhysicsBody;                              // +0x60
    CTCGraphicAppearance* GraphicAppearance;                 // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  ThingFlags;                               // +0x91 (bit 0: dead/removed)
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    {
        return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingSearchTools {
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, CThing** out) const;   // 0x007FD110
};
struct CWorldMap {
    float GetHeightZBelowPos(const C3DVector& position);     // 0x0063E480
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                         // 0x006A1440
    void RemoveThingFromPrimarySlot(bool drop);              // 0x006A1E00
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CModeJackOfBladesScream {
    unsigned char   _pad00[0x7c];
    CRightHandedSet SwordOrientation;                        // +0x7c
    bool            SwordDropping;                           // +0x94
    unsigned char   _pad95[0xa4 - 0x95];
    C3DVector       SwordTarget;                             // +0xa4
};
struct CCombatActionBase {                                   // retail sizeof 0x114
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CThingSearchTools* GetThingSearchTools() const;          // 0x00692990
    CWorldMap* GetWorldMap() const;                          // 0x00692C10
};
struct CCreatureAction_JackScream : CCombatActionBase {
    CModeJackOfBladesScream* PMode;                          // +0x114 CCountedPointer object
    void*                    PModeCount;                     // +0x118
    virtual void PerformAction();
};

void CCreatureAction_JackScream::PerformAction()
{
    C3DVector target;
    {
        CThing* marker = 0;
        {
            CCharString name("SwordDropMarker");
            CThingFilter_HasTrackName filter(name);
            const C3DVector& pos = GetCreature()->GetPosition();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            GetThingSearchTools()->GetPFirstThingInRoughArea(C2DBoxI(x - 20, y - 20, x + 20, y + 20), filter, &marker);
        }
        if (marker && !(marker->ThingFlags & 1))
            target = marker->GetPosition();
        else {
            const C3DVector& pos = GetCreature()->GetPosition();
            target = C3DVector(pos.X - 2.0f, pos.Y - 2.0f, pos.Z);
            target.Z = GetWorldMap()->GetHeightZBelowPos(target);
        }
    }
    target.Z += 1.4f;
    CTCCarrying* carrying;
    GetCreature()->GetInterface(&carrying, 0x46);
    CThing* sword = carrying->GetThingInPrimarySlot();
    sword->PhysicsBody->SetFixed(false);
    carrying->RemoveThingFromPrimarySlot(false);
    C3DVector position;
    CRightHandedSet orientation;
    GetCreature()->GetDummyObject("WEAPON_FOCUS_01", &position, &orientation);
    sword->PhysicsBody->SetPosition(&position);
    sword->PhysicsBody->SetOrientation(&orientation);
    PMode->SwordDropping = true;
    PMode->SwordOrientation = orientation;
    PMode->SwordTarget = target;
}
