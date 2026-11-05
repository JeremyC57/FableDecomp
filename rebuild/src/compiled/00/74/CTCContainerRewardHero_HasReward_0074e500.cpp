// CTCContainerRewardHero::HasReward() -- true (1) when the container holds items or the parent thing's gold component (interface 204) holds a positive amount; 0 otherwise
// Leaf: ?HasReward@CTCContainerRewardHero@@QAEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074E500, length 96 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (another instantiation of the
// 0x0040F020 search), CTCGold::GetGold @ 0x007CBDC0.
// Layout (retail): CTCBase::ParentThing +4, CTCContainer::ContainerItems +0xc (vector<pair<long,
// CCharString> >: start +0xc, finish +0x10, 8-byte elements). CThing: the interface presence bitset +0x20
// (type 204 -> word 6 bit 12: `mov edx,[ecx+38h] ; test dh,10h`), the sorted interface map +0x44.
// Shape: the gold is a zero-initialised local (`xor eax,eax` before the presence test) assigned the call
// result inside the landed out-parameter GetInterface template (0x007504E0; the key in the `push ecx`
// frame slot); `size() > 0 || gold > 0` is `sar ecx,3 ; test ; ja` then `test eax,eax ; jg`; a `long`
// result (`mov eax,1` / `xor eax,eax`).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCGold;

class CTCGold { public:
    long GetGold() const;                         // 0x007CBDC0
};

struct SContainerItem {                     // pair<long, CCharString>: 8 bytes
    long  first;
    void* second;
};

class CTCContainerRewardHero { public:
    void*           __vftable;              // +0x00
    CThing*         ParentThing;            // +0x04 (CTCBase)
    unsigned char   _pad08[4];              // +0x08
    SContainerItem* ItemsStart;             // +0x0c ContainerItems
    SContainerItem* ItemsFinish;            // +0x10
    SContainerItem* ItemsEndOfStorage;      // +0x14
    unsigned long ItemCount() const { return ItemsFinish - ItemsStart; }
    long HasReward();
};

long CTCContainerRewardHero::HasReward()
{
    long gold = 0;
    const CTCGold* goldComponent;
    if (ParentThing->PeekTC(TCI_GOLD, &goldComponent))
        gold = goldComponent->GetGold();
    if (ItemCount() > 0 || gold > 0)
        return 1;
    return 0;
}