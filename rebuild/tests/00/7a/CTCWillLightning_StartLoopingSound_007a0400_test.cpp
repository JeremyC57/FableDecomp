// Test for 0x007A0400 CTCWillLightning::StartLoopingSound: links the source object; with no handle and a sound component
// (interface 16) it plays the def's LoopingSound string and keeps the handle; with a live handle, or without the
// component, nothing is played.
#include <stdio.h>
#include <string.h>
static long g_played_id = -1;
static int g_plays, g_dtors;
class CCharString { public: void* PStringData; ~CCharString(); };
CCharString::~CCharString() { ++g_dtors; }
class CDefStringTable { public: CCharString GetString(long id) const; };
CCharString CDefStringTable::GetString(long id) const { CCharString s; s.PStringData = (void*)id; return s; }
CDefStringTable GDefStringTable;
unsigned long g_InvalidSoundHandle = 0xffffffffUL;
class CTCBase;
class CTCSoundPlayer { public: unsigned long PlayAttachedCriteria(const CCharString& name, bool a1, bool a2, bool a3, long a4); };   // 0x0066BD00 (Ego_r.pdb)
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& name, bool a1, bool a2, bool a3, long a4)
{
    ++g_plays; g_played_id = (long)name.PStringData;
    return (!a1 && !a2 && !a3 && a4 == -1) ? 42UL : 7UL;
}
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const ETCInterfaceType& type)
{
    SEntry* it = first;
    while (it != last && it->first < type) ++it;
    return it;
}
class CThing { public: unsigned char _pad00[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; };
class CSpecialAbilitiesLightningSpellDef { public: unsigned char _pad00[0x114]; long LoopingSound; };
class CTCWillLightning { public:
    void* __vftable; CThing* ParentThing; unsigned char _pad08[0x20];
    const CSpecialAbilitiesLightningSpellDef* m_lspDef; unsigned char _pad2c[0x6c];
    unsigned long LoopingSound;
    void StartLoopingSound();
};
int main()
{
    static CSpecialAbilitiesLightningSpellDef def; def.LoopingSound = 9;
    CTCSoundPlayer snd;
    CInterfaceMap::SEntry e[1] = { { (ETCInterfaceType)16, (CTCBase*)&snd } };
    CThing t; memset(&t, 0, sizeof(t));
    t.m_interfaces.first = e; t.m_interfaces.last = e + 1;
    CTCWillLightning w; memset(&w, 0, sizeof(w));
    w.ParentThing = &t; w.m_lspDef = &def; w.LoopingSound = g_InvalidSoundHandle;
    w.StartLoopingSound();                                  // component bit absent: nothing
    bool ok = g_plays == 0 && w.LoopingSound == g_InvalidSoundHandle;
    t.m_presentBits[0] |= 1UL << 16;
    w.StartLoopingSound();
    ok = ok && g_plays == 1 && g_played_id == 9 && w.LoopingSound == 42 && g_dtors >= 1;
    w.StartLoopingSound();                                  // already playing: nothing
    ok = ok && g_plays == 1 && w.LoopingSound == 42;
    if (!ok) { puts("007a0400_TEST FAIL"); return 1; }
    puts("007a0400_TEST PASS");
    return 0;
}
