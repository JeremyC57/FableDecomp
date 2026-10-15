// CTCTrophy::PlaySoundW(const CCharString&) -- plays a sound through the owner's sound component (interface 16) and returns its handle, or the invalid handle when the owner has no sound component
// Leaf: PlaySoundW
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DDCD0, length 95 bytes. PDB `?PlaySoundW@CTCTrophy@@AAEKABVCCharString@@@Z`
// (the W is windows.h's PlaySound macro applied to the original source).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CSoundInfo::PlaySound(name, 1, 0, 0, -1)
// @ 0x0066BD00 (on the component). The invalid-handle global @ 0x0129D25C is returned on the absent path.
// Shape: the presence word (+0x20) is loaded once and tested twice with the mask kept in ecx: an explicit HasTC
// test `&&` the inline out-parameter GetInterface template (landed 0x007504E0 / 0x00849250 shape: key 16 in the
// `push ecx` frame word, the map pointer a named local, end-or-larger-key -> the end entry).
struct CCharString;

struct CSoundComponent {
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00
};

struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
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

extern unsigned long g_InvalidSoundHandle;  // 0x0129D25C

struct CTCTrophy {
    void*   __vftable;                      // +0x00
    CThing* Thing;                          // +0x04
    unsigned long PlaySoundW(const CCharString& name);
};

unsigned long CTCTrophy::PlaySoundW(const CCharString& name)
{
    CSoundComponent* sound;
    if (Thing->HasTC(16) && Thing->GetInterface(&sound, 16))
        return sound->PlaySound(name, true, 0, 0, -1);
    return g_InvalidSoundHandle;
}
