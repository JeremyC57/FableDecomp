// CAIStateGroup_TavernKeeper::InitStateGroup(const CWorld&, CThingAICreature&) -- cache the walk speed, reset the shop stock state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009685C0, length 144 bytes.
// Callees: the creature's virtual slot 79 (+0x13C, a float speed query taking a const C3DVector&),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x90 = bit 16 of the presence word at +0x30),
// CTCShopKeeper::GetShopTC @ 0x00798F50 (x2), CTCShop::SetStockState(long) @ 0x00773C00.
//
// Shape: AverageWalkSpeed (+0x60, PDB name) = PBrain->PCreature->vslot79(C3DVector(0,0,0)): the zero vector is
// a NAMED local (three immediate float-zero stores into the frame BEFORE the creature load; an unnamed
// temporary argument lets VC hoist the PCreature load above the stores), the result stored with `fstp`. Then the shopkeeper component (bool GetInterface) with a shop resets the shop's stock
// state; GetShopTC is called twice (test, then use). Both parameters are unused (`ret 8`). `this` is the PDB
// header layout; PBrain lives in the flattened base bytes at +0x04.
// Leaf: InitStateGroup
#include "engine/CAIStateGroup_TavernKeeper.h"

class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class CThing { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual float GetAverageWalkSpeed(const C3DVector& direction);   // slot 79 (+0x13c)
    unsigned char _pad0[0x1c];                  // +0x04
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CTCShop { public:
    void SetStockState(long state);             // 0x00773C00
};

class CTCShopKeeper { public:                          // interface type 0x90 component
public:
    CTCShop* GetShopTC() const;   // 0x00798F50
};

class CWorld;
class CThingAICreature;

struct CAIStateGroup_TavernKeeper_Methods : CAIStateGroup_TavernKeeper {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_pad_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_TavernKeeper_Methods::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    C3DVector direction(0.0f, 0.0f, 0.0f);
    AverageWalkSpeed = GetPBrain()->PCreature->GetAverageWalkSpeed(direction);
    CTCShopKeeper* shopKeeper;
    if (GetPBrain()->PCreature->GetInterface(&shopKeeper, 0x90) && shopKeeper->GetShopTC() != 0)
        shopKeeper->GetShopTC()->SetStockState(0);
}