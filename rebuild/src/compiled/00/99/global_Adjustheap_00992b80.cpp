// _global::_Adjust_heap(std::pair<long,CSoundBankRecord>*, long, long, std::pair<long,CSoundBankRecord>, CKeyPairCompareLess) -- Dinkumware heap sift-down of 36-byte (long key, sound-bank record) pairs by signed key, then _Push_heap @ 0x00992940; destroys the by-value pair
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00992b80, 184 bytes.
// Callees: the record's copy constructor 0x009922F0 (landed as SRecord::SRecord), operator=
// 0x00992630 (landed as "CCamera::UpdateCleanupFlags", a propagated label), destructor 0x00992510
// (landed as CSoundBankRecord::~CSoundBankRecord), _Push_heap 0x00992940.
// first in ecx, hole in edx (link-time __fastcall); bottom, the 36-byte pair by value and the empty
// comparator follow (`ret 2Ch`); `top` lives in the `push ecx` slot.
// Element: 0x24 bytes (`lea eax,[esi+esi*8]` * 4) = long key +0, the 0x20-byte sound-bank record +4
// {two counted pointers, four CCharStrings} -- the elements of the CASoundManager vector the landed
// ~CSoundBankRecord describes.  Shape: implicit pair operator= = key move + the record's out-of-line
// operator= (result dropped); implicit pair copy constructor = key store + out-of-line record copy;
// the by-value pair's record is destroyed after the push.  The key test is a SIGNED `jge`.
// Label correction: pair<long,EHeroAbility> is 8 bytes; this 36-byte element is not it.
namespace FableHeap_00992b80 {

struct CSoundBankRecord {                                  // 0x20 bytes (see the landed 0x00992510)
    unsigned char m_body[0x20];
    CSoundBankRecord(const CSoundBankRecord& other);              // 0x009922F0
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                                          // 0x00992510
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CSoundBankRecord> SKeyedRecord;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedRecord& a, const SKeyedRecord& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(SKeyedRecord* first, long hole, long top, SKeyedRecord value, CKeyPairCompareLess pred);   // 0x00992940

void __fastcall _Adjust_heap(SKeyedRecord* first, long hole, long bottom, SKeyedRecord value, CKeyPairCompareLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}