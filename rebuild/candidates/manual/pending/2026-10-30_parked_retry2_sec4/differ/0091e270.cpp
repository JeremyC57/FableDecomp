// CAIStateGroup_HoverAttack::IsEnemyRunningAway() -- true when the creature perceives its enemy, the enemy moves away (normalised velocity dot the offset to it is positive), faces away, and is beyond the perception range
// Grade: DIFFER(ours v retail) -- 497v513, nd 76->67 (retry wave 2 sec4): enemy read before creature, range*range <
//   distSq. Left: this/&PEnemy/scratch rotation (retail ebx/edi/esi), the Normalise x87 scheduling, distSq computed
//   after the GetPerceptionRange call (retail before, spilled); tried no locals (135), !(<=) form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091E270, length 513 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined lookup 0x34 = CTCPerceiveThing), CTCPerceiveThing::
// GetPerceivedThing @ 0x007446A0 / GetPerceptionRange @ 0x00744460, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::GetPosition @ 0x004C73D0, the physics body's virtual GetFacing (slot 70, +0x118).
// Globals: inverse-square-root mantissa table @ 0x013CE558; 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C, 0.0f @ 0x0122DEDC.
// Shape: the sibling 0x0093D3A0 (MeleeCombatNearbyBase) with the velocity normalised by the inline table-driven
// Normalise (landed 0x007A6E20) before the dot product, and no scratchpad break-off range.
// Layout: Ego_r.pdb CAIStateGroup_HoverMovementBase is 0x1c bytes: the target intelligent pointer at +0x14; the
// physics body at CThing +0x60 holds Position +0x0c, PrevPosition +0x18.
struct CTCBase;
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
        float r;
        *(unsigned long*)&r = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                              (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
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
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
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
struct CAIStateGroup_HoverMovementBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14
};
struct CAIStateGroup_HoverAttack : CAIStateGroup_HoverMovementBase {
    bool IsEnemyRunningAway();
};

bool CAIStateGroup_HoverAttack::IsEnemyRunningAway()
{
    const CThing* me = PBrain->PCreature;
    if (me->HasTC(0x34)) {
        CTCPerceiveThing* perceive = me->GetTC<CTCPerceiveThing>(0x34);
        if (PEnemy.GetPItem() == perceive->GetPerceivedThing()) {
            const CThing* enemy = PEnemy.GetPItem();
            const CThing* creature = PBrain->PCreature;
            C3DVector toEnemy = enemy->GetPosition() - creature->GetPosition();
            C3DVector velocity = PEnemy.GetPItem()->m_pBody->GetPosition() - PEnemy.GetPItem()->m_pBody->GetPrevPosition();
            velocity.Normalise();
            if (Dot(velocity, toEnemy) > 0.0f) {
                C3DVector facing = PEnemy.GetPItem()->GetFacing();
                if (Dot(facing, toEnemy) > 0.0f) {
                    float distSq = Dot(toEnemy, toEnemy);
                    float range = perceive->GetPerceptionRange();
                    if (range * range < distSq)
                        return true;
                }
            }
        }
    }
    return false;
}
