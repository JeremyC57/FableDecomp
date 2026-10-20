// CTCVillage::RemoveTimedoutCrimes() -- drops every crime handle the hero's deed log no longer knows (the deed timed out); with no crimes left the village is no longer the hero's enemy because of crime
// Leaf: RemoveTimedoutCrimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A7A9, 169 bytes (plain `ret`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?RemoveTimedoutCrimes@CTCVillage@@QAEXXZ`.
// Callees: CWorld::GetCurrentPlayer() const 0x0049D9D0 (g_pMainGameComponent 0x013B86A0 +0x24); CPlayer::GetHeroThing()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020; the STLport tree increment Successor 0x004292C0 (`__cdecl`);
//          CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0; set<long>::erase(iterator) 0x00638D70 (the iterator
//          by value, built in its argument slot).
// Layout: retail CTCVillage PlayersCrimes (set<long>: header node, node count) +0x64/+0x68, IsEnemyBecauseOfCrime +0x7c
//         (PDB +0x108).
// Shape: the deed log comes from the unchecked out-parameter GetInterface (its address taken, so it lives in a frame word); the set is reached through a named pointer local (ebx, so `this` is
//        spilled to [ebp-8]); end() is cached (edi); the walk is the landed RemoveApologiseableCrimes 0x0065CE56 shape: a
//        separate `next` copy (kept in the lookup key's frame word) advanced before the lookup, the current node erased
//        when its deed is gone, then `it = next` (the `crime = it; ++it` and `it++` forms keep `next` in a register and
//        shrink the frame); the iterator has a user copy
//        constructor so erase's by-value argument is built in place (`push ecx ; mov eax,esp ; mov [eax],esi`).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};

struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
struct CMainGameComponent {
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0


struct COpinionDeed {
    long DeedType;                              // +0x00 EOpinionDeedType
};
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

struct SCrimeIterator {                         // STLport set<long>::iterator
    SNode* _M_node;
    SCrimeIterator(SNode* node) : _M_node(node) { }
    SCrimeIterator(const SCrimeIterator& other) : _M_node(other._M_node) { }
    long operator*() const { return _M_node->_M_value_field; }
    SCrimeIterator& operator++() { _M_node = Successor(_M_node); return *this; }
    bool operator!=(const SCrimeIterator& other) const { return _M_node != other._M_node; }
};
struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    SNode*        _M_header;                    // +0x00
    unsigned long _M_node_count;                // +0x04
    unsigned long _M_key_compare;               // +0x08 (empty less<long>, padded)
    SCrimeIterator begin() const { return SCrimeIterator(_M_header->_M_left); }
    SCrimeIterator end() const { return SCrimeIterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
    void erase(SCrimeIterator position);        // 0x00638D70
};

struct CTCVillage {
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    void RemoveTimedoutCrimes();
};

void CTCVillage::RemoveTimedoutCrimes()
{
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CCrimeSet* crimes = &PlayersCrimes;
        SCrimeIterator end = crimes->end();
        SCrimeIterator it = crimes->begin();
        SCrimeIterator next = it;
        while (it != end) {
            ++next;
            if (!log->GetDeedFromHandle(*it))
                crimes->erase(it);
            it = next;
        }
    }
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}