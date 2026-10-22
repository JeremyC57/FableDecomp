// CAIStateGroup_KidPlayHopscotch::IsRunnable() -- update the fatigue flag from the activation rate (tired above 6.5 per minute, rested below 6), then, if not fatigued, find the nearest free hopscotch board, take a free action point on it, cache start/face positions and the forward/right axes, and keep the board
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941900, length 529 bytes (+ int3 padding), virtual.
// Callees: CWorld::GetWorldFrame @ 0x0049D870 (static, twice), CThingCreatureBase::GetPosition @ 0x006A4D00,
// CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestLeafThing<CIsFreeHopscotchBoard,
// CThingCompare_Nearest, CThingObject> @ 0x00941C40, CVectorMap::LowerBound @ 0x0040F020 (inlined lookup 0x2d =
// CTCUserActionPointBase), CTCUserActionPointBase::GetFreeActionPoint @ 0x007FEDE0, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> 0x01238C6C).
// Globals: 6.0f @ 0x0129273C, 6.5f @ 0x01292738, 10.0f @ 0x01237F0C.
// Shape: the sibling 0x00941210 (KidWatchHopscotch) with the rate computed once ahead of the hysteresis; the action
// point component is an uninitialised result (the miss reads the board's out-pointer word).
// Layout: Ego_r.pdb CAIStateGroup_KidPlayHopscotch (PHopscotchBoard +0x15 unaligned, HopscotchStartIndex +0x20,
// StartPos +0x24, FacePos +0x30, HopscotchForwardVec +0x3c, HopscotchRightVec +0x48, CreationFrame +0x54,
// FramesPerMin +0x58, Fatigued +0x5c, ActivationCount +0x60, ActivationFrequency +0x64).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
inline C3DVector operator^(const C3DVector& a, const C3DVector& b) { return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    C3DVector GetRight() const { return Forward ^ Up; }
};
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingObject : CThing { };

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    template<class U> CIntelligentPointer(U* const& thing) : CBaseIntelligentPointer() { SetPItem(thing); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* position, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
};

struct CIsFreeHopscotchBoard { CIsFreeHopscotchBoard() { } };
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
struct CThingSearchTools {
    // GetPBestLeafThing<CIsFreeHopscotchBoard, CThingCompare_Nearest, CThingObject> @ 0x00941C40 (this TU)
    bool GetPBestLeafThing(const CIsFreeHopscotchBoard& filter, const CThingCompare_Nearest& compare, CThingObject** ppBest) const;
};
#pragma auto_inline(off)
CThingObject* SearchSink(const void* tools, const void* compare);   // stand-in: the search's own work (outside the graded bytes)
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CIsFreeHopscotchBoard&, const CThingCompare_Nearest& compare,
                                                              CThingObject** ppBest) const
{
    *ppBest = SearchSink(this, &compare);
    return *ppBest != 0;
}
#pragma auto_inline(on)
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
};

struct CAIBrain {
    virtual void v00();
    unsigned char _pad[0x14];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
};

#pragma pack(push, 1)
struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase {
    bool TookActionPoint;                       // +0x14
    CIntelligentPointer PHopscotchBoard;        // +0x15
    unsigned char _pad1d[3];
    long HopscotchStartIndex;                   // +0x20
    C3DVector StartPos;                         // +0x24
    C3DVector FacePos;                          // +0x30
    C3DVector HopscotchForwardVec;              // +0x3c
    C3DVector HopscotchRightVec;                // +0x48
    long CreationFrame;                         // +0x54
    long FramesPerMin;                          // +0x58
    bool Fatigued;                              // +0x5c
    unsigned char _pad5d[3];
    long ActivationCount;                       // +0x60
    float ActivationFrequency;                  // +0x64
    virtual bool IsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_KidPlayHopscotch::IsRunnable()
{
    if (CWorld::GetWorldFrame() > CreationFrame) {
        ActivationFrequency = (float)ActivationCount * FramesPerMin / (float)(CWorld::GetWorldFrame() - CreationFrame);
        if (Fatigued) {
            if (ActivationFrequency < 6.0f)
                Fatigued = false;
        } else if (ActivationFrequency > 6.5f)
            Fatigued = true;
    }
    if (!Fatigued) {
        CThingObject* board;
        C3DVector pos = PBrain->PCreature->GetPosition();
        if (PeekThingSearchTools().GetPBestLeafThing(CIsFreeHopscotchBoard(), CThingCompare_Nearest(pos), &board)) {
            CTCUserActionPointBase* actionPoints;
            if (board->HasTC(0x2d))
                actionPoints = board->GetTC<CTCUserActionPointBase>(0x2d);
            CRightHandedSet rhSet;
            if (actionPoints->GetFreeActionPoint(GetCreature(), &StartPos, &rhSet, &HopscotchStartIndex)) {
                HopscotchForwardVec = rhSet.Forward;
                HopscotchRightVec = rhSet.GetRight();
                FacePos = StartPos + HopscotchForwardVec * 10.0f;
                PHopscotchBoard = board;
                return true;
            }
        }
    }
    return false;
}
