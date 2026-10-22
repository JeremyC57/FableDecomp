// CAIStateGroup_WhisperRetreat::JumpToSafety() -- unless already range-attacking, somersault away from the base's target: forward when a 4-unit leap away from it is navigable and the creature faces away from it (or neither), backward otherwise, and wait for the action
// Leaf: JumpToSafety
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    __forceinline float Dot(const C3DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }
    static __forceinline float FastInvSqrt(float lengthSquared)
    {
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        return (3.0f - r * r * lengthSquared) * r * 0.5f;
    }
    __forceinline void Normalise()
    {
        *this *= FastInvSqrt(Z * Z + Y * Y + X * X);
    }
};
struct CPhysicsBody {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetForward() const;        // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PBody;                        // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CTCCreatureNavigation {                  // interface 7
    bool IsPosNavLineOfSightTo(const C3DVector& pos);   // 0x006CBA20
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCreatureActionBase {
    void* __vftable;
    unsigned char _pad04[0x114 - 0x04];
    bool Finished;                              // +0x114
    unsigned char _pad115[3];
    ~CCreatureActionBase();                     // 0x0062E900 (folded)
};
struct CCreatureAction_WhisperSommersaultForward : CCreatureActionBase {    // vtable 0x01296754
    CCreatureAction_WhisperSommersaultForward(CThing* creature, CThing* from);   // 0x0096BF10
};
struct CCreatureAction_WhisperSommersaultBackward : CCreatureActionBase {   // vtable 0x0129689C
    CCreatureAction_WhisperSommersaultBackward(CThing* creature, CThing* from);  // 0x0096BF90
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_WhisperRetreat : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    CBaseIntelligentPointer PArenaCentre;       // +0x1c
    bool bIsRangeAttack;                        // +0x24
    long NumHits;                               // +0x28
    long FrameOfLastHit;                        // +0x2c
    CThing* GetCreature() const { return PBrain->PCreature; }
    void JumpToSafety();
};

void CAIStateGroup_WhisperRetreat::JumpToSafety()
{
    if (bIsRangeAttack)
        return;
    CThing* creature = GetCreature();
    C3DVector away = creature->GetPosition() - PTarget.GetPItem()->GetPosition();
    away.Normalise();
    C3DVector offset = away * 4.0f;
    C3DVector landing = GetCreature()->GetPosition() + offset;
    bool facingAway = away.Dot(GetCreature()->PBody->GetForward()) > 0.0f;
    CTCCreatureNavigation* navigation;
    GetCreature()->GetInterface(&navigation, 7);
    if (navigation->IsPosNavLineOfSightTo(landing)) {
        if (facingAway)
            AIAction_PerformActionAndWait(&CCreatureAction_WhisperSommersaultForward(GetCreature(), PTarget.GetPItem()), false);
        else
            AIAction_PerformActionAndWait(&CCreatureAction_WhisperSommersaultBackward(GetCreature(), PTarget.GetPItem()), false);
    }
    else {
        if (!facingAway)
            AIAction_PerformActionAndWait(&CCreatureAction_WhisperSommersaultForward(GetCreature(), PTarget.GetPItem()), false);
        else
            AIAction_PerformActionAndWait(&CCreatureAction_WhisperSommersaultBackward(GetCreature(), PTarget.GetPItem()), false);
    }
}
