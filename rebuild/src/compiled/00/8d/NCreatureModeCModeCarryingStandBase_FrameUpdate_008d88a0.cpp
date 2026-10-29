// NCreatureMode::CModeCarryingStandBase::FrameUpdate() -- base frame update, then keep the carrying pose as a combination anim (type 0x18): drop it while the main anim is of type 1, otherwise add the derived mode's carrying animation when it is not already playing
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D88A0, 181 bytes (`ret`; vtable 0x01267400 slot 3).
// Callees: CTCCreatureModeManager::CModeBase::FrameUpdate 0x0073AC50 (qualified base call); CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, CTCAnimationComplex = type 0x5a, bit 26 of +0x28; the result is used
// unconditionally); CTCAnimationComplex::GetAnimation 0x0070B460 (manifest "CWorld::DrawGetEnvironment" is propagated);
// CAnimateThing::FindCombinationAnim 0x0070C4E0, AddCombinationAnim(const CAnimInit&, long, long, bool, bool) 0x0070D780,
// RemoveCombinationAnimsOfType 0x0070D8C0 (manifest "CSpellContainerList::AddChild" is propagated); CAnimInit::Set 0x0070C050
// (manifest "CInitRumble"); this mode's pure virtual slot 12 (+0x30: the carrying animation index, -1 for none).
// Layout: CModeBase ParentThing +0x04; CAnimateThing's main CAnimSet type byte +0x50.
// Shape: the CAnimInit record (inline member zeroing + Set) as in CModeBlock::AnimUpdate 0x008D8DD0; the zero words reuse the
// null result of FindCombinationAnim in eax.
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
    unsigned char _pad0[0x20];
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

struct CBlendPair {                        // the two floats at CAnimInit +0x18, zeroed by their ctor
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};

class CAnimInit { public:                         // 0x2c bytes
    long          Mode;                    // +0x00
    unsigned char AnimType;                // +0x04
    bool          Flag05;                  // +0x05
    long          AnimIndex;               // +0x08
    float         Scales[3];               // +0x0c
    CBlendPair    Blend;                   // +0x18
    unsigned long Flags;                   // +0x20
    long          Arg24;                   // +0x24
    long          Percent;                 // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};

class CAnimSet;

class CAnimateThing { public:
    unsigned char _pad00[0x50];
    unsigned char CurrentAnimType;         // +0x50 (main CAnimSet)
    CAnimSet* FindCombinationAnim(long type);   // 0x0070C4E0
    void AddCombinationAnim(const CAnimInit& init, long a, long b, bool c, bool d);   // 0x0070D780
    void RemoveCombinationAnimsOfType(long type);   // 0x0070D8C0
};

class CTCAnimationComplex { public:               // interface type 0x5a
    CAnimateThing& GetAnimation();         // 0x0070B460
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void FrameUpdate();            // slot 3, 0x0073AC50
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};

struct CModeStandBase : CModeBase {        // retail 0x68 (does not override FrameUpdate)
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {

struct CModeCarryingStandBase : CModeStandBase {   // vtable 0x01267400
    virtual void FrameUpdate();
    virtual long GetCarryingAnimIndex() = 0;        // slot 12 (+0x30)
};

void CModeCarryingStandBase::FrameUpdate()
{
    CModeStandBase::FrameUpdate();
    CTCAnimationComplex* animation;
    ParentThing->GetInterface(&animation, 0x5a);
    CAnimateThing& animate = animation->GetAnimation();
    if (animate.CurrentAnimType != 1) {
        long index = GetCarryingAnimIndex();
        if (index != -1 && animate.FindCombinationAnim(0x18) == 0) {
            CAnimInit init(0x18, 0, index, 1, true, 0, 100);
            animate.AddCombinationAnim(init, 0, 0, false, false);
        }
    }
    else
        animate.RemoveCombinationAnimsOfType(0x18);
}

}