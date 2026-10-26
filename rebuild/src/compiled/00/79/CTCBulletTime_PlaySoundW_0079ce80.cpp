// CTCBulletTime::PlaySoundW(unsigned long sound, unsigned long flags) -- play a sound through the parent thing's sound component (interface 16), or return the invalid sound handle when it has none
//
// Retail 0x0079CE80, 91 bytes (`ret 8`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the thing's sorted interface map at +0x44), the sound component's play method
// @ 0x0066BD00 (5 arguments: sound, 0, flags, 0, -1; the manifest calls it CSoundInfo::PlaySound).
// Global 0x0129D25C = the invalid sound handle (0xFFFFFFFF in the image).
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x007504E0 / 0x007BEA08 family:
// the presence bitset at +0x20 is tested on the raw masked word (type 16 -> `test [eax+20h],10000h`),
// the map is searched with LowerBound on the key in the `push ecx` frame slot (end, or a larger key,
// -> the end entry), and the component comes back through the out pointer. The name is the
// Unicode-renamed "PlaySound" (windows.h maps it to PlaySoundW). A byte-identical body at
// 0x0079DE00 belongs to the class whose CancelMove calls it (CTCPhysicalShieldSpell).

class CTCBase;

struct CTCSound                                     // the thing's interface-16 component
{
    unsigned long PlaySound(unsigned long sound, long a, unsigned long flags, long b, long c);   // 0x0066BD00
};

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

enum { TC_INTERFACE_SOUND = 16 };

#pragma pack(push, 1)
class CTCBulletTime
{ public:
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)

    unsigned long PlaySoundW(unsigned long sound, unsigned long flags);
};
#pragma pack(pop)

unsigned long CTCBulletTime::PlaySoundW(unsigned long sound, unsigned long flags)
{
    CTCSound* sounds;
    if (ParentThing->GetInterface(TC_INTERFACE_SOUND, &sounds))
        return sounds->PlaySound(sound, 0, flags, 0, -1);
    return g_InvalidSoundHandle;
}