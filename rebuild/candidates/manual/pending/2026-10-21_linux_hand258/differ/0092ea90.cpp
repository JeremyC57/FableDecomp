// CAIStateGroup_MinionFindPersonalSpace::Update() -- when the tracked creature invades the minion's space, step away from it by the personal-space distance (to the navigation component's nearest flee position) and terminate
// Grade: DIFFER(444v440) -- retail computes the direction Z, Y, X (X left on top of the x87 stack, never stored) and spills the normalised X late; ours computes X, Y, Z and spills Z early (fst), so the Normalise/scale sequence differs; retail also stores the navigation-present bool into the arrived slot ([esp+0xf]) where ours tests it in a register
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092EA90, length 440 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, IsSpaceInvader @ 0x0092E890, the static personal-space distance
// @ 0x00661CC0 (returns a float constant; no `this`), CThingCreatureBase::GetPosition @ 0x006A4D00 (three times),
// CVectorMap::LowerBound @ 0x0040F020 (inlined unchecked lookup 7 = CTCCreatureNavigation),
// CTCCreatureNavigation::GetNearestFleePosition @ 0x006CB840, AIAction_MoveToPos @ 0x0097EC00, Terminate @ 0x008FCF40.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C (C3DVector's inline
// fast Normalise, as landed 0x007A6E20).
// Layout: Ego_r.pdb CAIStateGroup_MinionFindPersonalSpace (PThingCreature +0x14); retail CThing interface bitset +0x20 /
// map +0x44. The engine header flattens the base, so the model is local.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
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
    template<class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* component;
        bool present = ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            component = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return component;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
    static float GetPersonalSpaceDistance();    // 0x00661CC0
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearestFleePosition(const C3DVector& from, float tolerance, C3DVector* out);   // 0x006CB840
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* arrived, long movement, bool a, bool b,
                            const CThing* ignore, bool c, float d);   // 0x0097EC00
};
struct CAIStateGroup_MinionFindPersonalSpace : CAIStateGroupBase {
    CBaseIntelligentPointer PThingCreature;     // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool IsSpaceInvader(CThing* thing);         // 0x0092E890
    virtual void Update();
};

void CAIStateGroup_MinionFindPersonalSpace::Update()
{
    if (IsSpaceInvader(PThingCreature.GetPItem())) {
        float distance = CThingCreatureBase::GetPersonalSpaceDistance();
        CThingCreatureBase* creature = GetCreature();
        C3DVector direction = creature->GetPosition() - PThingCreature.GetPItem()->GetPosition();
        direction.Normalise();
        C3DVector offset = direction * distance;
        C3DVector from = GetCreature()->GetPosition() + offset;
        CTCCreatureNavigation* navigation = GetCreature()->GetTCIfPresent<CTCCreatureNavigation>(7);
        C3DVector flee;
        if (navigation->GetNearestFleePosition(from, 0.5f, &flee)) {
            bool arrived;
            AIAction_MoveToPos(1, flee, &arrived, 3, true, true, 0, true, 0.0f);
        }
    }
    Terminate();
}
