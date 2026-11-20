// Behaviour test for 0x0077D5B0 CTCHeroCentre::FrameUpdate (links the REAL row object, with the template bodies it carries: the
// GetAllLeafThings instance, the interface-map LowerBound, vector erase / _M_insert_overflow and the CIntelligentPointer<CThing>
// vtable). No class is modelled: every object is raw native-layout storage, the row is entered and its 31 undefined callees / statics
// are bound through linker aliases of their retail names (std::__find as an explicit specialisation) (thiscall = fastcall with a spare edx):
//   hero centre   ParentThing +4, FrameQuestCardListFrom +0x14, QuestCards +0x18 (vector of 8-byte {vptr, PData} pointers),
//                 QuestCardPositions +0x24 (vector of 12-byte vectors), WaitingForTutorialToFinish +0x30
//   thing         component bitset +0x20 (bit 22 experience inventory = dword +0x20 & 0x400000; bit 108 quest card = dword +0x2C &
//                 0x1000), interface map begin +0x44 / end +0x48 (8-byte {type, component} pairs, sorted), PhysicsTC +0x60 (vtable slot
//                 72 = GetRHSet), def index word +0x8C, state byte +0x91 (dead 1, in scans 4, activated 8, limbo 0x20)
//   quest card    FeatIndex +0x14, QuestName +0x18, ScriptQuestName +0x28 (CCharString = one pointer to {chars, length})
//   quest manager AvailableQuestCardTypes +0x58 (list: sentinel pointer; node = next, prev, DefName +8, ScriptName +0xC, Replayable
//                 +0x10, FeatIndex +0x14), QuestCardsNeedUpdating +0x8F
//   player gui    SubtitleRenderer.DisplayingText +0xFD; world Paused +0xA4; game event Type +0, Player +4, EndPos +0x25, Valid +0x26,
//                 Replacement +0x27
// The intelligent-pointer callees keep the raw item in PData, so the QuestCards vector can be read back directly. The test
// compares the ordered event list (Kill, reserve, def lookups, Create with all its arguments, string assignments, AddData,
// AddGameEvent), the final card list, every quest card component and the centre's fields with the independent model in RunModel(),
// and checks that pointer constructions and destructions balance.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { ExitProcess(3); return 0; }

extern "C" void __fastcall CallFrameUpdate(void* self);
#pragma comment(linker, "/alternatename:@CallFrameUpdate@4=?FrameUpdate@CTCHeroCentre@@UAEXXZ")
#pragma comment(linker, "/alternatename:?AddData@CGameEvent@@QAEXJPBX@Z=@StubAddData@16")
#pragma comment(linker, "/alternatename:?PMainGameComponent@CMainGameComponent@@1PAV1@A=_g_pMainGameComponent")
#pragma comment(linker, "/alternatename:??4CCharString@@QAEABV0@ABV0@@Z=@StubStringAssign@12")
#pragma comment(linker, "/alternatename:??1CBaseIntelligentPointer@@UAE@XZ=@StubPointerDtor@4")
#pragma comment(linker, "/alternatename:?Compare@?$CBasicString@D@@KIJPBD0@Z=@StubCompare@8")
#pragma comment(linker, "/alternatename:?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ=@StubGetPItem@4")
#pragma comment(linker, "/alternatename:?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z=@StubSetPItem@12")
#pragma comment(linker, "/alternatename:??_G?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z=@StubPointerScalarDtor@12")
#pragma comment(linker, "/alternatename:??0CBaseIntelligentPointer@@QAE@XZ=@StubPointerCtor@4")
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A=_g_PlayerGuiPClass")
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A=_g_QuestManagerPClass")
#pragma comment(linker, "/alternatename:??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z=@StubFilterHasTC@12")
#pragma comment(linker, "/alternatename:?PeekTypeList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@J@Z=@StubPeekTypeList@12")
#pragma comment(linker, "/alternatename:?GetFrame@CWorld@@SIJXZ=@StubGetFrame@0")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStringDtor@4")
#pragma comment(linker, "/alternatename:?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z=@StubCreate@24")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StubStringCtor@16")
#pragma comment(linker, "/alternatename:?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z=@StubGetDefIndex@12")
#pragma comment(linker, "/alternatename:?Get@CGameDefinitionManager@@SIAAV1@XZ=@StubGetDefinitionManager@0")
#pragma comment(linker, "/alternatename:?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ=@StubPeekThingSearchTools@4")
#pragma comment(linker, "/alternatename:?reserve@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z=@StubReserve@12")
#pragma comment(linker, "/alternatename:?Kill@CThing@@QAEX_N@Z=@StubKill@12")
#pragma comment(linker, "/alternatename:?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z=@StubAddGameEvent@12")
#pragma comment(linker, "/alternatename:?GetMainPlayerNumber@CPlayerManager@@QBEJXZ=@StubGetMainPlayerNumber@4")
#pragma comment(linker, "/alternatename:?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ=@StubGetPControlledCreature@4")
#pragma comment(linker, "/alternatename:?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ=@StubGetMainPlayer@4")
#pragma comment(linker, "/alternatename:?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ=@StubPeekPlayerManager@4")
#pragma comment(linker, "/alternatename:?PeekWorld@CTCBase@@IBEABVCWorld@@XZ=@StubPeekWorld@4")

// ---- deterministic random numbers
static unsigned long g_seed = 0x13579bdf;
static unsigned long Rnd() { g_seed = g_seed * 1664525ul + 1013904223ul; return g_seed >> 8; }
static long RndRange(long lo, long hi) { return lo + (long)(Rnd() % (unsigned long)(hi - lo + 1)); }
template <class T> static T& At(void* base, int off) { return *(T*)((char*)base + off); }
static int g_fail;
static void Fail(const char* what) { if (!g_fail) printf("FAIL %s\n", what); g_fail = 1; }

// ---- strings: a CCharString is one pointer to {chars, length}
struct SData { const char* chars; long length; };
static const char* g_texts[] = { "OBJECT_QUEST_CARD_A", "OBJECT_QUEST_CARD_B", "OBJECT_QUEST_CARD_C", "FEAT_ONE", "FEAT_TWO", "Q_WASP", "Q_ARENA", "Q_BANDIT" };
enum { TEXTS = 8, STRINGS = 2 * TEXTS, TEMP_STRINGS = 40 };
static SData g_strings[STRINGS];                     // every text twice: same characters, two data blocks
static SData g_tempStrings[TEMP_STRINGS]; static int g_tempUsed, g_tempLive;

// ---- event log (the row's in g_ev, the model's in g_want)
struct Ev { char t; long a, b, c, d, e; unsigned long v[9]; };
enum { MAX_EV = 160 };
static Ev g_ev[MAX_EV], g_want[MAX_EV]; static int g_evCount, g_wantCount, g_modelling;
static Ev& Log(char t, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0)
{
    static Ev junk;
    int& n = g_modelling ? g_wantCount : g_evCount; Ev* list = g_modelling ? g_want : g_ev;
    if (n >= MAX_EV) { n++; return junk; }
    Ev& ev = list[n++]; memset(&ev, 0, sizeof ev); ev.t = t; ev.a = a; ev.b = b; ev.c = c; ev.d = d; ev.e = e; return ev;
}

// ---- the world
enum { THINGS = 14, MAX_OLD = 5, MAX_TYPES = 6, MAX_POS = 4, MAX_LEAF = 8 };
struct Thing { unsigned char raw[0xa0]; unsigned long map[6]; unsigned char card[0x34]; };
static Thing g_things[THINGS];                       // 0..7 world things, 8..13 the things Create hands out
static int g_createNext;
static unsigned char g_centre[0x34], g_parent[0xa0], g_physics[8], g_hero[0xa0];
static float g_rhset[6];
static float g_positions[MAX_POS][3];
struct TypeNode { TypeNode* next; TypeNode* prev; SData* def_name; SData* script_name; unsigned char replayable; long feat_index; };
static TypeNode g_typeSentinel, g_types[MAX_TYPES]; static int g_typeCount;
struct LeafNode { LeafNode* next; LeafNode* prev; void* thing; };
static LeafNode g_leafSentinel, g_leaves[MAX_LEAF]; static LeafNode* g_leafList = &g_leafSentinel; static int g_leafCount;
static unsigned char g_questManager[0x98], g_playerGui[0x100], g_world[0xb0], g_playerManager[8], g_player[8], g_definitionManager[8],
                     g_mainGameComponent[8], g_searchTools[8];
extern "C" { void* g_pMainGameComponent = g_mainGameComponent; void* g_PlayerGuiPClass[2] = { g_playerGui, 0 }; void* g_QuestManagerPClass[2] = { g_questManager, 0 }; }
static long g_frame, g_mainPlayerNumber, g_dummyDefIndex, g_reserveWorks, g_createFails;
static long g_defIndexOfText[TEXTS];
static long g_pointerCtors, g_pointerDtors;

static int ThingIndex(const void* p) { for (int i = 0; i < THINGS; i++) if (p == g_things[i].raw) return i; return p ? -2 : -1; }
static int StringIndex(const SData* s)
{
    if (!s) return -1;
    if (s >= g_strings && s < g_strings + STRINGS) return (int)(s - g_strings);
    if (s >= g_tempStrings && s < g_tempStrings + TEMP_STRINGS) return 100 + (int)(s - g_tempStrings);
    return -2;
}
static long DefIndexOfChars(const char* chars)
{
    if (!chars) return -1;
    if (!strcmp(chars, "OBJECT_FEAT_CARD_DUMMY")) return g_dummyDefIndex;
    for (int i = 0; i < TEXTS; i++) if (!strcmp(chars, g_texts[i])) return g_defIndexOfText[i];
    return -7;
}

// ---- the callees
static void* g_elementVtable[1];
extern "C" void __fastcall StubPointerCtor(void* self) { At<void*>(self, 4) = 0; g_pointerCtors++; }
extern "C" void __fastcall StubPointerDtor(void* self) { (void)self; g_pointerDtors++; }
extern "C" void* __fastcall StubPointerScalarDtor(void* self, int, unsigned long flags) { if (flags) Fail("element deleted"); g_pointerDtors++; return self; }
extern "C" void* __fastcall StubGetPItem(void* self) { return At<void*>(self, 4); }
extern "C" void __fastcall StubSetPItem(void* self, int, void* item) { At<void*>(self, 4) = item; }
extern "C" void __fastcall StubKill(void* self, int, unsigned long flag) { Log('K', ThingIndex(self), (long)(flag & 0xff)); }
extern "C" void __fastcall StubReserve(void* self, int, unsigned long n)
{
    Log('R', (long)((char*)self - (char*)g_centre), (long)n);
    char*& start = At<char*>(self, 0); char*& finish = At<char*>(self, 4); char*& end = At<char*>(self, 8);
    if (start != finish) { Fail("reserve on a non-empty list"); return; }
    if (g_reserveWorks && (unsigned long)(end - start) < n * 8) { free(start); start = finish = (char*)malloc(n * 8 + 8); end = start + n * 8; }
}
// std::__find for the card pointers: its retail name is too long for a linker alias, so it is defined as the explicit specialisation
// of the declared template (the two class names are only declared; the elements are handled as raw 8-byte {vptr, PData} blocks).
class CThing;
template <class T> class CIntelligentPointer;
namespace std
{
    struct random_access_iterator_tag { };
    template <class It, class T> It __fastcall __find(It first, It last, const T& value, const random_access_iterator_tag& tag);
    template <> CIntelligentPointer<CThing>* __fastcall __find<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing> >(
        CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, const CIntelligentPointer<CThing>& value, const random_access_iterator_tag&)
    {
        for (char* p = (char*)first; p != (char*)last; p += 8)
            if (At<void*>(p, 4) == At<void*>((void*)&value, 4)) return (CIntelligentPointer<CThing>*)p;
        return last;
    }
}
extern "C" bool __fastcall StubFilterHasTC(void* self, int, void* thing)
{
    if (At<long>(self, 0) != 0x6c) Fail("filter type");
    if (ThingIndex(thing) < 0) Fail("filter thing");
    return (At<unsigned long>(thing, 0x2c) & 0x1000) != 0;
}
extern "C" void* __fastcall StubPeekTypeList(void* self, int, long type)
{
    if (self != g_searchTools || type != 5) Fail("PeekTypeList");
    return &g_leafList;
}
extern "C" long __fastcall StubGetFrame() { return g_frame; }
extern "C" void* __fastcall StubStringCtor(void* self, int, const char* text, long length)
{
    if (length != -1 || g_tempUsed >= TEMP_STRINGS) { Fail("string constructor"); return self; }
    SData* d = &g_tempStrings[g_tempUsed++]; d->chars = text; d->length = (long)strlen(text);
    At<SData*>(self, 0) = d; g_tempLive++;
    return self;
}
extern "C" void __fastcall StubStringDtor(void* self) { if (StringIndex(At<SData*>(self, 0)) < 100) Fail("string destructor"); g_tempLive--; }
extern "C" void* __fastcall StubStringAssign(void* self, int, void* other)
{
    int thing = -1, field = -1;
    for (int i = 0; i < THINGS; i++)
        if ((unsigned char*)self >= g_things[i].card && (unsigned char*)self < g_things[i].card + 0x34) { thing = i; field = (int)((unsigned char*)self - g_things[i].card); }
    Log('=', thing, field, StringIndex(At<SData*>(other, 0)));
    At<SData*>(self, 0) = At<SData*>(other, 0);
    return self;
}
extern "C" long __fastcall StubCompare(const char* a, const char* b) { return (long)strcmp(a, b); }
extern "C" void* __fastcall StubGetDefinitionManager() { return g_definitionManager; }
extern "C" long __fastcall StubGetDefIndex(void* self, int, void* name)
{
    if (self != g_definitionManager) Fail("definition manager");
    SData* d = At<SData*>(name, 0);
    long index = DefIndexOfChars(d ? d->chars : 0);
    Log('G', StringIndex(d) >= 100 ? 100 : StringIndex(d), index);
    return index;
}
extern "C" void* __fastcall StubCreate(long def_index, const float* position, long player, const float* rhset, unsigned long flags, void* name)
{
    Ev& ev = Log('C', def_index, player, (long)flags, At<SData*>(name, 0) ? (long)strlen(At<SData*>(name, 0)->chars) : -1, g_createNext);
    memcpy(ev.v, position, 12); memcpy(ev.v + 3, rhset, 24);
    if ((g_createFails && def_index != g_dummyDefIndex) || g_createNext >= THINGS) { g_createNext++; return 0; }   // the feat-card path uses the result unchecked
    Thing& t = g_things[g_createNext++];
    At<unsigned short>(t.raw, 0x8c) = (unsigned short)def_index;
    return t.raw;
}
extern "C" void __fastcall StubAddData(void* self, int, long size, const void* data)
{
    Ev& ev = Log('A', At<long>(self, 0), size, At<signed char>(self, 4)); memcpy(ev.v, data, size > 36 ? 36 : size);   // the player is still unset here
}
extern "C" void __fastcall StubAddGameEvent(void* self, int, void* game_event)
{
    if (self != g_mainGameComponent) Fail("main game component");
    Log('E', At<long>(game_event, 0), At<signed char>(game_event, 4), At<unsigned char>(game_event, 0x25), At<unsigned char>(game_event, 0x26), At<unsigned char>(game_event, 0x27));
}
extern "C" long __fastcall StubGetMainPlayerNumber(void* self) { if (self != g_playerManager) Fail("player manager"); return g_mainPlayerNumber; }
extern "C" void* __fastcall StubGetMainPlayer(void* self) { if (self != g_playerManager) Fail("player manager"); return g_player; }
extern "C" void* __fastcall StubGetPControlledCreature(void* self) { if (self != g_player) Fail("player"); return g_hero; }
extern "C" void* __fastcall StubPeekPlayerManager(void* self) { if (self != g_centre) Fail("PeekPlayerManager this"); return g_playerManager; }
extern "C" void* __fastcall StubPeekWorld(void* self) { if (self != g_centre) Fail("PeekWorld this"); return g_world; }
extern "C" void* __fastcall StubPeekThingSearchTools(void* self) { if (self != g_centre) Fail("PeekThingSearchTools this"); return g_searchTools; }
static const float* __fastcall PhysicsGetRHSet(void* self) { if (self != g_physics) Fail("physics"); return g_rhset; }
static void __fastcall PhysicsTrap(void*) { Fail("unexpected physics virtual"); }
static void* g_physicsVtable[80];

// ---- model state: what each quest card component holds, and the card list
struct CardModel { long feat_index; SData* quest_name; SData* script_name; };
static CardModel g_cardModel[THINGS];
static int g_listModel[64]; static int g_listModelCount;
static int g_oldCards[MAX_OLD]; static int g_oldCount;      // thing index, -1 for an empty pointer
static int g_positionCount, g_modelListRan;
static long g_statTutorialWait, g_statTutorialEvent, g_statTutorialNoInventory, g_statSkip, g_statKillOld, g_statNoPositions, g_statReuse,
            g_statCreate, g_statCreateNull, g_statDummyReuse, g_statDummyCreate, g_statDummyMissing, g_statNameEqualData, g_statNameEqualText,
            g_statNameDiffer, g_statNameNull, g_statScriptSet, g_statLastPosition, g_statKillLeft, g_statOverflow, g_statFeatDiffer, g_statNoCard;

static bool HasCard(int thing) { return (At<unsigned long>(g_things[thing].raw, 0x2c) & 0x1000) != 0; }
static bool StringsEqual(const SData* a, const SData* b)
{
    if (a == b) { g_statNameEqualData++; return true; }
    if (!a || !b) { g_statNameNull++; return false; }
    if (a->length != b->length) { g_statNameDiffer++; return false; }
    if (strcmp(a->chars, b->chars) == 0) { g_statNameEqualText++; return true; }
    g_statNameDiffer++; return false;
}

static void RunModel()
{
    int i;
    if (At<unsigned char>(g_centre, 0x30))
    {
        if (At<unsigned char>(g_playerGui, 0xfd) || At<unsigned char>(g_world, 0xa4)) { g_statTutorialWait++; return; }
        At<unsigned char>(g_centre, 0x30) = 0;
        if (At<unsigned long>(g_hero, 0x20) & 0x400000)
        {
            Ev& ev = Log('A', 0x1a, 4, -1); ev.v[0] = 0xb;
            Log('E', 0x1a, (signed char)g_mainPlayerNumber, 0, 1, 0);
            g_statTutorialEvent++;
        }
        else g_statTutorialNoInventory++;
        return;
    }
    if (At<unsigned long>(g_centre, 0x14) != 0 && !At<unsigned char>(g_questManager, 0x8f)) { g_statSkip++; return; }
    g_modelListRan = 1;
    for (i = 0; i < g_oldCount; i++)
        if (g_oldCards[i] >= 0 && !(At<unsigned char>(g_things[g_oldCards[i]].raw, 0x91) & 1)) { Log('K', g_oldCards[i], 0); g_statKillOld++; }
    g_listModelCount = 0;
    Log('R', 0x18, g_typeCount);
    // the things a leaf scan for quest cards finds
    int existing[MAX_LEAF], existing_count = 0;
    for (i = 0; i < g_leafCount; i++)
    {
        int t = ThingIndex(g_leaves[i].thing); unsigned char f = At<unsigned char>(g_things[t].raw, 0x91);
        if ((f & 4) && (f & 8) && !(f & 1) && !(f & 0x20) && HasCard(t)) existing[existing_count++] = t;
    }
    long position_index = 0;
    if (g_positionCount == 0) g_statNoPositions++;
    for (int n = 0; n < g_typeCount && g_positionCount != 0; n++)
    {
        const TypeNode& type = g_types[n];
        int card = -1; bool have = false;
        long def_index = DefIndexOfChars(type.def_name ? type.def_name->chars : 0);
        Log('G', StringIndex(type.def_name), def_index);
        if (def_index > 0)
        {
            for (i = 0; i < existing_count; i++)
                if ((long)At<unsigned short>(g_things[existing[i]].raw, 0x8c) == def_index) { card = existing[i]; have = true; g_statReuse++; break; }
            if (!have)
            {
                Ev& ev = Log('C', def_index, 5, 0, 0, g_createNext); memcpy(ev.v, g_positions[position_index], 12); memcpy(ev.v + 3, g_rhset, 24);
                if (g_createFails) { g_createNext++; g_statCreateNull++; }
                else { card = g_createNext++; At<unsigned short>(g_things[card].raw, 0x8c) = (unsigned short)def_index; have = true; g_statCreate++; }
            }
        }
        else
        {
            def_index = g_dummyDefIndex;
            Log('G', 100, def_index);
            if (def_index <= 0) { g_statDummyMissing++; continue; }
            for (i = 0; i < existing_count; i++)
                if ((long)At<unsigned short>(g_things[existing[i]].raw, 0x8c) == def_index)
                {
                    const CardModel& c = g_cardModel[existing[i]];
                    if (c.feat_index != type.feat_index) { g_statFeatDiffer++; continue; }
                    if (StringsEqual(c.script_name, type.script_name)) { card = existing[i]; have = true; g_statDummyReuse++; break; }
                }
            if (!have)
            {
                Ev& ev = Log('C', def_index, 5, 0, 0, g_createNext); memcpy(ev.v, g_positions[position_index], 12); memcpy(ev.v + 3, g_rhset, 24);
                card = g_createNext++; At<unsigned short>(g_things[card].raw, 0x8c) = (unsigned short)def_index; have = true; g_statDummyCreate++;
                if (HasCard(card))
                {
                    g_cardModel[card].feat_index = type.feat_index;
                    Log('=', card, 0x18, StringIndex(type.def_name)); g_cardModel[card].quest_name = type.def_name;
                    Log('=', card, 0x28, StringIndex(type.script_name)); g_cardModel[card].script_name = type.script_name;
                }
                else g_statNoCard++;
            }
        }
        if (have)
        {
            if (g_cardModel[card].feat_index == -1)
            {
                Log('=', card, 0x28, StringIndex(type.script_name)); g_cardModel[card].script_name = type.script_name; g_statScriptSet++;
            }
            g_listModel[g_listModelCount++] = card;
            if ((unsigned long)position_index < (unsigned long)g_positionCount - 1) position_index++; else g_statLastPosition++;
        }
    }
    for (i = 0; i < existing_count; i++)
    {
        bool listed = false;
        for (int k = 0; k < g_listModelCount; k++) if (g_listModel[k] == existing[i]) listed = true;
        if (!listed) { Log('K', existing[i], 0); g_statKillLeft++; }
    }
    At<long>(g_centre, 0x14) = g_frame;
    At<unsigned char>(g_questManager, 0x8f) = 0;
}

static void BuildCase()
{
    int i;
    for (i = 0; i < (int)sizeof g_centre; i++) g_centre[i] = (unsigned char)Rnd();
    for (i = 0; i < (int)sizeof g_questManager; i++) g_questManager[i] = (unsigned char)Rnd();
    for (i = 0; i < (int)sizeof g_playerGui; i++) g_playerGui[i] = (unsigned char)Rnd();
    for (i = 0; i < (int)sizeof g_world; i++) g_world[i] = (unsigned char)Rnd();
    for (i = 0; i < (int)sizeof g_hero; i++) g_hero[i] = (unsigned char)Rnd();
    for (i = 0; i < 6; i++) g_rhset[i] = (float)RndRange(-100, 100) / 16.0f;
    At<void*>(g_parent, 0x60) = g_physics; At<void*>(g_physics, 0) = g_physicsVtable; At<void*>(g_centre, 4) = g_parent;
    int mode = (int)(Rnd() % 8);                               // 0, 1: the tutorial wait; the rest: the card list
    At<unsigned char>(g_centre, 0x30) = (unsigned char)(mode < 2 ? RndRange(1, 255) : 0);
    At<unsigned char>(g_playerGui, 0xfd) = (unsigned char)(Rnd() % 3 == 0 ? RndRange(1, 255) : 0);
    At<unsigned char>(g_world, 0xa4) = (unsigned char)(Rnd() % 3 == 0 ? RndRange(1, 255) : 0);
    At<long>(g_centre, 0x14) = (Rnd() % 3 == 0) ? 0 : RndRange(1, 100000);
    At<unsigned char>(g_questManager, 0x8f) = (unsigned char)(Rnd() % 3 == 0 ? 0 : RndRange(1, 255));
    g_frame = RndRange(1, 1000000); g_mainPlayerNumber = RndRange(-1, 3);
    g_dummyDefIndex = (Rnd() % 5 == 0) ? RndRange(-2, 0) : (Rnd() % 4 == 0 ? 1 : 900);      // 1 is the smallest index that is a def
    g_reserveWorks = (long)(Rnd() % 3 != 0); g_createFails = (long)(Rnd() % 9 == 0);
    for (i = 0; i < TEXTS; i++) g_defIndexOfText[i] = i < 3 ? 300 + i : (i < 5 ? RndRange(-3, 0) : 700 + i);
    if (Rnd() % 5 == 0) g_defIndexOfText[RndRange(0, 2)] = 2;                   // 1 and 2: the smallest def indices
    if (Rnd() % 5 == 0) g_defIndexOfText[RndRange(0, 2)] = (g_dummyDefIndex == 1) ? 2 : 1;
    if (Rnd() % 6 == 0) g_defIndexOfText[RndRange(0, 2)] = 0;                   // a card def that is not a def any more
    // things: world things 0..7 (some of them quest cards already), 8..13 for Create
    for (i = 0; i < THINGS; i++)
    {
        Thing& t = g_things[i]; int k;
        for (k = 0; k < (int)sizeof t.raw; k++) t.raw[k] = (unsigned char)Rnd();
        for (k = 0; k < (int)sizeof t.card; k++) t.card[k] = (unsigned char)Rnd();
        bool card = i >= 8 ? true : (Rnd() % 4 != 0);
        unsigned long& bits = At<unsigned long>(t.raw, 0x2c); bits &= ~0x1000ul;
        int entries = 0;
        if (Rnd() % 2) { t.map[entries * 2] = (unsigned long)RndRange(1, 0x6b); t.map[entries * 2 + 1] = 0x11111111; entries++; }
        if (card) { bits |= 0x1000; t.map[entries * 2] = 0x6c; t.map[entries * 2 + 1] = (unsigned long)(size_t)t.card; entries++; }
        if (Rnd() % 2) { t.map[entries * 2] = (unsigned long)RndRange(0x6d, 270); t.map[entries * 2 + 1] = 0x22222222; entries++; }
        At<void*>(t.raw, 0x44) = t.map; At<void*>(t.raw, 0x48) = t.map + entries * 2;
        At<unsigned short>(t.raw, 0x8c) = (unsigned short)(i >= 8 ? 0 : (Rnd() % 3 == 0 ? (g_dummyDefIndex > 0 ? g_dummyDefIndex : 900) : (Rnd() % 6 == 0 ? RndRange(1, 2) : 300 + RndRange(0, 3))));
        At<unsigned char>(t.raw, 0x91) = (unsigned char)(i >= 8 ? 0x0c : ((Rnd() % 6 == 0) ? Rnd() : (0x0c | (Rnd() & 0xd2))));
        g_cardModel[i].feat_index = (i >= 8 && Rnd() % 2) ? -1 : (Rnd() % 3 == 0 ? -1 : RndRange(0, 2));
        g_cardModel[i].quest_name = (Rnd() % 4 == 0) ? 0 : &g_strings[RndRange(0, STRINGS - 1)];
        g_cardModel[i].script_name = (Rnd() % 5 == 0) ? 0 : &g_strings[RndRange(5, 7) + (Rnd() % 2 ? TEXTS : 0)];
        At<long>(t.card, 0x14) = g_cardModel[i].feat_index; At<SData*>(t.card, 0x18) = g_cardModel[i].quest_name; At<SData*>(t.card, 0x28) = g_cardModel[i].script_name;
    }
    g_createNext = 8;
    // the leaf list of thing objects
    g_leafCount = (int)RndRange(0, MAX_LEAF);
    int order[8]; for (i = 0; i < 8; i++) order[i] = i;
    for (i = 7; i > 0; i--) { int j = (int)RndRange(0, i); int tmp = order[i]; order[i] = order[j]; order[j] = tmp; }
    LeafNode* prev = &g_leafSentinel;
    for (i = 0; i < g_leafCount; i++) { g_leaves[i].thing = g_things[order[i]].raw; g_leaves[i].prev = prev; prev->next = &g_leaves[i]; prev = &g_leaves[i]; }
    prev->next = &g_leafSentinel; g_leafSentinel.prev = prev;
    // the available quest card types
    g_typeCount = (int)RndRange(0, MAX_TYPES);
    TypeNode* tprev = &g_typeSentinel;
    for (i = 0; i < g_typeCount; i++)
    {
        TypeNode& n = g_types[i];
        n.def_name = (Rnd() % 12 == 0) ? 0 : &g_strings[RndRange(0, 4) + (Rnd() % 2 ? TEXTS : 0)];
        n.script_name = (Rnd() % 6 == 0) ? 0 : &g_strings[RndRange(5, 7) + (Rnd() % 2 ? TEXTS : 0)];
        n.replayable = (unsigned char)Rnd(); n.feat_index = (Rnd() % 3 == 0) ? -1 : RndRange(0, 2);
        n.prev = tprev; tprev->next = &n; tprev = &n;
    }
    tprev->next = &g_typeSentinel; g_typeSentinel.prev = tprev;
    At<void*>(g_questManager, 0x58) = &g_typeSentinel;
    // the centre's two vectors
    g_oldCount = (int)RndRange(0, MAX_OLD);
    int capacity = g_oldCount + (int)RndRange(0, 2);
    char* cards = (char*)malloc(capacity * 8 + 8);
    for (i = 0; i < g_oldCount; i++)
    {
        g_oldCards[i] = (Rnd() % 5 == 0) ? -1 : (int)RndRange(0, 7);
        At<void*>(cards, i * 8) = g_elementVtable; At<void*>(cards, i * 8 + 4) = g_oldCards[i] < 0 ? 0 : (void*)g_things[g_oldCards[i]].raw;
    }
    At<char*>(g_centre, 0x18) = cards; At<char*>(g_centre, 0x1c) = cards + g_oldCount * 8; At<char*>(g_centre, 0x20) = cards + capacity * 8;
    g_positionCount = (Rnd() % 7 == 0) ? 0 : (int)RndRange(1, MAX_POS);
    for (i = 0; i < MAX_POS; i++) for (int k = 0; k < 3; k++) g_positions[i][k] = (float)RndRange(-4000, 4000) / 8.0f;
    At<void*>(g_centre, 0x24) = g_positions; At<void*>(g_centre, 0x28) = g_positions[g_positionCount]; At<void*>(g_centre, 0x2c) = g_positions[MAX_POS];
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    int i;
    for (i = 0; i < 80; i++) g_physicsVtable[i] = (void*)PhysicsTrap;
    g_physicsVtable[72] = (void*)PhysicsGetRHSet;
    g_elementVtable[0] = (void*)StubPointerScalarDtor;
    for (i = 0; i < STRINGS; i++) { g_strings[i].chars = g_texts[i % TEXTS]; g_strings[i].length = (long)strlen(g_texts[i % TEXTS]); }

    int cases = 0;
    for (int n = 0; n < 60000; n++)
    {
        BuildCase();
        static unsigned char centre0[0x34], manager0[0x98], gui0[0x100], world0[0xb0];
        static Thing things0[THINGS];
        memcpy(centre0, g_centre, sizeof centre0); memcpy(manager0, g_questManager, sizeof manager0); memcpy(gui0, g_playerGui, sizeof gui0);
        memcpy(world0, g_world, sizeof world0); memcpy(things0, g_things, sizeof things0);
        int capacity0 = (int)(At<char*>(g_centre, 0x20) - At<char*>(g_centre, 0x18)) / 8;

        // the model first, on the same objects; then the objects are put back and the row runs
        g_evCount = g_wantCount = 0; g_tempUsed = 0; g_tempLive = 0; g_createNext = 8;
        g_modelListRan = 0; g_modelling = 1; RunModel(); g_modelling = 0;
        static unsigned char centre_model[0x34]; static unsigned char manager_model[0x98];
        memcpy(centre_model, g_centre, sizeof centre_model); memcpy(manager_model, g_questManager, sizeof manager_model);
        int created_model = g_createNext;
        memcpy(g_centre, centre0, sizeof centre0); memcpy(g_questManager, manager0, sizeof manager0); memcpy(g_things, things0, sizeof things0);
        g_createNext = 8; g_pointerCtors = g_pointerDtors = 0;

        CallFrameUpdate(g_centre);

        int bad = g_fail;
        if (g_evCount != g_wantCount) { printf("FAIL case %d: %d events, model %d\n", n, g_evCount, g_wantCount); bad = 1; }
        for (i = 0; i < g_evCount && i < g_wantCount && i < MAX_EV && !bad; i++)
            if (memcmp(&g_ev[i], &g_want[i], sizeof(Ev)))
            {
                printf("FAIL case %d: event %d is %c(%ld,%ld,%ld,%ld,%ld), model %c(%ld,%ld,%ld,%ld,%ld)\n", n, i, g_ev[i].t, g_ev[i].a, g_ev[i].b, g_ev[i].c, g_ev[i].d, g_ev[i].e,
                       g_want[i].t, g_want[i].a, g_want[i].b, g_want[i].c, g_want[i].d, g_want[i].e);
                bad = 1;
            }
        bool list_ran = g_modelListRan != 0;
        char* start = At<char*>(g_centre, 0x18); char* finish = At<char*>(g_centre, 0x1c); char* end = At<char*>(g_centre, 0x20);
        int count = (int)(finish - start) / 8;
        if (!bad && list_ran)
        {
            if (count != g_listModelCount) { printf("FAIL case %d: %d cards listed, model %d\n", n, count, g_listModelCount); bad = 1; }
            for (i = 0; i < count && !bad; i++)
                if (ThingIndex(At<void*>(start, i * 8 + 4)) != g_listModel[i]) { printf("FAIL case %d: card %d is thing %d, model %d\n", n, i, ThingIndex(At<void*>(start, i * 8 + 4)), g_listModel[i]); bad = 1; }
            if (!bad && g_pointerCtors - g_pointerDtors != count - g_oldCount) { printf("FAIL case %d: pointer constructions %ld, destructions %ld, list %d -> %d\n", n, g_pointerCtors, g_pointerDtors, g_oldCount, count); bad = 1; }
            if (!bad && (end < finish || (end - start) / 8 > 64)) { printf("FAIL case %d: list storage\n", n); bad = 1; }
            if ((end - start) / 8 != (g_typeCount > capacity0 && g_reserveWorks ? g_typeCount : capacity0) && count > 0) g_statOverflow++;
        }
        else if (!bad)
        {
            if (count != g_oldCount || g_pointerCtors || g_pointerDtors) { printf("FAIL case %d: the card list was touched\n", n); bad = 1; }
        }
        if (!bad && g_tempLive != 0) { printf("FAIL case %d: %d string temporaries left\n", n, g_tempLive); bad = 1; }
        if (!bad && g_createNext != created_model) { printf("FAIL case %d: Create count\n", n); bad = 1; }
        // the centre apart from its list words, the manager, and every quest card component
        At<char*>(centre_model, 0x18) = start; At<char*>(centre_model, 0x1c) = finish; At<char*>(centre_model, 0x20) = end;
        if (!bad && memcmp(g_centre, centre_model, sizeof centre_model)) { printf("FAIL case %d: centre fields differ\n", n); bad = 1; }
        if (!bad && memcmp(g_questManager, manager_model, sizeof manager_model)) { printf("FAIL case %d: quest manager differs\n", n); bad = 1; }
        if (!bad && (memcmp(g_playerGui, gui0, sizeof gui0) || memcmp(g_world, world0, sizeof world0))) { printf("FAIL case %d: gui or world written\n", n); bad = 1; }
        for (i = 0; i < THINGS && !bad; i++)
        {
            Thing& t = g_things[i];
            if (At<long>(t.card, 0x14) != g_cardModel[i].feat_index || At<SData*>(t.card, 0x18) != g_cardModel[i].quest_name || At<SData*>(t.card, 0x28) != g_cardModel[i].script_name)
            { printf("FAIL case %d: quest card of thing %d differs\n", n, i); bad = 1; }
            At<long>(things0[i].card, 0x14) = At<long>(t.card, 0x14); At<SData*>(things0[i].card, 0x18) = At<SData*>(t.card, 0x18); At<SData*>(things0[i].card, 0x28) = At<SData*>(t.card, 0x28);
            At<unsigned short>(things0[i].raw, 0x8c) = At<unsigned short>(t.raw, 0x8c);
            if (!bad && memcmp(&t, &things0[i], sizeof(Thing))) { printf("FAIL case %d: thing %d was written\n", n, i); bad = 1; }
        }
        free(At<char*>(g_centre, 0x18));
        if (bad) { printf("FAIL\n"); return 1; }
        cases++;
    }
    long* stats[] = { &g_statTutorialWait, &g_statTutorialEvent, &g_statTutorialNoInventory, &g_statSkip, &g_statKillOld, &g_statNoPositions, &g_statReuse,
                      &g_statCreate, &g_statCreateNull, &g_statDummyReuse, &g_statDummyCreate, &g_statDummyMissing, &g_statNameEqualData, &g_statNameEqualText,
                      &g_statNameDiffer, &g_statNameNull, &g_statScriptSet, &g_statLastPosition, &g_statKillLeft, &g_statOverflow, &g_statFeatDiffer };
    for (int k = 0; k < (int)(sizeof stats / sizeof stats[0]); k++)
        if (*stats[k] == 0) { printf("FAIL path %d never taken\n", k); return 1; }
    printf("cases %d: tutorial wait %ld, event %ld, no inventory %ld; list skipped %ld; old cards killed %ld, no positions %ld, reused %ld, created %ld (null %ld),\n",
           cases, g_statTutorialWait, g_statTutorialEvent, g_statTutorialNoInventory, g_statSkip, g_statKillOld, g_statNoPositions, g_statReuse, g_statCreate, g_statCreateNull);
    printf("  feat card reused %ld, created %ld, no dummy def %ld, feat differs %ld, names equal by data %ld / by text %ld / differ %ld / null %ld, script name set %ld, last position %ld, leftovers killed %ld, list regrown %ld\n",
           g_statDummyReuse, g_statDummyCreate, g_statDummyMissing, g_statFeatDiffer, g_statNameEqualData, g_statNameEqualText, g_statNameDiffer, g_statNameNull, g_statScriptSet,
           g_statLastPosition, g_statKillLeft, g_statOverflow);
    printf("0077d5b0_TEST PASS\n");
    return 0;
}
