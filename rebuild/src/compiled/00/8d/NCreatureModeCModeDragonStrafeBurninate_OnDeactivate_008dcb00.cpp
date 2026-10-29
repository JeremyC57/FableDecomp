// NCreatureMode::CModeDragonStrafeBurninate::OnDeactivate() -- switch the type-9 component off, end a still-active flame beam, then the flying deactivation
// Leaf: ?OnDeactivate@CModeDragonStrafeBurninate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DCB00, 94 bytes (+ int3 padding), `ret`; vtable 0x01268718 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface, type 9 = bit 9 of the +0x20 dword),
//   CModeDragonStrafeBurninate::EndBeamEffect @ 0x008D7BB0 (landed; manifest "CTCKraken::EndHeadEffects" is propagated),
//   CModeDragonFlying::OnDeactivate @ 0x008DC590 (landed, qualified).
// Name: FableWin.pdb `?OnDeactivate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ` (slot 6).
// Layout (retail, the landed ctor 0x008E1580): FlameActive +0xbc; the type-9 component's flag +0x10.
// Shape: the GetInterface result is not tested: the absent path joins with the never-written out variable (`mov eax,[esp+4]`)
//   before the single flag store.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
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

struct CTCType9 {                          // the type-9 component (as the landed 0x008E7B90)
    unsigned char _pad00[0x10];
    bool          m_active;                // +0x10
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

namespace NCreatureMode {
struct __declspec(novtable) CModeDragonFlyingBase : CModeBase {
    unsigned char FlightSpline[0x14];      // +0x1c CCardinalSpline
    float         SplinePercentageComplete;   // +0x30
    float         LegAnimAlpha;            // +0x34
};
struct CModeDragonFlying : CModeDragonFlyingBase {   // retail sizeof 0x94
    unsigned char _flying[0x94 - 0x38];
    void OnActivate();                     // 0x008DBB30
    void OnDeactivate();                   // 0x008DC590
};
struct CModeDragonStrafeBurninate : CModeDragonFlying {   // vtable 0x01268718
    long          Timer;                   // +0x94
    unsigned char PBeamWeaponEffect[8];    // +0x98 CIntelligentPointer<CThing>
    long          LastExplodeFrame;        // +0xa0
    float         FlameStartPos[3];        // +0xa4
    float         FlameEndPos[3];          // +0xb0
    bool          FlameActive;             // +0xbc
    float         FlameStartPercentage;    // +0xc0
    float         FlameStopPercentage;     // +0xc4
    void EndBeamEffect();                  // 0x008D7BB0
    void OnDeactivate();
};
}

void NCreatureMode::CModeDragonStrafeBurninate::OnDeactivate()
{
    CTCType9* tc;
    ParentThing->GetInterface(&tc, 9);
    tc->m_active = false;
    if (FlameActive)
        EndBeamEffect();
    CModeDragonFlying::OnDeactivate();
}