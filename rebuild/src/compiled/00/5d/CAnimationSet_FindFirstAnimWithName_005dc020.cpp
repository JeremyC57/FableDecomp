// CAnimationSet::FindFirstAnimWithName(const CCharString& name) const -- the first animation-map entry whose key is the name's CRC, or end() when there is none
// Leaf: ?FindFirstAnimWithName@CAnimationSet@@
// Retail: Fable.exe @ 0x005DC020, 80 bytes, __fastcall `ret 4`. Ego_r.pdb
// `?FindFirstAnimWithName@CAnimationSet@@QBE?AV?$_Vector_const_iterator@...pair@KVCAnimationEntry@@...@std@@ABVCCharString@@@Z`
// (an STLport raw `const pair*` iterator in TLC). Callers: CAnimationSet::GetNoAnims 0x005DC290 and its siblings
// 0x005DC2E0/0x005DC340, 0x005DCC30, 0x005DCD50, CThingCreatureBase helpers 0x0066696C/0x00667B74.
// Callees: CCRC::Calc(unsigned long, const void*, unsigned long) @ 0x004014A0 (static `__fastcall`, Ego_r `SIKKPBXK@Z`),
// CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) const
// @ 0x005DF960 (20-byte pairs). Empty-string literal 0x0129AAF4.
// Layout: Ego_r CAnimationSet: Anims (the CVectorMap, its vector's begin/end at +0/+4) first; CAnimationEntry 16 bytes;
// CCharString::PStringData -> CCharStringData { CBasicString<char> Data (chars +0, length +4); ... }.
// Shape: speed TU; CCharString::GetCRC() inlined (length then chars, each under its own null-first test: right-to-left
// arguments); the key is homed in the dead `name` argument slot for LowerBound's reference and kept in edi for the
// compare (needs LowerBound's same-TU body; declared only, the key is re-read from the slot); end() is read once.
// Label: the catalog's CVectorMap_LookupOrGetEnd is this method.
class CCharStringData {
public:
    const char*   Data;                                 // +0x00 CBasicString<char>
    unsigned long Length;                               // +0x04
};
extern const char g_EmptyCharString[];                  // 0x0129AAF4 ""
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long crc, const void* data, unsigned long length);   // 0x004014A0
};
class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    const char* GetAsCharArray() const { return PStringData == 0 ? g_EmptyCharString : PStringData->Data; }
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetAsCharArray(), GetLength()); }
};
class CAnimationEntry {                                 // 16 bytes (Ego_r): BankIndex, AnimName, GroupName, Components
public:
    long  BankIndex;
    void* AnimName;
    void* GroupName;
    void* Components;
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Cmp> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const value_type* _M_start;                         // +0x00 (the STLport vector)
    const value_type* _M_finish;                        // +0x04
    const value_type* _M_end_of_storage;                // +0x08
    const_iterator end() const { return _M_finish; }
    const_iterator LowerBound(const K& key) const;      // 0x005DF960
};
#pragma auto_inline(off)
// Same-TU body (retail instantiates the map template in this TU): STLport lower_bound over the sorted pairs; reading
// the key through its reference and never writing it lets the caller keep the key in edi across the call.
template <class K, class V, class Cmp>
__declspec(noinline) typename CVectorMap<K, V, Cmp>::const_iterator CVectorMap<K, V, Cmp>::LowerBound(const K& key) const
{
    const value_type* first = _M_start;
    int len = _M_finish - _M_start;
    while (len > 0) {
        int half = len / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)
class CAnimationSet {
public:
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;                                     // +0x00
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const;
};
CAnimationSet::TAnimMap::const_iterator CAnimationSet::FindFirstAnimWithName(const CCharString& name) const
{
    unsigned long key = name.GetCRC();
    TAnimMap::const_iterator it = Anims.LowerBound(key);
    if (it == Anims.end() || it->first != key)
        return Anims.end();
    return it;
}