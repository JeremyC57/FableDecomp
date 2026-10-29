// CTCChest::UpdateAnimation(float time) -- advance the chest's animation (its CTCAnimationComplex's animate-thing) by the given time step
// Retail 0x00750470, 110 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`mov eax,[ecx+0Ch]`; manifest
// "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::UpdateFrameLogic(const update&) @ 0x0070E8B0.
// Shape: the component comes from the zero-initialised out-parameter GetInterface (its result is not tested) whose
// entry pick is one end/hit ternary: the preloaded null stays in ecx (`xor ecx,ecx`) and a hit loads the component
// straight into ecx, with the whole-word bit test (`test edx,4000000h`); the 12-byte update record {time, 0, 0} is a
// frame local whose inline constructor zeroes the two reserved words in its init list and assigns the time in the
// body (the zero words are stored before the time).
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    long  Reserved0;                                        // +0x04
    long  Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Reserved0(0), Reserved1(0) { Time = time; }
};

class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

class CTCAnimationComplex { public:
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

class CTCChest { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void UpdateAnimation(float time);
};

void CTCChest::UpdateAnimation(float time)
{
    CTCAnimationComplex* complex = 0;
    ParentThing->GetInterface(&complex, 0x5a);
    CAnimFrameUpdate update(time);
    CAnimateThing* animation = complex->GetAnimation();
    animation->UpdateFrameLogic(update);
}