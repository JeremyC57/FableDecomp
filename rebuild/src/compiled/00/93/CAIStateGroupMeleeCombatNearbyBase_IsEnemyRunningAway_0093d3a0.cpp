// CAIStateGroup_MeleeCombatNearbyBase::IsEnemyRunningAway() -- true when the creature perceives its enemy, the enemy is moving and facing away from it, and it is beyond the scratchpad's fleeing break-off range or the perceiver's range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D3A0, length 480 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined lookups: 0x34 = CTCPerceiveThing, 0xAA = CTCAIScratchpad),
// CTCPerceiveThing::GetPerceivedThing @ 0x007446A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy +0x14),
// CThing::GetPosition @ 0x004C73D0, the physics body's virtual GetFacing (slot 70, +0x118), CTCPerceiveThing float getter
// @ 0x00744460 (manifest "CMap::DrawGetSeaLevel" is a propagated label; real name unknown).
// Shape: the 0x34 lookup writes through an out pointer (the component goes to its frame word, not a register); naming the
// perceived thing first fixes the call order; the velocity uses inline body GetPosition/GetPrevPosition accessors (the
// previous position folds into [edi+18h]); the facing goes through an inline CThing::GetFacing forwarder, which puts the
// body in ecx before the vptr load; the scratchpad pointer is uninitialised on a miss (retail reads the frame word the
// perceiver occupies, as VC packs both there); `bool b = false; if (..) b = true;` gives `xor bl,bl ; ... ; mov bl,1`.
// Layout: engine/CAIStateGroup_MeleeCombatNearbyBase.h flattens the base; local model with its PEnemy (+0x14).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CPhysicsBody {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
    unsigned char _pad04[0x08];
    C3DVector Position;                         // +0x0c
    C3DVector PrevPosition;                     // +0x18
    const C3DVector& GetPrevPosition() const { return PrevPosition; }
    const C3DVector& GetPosition() const { return Position; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x14];
    CPhysicsBody* m_pBody;                      // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    C3DVector GetFacing() const { return m_pBody->GetFacing(); }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline void GetTC(T** ppOut, long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CTCPerceiveThing {                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    float GetPerceptionRange();                 // 0x00744460
};
struct CTCAIScratchpad {                        // interface 0xAA
    unsigned char _pad[0x60];
    float CombatNearbyEnemyFleeingBreakOffRange;   // +0x60
};

struct CAIBrain {
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
    bool IsEnemyRunningAway();
};

bool CAIStateGroup_MeleeCombatNearbyBase::IsEnemyRunningAway()
{
    const CThing* me = PBrain->PCreature;
    if (me->HasTC(0x34)) {
        CTCPerceiveThing* perceive;
        me->GetTC(&perceive, 0x34);
        const CThing* perceived = perceive->GetPerceivedThing();
        if (PEnemy.GetPItem() == perceived) {
            const CThing* creature = PBrain->PCreature;
            const CThing* enemy = PEnemy.GetPItem();
            C3DVector toEnemy = enemy->GetPosition() - creature->GetPosition();
            if (Dot(PEnemy.GetPItem()->m_pBody->GetPosition() - PEnemy.GetPItem()->m_pBody->GetPrevPosition(), toEnemy) > 0.0f) {
                C3DVector facing = PEnemy.GetPItem()->GetFacing();
                if (Dot(facing, toEnemy) > 0.0f) {
                    float distSq = Dot(toEnemy, toEnemy);
                    const CThing* self = PBrain->PCreature;
                    const CTCAIScratchpad* scratchpad;
                    if (self->HasTC(0xaa))
                        scratchpad = self->GetTC<CTCAIScratchpad>(0xaa);
                    float breakOff = scratchpad->CombatNearbyEnemyFleeingBreakOffRange;
                    bool beyondBreakOff = false;
                    if (distSq > breakOff * breakOff)
                        beyondBreakOff = true;
                    float range = perceive->GetPerceptionRange();
                    if (distSq > range * range || beyondBreakOff)
                        return true;
                }
            }
        }
    }
    return false;
}