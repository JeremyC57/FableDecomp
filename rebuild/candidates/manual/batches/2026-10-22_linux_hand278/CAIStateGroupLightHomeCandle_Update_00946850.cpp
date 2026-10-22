// CAIStateGroup_LightHomeCandle::Update() -- reserve a free action point on the candle, walk to it and look at the candle while it still needs changing, then use the candle, release the point, clear the candle and terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946850, length 653 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, CVectorMap::LowerBound @ 0x0040F020,
// CTCUserActionPointBase::GetNumberOfActionPointsInUse @ 0x007FEC10 / GetFreeActionPoint @ 0x007FEDE0 / FreeActionPoint
// @ 0x007FEF40, IsCandleInNeedOfChanging @ 0x00946730, AIAction_MoveToPos @ 0x0097EC00, CThing::GetPosition @ 0x004C73D0,
// AIAction_LookAtPos @ 0x0097D580, CCreatureAction_UseObject ctor @ 0x0069F230 / dtor @ 0x006290E0,
// AIAction_PerformActionAndWait @ 0x0097DB70, CIntelligentPointer<CThing> ctor @ 0x004A96F0 / operator= @ 0x004A9760,
// Terminate @ 0x008FCF40.
// Shape: the HasTC result is stored in the bool that later receives the arrival flag (frame byte shared); the action-point
// component is the uninitialised out-parameter GetTC (the absent branch reads the key slot); the candle position is a
// frame C3DVector (user copy ctor); the pointer clear goes through a named null CIntelligentPointer.
// Layout: Ego_r.pdb CAIStateGroup_LightHomeCandle (0x1c): PCandle +0x14; retail CCreatureAction_UseObject is 0xe0 bytes.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long t) const { return (m_presentBits[t >> 5] >> (t & 31)) & 1; }
    template<class T> __forceinline void GetTC(T** ppOut, long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing { };
struct CTCUserActionPointBase {                 // interface 0x2d
    long GetNumberOfActionPointsInUse();        // 0x007FEC10
    bool GetFreeActionPoint(CThingCreatureBase* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);               // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x004A9760
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[0xa4];
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    unsigned char _padA8[0x38];
    CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* usedThing, bool allowTurn);   // 0x0069F230
    ~CCreatureAction_UseObject();               // 0x006290E0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* arrived, long movement, bool a, bool b,
                            CThing* avoid, bool c, float speed);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_LightHomeCandle : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCandle;        // +0x14
    bool IsCandleInNeedOfChanging(CThing* candle);   // 0x00946730
    void Update();
};

void CAIStateGroup_LightHomeCandle::Update()
{
    if (PCandle.GetPItem() == 0) {
        Terminate();
        return;
    }
    CTCUserActionPointBase* points;
    CThing* candle = PCandle.GetPItem();
    bool arrived = candle->HasTC(0x2d);
    if (arrived)
        candle->GetTC(&points, 0x2d);
    if (points->GetNumberOfActionPointsInUse() != 0 || !IsCandleInNeedOfChanging(PCandle.GetPItem())) {
        Terminate();
        return;
    }
    long index;
    C3DVector pos;
    CRightHandedSet orientation;
    if (!points->GetFreeActionPoint(GetCreature(), &pos, &orientation, &index)) {
        Terminate();
        return;
    }
    long result;
    do {
        if (PCandle.GetPItem() == 0 || !IsCandleInNeedOfChanging(PCandle.GetPItem())) {
            points->FreeActionPoint(index);
            Terminate();
            return;
        }
        result = AIAction_MoveToPos(2, pos, &arrived, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2 || PCandle.GetPItem() == 0 || !IsCandleInNeedOfChanging(PCandle.GetPItem())) {
        points->FreeActionPoint(index);
        Terminate();
        return;
    }
    C3DVector candlePos = PCandle.GetPItem()->GetPosition();
    do {
        if (PCandle.GetPItem() == 0 || !IsCandleInNeedOfChanging(PCandle.GetPItem())) {
            points->FreeActionPoint(index);
            Terminate();
            return;
        }
        result = AIAction_LookAtPos(2, candlePos, 0x11, 0.0027777f);
    } while (result == 4);
    if (result != 2 || PCandle.GetPItem() == 0 || !IsCandleInNeedOfChanging(PCandle.GetPItem())) {
        points->FreeActionPoint(index);
        return;
    }
    AIAction_PerformActionAndWait(&CCreatureAction_UseObject(GetCreature(), PCandle.GetPItem(), false), false);
    points->FreeActionPoint(index);
    {
        CIntelligentPointer<CThing> none(0);
        PCandle = none;
    }
    Terminate();
}
