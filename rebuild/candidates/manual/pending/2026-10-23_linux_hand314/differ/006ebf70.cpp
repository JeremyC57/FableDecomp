// CCombatRing::AssignAttackersToRing(CWorldMap* worldMap, CCombatWheel* wheel, map<CThingCreatureBase*, CAttackingCombatantInfo*>* attackers, CThingCreatureBase* defender) -- partition the ring's prongs into free / taken; with a single attacker on an inner, unrotated ring turn the ring so its first prong faces that attacker; then, if there are at least as many attackers as free prongs, give each free prong its best attacker, otherwise give each attacker its best free prong, removing assigned attackers from the map
// Grade: DIFFER(1022v1026) -- 32 blocks: register choice (def spilled / map in ebp), x87 operand order of the length and cross terms, prong-loop counter homing
// Leaf: AssignAttackersToRing
// Retail 0x006EBF70, 1040 bytes, `ret 0x10`.
// Callees: CThingCreatureBase::GetCombatTypeDef 0x00666D10; prong partition 0x006F2B30 (LTCG `__fastcall`); CThing::
//   GetPosition 0x004C73D0; C3DVector::Normalise 0x00A14440; prong direction 0x006E7FD0; acos 0x00BFEBA2; for_each
//   <CGetBestAttackerForProng> 0x006F34B0 (`__fastcall`, hidden return); map find 0x006F1070 / erase(key) 0x006F3340 /
//   iterator++(int) 0x006EF560; AssignAttackerToProng 0x006EBEE0; CAttackerPositionEvaluator::operator() 0x006F2C00;
//   _Rb_global::_Rebalance_for_erase 0x0042A34E; free 0x00BFEA14.
// Layout: from the PDB (struct_layouts_egor.tsv) -- CCombatRing {RotationAngle +0, DeltaRotation +4, Index +8, RingThickness
//   +0xc, Prongs +0x10 (12-byte STLport vector)}; CProng 0x1c bytes; combat type def ring count +0x48 (this function's use);
//   map node key +0x10 / value +0x14.
extern "C" double __cdecl acos(double);
extern "C" double __cdecl fabs(double);
#pragma intrinsic(acos, fabs)
extern "C" void __cdecl free(void* p);
extern float g_Epsilon;                                // 0x0129BA3C
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                  // 0x00A14440
};
struct CWorldMap;
struct CCombatWheel;
struct CAttackingCombatantInfo;
struct CCombatTypeDef {
    unsigned char _pad00[0x48];
    long          RingCount;                           // +0x48
};
struct CThing {
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef() const;          // 0x00666D10
};
struct CProng {                                        // 0x1c bytes
    bool          Blocked;
    C3DVector     Prong;                               // +0x04
    long          ProngBlockageTimer;                  // +0x10
    void*         POccupier[2];                        // +0x14
    C3DVector GetDirection(float offset) const;        // 0x006E7FD0
};
struct SAttackerNode {
    long                     Color;
    SAttackerNode*           Parent;
    SAttackerNode*           Left;
    SAttackerNode*           Right;
    CThingCreatureBase*      first;                    // +0x10
    CAttackingCombatantInfo* second;                   // +0x14
};
SAttackerNode* __cdecl Rebalance_for_erase(SAttackerNode* z, SAttackerNode*& root, SAttackerNode*& leftmost,
                                           SAttackerNode*& rightmost);   // 0x0042A34E
struct SAttackerIterator {
    SAttackerNode* Node;
    SAttackerIterator() { }
    SAttackerIterator(SAttackerNode* node) : Node(node) { }
    SAttackerIterator(const SAttackerIterator& other) : Node(other.Node) { }
    SAttackerIterator operator++(int);                 // 0x006EF560
    __forceinline SAttackerIterator& operator++()
    {
        SAttackerNode* node = Node;
        if (node->Right) {
            node = node->Right;
            while (node->Left)
                node = node->Left;
        } else {
            SAttackerNode* parent = node->Parent;
            while (node == parent->Right) {
                node = parent;
                parent = parent->Parent;
            }
            if (node->Right != parent)
                node = parent;
        }
        Node = node;
        return *this;
    }
    bool operator!=(const SAttackerIterator& other) const { return Node != other.Node; }
    SAttackerNode* operator->() const { return Node; }
};
struct CAttackerMap {
    SAttackerNode* Header;
    unsigned long  Count;
    SAttackerIterator begin() const { return SAttackerIterator(Header->Left); }
    SAttackerIterator end() const { return SAttackerIterator(Header); }
    unsigned long size() const { return Count; }
    SAttackerIterator find(CThingCreatureBase* const& key);   // 0x006F1070
    unsigned long erase(CThingCreatureBase* const& key);      // 0x006F3340
    __forceinline void erase(SAttackerIterator pos)
    {
        SAttackerNode* node = Rebalance_for_erase(pos.Node, Header->Parent, Header->Left, Header->Right);
        if (node)
            free(node);
        --Count;
    }
};
struct CIsProngFree { char unused; };
struct SRandomTag { SRandomTag() { } };
CProng* __fastcall PartitionProngs(CProng* first, CProng* last, CIsProngFree pred, const SRandomTag& tag);   // 0x006F2B30
class CCombatRing;
struct CGetBestAttackerForProng {
    CCombatRing*        Ring;
    CCombatWheel*       Wheel;
    CProng*             Prong;
    CThingCreatureBase* Defender;
    CWorldMap*          WorldMap;
    float               BestScore;
    CThingCreatureBase* BestAttacker;
    CGetBestAttackerForProng(CCombatRing* ring, CCombatWheel* wheel, CProng* prong, CThingCreatureBase* defender,
                             CWorldMap* worldMap)
        : Ring(ring), Wheel(wheel), Prong(prong), Defender(defender), WorldMap(worldMap), BestScore(-1.0f),
          BestAttacker(0) { }
};
CGetBestAttackerForProng __fastcall for_each(SAttackerIterator first, SAttackerIterator last,
                                             CGetBestAttackerForProng f);   // 0x006F34B0
struct CAttackerPositionEvaluator {
    CWorldMap*               WorldMap;
    CThingCreatureBase*      Attacker;
    CThingCreatureBase*      Defender;
    CAttackingCombatantInfo* Info;
    CCombatRing*             Ring;
    CCombatWheel*            Wheel;
    float                    BestScore;
    CProng*                  BestProng;
    CAttackerPositionEvaluator(CWorldMap* worldMap, CThingCreatureBase* attacker, CThingCreatureBase* defender,
                               CAttackingCombatantInfo* info, CCombatRing* ring, CCombatWheel* wheel)
        : WorldMap(worldMap), Attacker(attacker), Defender(defender), Info(info), Ring(ring), Wheel(wheel),
          BestScore(-1.0f), BestProng(0) { }
    void operator()(CProng& prong);                    // 0x006F2C00
};
template<class It, class F> __forceinline F for_each(It first, It last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
struct CProngVector {
    CProng* Start;
    CProng* Finish;
    CProng* End;
};
class CCombatRing {
public:
    float        RotationAngle;                        // +0x00
    float        DeltaRotation;                        // +0x04
    long         Index;                                // +0x08
    float        RingThickness;                        // +0x0c
    CProngVector Prongs;                               // +0x10
    void AssignAttackerToProng(CProng* prong, CThingCreatureBase* attacker, CAttackingCombatantInfo* info,
                               bool outerRing);        // 0x006EBEE0
    void AssignAttackersToRing(CWorldMap* worldMap, CCombatWheel* wheel, CAttackerMap* attackers,
                               CThingCreatureBase* defender);
};

void CCombatRing::AssignAttackersToRing(CWorldMap* worldMap, CCombatWheel* wheel, CAttackerMap* attackers,
                                        CThingCreatureBase* defender)
{
    CCombatTypeDef* def = defender->GetCombatTypeDef();
    bool outerRing = Index >= def->RingCount;
    CIsProngFree pred;
    CProng* freeEnd = PartitionProngs(Prongs.Start, Prongs.Finish, pred, SRandomTag());
    unsigned long attackerCount = attackers->size();
    CProng* first = Prongs.Start;
    long freeCount = freeEnd - first;
    if (freeCount == 0)
        return;
    if (Index < def->RingCount && attackerCount == 1 && !((float)fabs(DeltaRotation) > g_Epsilon)) {
        CThingCreatureBase* attacker = attackers->begin()->first;
        const C3DVector& defenderPos = defender->GetPosition();
        const C3DVector& attackerPos = attacker->GetPosition();
        C3DVector toAttacker(attackerPos.X - defenderPos.X, attackerPos.Y - defenderPos.Y, 0.0f);
        if (toAttacker.Y * toAttacker.Y + toAttacker.X * toAttacker.X + toAttacker.Z * toAttacker.Z > g_Epsilon * g_Epsilon)
            toAttacker.Normalise();
        C3DVector direction = first->GetDirection(0.0f);
        float cosine = direction.Y * toAttacker.Y;
        cosine += direction.X * toAttacker.X;
        cosine += direction.Z * toAttacker.Z;
        if (cosine < -1.0f)
            cosine = -1.0f;
        else if (cosine > 1.0f)
            cosine = 1.0f;
        float angle = (float)acos(cosine) * 0.15915494f;
        if (toAttacker.Y * direction.X - toAttacker.X * direction.Y > 0.0f)
            angle *= -1.0f;
        RotationAngle = angle;
    }
    if ((long)attackerCount >= freeCount) {
        for (CProng* prong = Prongs.Start; prong != freeEnd; ++prong) {
            if (attackerCount == 0)
                return;
            CThingCreatureBase* best =
                for_each(attackers->begin(), attackers->end(),
                         CGetBestAttackerForProng(this, wheel, prong, defender, worldMap)).BestAttacker;
            if (best) {
                SAttackerIterator it = attackers->find(best);
                AssignAttackerToProng(prong, best, it->second, outerRing);
                attackers->erase(best);
                --attackerCount;
            }
        }
    } else {
        SAttackerIterator it = attackers->begin();
        while (it != attackers->end()) {
            CThingCreatureBase* attacker = it->first;
            CAttackingCombatantInfo* info = it->second;
            CProng* best = for_each(Prongs.Start, freeEnd,
                                    CAttackerPositionEvaluator(worldMap, attacker, defender, info, this, wheel)).BestProng;
            if (best) {
                AssignAttackerToProng(best, attacker, info, outerRing);
                attackers->erase(it++);
            } else
                ++it;
        }
    }
}
