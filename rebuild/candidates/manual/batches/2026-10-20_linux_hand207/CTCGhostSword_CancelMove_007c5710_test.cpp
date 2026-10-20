#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                    // CIntelligentPointer<CThingAICreature>: 8 bytes
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
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

struct CTCGhostSword {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void* PDef;                                     // +0x0C CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CBaseIntelligentPointer> Minions; // +0x10
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

// ---- test stubs for the retail callees ----
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() {}
void CTCGhostSword::v00() {}

int main()
{
    int ok = 1;
    CTCMinionTimer t1, t2, t3;
    t1.Stopped = false; t1.Current = 1; t1.Target = 10;
    t2.Stopped = true;  t2.Current = 2; t2.Target = 20;
    t3.Stopped = false; t3.Current = 3; t3.Target = 30;
    CInterfaceMap::SEntry e1, e2, e3;
    e1.type = 0x9b; e1.component = (CTCBase*)&t1; e2.type = 0x9b; e2.component = (CTCBase*)&t2; e3.type = 0x9b; e3.component = (CTCBase*)&t3;
    CThing m1, m2, m3;
    std::memset(&m1, 0, sizeof(m1)); std::memset(&m2, 0, sizeof(m2)); std::memset(&m3, 0, sizeof(m3));
    m1.m_interfaces.first = &e1; m1.m_interfaces.last = &e1 + 1; m1.m_presentBits[4] = 1u << 27;
    m2.m_interfaces.first = &e2; m2.m_interfaces.last = &e2 + 1; m2.m_presentBits[4] = 1u << 27;
    m3.m_interfaces.first = &e3; m3.m_interfaces.last = &e3 + 1; m3.m_presentBits[4] = 1u << 27; m3.m_flags91 = 1;
    CBaseIntelligentPointer minions[4];
    minions[0].m_pItem = &m1; minions[1].m_pItem = &m2; minions[2].m_pItem = &m3; minions[3].m_pItem = 0;
    CTCGhostSword gs; gs.Minions._M_start = minions; gs.Minions._M_finish = minions + 4; gs.Minions._M_end_of_storage = minions + 4;
    gs.CancelMove();
    if (!t1.Stopped || t1.Current != 10) ok = 0;     // stopped once
    if (!t2.Stopped || t2.Current != 2) ok = 0;      // already stopped: untouched
    if (t3.Stopped || t3.Current != 3) ok = 0;       // dead minion: untouched
    gs.Minions._M_finish = minions; gs.CancelMove();  // empty
    if (!ok) { std::printf("007c5710_TEST FAIL\n"); return 1; }
    std::printf("007c5710_TEST PASS\n");
    return 0;
}
