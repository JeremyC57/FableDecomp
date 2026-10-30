// CAIStateGroup_MinionWorkInInn::CleanBench() -- take a free action point on a random bench, walk to it, face along the point's forward axis and play ST_WIPE_TABLE; free the action point afterwards (or when a step fails)
// Grade: DIFFER(ours v retail) -- 399v399, nd 50->26 (retry wave 2 sec4): the look target is a named C3DVector local
//   (target = Forward + position), so the x87 adds run before the argument pushes as retail. Left: frame slot order
//   (retail spilled presence bool +0xb, points +0xc, key/index +0x10, reached +0x23; ours bool +0x13, points +8,
//   key/index +0xc, reached +0x2a); tried reached/points declared first, member-wise operator+ (worse)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BD40, length 400 bytes (+ int3 padding).
// Callees: the bench picker @ 0x0090BC60 (manifest "CleanBench_PickRandomMarker"), the inlined GetInterface 0x2d
//          (CTCUserActionPointBase, bit 13 of +0x24 as a bitset::test byte), CTCUserActionPointBase::GetFreeActionPoint
//          @ 0x007FEDE0 / the action-point release @ 0x007FEF40 (manifest "CTCCameraPointScriptedSpline::
//          EditCyclePreviousKeyCamera" is wrong), CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 / AIAction_LookAtPos
//          @ 0x0097D580 / AIAction_PerformAction @ 0x0097D8B0, CThing::GetPosition @ 0x004C73D0, CCharString(const char*,
//          long) @ 0x0099EBF0 ("ST_WIPE_TABLE" @ 0x0128F120) / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation(
//          creature, const CCharString&, ...) @ 0x008424F0, ~CCreatureActionBase @ 0x00693EF0.
// Shape: the look target `forward + position` is built by the three-float ctor straight into the by-value argument
//          (Z first); each failure frees the action point and returns; the animation is a block with a named name.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00 (descriptive)
    C3DVector Forward;                          // +0x0c (descriptive)
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
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThing* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionWorkInInn : CAIStateGroupBase {
    CThing* GetBench();                         // 0x0090BC60
    void CleanBench();
};

void CAIStateGroup_MinionWorkInInn::CleanBench()
{
    CThing* bench = GetBench();
    if (!bench)
        return;
    CTCUserActionPointBase* points;
    bench->GetInterfaceT(&points, 0x2d);
    long index;
    C3DVector pos;
    CRightHandedSet orientation;
    if (!points->GetFreeActionPoint(GetCreature(), &pos, &orientation, &index))
        return;
    bool reached;
    if (AIAction_MoveToPos(1, pos, &reached, 3, true, true, 0, true, 0.0f) != 2) {
        points->FreeActionPoint(index);
        return;
    }
    C3DVector target = orientation.Forward + GetCreature()->GetPosition();
    if (AIAction_LookAtPos(1, target, 0x11, 0.0027777f) != 2) {
        points->FreeActionPoint(index);
        return;
    }
    {
        CCharString animation("ST_WIPE_TABLE");
        AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            false, true, false, 0, 0));
    }
    points->FreeActionPoint(index);
}
