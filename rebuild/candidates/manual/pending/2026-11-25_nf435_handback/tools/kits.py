# kits for mktest.py: placeholders expanded in a test spec
TCKIT = r'''typedef CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > NF435Map;
// CVectorMap::LowerBound comes from the row object (the header's template body, the same code retail calls out of line).
static unsigned char g_thing[0x200];                       // CThing: UsingComponents +0x20, Components +0x44 / +0x48, dead flag byte +0x91
static unsigned char g_thing2[0x200];
static NF435Map::value_type g_pairs[3];
static void nf435_plant(unsigned char* thing, int id, void* tc)   // three sorted entries around id, the bit for id set
{
    memset(thing, 0, 0x200);
    g_pairs[0].first = (ETCInterfaceType)(id - 1); g_pairs[0].second = 0;
    g_pairs[1].first = (ETCInterfaceType)id;       g_pairs[1].second = (CTCBase*)tc;
    g_pairs[2].first = (ETCInterfaceType)(id + 1); g_pairs[2].second = 0;
    *(void**)(thing + 0x44) = g_pairs; *(void**)(thing + 0x48) = g_pairs + 3;
    thing[0x20 + id / 8] |= (unsigned char)(1 << (id % 8));
}
static void nf435_clear(unsigned char* thing, int id) { thing[0x20 + id / 8] &= (unsigned char)~(1 << (id % 8)); }'''
SCRIPTKIT = r'''static void* g_ret = 0; static void* g_ret2 = 0; static bool g_alive = true; static int g_gets = 0;
extern "C" void* __fastcall nf435_get_pthing(const void*) { ++g_gets; return g_ret; }     // CScriptThing slot 11 (+0x2c)
extern "C" void* __fastcall nf435_get_pthing2(const void*) { ++g_gets; return g_ret2; }
extern "C" bool __fastcall nf435_is_alive(const void*) { return g_alive; }                // CScriptThing slot 75 (+0x12c)
static void* g_svtbl[79]; static void* g_svtbl2[79]; static void* g_script[3]; static void* g_script2[3];
static void nf435_scripts(void)
{
    g_svtbl[11] = (void*)&nf435_get_pthing; g_svtbl[75] = (void*)&nf435_is_alive; g_script[0] = g_svtbl;
    g_svtbl2[11] = (void*)&nf435_get_pthing2; g_svtbl2[75] = (void*)&nf435_is_alive; g_script2[0] = g_svtbl2;
}
#define NF435_SCRIPT (*(const CScriptThing*)g_script)
#define NF435_SCRIPT2 (*(const CScriptThing*)g_script2)'''
HEROKIT = r'''static void* g_hero = 0; static long g_player[0x40]; static long g_pm[0x40]; static int g_hero_gets = 0;
CPlayer& CPlayerManager::GetMainPlayer() { return *(CPlayer*)g_player; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { ++g_hero_gets; return (CThingPlayerCreature*)g_hero; }
static long g_gsi[0x50 / 4];
static void nf435_hero(void) { g_gsi[0x14 / 4] = (long)g_pm; }'''
HEROPEEK = r'''static void* g_hero = 0; static long g_player[0x40]; static long g_pm[0x40]; static int g_hero_gets = 0;
CPlayer& CPlayerManager::GetMainPlayer() { return *(CPlayer*)g_player; }
const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const { ++g_hero_gets; return (const CThingPlayerCreature*)g_hero; }
static long g_gsi[0x50 / 4];
static void nf435_hero(void) { g_gsi[0x14 / 4] = (long)g_pm; }'''
def expand(t):
    return t.replace('@TCKIT@', TCKIT).replace('@SCRIPTKIT@', SCRIPTKIT).replace('@HEROKIT@', HEROKIT).replace('@HEROPEEK@', HEROPEEK)
