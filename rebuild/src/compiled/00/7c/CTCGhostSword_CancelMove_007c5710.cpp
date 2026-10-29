// CTCGhostSword::CancelMove() -- stop the interface-0x9B timer component of every live ghost-sword minion once (copy its target into its current value and set its stopped flag)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C5710, length 175 bytes (+ int3 padding). __fastcall, `ret`.
//         Ego_r.pdb: `?CancelMove@CTCGhostSword@@QAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE pair, then the lookup thing),
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (interface 0x9B, word 4 bit 27).
// Shape: an unsigned index loop over Minions (`size()` re-read each pass, `jb`), each element reached through the
//        STLport `*(begin() + i)` operator[] (the scaled index kept in edi, begin() re-read for the third GetPItem);
//        the timer is the out-parameter GetInterface; the stop is the landed CTCSummonSpell::CancelMove (0x0079A820) step.
// Layout: retail CTCGhostSword: PDef +0x0C, Minions +0x10 (12-byte STLport vector of CIntelligentPointer<CThingAICreature>).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCMinionTimer {                             // interface 0x9B component
    unsigned char _pad00[0x0c];
    bool  Stopped;                                  // +0x0C
    unsigned char _pad0d[7];
    long  Current;                                  // +0x14
    long  Target;                                   // +0x18
};

class CTCGhostSword { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void* PDef;                                     // +0x0C CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CIntelligentPointer<CThing> > Minions; // +0x10
    void CancelMove();
};

void CTCGhostSword::CancelMove()
{
    for (unsigned long i = 0; i < Minions.size(); ++i) {
        if (IS_THING_ALIVE(Minions[i])) {
            CTCMinionTimer* timer;
            if (Minions[i].GetPItem()->GetInterface(&timer, 0x9b)) {
                if (!timer->Stopped) {
                    timer->Current = timer->Target;
                    timer->Stopped = true;
                }
            }
        }
    }
}