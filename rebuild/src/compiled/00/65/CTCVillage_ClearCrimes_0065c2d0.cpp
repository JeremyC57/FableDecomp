// CTCVillage::ClearCrimes() -- forget every crime the hero's deed log still knows (RemoveCrime on each handle), then reset the fines paid and the crime-enemy flag
// Leaf: ?ClearCrimes@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065C2D0, 158 bytes, thiscall `leave ; ret`. Name: Ego_r.pdb `?ClearCrimes@CTCVillage@@QAEXXZ`
// (name set); the landed callers EnableGuards 0x0065C45A ("most likely ClearCrimes") and PayFines 0x0065C760 call it,
// plus 0x004A547E, 0x0065C54F, 0x006A95CB, 0x007B36FE, 0x007E2BEF.
// Callees: CWorld::GetCurrentPlayer 0x0049D9D0 (g_pMainGameComponent 0x013B86A0 +0x24), CPlayer::GetPControlledCreature 0x00487DC0,
// std::bitset::test 0x00410DE0 (interface 0x51 = CTCHeroOpinionDeedLog), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// 0x0040F020, _Rb_global<bool>::_M_increment 0x004292C0 (cdecl), CTCHeroOpinionDeedLog::GetDeedFromHandle 0x006C58F0,
// CTCVillage::RemoveCrime 0x0065C146 (landed).
// Label correction: the catalog "CTCHeroOpinionDeedLog_ProcessCrimeDeeds" is descriptive; this is CTCVillage::ClearCrimes.
// Shape: the landed RemoveCrime model: the deed log from the unchecked out-parameter GetInterface (the absent arm leaves
// the log's frame word [ebp-8] unwritten -- it shares the interface key's home), then a walk of PlayersCrimes with end()
// cached in a named iterator (edi survives the RemoveCrime calls that erase from the set), the handle read before the
// `++it` (so erasing the current node is safe), RemoveCrime for every handle the log still has a deed for; finally
// AmountOfFineAlreadyPaid = 0 (`and dword ptr [ebx+0A0h],0`) and IsEnemyBecauseOfCrime = false.
// Layout: retail CTCVillage PlayersCrimes (set<long>: header, count, compare) +0x64, IsEnemyBecauseOfCrime +0x7c,
// AmountOfFineAlreadyPaid +0xa0 (PDB +0x12C; retail lists are 4 bytes) -- the landed CTCVillage local model.
// Opt: frame-pointer size TU: `optimize("y", off)` + `optimize("s", on)`.
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
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
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
};

struct _Rb_tree_node_base {
    bool                _M_color;               // +0x00
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};
struct SCrimeNode : _Rb_tree_node_base {
    long _M_value_field;                        // +0x10
};
_Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0 _Rb_global<bool>::_M_increment

struct CCrimeSetIterator {
    _Rb_tree_node_base* _M_node;
    CCrimeSetIterator(_Rb_tree_node_base* x) : _M_node(x) { }
    long operator*() const { return ((SCrimeNode*)_M_node)->_M_value_field; }
    CCrimeSetIterator& operator++() { _M_node = _M_increment(_M_node); return *this; }
    bool operator!=(const CCrimeSetIterator& o) const { return _M_node != o._M_node; }
};

struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    _Rb_tree_node_base* _M_header;              // +0x00
    unsigned long       _M_node_count;          // +0x04
    unsigned long       _M_key_compare;         // +0x08 (empty less<long>, padded)
    CCrimeSetIterator begin() { return CCrimeSetIterator(_M_header->_M_left); }
    CCrimeSetIterator end() { return CCrimeSetIterator(_M_header); }
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    unsigned char _pad_0x7d[0x23];
    long          AmountOfFineAlreadyPaid;      // +0xa0
    void RemoveCrime(long deedHandle);          // 0x0065C146
    void ClearCrimes();
};

void CTCVillage::ClearCrimes()
{
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CCrimeSetIterator last = PlayersCrimes.end();
        for (CCrimeSetIterator it = PlayersCrimes.begin(); it != last; ) {
            long handle = *it;
            ++it;
            if (log->GetDeedFromHandle(handle))
                RemoveCrime(handle);
        }
    }
    AmountOfFineAlreadyPaid = 0;
    IsEnemyBecauseOfCrime = false;
}