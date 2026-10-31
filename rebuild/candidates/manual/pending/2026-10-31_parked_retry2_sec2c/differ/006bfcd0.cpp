// Grade: DIFFER(764v764) 84 diff lines (parked 739v764/86): named ahead = Forward*3 then base = ahead + position (frame 0x68 v 0x6c, size now equal); left: one more 4-byte slot (retail C2DVector forward at +0x40), final candidate.x add order. Tried: single chained expression (741), named perpendicular (no change) (retry wave 2 sec2c)
// Was: DIFFER(739v764): frame 0x5c v 0x6c (retail keeps 16 more bytes of vector temporaries: ahead +0x28, the ahead+position temp +0x58 with only z spilled) and the final candidate.x add order (retail fxch st2; fadd st(2))
// CTCBuyableHouse::GetSignPostPos(bool main, C3DVector* pos, CRightHandedSet* orientation) -- where a sign post goes: on the existing sign post among the building's contents (by marker def name), else beside the building's sign-post marker (3 ahead, 2 or 4 to the side, ground height), trying the marker's facing and its quarter turn until the spot is outside the building
// Leaf: GetSignPostPos
// Retail 0x006BFCD0, 768 bytes, `ret 0Ch`. Callees: LowerBound 0x0040F020, CCharString 0x0099EBF0 / dtor 0x0099EAE0,
// CTCBase::GetDefinitions 0x006869D0, GetDefGlobalIndexFromName 0x009AD410, CIntelligentPointer::GetPItem
// 0x00A01B50, CThing::GetPosition 0x004C73D0, the placement object's virtual +0x120 (orientation), CRightHandedSet::
// RotateAboutAxis 0x00A22F80, CTCBase::GetWorldMap 0x00686D20, CWorldMap::GetGroundSizeZAt 0x0063A300,
// CThingBuilding::IsPointInside 0x0082D000. Constants 0.0f 0x0122DEDC, 3.0f 0x0122DED4.
// Layout: CThing interface bitset +0x20 (0x21 building contents, 0x26 sign-post marker), map +0x44, placement
// object +0x60, def index +0x8c, dead bit +0x91, thing type byte +0x10 (3 = building); building contents list head
// +0x0c (landed 0x006C0160); CRightHandedSet {Up, Forward} (Ego_r.pdb).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    void RotateAboutAxis(const C3DVector& axis, float angle);   // 0x00A22F80
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPlacement {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual const CRightHandedSet& GetOrientation();   // +0x120
};
struct CThing {
    unsigned char  _pad00[0x10];
    unsigned char  Type;                                // +0x10
    unsigned char  _pad11[0x20 - 0x11];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CPlacement*    Placement;                           // +0x60
    unsigned char  _pad64[0x8c - 0x64];
    unsigned short DefIndex;                            // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  StateFlags;                          // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& point);         // 0x0082D000
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct SThingListNode {
    SThingListNode*         next;
    SThingListNode*         prev;
    CBaseIntelligentPointer value;                      // +0x08
};
struct CTCBuildingContents {
    unsigned char   _pad00[0x0c];
    SThingListNode* ThingsHead;                         // +0x0c
};
struct CDefinitionManager { long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CWorldMap { float GetGroundSizeZAt(const C3DVector& pos) const; };              // 0x0063A300

struct CTCBuyableHouse {
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    CDefinitionManager* GetDefinitions() const;         // 0x006869D0
    CWorldMap* GetWorldMap() const;                     // 0x00686D20
    bool GetSignPostPos(bool main, C3DVector* pos, CRightHandedSet* orientation);
};

bool CTCBuyableHouse::GetSignPostPos(bool main, C3DVector* pos, CRightHandedSet* orientation)
{
    float side = main ? 2.0f : 4.0f;
    if (ParentThing->HasTC(0x21)) {
        CTCBuildingContents* contents = ParentThing->GetTC<CTCBuildingContents>(0x21);
        CThing* marker = 0;
        CThing* signPost = 0;
        long index;
        if (main)
            index = GetDefinitions()->GetDefGlobalIndexFromName("MARKER_BUYABLE_HOUSE_MAIN_SIGNPOST");
        else
            index = GetDefinitions()->GetDefGlobalIndexFromName("MARKER_BUYABLE_HOUSE_DRESSING_SIGNPOST");
        SThingListNode* end = contents->ThingsHead;
        for (SThingListNode* node = end->next; node != end; node = node->next) {
            CThing* thing = node->value.GetPItem();
            if (thing && !(thing->StateFlags & 1)) {
                if (thing->HasTC(0x26))
                    marker = thing;
                else if (thing->DefIndex == index)
                    signPost = thing;
            }
        }
        if (signPost) {
            *pos = signPost->GetPosition();
            if (orientation)
                *orientation = signPost->Placement->GetOrientation();
            return true;
        }
        if (marker) {
            for (long i = 0; i < 2; ++i) {
                CRightHandedSet set = marker->Placement->GetOrientation();
                if (i)
                    set.RotateAboutAxis(C3DVector(0.0f, 0.0f, 1.0f), 0.5f);
                C2DVector forward(set.Forward.X, set.Forward.Y);
                C3DVector ahead = set.Forward * 3.0f;
                C3DVector base = ahead + marker->GetPosition();
                C3DVector candidate = base + C3DVector(-forward.Y, forward.X, 0.0f) * side;
                candidate.Z = GetWorldMap()->GetGroundSizeZAt(candidate);
                if (orientation)
                    *orientation = set;
                *pos = candidate;
                if (ParentThing->Type == 3 && !((CThingBuilding*)ParentThing)->IsPointInside(*pos))
                    return true;
            }
        }
    }
    return false;
}
