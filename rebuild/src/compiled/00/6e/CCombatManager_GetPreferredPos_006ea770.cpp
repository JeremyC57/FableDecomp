// CCombatManager::GetPreferredPos(const CThingCreatureBase& creature) const -- where a registered attacker should stand: the
// Grade: RELOCATION_MATCH -- the frame-slot gap (sub esp,60h v 5Ch: the first UID find key kept its own slot because VC7.1 treated the
//   extern find's result as possibly derived from the key it was handed, so the spilled &info.AttackerInfo kept that slot alive) closes
//   with a same-TU __declspec(noinline) body for CCombatantMap::find (the STLport _Rb_tree lower-bound walk): the key slot is then
//   released after the call and both keys share local[8] with the wheel spill, as retail.
// attacked creature's position extrapolated seven frames by its physics velocity, then (when the attacker owns a combat-wheel
// ring) walked in from the wheel's attack vector in 0.25 steps until a navigable spot on the attacked's navigation layer is
// found; otherwise the prong is blocked and the extrapolated position returned.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA770, 799 bytes, `ret 8` (hidden C3DVector result + creature).
// Callees: CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 (FINDINGS chunk 407; was "IsRegisteredCombatant"), combatant map find 0x006F0250 (twice, unchecked),
//   CThing::GetPosition 0x004C73D0, CCombatWheel::ComputeAttackVector 0x006E89B0, CInterfaceMap::LowerBound (const) 0x004365B0,
//   CThing::GetThingStandingOn (vtable +0x98), CThingCreatureBase::GetNavigationLevelAt 0x00661D20,
//   CThingFindNavigationLayerAt::operator() 0x0063EFB0, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620,
//   CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked 0x006E9200 (Ego_r.pdb ?SetAttackerProngAsBlocked@
//   CCombatWheel@CDefendingCombatantInfo@@QAEXABVCThingCreatureBase@@AAVCAttackingCombatantInfo@@@Z: non-const, so the const
//   method casts the wheel and the info; the manifest label BlockAttackerProng is descriptive). Globals: 7.0f 0x01254D80, 1.0f 0x0122DED8, g_Epsilon 0x0129BA3C.
// Shape notes: the normalise goes through an inline GetLength() (keeps 1/len on the x87 stack to the end: `fld Z; fmul st(1);
//   fstp Z; fstp st(0)`); extern const g_Epsilon stops the clamp-then-test fold; user C3DVector copy ctor gives the return copy;
//   pack(4) keeps the __int64 UID at +0x18 in the polymorphic CThing.
// Frame: the combatant map find has a same-TU noinline body (below) so VC7.1 knows its node result is not derived from the key
//   it was handed; with the extern declaration alone the first key's slot stays alive while &info.AttackerInfo (spilled for
//   SetAttackerProngAsBlocked) lives, and the frame grows to 60h (key spellings, pointer/reference info, block scopes, /Oa, QFE do not help).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CPhysicsObject {
    unsigned char _pad00[0x0c];
    C3DVector Pos;                                    // +0x0c
    C3DVector LastPos;                                // +0x18
};
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector* from, const C3DVector* to, float tolerance);   // 0x006CB620
};
#pragma pack(push, 4)
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37();
    virtual CThing* GetThingStandingOn() const;       // slot 38 (+0x98)
    unsigned char _pad04[0x14];
    unsigned __int64 UniqueID;                        // +0x18
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsObject* PPhysics;                         // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char m_flags6c;                          // +0x6c
    unsigned __int64 GetUID() const { return UniqueID; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template<class T> __forceinline T* GetTCIfPresent(long type) const
    {
        T* component = 0;
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            component = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
        }
        return component;
    }
};
#pragma pack(pop)
class CThingCreatureBase : public CThing {          // a class in Ego_r.pdb (mangled V)
public:
    bool IsInMeleeAsAttacker() const;                 // 0x00662410
    long GetNavigationLevelAt(const C3DVector& pos) const;   // 0x00661D20 (Ego_r.pdb, landed under this name)
};
extern const float g_Epsilon;                        // 0x0129BA3C (1e-4f)
struct CThingFindNavigationLayerAt {
    C3DVector Pos;                                    // +0x00
    long      Layer;                                  // +0x0c
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) {}
    bool operator()(CThing* thing);                   // 0x0063EFB0
};
class CAttackingCombatantInfo {                     // a class in Ego_r.pdb (mangled V)
public:
    CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                              // +0x04
    long CombatWheelRingIndex;                        // +0x08
    unsigned char _rest[0x30 - 0x0c];
};
class CDefendingCombatantInfo {
public:
    class CCombatWheel {
    public:
        unsigned char CombatRings[12];
        C3DVector ComputeAttackVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring) const;   // 0x006E89B0
        void SetAttackerProngAsBlocked(const CThingCreatureBase& attacker, CAttackingCombatantInfo& attacker_info);   // 0x006E9200
    };
};
typedef CDefendingCombatantInfo::CCombatWheel CCombatWheel;
struct CCombatantInfo {
    CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;             // +0x04
    unsigned char Attackers[0x0c];                    // +0x34
    CCombatWheel CombatWheel;                         // +0x40
    unsigned char _rest[0x70 - 0x4c];
};
struct CCombatantMap {
    struct node {
        long colour; node* parent; node* left; node* right;
        unsigned __int64 key;                         // +0x10
        CCombatantInfo value;                         // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;    // 0x006F0250
};
// same-TU stand-in for the combatant map find (0x006F0250): the STLport _Rb_tree lower-bound walk; VC7.1 then knows the
// returned node pointer is not derived from the key it was handed.
__declspec(noinline) CCombatantMap::node* CCombatantMap::find(const unsigned __int64& key) const
{
    node* y = header;
    node* x = header->parent;
    while (x) {
        if (!(x->key < key)) { y = x; x = x->left; }
        else x = x->right;
    }
    if (y == header || key < y->key)
        y = header;
    return y;
}
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                    // +0x04
    CCombatantMap Combatants;                         // +0x10
    const CCombatantInfo& GetCombatantInfo(unsigned __int64 uid) const { return Combatants.find(uid)->value; }
    C3DVector GetPreferredPos(const CThingCreatureBase& creature) const;
};

C3DVector CCombatManager::GetPreferredPos(const CThingCreatureBase& creature) const
{
    if (creature.IsInMeleeAsAttacker()) {
        const CCombatantInfo& info = GetCombatantInfo(creature.GetUID());
        const CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
        const CCombatantInfo& attackedInfo = GetCombatantInfo(attacked->GetUID());
        CPhysicsObject* physics = attacked->PPhysics;
        C3DVector predicted = (physics->Pos - physics->LastPos) * 7.0f + attacked->GetPosition();
        if (info.AttackerInfo.CombatWheelRingIndex >= 0) {
            const CCombatWheel& wheel = attackedInfo.CombatWheel;
            C3DVector direction = wheel.ComputeAttackVector(creature, *attacked, info.AttackerInfo.CombatWheelRingIndex);
            float distance = direction.GetLength();
            direction *= 1.0f / distance;
            CTCCreatureNavigation* navigation = creature.GetTCIfPresent<CTCCreatureNavigation>(7);
            CThing* standingOn = 0;
            if (attacked->GetThingStandingOn() && (attacked->GetThingStandingOn()->m_flags6c & 2))
                standingOn = attacked->GetThingStandingOn();
            long region = attacked->GetNavigationLevelAt(attacked->GetPosition());
            C3DVector candidate;
            do {
                candidate = predicted + direction * distance;
                bool sameLayer = true;
                if (standingOn) {
                    CThingFindNavigationLayerAt finder(candidate);
                    finder(standingOn);
                    sameLayer = (float)finder.Layer == (float)region;
                }
                if (sameLayer && navigation->IsAbleToNavigateFromPosToPos(&creature.GetPosition(), &candidate, 0.5f))
                    return candidate;
                distance -= 0.25f;
                if (distance < 0.0f)
                    distance = 0.0f;
            } while (distance > g_Epsilon);
            const_cast<CCombatWheel&>(wheel).SetAttackerProngAsBlocked(creature, const_cast<CAttackingCombatantInfo&>(info.AttackerInfo));
            return predicted;
        }
        return creature.GetPosition();
    }
    return creature.GetPosition();
}