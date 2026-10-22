// Grade: DIFFER(550v609) -- retail keeps a dead board-interface lookup (HasTC byte store + lower_bound loop, result unused) inside the walk loop that ours folds away; frame -0xc
// CAIStateGroup_KidPlayHopscotch::Update() -- while the hopscotch board lives, walk to the start position, look at the face position and play the hopscotch action; if the kid then faces down the board (normalised offset from the start along the forward vector above 0.9) play it once more; then do nothing and terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941690, length 609 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, AIAction_MoveToPos @ 0x0097EC00, AIAction_LookAtPos
// @ 0x0097D580, hopscotch action ctor @ 0x00941B20 / dtor @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70,
// CThing::GetPosition @ 0x004C73D0, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.9f 0x01292740.
// Layout: Ego_r.pdb CAIStateGroup_KidPlayHopscotch PHopscotchBoard +0x15, StartPos +0x24, FacePos +0x30,
// HopscotchForwardVec +0x3c.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
inline CInterfaceMap::SEntry* LowerBound(CInterfaceMap::SEntry* first, CInterfaceMap::SEntry* last, long key)
{
    long count = last - first;
    while (0 < count) {
        long count2 = count / 2;
        CInterfaceMap::SEntry* mid = first;
        mid += count2;
        if (mid->type < key) {
            first = ++mid;
            count -= count2 + 1;
        } else
            count = count2;
    }
    return first;
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        bool has = (m_presentBits[type >> 5] >> (type & 31)) & 1;
        if (has) {
            CInterfaceMap::SEntry* it = LowerBound(m_interfaces.first, m_interfaces.last, type);
            *ppOut = (T*)(it == m_interfaces.last || type < it->type ? m_interfaces.last : it)->component;
        }
        return has;
    }
};
struct CThingCreatureBase : CThing { };
struct CTCHopscotchBoard;                       // interface 0x2d
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x98];
};
struct CCreatureAction_PlayHopscotch : CCreatureActionBase {
    CCreatureAction_PlayHopscotch(CThingCreatureBase* creature);   // 0x00941B20
    ~CCreatureAction_PlayHopscotch();           // 0x00693EF0
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
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& moving, long anim, bool lookAround, bool b6,
                            const CThing* ignoreThing, bool b8, float speedScale);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    void AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};
#pragma pack(push, 1)
struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase {
    bool TookActionPoint;                       // +0x14
    CBaseIntelligentPointer PHopscotchBoard;    // +0x15
    unsigned char _pad1d[3];
    long HopscotchStartIndex;                   // +0x20
    C3DVector StartPos;                         // +0x24
    C3DVector FacePos;                          // +0x30
    C3DVector HopscotchForwardVec;              // +0x3c
    C3DVector HopscotchRightVec;                // +0x48
    void Update();
};
#pragma pack(pop)

void CAIStateGroup_KidPlayHopscotch::Update()
{
    for (;;) {
        if (!IS_THING_ALIVE(PHopscotchBoard)) {
            Terminate();
            return;
        }
        CTCHopscotchBoard* board;
        PHopscotchBoard.GetPItem()->GetInterface(&board, 0x2d);
        bool moving;
        long result = AIAction_MoveToPos(2, StartPos, moving, 5, true, true, 0, true, 0.0f);
        if (result == 4)
            continue;
        if (result != 2) {
            Terminate();
            return;
        }
        break;
    }
    if (!IS_THING_ALIVE(PHopscotchBoard)) {
        Terminate();
        return;
    }
    for (;;) {
        if (PHopscotchBoard.GetPItem() == 0) {
            Terminate();
            return;
        }
        long result = AIAction_LookAtPos(2, FacePos, 1, 0.013888889f);
        if (result == 4)
            continue;
        if (result != 2) {
            Terminate();
            return;
        }
        break;
    }
    if (PHopscotchBoard.GetPItem() == 0) {
        Terminate();
        return;
    }
    if (AIAction_PerformActionAndWait(&CCreatureAction_PlayHopscotch(GetCreature()), false) != 2) {
        Terminate();
        return;
    }
    C3DVector offset = GetCreature()->GetPosition() - StartPos;
    offset.Normalise();
    if (Dot(offset, HopscotchForwardVec) > 0.9f) {
        if (AIAction_PerformActionAndWait(&CCreatureAction_PlayHopscotch(GetCreature()), false) != 2) {
            Terminate();
            return;
        }
    }
    AIAction_DoNothing();
    Terminate();
}
