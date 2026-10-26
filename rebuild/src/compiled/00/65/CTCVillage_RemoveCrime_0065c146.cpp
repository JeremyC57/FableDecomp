// CTCVillage::RemoveCrime(long deedHandle) -- forgets one of the hero's crimes: the deed log drops the deed and every witness deed chained to it, the village drops the handle, and with no crimes left the village is no longer the hero's enemy because of crime
// Leaf: ?RemoveCrime@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065C146, 182 bytes (`ret 4`, `leave`): frame-pointer size TU,
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Ego_r.pdb `?RemoveCrime@CTCVillage@@QAEXJ@Z`.
// Callees: CWorld::GetCurrentPlayer() const 0x0049D9D0 (g_pMainGameComponent 0x013B86A0 +0x24); CPlayer::GetHeroThing()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 0x51); CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound 0x0040F020; CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0,
//          ::RemoveDeedStillSearchingForWitnesses(long) 0x006C5920, ::RemoveDeed(long) 0x006C59E0 (landed spellings),
//          ::FindWitnessDeed(long) 0x006C5710 (Ego_r.pdb `?FindWitnessDeed@CTCHeroOpinionDeedLog@@QAEJJ@Z`);
//          set<long>::erase(const long&) 0x00639590.
// Layout: retail CTCVillage PlayersCrimes (set<long>: header node, node count) +0x64/+0x68, IsEnemyBecauseOfCrime +0x7c.
// Shape: the deed log comes from the unchecked out-parameter GetInterface (the absent arm reads the never-written log's
//        home, the handle's argument slot -- the handle itself lives on in ebx); the witness chain is a `for (;;)` that
//        removes each deed found for the ORIGINAL handle; `this` is spilled to [ebp-4]; the set erase takes the handle
//        parameter by address.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
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

class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0


class CTCHeroOpinionDeedLog { public:
    class COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    long FindWitnessDeed(long handle);          // 0x006C5710
    void RemoveDeedStillSearchingForWitnesses(long handle);   // 0x006C5920
    void RemoveDeed(long handle);               // 0x006C59E0
};

struct SNode;
struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    SNode*        _M_header;                    // +0x00
    unsigned long _M_node_count;                // +0x04
    unsigned long _M_key_compare;               // +0x08 (empty less<long>, padded)
    unsigned long size() const { return _M_node_count; }
    unsigned long erase(const long& key);       // 0x00639590
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    void RemoveCrime(long deedHandle);
};

// R&D I-3 sweep: a same-TU out-of-line set<long>::erase(const long&) (the key is only read) lets the crime handle stay
// in its argument slot as in retail.
#pragma auto_inline(off)
unsigned long CCrimeSet::erase(const long&)
{
    unsigned long erased = _M_node_count != 0;
    _M_node_count -= erased;
    return erased;
}
#pragma auto_inline(on)
void CTCVillage::RemoveCrime(long deedHandle)
{
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        long handle = deedHandle;
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        if (log->GetDeedFromHandle(handle)) {
            log->RemoveDeedStillSearchingForWitnesses(handle);
            long deed = handle;
            for (;;) {
                log->RemoveDeed(deed);
                long witness = log->FindWitnessDeed(handle);
                if (witness == -1)
                    break;
                log->RemoveDeedStillSearchingForWitnesses(witness);
                deed = witness;
            }
        }
    }
    PlayersCrimes.erase(deedHandle);
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}