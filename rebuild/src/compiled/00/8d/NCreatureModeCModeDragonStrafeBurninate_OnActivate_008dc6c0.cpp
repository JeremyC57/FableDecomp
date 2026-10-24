// NCreatureMode::CModeDragonStrafeBurninate::OnActivate() -- the flying activation, the leg-animation blend reset to 0, and the type-9 component switched on
// Leaf: ?OnActivate@CModeDragonStrafeBurninate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC6C0, 85 bytes (+ int3 padding), `ret`; vtable 0x01268718 slot 5.
// Callees: CModeDragonFlying::OnActivate @ 0x008DBB30 (qualified), CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter
//   GetInterface, type 9 = bit 9 of the +0x20 dword).
// Name: FableWin.pdb `?OnActivate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ` (slot 5).
// Layout (retail, the landed ctor 0x008E1580): CModeDragonFlyingBase LegAnimAlpha +0x34; the type-9 component's flag +0x10.
// Shape: the GetInterface result is not tested (as the landed CModeCarryingStandBase::FrameUpdate 0x008D88A0): the absent
//   path stores through the never-written out variable in the key's frame word, and VC7.1 duplicates the store and the
//   epilogue into both paths.
struct CTCBase;

struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);  // 0x0040F020
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];        // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;            // +0x44
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
    void OnActivate();
};
}

void NCreatureMode::CModeDragonStrafeBurninate::OnActivate()
{
    CModeDragonFlying::OnActivate();
    LegAnimAlpha = 0.0f;
    CTCType9* tc;
    ParentThing->GetInterface(&tc, 9);
    tc->m_active = true;
}