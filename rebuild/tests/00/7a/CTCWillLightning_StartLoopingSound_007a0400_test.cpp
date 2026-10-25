// Test for 0x007A0400 CTCWillLightning::StartLoopingSound: links the source object; with no handle and a sound component
// (interface 16) it plays the def's LoopingSound string and keeps the handle; with a live handle, or without the
// component, nothing is played.
#include <stdio.h>
#include <string.h>
static long g_played_id = -1;
static int g_plays, g_dtors;
struct CCharString { void* PStringData; ~CCharString(); };
CCharString::~CCharString() { ++g_dtors; }
struct CDefStringTable { CCharString GetString(long id); };
CCharString CDefStringTable::GetString(long id) { CCharString s; s.PStringData = (void*)id; return s; }
CDefStringTable g_DefStringTable;
unsigned long g_InvalidSoundHandle = 0xffffffffUL;
class CTCBase;
struct CTCSound { unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c); };
unsigned long CTCSound::PlaySound(const CCharString& name, bool positional, long a, long b, long c)
{
    ++g_plays; g_played_id = (long)name.PStringData;
    return (!positional && a == 0 && b == 0 && c == -1) ? 42UL : 7UL;
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
struct CThing { unsigned char _pad00[0x20]; unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; };
struct CSpecialAbilitiesLightningSpellDef { unsigned char _pad00[0x114]; long LoopingSound; };
struct CTCWillLightning {
    void* __vftable; CThing* ParentThing; unsigned char _pad08[0x20];
    const CSpecialAbilitiesLightningSpellDef* m_lspDef; unsigned char _pad2c[0x6c];
    unsigned long LoopingSound;
    void StartLoopingSound();
};
int main()
{
    static CSpecialAbilitiesLightningSpellDef def; def.LoopingSound = 9;
    CTCSound snd;
    CInterfaceMap::SEntry e[1] = { { 16, (CTCBase*)&snd } };
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
