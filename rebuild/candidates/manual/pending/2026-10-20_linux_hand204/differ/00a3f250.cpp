// Grade: DIFFER(236v236) -- register naming only: retail loads soundName into edx (then lea ecx for the key) and passes &entry in eax; the lane uses ecx / edx (by-value handle in Add tried). Every instruction and offset else matches.
// CSoundManagerLH::vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b) -- register a new named sound: refuse a (bank, sound) pair that already exists, an unknown bank or sample, or a failed start; else record the handle under the pair in the sorted sound map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3F250, 235 bytes (+ int3 padding), `ret 14h`. Vtable 0x0129D2C4 (CSoundManagerLH,
//   rtti-vtables.tsv) slot 39. The audio layer is not in Ego_r.pdb: method, member and argument names are descriptive.
// Leaf: vfunc_39
// Callees: CSoundManagerLH::Lookup(key, name) @ 0x00A3EE50 (landed name, as in vfunc_40 0x00A3EED0), the bank lookup
//   @ 0x00A3ED30, the sample lookup @ 0x00991840 (both `this` calls; manifest labels propagated), the bank's slot 1 (+4),
//   pair<CCharString, CCharString>(const CCharString&, const CCharString&) @ 0x0076DD40 (out of line; the manifest label names a
//   byte twin), CCharString(const CCharString&) @ 0x0099EC30, the map vector's push_back @ 0x00A41840, ~CCharString @ 0x0099EAE0,
//   the map's sort @ 0x00A42100.
// Shape: three early `return false` exits each with their own epilogue; the handle rides in ebx; CVectorMap::Add is inlined
//   (the key pair built out of line in the frame, the value pair copy-built from it with the handle, pushed, destroyed, Dirty
//   set, then the key destroyed at the end of the statement), the map then re-sorted out of line.
// Retail layout: the sound map CVectorMap at +0x54 (12-byte vector, comparator +0x60, Dirty +0x61).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
template <class T1, class T2> struct pair {             // STLport pair
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);
};
template <> struct pair<CCharString, CCharString> {
    CCharString first;
    CCharString second;
    pair(const CCharString& a, const CCharString& b);   // 0x0076DD40
};
typedef pair<CCharString, CCharString> CSoundKey;
struct CSoundMapEntry {                                 // pair<CSoundKey, long>: 12 bytes
    CSoundKey first;
    long      second;
    __forceinline CSoundMapEntry(const CSoundKey& key, const long& value) : first(key), second(value) { }
};
template <class T> struct vector {                      // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                     // 0x00A41840
};
struct CSoundKeyCompare { };
struct CSoundMap {                                      // CVectorMap<CSoundKey, long, compare>
    vector<CSoundMapEntry> Data;                        // +0x00
    CSoundKeyCompare       Compare;                     // +0x0c
    bool                   Dirty;                       // +0x0d
    __forceinline void Add(const CSoundKey& key, const long& value) { Data.push_back(CSoundMapEntry(key, value)); Dirty = true; }
    void Sort();                                        // 0x00A42100
};

struct CSoundSample {
    unsigned char _pad[0x10];
    long          Id;                                   // +0x10
};
struct CSoundBankLH {
    virtual void v00();
    virtual long Start(long sampleId, long a, long b);  // slot 1 (+0x04)
};
struct CSoundEntry;

struct CSoundManagerLH {                                // vtable 0x0129D2C4
    virtual void v00();
    unsigned char _pad04[0x54 - 0x04];
    CSoundMap     Sounds;                               // +0x54
    CSoundEntry*  Lookup(const CCharString& bankName, const CCharString& soundName);   // 0x00A3EE50
    CSoundBankLH* FindBank(const CCharString& bankName);   // 0x00A3ED30
    CSoundSample* FindSample(long sample);              // 0x00991840
    virtual bool vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b);
};

bool CSoundManagerLH::vfunc_39(const CCharString& bankName, const CCharString& soundName, long sample, long a, long b)
{
    if (Lookup(bankName, soundName))
        return false;
    CSoundBankLH* bank = FindBank(bankName);
    if (!bank)
        return false;
    CSoundSample* info = FindSample(sample);
    if (!info)
        return false;
    long handle = bank->Start(info->Id, a, b);
    if (!handle)
        return false;
    Sounds.Add(CSoundKey(bankName, soundName), handle);
    Sounds.Sort();
    return true;
}
