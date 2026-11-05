// CTCBoastingPodium::GetNumberOfActiveBoastingPositions() const -- count the podium's boasting positions whose position component reports itself activated
// Leaf: ?GetNumberOfActiveBoastingPositions@CTCBoastingPodium@@QBEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00813B80, length 198 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0
// (inlined GetInterface for CTCBoastingPosition, type 0x71 = bit 17 of CThing+0x2C), CTCBoastingPosition::IsPositionActivated
// (long&) const @ 0x00781D80 (?IsPositionActivated@CTCBoastingPosition@@QBE_NAAJ@Z).
// Layout: Ego_r.pdb CTCBoastingPodium BoastingPositions +0x0C (CVectorMap<long, CIntelligentPointer<CThing>>: a retail 12-byte
// vector of 12-byte {key, intelligent pointer} pairs).
// Shape: `for (unsigned long i = 0; i < size(); ++i)` with size() re-evaluated (the divide by 12 at the loop tail) and the element
// reached through `*(begin() + i)`; the unchecked out-parameter GetInterface shares its key word with the never-initialised
// position pointer (held in edi across iterations, as retail does); the activation cost is an unused out local.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct SBoastingPositionEntry {                 // pair<long, CIntelligentPointer<CThing> >
    long                    first;
    CIntelligentPointer<CThing> second;
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
class CTCBoastingPosition {
public:
    bool IsPositionActivated(long& cost) const; // 0x00781D80
};
class CTCBoastingPodium {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CRetailVector<SBoastingPositionEntry> BoastingPositions;   // +0x0c
    long GetNumberOfActiveBoastingPositions() const;
};

long CTCBoastingPodium::GetNumberOfActiveBoastingPositions() const
{
    long count = 0;
    for (unsigned long i = 0; i < BoastingPositions.size(); ++i) {
        const CTCBoastingPosition* position;
        BoastingPositions[i].second.Get()->PeekTC(TCI_BOASTING_POSITION, &position);
        long cost;
        if (position->IsPositionActivated(cost))
            ++count;
    }
    return count;
}