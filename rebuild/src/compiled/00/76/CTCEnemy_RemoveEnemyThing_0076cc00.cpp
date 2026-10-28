// CTCEnemy::RemoveEnemyThing(const CThing* enemy) -- forget a thing as an explicit enemy; when it is the hero and this creature's opinion lists it as an enemy, also clear the creature's hero-enemy opinion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076CC00, length 210 bytes (+ int3 padding). __fastcall, `ret 4`.
//         Ego_r.pdb: `?RemoveEnemyThing@CTCEnemy@@QAEXPBVCThing@@@Z`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x4F, CTCCreatureOpinionOfHero),
//          CTCCreatureOpinionOfHero::IsHeroOpinionEnemy @ 0x006827A0 / SetHeroOpinionEnemy(bool) @ 0x006815D0,
//          FindThingInList(first, last, value) @ 0x0076D6F0 (__fastcall, hidden iterator result in ecx, value in edx),
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10 (CIntelligentPointer<const CThing>
//          vtable 0x01237F1C), list<CIntelligentPointer<const CThing>>::remove(const value_type&) @ 0x0076D7B0.
// Shape: the two presence tests (the enemy's interface 0x51 = word 2 bit 17, the owner's 0x4F = word 2 bit 15) are
//        `shr ; and al,1` reads into two named bool locals of nested blocks, each stored to the same frame byte (one
//        reused variable or an `&&` chain keeps only one store); the find's iterators are built in their argument slots
//        (user copy ctor) and the value is the `enemy` parameter slot itself; the removal temporary's inline ctor takes
//        the pointer by reference (read from the slot after the base ctor).
// Layout: Ego_r.pdb CTCEnemy matches retail up to EnemyThings +0x10 (STLport list: the sentinel pointer).
class CThing;
class CBaseObject;
class CTCBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct SThingListNode;
struct CThingListIterator {
    SThingListNode* m_pNode;
    CThingListIterator() { }
    CThingListIterator(SThingListNode* node) : m_pNode(node) { }
    CThingListIterator(const CThingListIterator& other) : m_pNode(other.m_pNode) { }
};
struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
};
CThingListIterator __fastcall FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value);   // 0x0076D6F0

template<class T> struct CRetailList {              // STLport list: the sentinel node pointer
    SThingListNode* _M_node;
    CThingListIterator begin() const { return CThingListIterator(_M_node->_M_next); }
    CThingListIterator end() const { return CThingListIterator(_M_node); }
    void remove(const T& value);                    // 0x0076D7B0
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CTCCreatureOpinionOfHero {                    // interface 0x4F
public:
    void SetHeroOpinionEnemy(bool enemy);           // 0x006815D0
public:
    bool IsHeroOpinionEnemy() const;   // 0x006827A0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    bool HasTC(long type) const { return (UsingComponents[type >> 5] >> (type & 31)) & 1; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CTCEnemy {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool FriendsWithEverythingFlag;                 // +0x0C
    bool EnableFollowersEnemyProxy;                 // +0x0D
    CRetailList< CIntelligentPointer<const CThing> > EnemyThings;   // +0x10
    void RemoveEnemyThing(const CThing* enemy);
};

void CTCEnemy::RemoveEnemyThing(const CThing* enemy)
{
    if (enemy) {
        bool isHero = enemy->HasTC(0x51);
        if (isHero) {
            bool hasOpinion = ParentThing->HasTC(0x4f);
            if (hasOpinion) {
                CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)ParentThing->GetTC(0x4f);
                if (opinion->IsHeroOpinionEnemy()) {
                    if (FindThingInList(EnemyThings.begin(), EnemyThings.end(), const_cast<CThing* const&>(enemy)).m_pNode != EnemyThings._M_node)
                        opinion->SetHeroOpinionEnemy(false);
                }
            }
        }
    }
    EnemyThings.remove(CIntelligentPointer<const CThing>(enemy));
}