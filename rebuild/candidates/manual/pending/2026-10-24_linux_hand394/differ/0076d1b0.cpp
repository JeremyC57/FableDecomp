// CTCEnemy::AddEnemyThing(const CThing* enemy) -- add a thing to the explicit enemy list unless it is already there; when it is the hero (deed log, interface 0x51) and the owner has a hero opinion (interface 0x4f), mark the owner's opinion of the hero as enemy
// Leaf: ?AddEnemyThing@CTCEnemy
// Grade: DIFFER(177v179) -- only the opinion call's tail differs: retail `mov eax,[eax+4] ; push 1 ; mov ecx,eax`, ours `mov ecx,[eax+4] ; push 1` (the component pointer goes through eax); next: name the opinion in a local / an out-pointer GetTC<CTCCreatureOpinionOfHero>(&opinion) inline (the PDB `GetTC<T>(T**)` form)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076D1B0, 179 bytes (+13 int3), `ret 4`.
// Name: FableWin.pdb `?AddEnemyThing@CTCEnemy@@QAEXPBVCThing@@@Z`; the landed CTCVillage::SetEnemyToHeroStatus 0x0065A852
//   calls it as CTCEnemy::AddEnemyThing, as do CThingManager::SetAllBossesToBossPhase 0x0051FA00, CGameScriptInterface::
//   EntitySetThingAsEnemyOfThing 0x0089C000 and GiveThingBestEnemyTarget 0x008A0260. Label correction: catalog
//   "CCreature_AddEnemyToList" -> CTCEnemy::AddEnemyThing.
// Callees: STLport __find(first, last, CThing* const&, const input_iterator_tag&) 0x0076D5D0 (__fastcall, hidden iterator
//   result in ecx, the value in edx, `ret 0Ch`); CBaseIntelligentPointer ctor / SetPItem / dtor 0x00A01B10 / 0x00A01B90 /
//   0x00A01C10 (CIntelligentPointer<const CThing> vtable 0x01237F1C); list<CIntelligentPointer<const CThing> >::push_back
//   0x0065617D (a folded COMDAT); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020;
//   CTCCreatureOpinionOfHero::SetHeroOpinionEnemy(bool) 0x006815D0.
// Layout: the landed sibling CTCEnemy::RemoveEnemyThing 0x0076CC00 model: EnemyThings +0x10 (STLport list: the sentinel
//   pointer); CThing UsingComponents +0x20 (word 2 bit 17 = 0x51, bit 15 = 0x4f), the interface map +0x44.
// Shape: end() is read once into a local and compared with the find's result; the find's iterators are built in their
//   argument slots (user copy constructor); the value and the empty iterator tag are both the `enemy` parameter slot.
//   The pushed temporary's inline constructor takes the pointer by reference (re-read after the base constructor).
class CThing;
class CBaseObject;
class CTCBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
    CThing* GetPItem() const;                       // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

namespace std {
struct input_iterator_tag {
    input_iterator_tag() { }
};
}

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
};
struct CThingListIterator {
    SThingListNode* _M_node;
    CThingListIterator(SThingListNode* node) : _M_node(node) { }
    CThingListIterator(const CThingListIterator& other) : _M_node(other._M_node) { }
    bool operator==(const CThingListIterator& other) const { return _M_node == other._M_node; }
};

struct SThingListValueNode : SThingListNode {
    CIntelligentPointer<const CThing> _M_data;      // +0x08
};
namespace std {
#pragma auto_inline(off)
// Same-TU stand-in for the STLport COMDAT 0x0076D5D0 (the retail body: it reads the value and never touches the tag),
// so VC7.1 sees that neither reference escapes: the tag shares the `enemy` slot and `enemy` stays in a register.
__declspec(noinline) CThingListIterator __fastcall __find(CThingListIterator first, CThingListIterator last,
                                                          CThing* const& value, const input_iterator_tag&)   // 0x0076D5D0
{
    while (!(first == last) && ((SThingListValueNode*)first._M_node)->_M_data.GetPItem() != value)
        first._M_node = first._M_node->_M_next;
    return first;
}
#pragma auto_inline(on)
inline CThingListIterator find(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    return __find(first, last, value, input_iterator_tag());
}
}

template<class T> struct CRetailList {              // STLport list: the sentinel node pointer
    SThingListNode* _M_node;
    CThingListIterator begin() const { return CThingListIterator(_M_node->_M_next); }
    CThingListIterator end() const { return CThingListIterator(_M_node); }
    void push_back(const T& value);                 // 0x0065617D
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
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
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
    void AddEnemyThing(const CThing* enemy);
};

void CTCEnemy::AddEnemyThing(const CThing* enemy)
{
    CThingListIterator end = EnemyThings.end();
    if (std::find(EnemyThings.begin(), end, const_cast<CThing* const&>(enemy)) == end) {
        EnemyThings.push_back(CIntelligentPointer<const CThing>(enemy));
        if (enemy->HasTC(0x51) && ParentThing->HasTC(0x4f))
            ((CTCCreatureOpinionOfHero*)ParentThing->GetTC(0x4f))->SetHeroOpinionEnemy(true);
    }
}
