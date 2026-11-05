// std::vector<CSoundBankEntry>::push_back(const CSoundBankEntry& x) -- STLport push_back of a 36-byte (key, sound-bank record) entry: copy-construct at finish and advance, else grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@U?$pair@JVCSoundBankRecord@@@std@@
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009936E0, length 69 bytes (+ int3 padding), `ret 4`.
// Callees: the implicit CSoundBankRecord copy constructor out of line @ 0x009922F0 (landed as SRecord::SRecord),
// std::vector<CSoundBankEntry>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00993310.
// Label: the manifest's "NScript_CIsThingAlive_Vector_PushBack" is propagated.
// Element: std::pair<long, CSoundBankRecord> (0x24 bytes) -- the CASoundManager's bank map entries (see the landed
// ~CSoundBankRecord 0x00992510 and the sort family 0x00992750..0x00993420); record = two CCountedPointers + four
// CCharStrings with an implicit copy constructor (here called out of line, retail 0x009922F0) and out-of-line
// assignment / destructor. Record / comparator names are descriptive (the sound module is absent from the Ego PDB).
// Shape: STLport 4.6 push_back -- placement-new copy under the null test (the pair's key stored inline, the record copied
// out of line), `add [esi+4],24h`; the overflow arm's __false_type temporary is homed on the x argument slot.
#include <new>
#include <stddef.h>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class CCPPointerInfo {
public:
    long RefCount;                              // +0x00
};

template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CSoundBank;
class CSoundBankData;

// The 0x20-byte sound-bank record (descriptive; see the landed ~CSoundBankRecord @ 0x00992510): implicit copy
// constructor, out-of-line assignment and destructor.
class CSoundBankRecord {
public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                        // 0x00992510
};

#include "engine/CVectorMap.h"

typedef std::pair<long, CSoundBankRecord> CSoundBankEntry;     // 0x24 bytes

// CVectorMap<long, CSoundBankRecord>'s key order (an empty functor passed by value)
struct CSoundBankEntryLess {
    bool operator()(const CSoundBankEntry& a, const CSoundBankEntry& b) const { return a.first < b.first; }
};




template void std::vector<CSoundBankEntry>::push_back(const CSoundBankEntry&);