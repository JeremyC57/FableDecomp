// CTCDivineWrath::PlaySoundW(unsigned long sound, unsigned long flags) -- make a "SIMPLE_NOISE" noise at the parent creature and play the sound through its sound component (interface 16), or return the invalid sound handle when it has none
//
// Retail 0x0078EBF0, 131 bytes (`ret 8`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the thing's sorted interface map at +0x44), CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 ("SIMPLE_NOISE" @ 0x01268A5C), CThingCreatureBase::MakeNoise(const CCharString&) @
// 0x00662110, ~CCharString @ 0x0099EAE0, the sound component's play method @ 0x0066BD00 (5 arguments:
// sound, 0, flags, 0, -1; manifest "CSoundInfo::PlaySound"). Global 0x0129D25C = the invalid sound handle.
// Shape: the landed CTCBulletTime::PlaySoundW (0x0079DE00) with a noise in between: the parent is a
// named local (edi), the component is taken into a named local right after the lookup (esi) and the
// noise name is a CCharString in its own `{}` block, sharing the lookup key's `push ecx` frame word.
// The name is the Unicode-renamed "PlaySound" (windows.h maps it to PlaySoundW).
struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CTCSound                                             // the thing's interface-16 component
{
    unsigned long PlaySound(unsigned long sound, long a, unsigned long flags, long b, long c);   // 0x0066BD00
};

struct CInterfaceMap                                        // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    void MakeNoise(const CCharString& noiseName);           // 0x00662110 (CThingCreatureBase)
};

extern unsigned long g_InvalidSoundHandle;                  // 0x0129D25C

struct CTCDivineWrath
{
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    unsigned long PlaySoundW(unsigned long sound, unsigned long flags);
};

unsigned long CTCDivineWrath::PlaySoundW(unsigned long sound, unsigned long flags)
{
    CThing* thing = ParentThing;
    if (thing->HasInterface(16))
    {
        CTCSound* sounds = (CTCSound*)thing->GetInterface(16);
        {
            CCharString noise("SIMPLE_NOISE", -1);
            thing->MakeNoise(noise);
        }
        return sounds->PlaySound(sound, 0, flags, 0, -1);
    }
    return g_InvalidSoundHandle;
}
