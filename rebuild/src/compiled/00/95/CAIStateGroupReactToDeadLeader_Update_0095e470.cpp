// CAIStateGroup_ReactToDeadLeader::Update() -- while the enemy lives: ask the creature's navigation component for the nearest flee position 20 units from the enemy, run there (re-issuing while the move is pending, the state is not stopping, the enemy lives and the frame budget lasts), forget the enemy when the move did not succeed or once it is more than 20 units away; then Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E470, length 608 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8` frame.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CThingCreatureBase::GetPosition @ 0x006A4D00, CTCCreatureNavigation::GetNearestFleePosition @ 0x006CB840,
// AIAction_MoveToPosProximity @ 0x0097E890, IsStopping @ 0x008FD010, CThing::GetPosition @ 0x004C73D0, Terminate @ 0x008FCF40.
// Data: the frame budget g_scaled_013bb738 @ 0x013BB738; 400.0f @ 0x0125D824; 20.0f / 1.0f immediates.
// Layout: local model (PBrain +4; brain PCreature +0x18; PEnemy +0x14 CIntelligentPointer<CThingCreatureBase>, FramesRunning +0x1c;
// thing presence bits +0x20, interface map +0x44; interface 7 = CTCCreatureNavigation).
// Shape: the navigation lookup inlines the vector-map lower_bound (len/2 halving) on the word-local HasTC; the enemy is cleared
// through an 8-byte intelligent-pointer temporary inside the loop (the `and esp,-8` frame); lower_bound copies `first` into
// the walking pointer after the length; `reached` sits in its own block around the do-loop (shares the HasTC byte slot).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    float GetMagnitudeSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    static __forceinline SEntry* LowerBound(SEntry* first, SEntry* end, long key)
    {
        long len = end - first;
        SEntry* it = first;
        while (len > 0) {
            long half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < key) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            CInterfaceMap::SEntry* end = m_interfaces.last;
            CInterfaceMap::SEntry* it = CInterfaceMap::LowerBound(m_interfaces.first, end, interfaceType);
            *ppOut = (T*)((it == end || interfaceType < it->type) ? end : it)->component;
        }
        return present != 0;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearestFleePosition(const C3DVector& centre, float radius, C3DVector* result);   // 0x006CB840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
    CThingCreatureBase* GetPCreature() const { return (CThingCreatureBase*)GetPItem(); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
extern long g_scaled_013bb738;                  // 0x013BB738 frame budget
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float distance, bool* reached, long movement,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);        // 0x0097E890
};
struct CAIStateGroup_ReactToDeadLeader : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    long FramesRunning;                         // +0x1c
    virtual void Update();
};

void CAIStateGroup_ReactToDeadLeader::Update()
{
    while (IS_THING_ALIVE(PEnemy)) {
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterface(&navigation, 7);
        C3DVector fleePos;
        if (!navigation->GetNearestFleePosition(PEnemy.GetPCreature()->GetPosition(), 20.0f, &fleePos))
            break;
        long result;
        {
            bool reached = false;
            do {
                result = AIAction_MoveToPosProximity(2, fleePos, 1.0f, &reached, 5, true, false, true, 0, true, 0.0f);
                ++FramesRunning;
            } while (!IsStopping() && IS_THING_ALIVE(PEnemy) && FramesRunning < g_scaled_013bb738 && result == 4);
        }
        if (result != 2)
            PEnemy = CIntelligentPointerCreature(0);
        if (!IsStopping() && IS_THING_ALIVE(PEnemy) && FramesRunning < g_scaled_013bb738) {
            const CThing* creature = GetCreature();
            const CThing* enemy = PEnemy.GetPItem();
            if ((enemy->GetPosition() - creature->GetPosition()).GetMagnitudeSquared() > 400.0f)
                PEnemy = CIntelligentPointerCreature(0);
        }
    }
    Terminate();
}