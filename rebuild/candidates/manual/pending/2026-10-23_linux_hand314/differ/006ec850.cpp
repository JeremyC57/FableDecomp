// CCombatWheel::PopulateCombatRings(void* unused, CWorldMap* worldMap, map<CThingCreatureBase*, CAttackingCombatantInfo*>* attackers, CThingCreatureBase* defender) -- split the defender's attackers into inner-ring and outer-ring-preferring sets (melee outer attackers queued first), move some outer attackers inward when the inner set is empty or there is no inner ring, reset every ring, fill inner rings then outer rings (adding rings as needed), drop the unused rings and mark every unplaced attacker's ring index -1
// Grade: DIFFER(1606v1606) -- 73 blocks: same size and flow; frame 0x3c v 0x30 (retail shares the insert_unique result slot with the interface-key slot and between the two inserts; ours gives each its own), list-node link store order
// Leaf: PopulateCombatRings
// Retail 0x006EC850, 1616 bytes, `ret 0x10` (the first argument is not read by this body).
// Callees: malloc 0x00BFEA0E; free 0x00BFEA14; CThingCreatureBase::GetCombatTypeDef 0x00666D10; CVectorMap::LowerBound
//   0x004365B0; CTCCarrying::GetThingInPrimarySlot 0x006A1440; CTCWeapon::IsMeleeWeapon 0x005DAA00; map insert_unique
//   0x006F2A40 / _M_erase 0x006EFCE0; _Rb_global::_Rebalance_for_erase 0x0042A34E; CCombatRing::Reset 0x006E8B60 /
//   AssignAttackersToRing 0x006EBF70; add-ring 0x006EC7D0; ring copy 0x006F4470 (LTCG `__fastcall`);
//   ~CBaseIntelligentPointer 0x00A01C10.
// Layout: CCombatWheel {CombatRings +0 (12-byte STLport vector of 0x1c-byte CCombatRing)}; CCombatTypeDef as retail
//   (12-byte vectors): DefenderCombatRingOccupationNumbers +0x3c, DefenderWheelOuterRingStartIndex +0x48, PreferOuterRing
//   +0x68; CAttackingCombatantInfo::CombatWheelRingIndex +8 (PDB); attack-mode component (interface 0x30) mode +0x6c.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
struct CWorldMap;
struct CTCBase { virtual void v00(); };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);              // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                             // +0x91
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCAttackMode : CTCBase {
    unsigned char _pad04[0x6c - 0x04];
    long          Mode;                                // +0x6c
};
struct CTCCarrying : CTCBase {
    CThing* GetThingInPrimarySlot();                   // 0x006A1440
};
struct CTCWeapon : CTCBase {
    bool IsMeleeWeapon();                              // 0x005DAA00
};
struct CCombatTypeDef {
    unsigned char _pad00[0x3c];
    long*         RingOccupationStart;                 // +0x3c
    long*         RingOccupationFinish;                // +0x40
    long*         RingOccupationEnd;                   // +0x44
    long          OuterRingStartIndex;                 // +0x48
    unsigned char _pad4c[0x68 - 0x4c];
    bool          PreferOuterRing;                     // +0x68
};
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef() const;          // 0x00666D10
};
struct CAttackingCombatantInfo {
    CThingCreatureBase* Attacked;                      // +0x00
    long                AttackerNumber;                // +0x04
    long                CombatWheelRingIndex;          // +0x08
};
struct SAttackerPair { CThingCreatureBase* first; CAttackingCombatantInfo* second; };
struct SAttackerNode {
    unsigned char  Color;
    SAttackerNode* Parent;
    SAttackerNode* Left;
    SAttackerNode* Right;
    SAttackerPair  Value;                              // +0x10
};
SAttackerNode* __cdecl Rebalance_for_erase(SAttackerNode* z, SAttackerNode*& root, SAttackerNode*& leftmost,
                                           SAttackerNode*& rightmost);   // 0x0042A34E
__forceinline SAttackerNode* Increment(SAttackerNode* node)
{
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
    return node;
}
struct SInsertResult { SAttackerNode* it; bool inserted; };
struct CAttackerMap {
    struct SLess { };
    SAttackerNode* Header;
    unsigned long  Count;
    SLess          Compare;
    CAttackerMap() : Header(0)
    {
        Header = (SAttackerNode*)malloc(sizeof(SAttackerNode));
        Count = 0;
        Header->Color = 0;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
    __forceinline ~CAttackerMap()
    {
        if (Count != 0) {
            SAttackerNode* node = Header->Parent;
            while (node) {
                EraseSubtree(node->Right);
                SAttackerNode* left = node->Left;
                free(node);
                node = left;
            }
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            Count = 0;
        }
        if (Header)
            free(Header);
    }
    void EraseSubtree(SAttackerNode* node);            // 0x006EFCE0
    SInsertResult insert(const SAttackerPair& value);  // 0x006F2A40
    __forceinline SAttackerNode* find(CThingCreatureBase* key) const
    {
        SAttackerNode* result = Header;
        SAttackerNode* node = Header->Parent;
        while (node) {
            if (!(node->Value.first < key)) {
                result = node;
                node = node->Left;
            } else
                node = node->Right;
        }
        if (result == Header || key < result->Value.first)
            result = Header;
        return result;
    }
    __forceinline void erase(SAttackerNode* pos)
    {
        SAttackerNode* node = Rebalance_for_erase(pos, Header->Parent, Header->Left, Header->Right);
        if (node)
            free(node);
        --Count;
    }
};
struct SListNode {
    SListNode*          Next;
    SListNode*          Prev;
    CThingCreatureBase* Data;
};
struct CCreatureList {
    SListNode* Node;
    CCreatureList()
    {
        Node = (SListNode*)malloc(sizeof(SListNode));
        Node->Next = Node;
        Node->Prev = Node;
    }
    ~CCreatureList()
    {
        SListNode* cur = Node->Next;
        while (cur != Node) {
            SListNode* tmp = cur;
            cur = cur->Next;
            free(tmp);
        }
        Node->Next = Node;
        Node->Prev = Node;
        free(Node);
    }
    __forceinline void insert(SListNode* pos, CThingCreatureBase* const& value)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        CThingCreatureBase** data = &node->Data;
        if (data)
            *data = value;
        node->Next = pos;
        node->Prev = pos->Prev;
        pos->Prev->Next = node;
        pos->Prev = node;
    }
};
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    ~CBaseIntelligentPointer();                        // 0x00A01C10
};
struct CProng {                                        // 0x1c bytes
    bool                    Blocked;
    float                   Prong[3];                  // +0x04
    long                    ProngBlockageTimer;        // +0x10
    CBaseIntelligentPointer POccupier;                 // +0x14
};
class CCombatWheel;
class CCombatRing {                                    // 0x1c bytes
public:
    float   RotationAngle;
    float   DeltaRotation;
    long    Index;
    float   RingThickness;
    CProng* ProngsStart;                               // +0x10
    CProng* ProngsFinish;                              // +0x14
    CProng* ProngsEnd;                                 // +0x18
    ~CCombatRing()
    {
        for (CProng* p = ProngsStart; p != ProngsFinish; ++p)
            p->~CProng();
        if (ProngsStart)
            free(ProngsStart);
    }
    void Reset();                                      // 0x006E8B60
    void AssignAttackersToRing(CWorldMap* worldMap, CCombatWheel* wheel, CAttackerMap* attackers,
                               CThingCreatureBase* defender);   // 0x006EBF70
};
struct SRandomTag { SRandomTag() { } };
CCombatRing* __fastcall CopyRings(CCombatRing* first, CCombatRing* last, CCombatRing* out, const SRandomTag& tag,
                                  long* distance);     // 0x006F4470
class CCombatWheel {
public:
    CCombatRing* RingsStart;                           // +0x00
    CCombatRing* RingsFinish;                          // +0x04
    CCombatRing* RingsEnd;                             // +0x08
    CCombatRing* AddRing(CCombatTypeDef* def);         // 0x006EC7D0
    __forceinline unsigned long size() const { return RingsFinish - RingsStart; }
    __forceinline CCombatRing* GetRing(long index)
    {
        if (index >= 0 && (unsigned long)index < size())
            return &RingsStart[index];
        return 0;
    }
    __forceinline void erase(CCombatRing* pos)
    {
        if (pos + 1 != RingsFinish)
            CopyRings(pos + 1, RingsFinish, pos, SRandomTag(), 0);
        --RingsFinish;
        RingsFinish->~CCombatRing();
    }
    void PopulateCombatRings(void* unused, CWorldMap* worldMap, CAttackerMap* attackers, CThingCreatureBase* defender);
};

void CCombatWheel::PopulateCombatRings(void* unused, CWorldMap* worldMap, CAttackerMap* attackers,
                                       CThingCreatureBase* defender)
{
    CAttackerMap inner;
    CAttackerMap outer;
    CCreatureList outerQueue;
    CCombatTypeDef* def = defender->GetCombatTypeDef();
    for (SAttackerNode* it = attackers->Header->Left; it != attackers->Header; it = Increment(it)) {
        CThingCreatureBase* attacker = it->Value.first;
        bool melee = false;
        CTCAttackMode* mode;
        if (attacker->GetInterface(&mode, 0x30) && mode->Mode != 0) {
            if (mode->Mode == 4)
                melee = true;
        } else {
            CTCCarrying* carrying;
            if (attacker->GetInterface(&carrying, 0x46)) {
                CThing* weapon = carrying->GetThingInPrimarySlot();
                CTCWeapon* weaponTC;
                if (weapon && !(weapon->Flags91 & 1) && weapon->GetInterface(&weaponTC, 0x48) &&
                    weaponTC->IsMeleeWeapon())
                    melee = true;
            }
        }
        if (attacker->GetCombatTypeDef()->PreferOuterRing) {
            outer.insert(it->Value);
            if (melee)
                outerQueue.insert(outerQueue.Node->Next, attacker);
            else
                outerQueue.insert(outerQueue.Node, attacker);
        } else
            inner.insert(it->Value);
    }
    long moveCount = inner.Count == 0 && outer.Count != 0;
    if (def->OuterRingStartIndex == 0)
        moveCount = outer.Count;
    for (long i = moveCount; i > 0; --i) {
        SListNode* front = outerQueue.Node->Next;
        SAttackerNode* found = outer.find(front->Data);
        inner.insert(found->Value);
        outer.erase(found);
        front->Prev->Next = front->Next;
        front->Next->Prev = front->Prev;
        free(front);
    }
    for (CCombatRing* ring = RingsStart; ring != RingsFinish; ++ring)
        ring->Reset();
    long ringCount = def->RingOccupationFinish - def->RingOccupationStart;
    long ringIndex = 0;
    while (inner.Count != 0 && ringIndex < ringCount) {
        CCombatRing* ring = GetRing(ringIndex++);
        if (!ring)
            ring = AddRing(def);
        ring->AssignAttackersToRing(worldMap, this, &inner, defender);
    }
    if (def->OuterRingStartIndex > ringIndex)
        ringIndex = def->OuterRingStartIndex;
    while (outer.Count != 0 && ringIndex < ringCount) {
        CCombatRing* ring = GetRing(ringIndex++);
        if (!ring)
            ring = AddRing(def);
        ring->AssignAttackersToRing(worldMap, this, &outer, defender);
    }
    long total = size();
    if (ringIndex < total) {
        long extra = total - ringIndex;
        do {
            if (RingsStart != RingsFinish)
                erase(RingsFinish - 1);
        } while (--extra);
    }
    if (inner.Count != 0 || outer.Count != 0) {
        for (SAttackerNode* it = inner.Header->Left; it != inner.Header; it = Increment(it))
            it->Value.second->CombatWheelRingIndex = -1;
        for (SAttackerNode* it = outer.Header->Left; it != outer.Header; it = Increment(it))
            it->Value.second->CombatWheelRingIndex = -1;
    }
}
