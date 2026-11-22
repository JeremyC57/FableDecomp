// CThingScriptUIDReplacer::CThingScriptUIDReplacer
// Leaf: ??0CThingScriptUIDReplacer@@QAE@ABVCCharString@@@Z
// Flags: default
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_REPLACE_EGOD_PASS24
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_IMPLICIT_DTOR_PASS27
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_ALLOCATEUID
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CWORLDMAP_CTOR
#define FABLE_RETAIL_CWORLDMAP_PLAYERPOSITION_PASS27
#define FABLE_RETAIL_CDRAWMAPINFO_PASS23
#define private public
#define protected public
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingManager.h"
#include "engine/fable_script_text.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CThingScriptUIDReplacer.h"
#include "fable_assert.h"

CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64);   // 0x0099F930


inline CCharString CThingScriptUIDReplacer::GetSwapUniqueIdentifier(long unique_identifier) const
{
    CCharString id = "__UID_SWAP_NO_" + GFIntToCharString(unique_identifier) + "__";
    return id;
}

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline unsigned __int64 __fastcall CPersistContext::GetDefaultVal<unsigned __int64>() { return 0; }

#pragma auto_inline(off)
template <> inline void CPersistContext::Transfer<unsigned __int64>(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteUHUGE(value);
        break;
    case MODE_LOAD_BINARY: {
        CPersistTraits<unsigned __int64> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<unsigned __int64> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<unsigned __int64> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        // Inferred: only the five persistence modes reach the switch. Retail 0x0044BB11
        // and Ego_r 0x004462C1 both dispatch directly from Mode without a range test.
#ifdef _MSC_VER
        __assume(0);
#elif defined(__GNUC__)
        __builtin_unreachable();
#endif
    }
}
#pragma auto_inline(on)

CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString& previous_script)
{
    ReplacedUIDScript = "";
    ReplacedUIDScript = previous_script;
    CCharString& script = ReplacedUIDScript;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    FABLE_RETAIL_ASSERT(phero != 0);
    C3DVector pos(GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos());
    long level = GetWorldMap().GetMapNumberAt(pos);

    std::list<unsigned __int64> replaced_uids;
    CStringParser parser(script);
    while (parser.ReadAsStringUntilString("NewThing") != "") {
        parser.SkipPastString("NewThing");
        parser.SkipWhitespace();
        parser.ReadAsStringUntilString(";");
        parser.SkipPastString(";");
        CCharString sub_script = parser.ReadAsStringUntilString("EndThing");
        CStringParser sub_parser(sub_script);
        CPersistContext sub_context;
        sub_context.Init(sub_parser, 0, true, 0, 0, 0);
        CCharString def_type;
        unsigned __int64 old_uid;
        sub_context.Transfer("DefinitionType", def_type);
        sub_context.Transfer("UID", old_uid);
        if (old_uid != 0)
            replaced_uids.push_back(old_uid);
    }

    long unique_identifier = 0;
    for (std::list<unsigned __int64>::iterator it = replaced_uids.begin(); it != replaced_uids.end(); ++it) {
        unsigned __int64 old_uid = *it;
        CCharString old_uid_string = GFUHUGEToCharString(old_uid);
        CCharString delimiter_string = GetSwapUniqueIdentifier(unique_identifier);
        CCharString new_uid_string = delimiter_string;
        script.Replace(old_uid_string, new_uid_string);
        ++unique_identifier;
    }

    unique_identifier = 0;
    for (std::list<unsigned __int64>::iterator it = replaced_uids.begin(); it != replaced_uids.end(); ++it) {
        CCharString delimiter_string = GetSwapUniqueIdentifier(unique_identifier);
        CCharString replace_string = delimiter_string;
        unsigned __int64 new_uid = GetThingManager().AllocateNewThingUID(level, false, false);
        CCharString new_uid_string = GFUHUGEToCharString(new_uid);
        script.Replace(replace_string, new_uid_string);
        ++unique_identifier;
    }
}

// The game singleton selects the fixture's typed world and player boundaries.
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CDrawMapInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#undef protected
#undef private
CCharString::CCharString() : PStringData(0) {}
CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    if (length < 0)
        length = strlen(text);
    PStringData->NoRefs = 1;
    PStringData->Data.PData = new char[length + 1];
    PStringData->Data.StringLength = length;
    PStringData->Data.DataLength = length + 1;
    PStringData->Data.UseFastExtend = false;
    memcpy(PStringData->Data.PData, text, length);
    PStringData->Data.PData[length] = 0;
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData)
{
    if (PStringData)
        ++PStringData->NoRefs;
}
CCharString::~CCharString()
{
    if (PStringData && --PStringData->NoRefs == 0) {
        delete[] PStringData->Data.PData;
        delete PStringData;
    }
}
static const char* NarrowText(const CCharString& text)
{
    return text.PStringData ? text.PStringData->Data.PeekData() : "";
}
const char CCharString::NullChar = 0;
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (other.PStringData != PStringData) {
        if (other.PStringData)
            ++other.PStringData->NoRefs;
        if (PStringData && --PStringData->NoRefs == 0) {
            delete[] PStringData->Data.PData;
            delete PStringData;
        }
        PStringData = other.PStringData;
    }
    return *this;
}

template<> boost::scoped_ptr<NThingMap::CThingMapManager>::~scoped_ptr() {}
CIDrawWorldMap::~CIDrawWorldMap() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CWorldMap::CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI&, unsigned long)
    : World(world), Component(component), Granularity(1), GranularityShift(0) {}
CWorldMap::~CWorldMap() {}
// Unused vtable entries fail if the leaf calls them.
void CWorldMap::DeleteAllLevels() { abort(); }
void CWorldMap::DeleteAllRegions() { abort(); }
void CWorldMap::LoadFromFile(CAFile&, long) { abort(); }
void CWorldMap::SaveToFile(CAFile&) { abort(); }
void CWorldMap::RemoveLevel(long) { abort(); }
void CWorldMap::LoadTopologyFromFile(long, CCountedPointer<CAFile>, CASuspendableProcess&) { abort(); }
void CWorldMap::PostLoadTopology(long) { abort(); }
void CWorldMap::ForceLoad(long) { abort(); }
void CWorldMap::ForceUnload(long) { abort(); }
bool CWorldMap::IsMapUsed(long) const { abort(); }
long CWorldMap::GetGranularity() const { abort(); }
const C2DBoxI& CWorldMap::GetDimensions() const { abort(); }
CMap& CWorldMap::GetMapAt(const C2DCoordI&) { abort(); }
const CMap& CWorldMap::PeekMapAt(const C2DCoordI&) const { abort(); }
long CWorldMap::GetMapNumberAt(const C2DCoordI&) const { abort(); }
CMap& CWorldMap::GetMap(long) { abort(); }
const CMap& CWorldMap::PeekMap(long) const { abort(); }
void CWorldMap::GetAllMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::GetAllLoadedMapsInArea(const C2DBoxI&, std::vector<unsigned long, std::allocator<unsigned long> >&) const { abort(); }
void CWorldMap::PostRegionLoad(long) { abort(); }
C2DCoordI CWorldMap::GetMapPos(long) const { abort(); }
bool CWorldMap::IsMapChangeable(long) const { abort(); }
bool CWorldMap::IsPosChangeable(const C2DCoordI&) const { abort(); }
std::set<CCharString, std::less<CCharString>, std::allocator<CCharString> > CWorldMap::GetLoadableSections() const { abort(); }
void CWorldMap::Update() { abort(); }
void CWorldMap::HeightAreaChanged(const C2DBoxI*) { abort(); }
long CWorldMap::DrawGetFrame() const { abort(); }
long CWorldMap::DrawGetNoMaps() const { abort(); }
CDrawMapInfo CWorldMap::DrawGetMapInfo(long) const { abort(); }
C2DBoxI CWorldMap::DrawGetDimensions() const { abort(); }
long CWorldMap::DrawGetGranularity() const { abort(); }
bool CWorldMap::DrawIsPosInMap(const C3DVector&) const { abort(); }
float CWorldMap::DrawGetSizeZAt(const C3DVector&, float) const { abort(); }
C3DVector CWorldMap::DrawConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const { abort(); }
long CWorldMap::DrawGetMyPlayerMapNumber() const { abort(); }
bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { abort(); }
void CWorldMap::LoadScriptBrushesFromFile(long, CCountedPointer<CAFile>) { abort(); }
long CWorldMap::AllocateNewMap() { abort(); }


CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
const CCharString& CCharString::operator=(const char* value)
{
    return *this = CCharString(value);
}
bool CCharString::operator!=(const char* value) const
{
    return strcmp(NarrowText(*this), value) != 0;
}
const CCharString& CCharString::operator+=(const CCharString& value)
{
    const char* left = NarrowText(*this);
    const char* right = NarrowText(value);
    char buffer[1024];
    if (strlen(left) + strlen(right) >= sizeof(buffer))
        abort();
    strcpy(buffer, left);
    strcat(buffer, right);
    *this = buffer;
    return *this;
}
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right)
{
    CCharString result(left);
    result += right;
    return result;
}
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right)
{
    CCharString result(left);
    result += CCharString(right);
    return result;
}
CCharString FABLE_FASTCALL GFIntToCharString(long value)
{
    char text[32];
    sprintf(text, "%ld", value);
    return CCharString(text);
}
CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64 value)
{
    char text[32];
    sprintf(text, "%I64u", value);
    return CCharString(text);
}

void CStringParser::Init(const char* text, void (FABLE_FASTCALL* error)(const CCharString&),
                         const CCharString& filename, long offset)
{
    String = text;
    ErrorCallback = error;
    Filename = filename;
    BaseLineOffset = offset;
    CursorPos.StringPos = 0;
    CursorPos.Line = 0;
    CursorPos.InQuotes = false;
}
CCharString CStringParser::ReadAsStringUntilString(const CCharString& delimiter)
{
    const char* start = String + CursorPos.StringPos;
    const char* end = strstr(start, NarrowText(delimiter));
    if (!end) {
        CursorPos.StringPos += strlen(start);
        return CCharString();
    }
    CursorPos.StringPos = static_cast<unsigned long>(end - String);
    return CCharString(start, static_cast<long>(end - start));
}
bool CStringParser::SkipPastString(const CCharString& delimiter)
{
    const char* text = NarrowText(delimiter);
    const char* at = strstr(String + CursorPos.StringPos, text);
    if (!at)
        return false;
    CursorPos.StringPos = static_cast<unsigned long>(at - String + strlen(text));
    return true;
}
long CStringParser::SkipWhitespace()
{
    unsigned long start = CursorPos.StringPos;
    while (isspace(static_cast<unsigned char>(String[CursorPos.StringPos])))
        ++CursorPos.StringPos;
    return static_cast<long>(CursorPos.StringPos - start);
}

static int text_loads, types_seen;
void CPersistContext::Init(CStringParser& parser, void (FABLE_FASTCALL*)(const CCharString&),
                           bool defaults, const CDefinitionManager*, const CSymbolMap*, CCharString*)
{
    PParser = &parser;
    UseDefaults = defaults;
    Mode = MODE_LOAD_TEXT;
}
void CPersistContext::CheckCRC(const char* key)
{
    if (strcmp(key, "UID") != 0)
        abort();
}
template <> void CPersistContext::Transfer<CCharString>(const char* key, CCharString& value, const CCharString&)
{
    if (strcmp(key, "DefinitionType") != 0 || Mode != MODE_LOAD_TEXT)
        abort();
    ++types_seen;
    if (PParser->SkipPastString(CCharString("DefinitionType=")))
        value = PParser->ReadAsStringUntilString(CCharString(";"));
}
void CPersistContext::TransferObjectLoadText(const char* key, CPersistContext_TransferableComponentGenericType& item)
{
    if (strcmp(key, "UID") != 0 || Mode != MODE_LOAD_TEXT)
        abort();
    ++text_loads;
    item.TransferIn(*this, *PParser);
}
void CPersistContext::TransferObjectSaveText(const char*, CPersistContext_TransferableComponentGenericType&) { abort(); }
void CDataOutputStream::WriteUHUGE(unsigned __int64) { abort(); }
void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream&, unsigned __int64&) { abort(); }
template <> void CPersistContext_TransferableComponent<unsigned __int64>::TransferOut(CPersistContext&, CCharString&) { abort(); }
template <> void CPersistContext_TransferableComponent<unsigned __int64>::TransferIn(CPersistContext&, CStringParser& parser)
{
    if (!parser.SkipPastString(CCharString("UID=")))
        return;
    CCharString value = parser.ReadAsStringUntilString(CCharString(";"));
    const char* digit = NarrowText(value);
    unsigned __int64 parsed = 0;
    while (*digit >= '0' && *digit <= '9')
        parsed = parsed * 10 + (*digit++ - '0');
    *Object = parsed;
}

static CPlayerManager* player_manager;
static CPlayer* player;
static CThingPlayerCreature* hero;
static CThingManager* things;
static CWorldMap* map;
static const C3DVector player_position(7.5f, -2.0f, 11.0f);
static unsigned __int64 allocations[4];
static int allocation_count, map_queries, player_queries, controlled_creature_queries;

CPlayerManager& CThingScriptUIDReplacer::GetPlayerManager() { return *player_manager; }
CWorldMap& CThingScriptUIDReplacer::GetWorldMap() { return *map; }
CThingManager& CThingScriptUIDReplacer::GetThingManager() { return *things; }
const CCharString& CThingScriptUIDReplacer::GetReplacedUIDScript() const { return ReplacedUIDScript; }
CPlayer& CPlayerManager::GetMainPlayer()
{
    if (this != player_manager)
        abort();
    ++player_queries;
    return *player;
}
CThingPlayerCreature* CPlayer::GetPControlledCreature() const
{
    if (this != player)
        abort();
    ++controlled_creature_queries;
    return hero;
}
const C3DVector& CThing::GetPos() const
{
    if (this != static_cast<CThing*>(hero))
        abort();
    return player_position;
}
long CWorldMap::GetMapNumberAt(const C3DVector& position) const
{
    if (this != map || position.X != 7.5f || position.Y != -2.0f || position.Z != 11.0f)
        abort();
    ++map_queries;
    return 12;
}
bool CWorldMap::IsMapLoaded(long) const { abort(); }
bool CWorldMap::IsPosPassable(const C2DCoordI&) const { abort(); }
unsigned __int64 CThingManager::AllocateNewThingUID(long level, bool persistent, bool is_hero)
{
    if (this != things || level != 12 || persistent || is_hero || allocation_count >= 4)
        abort();
    return allocations[allocation_count++];
}

static bool Check(const char* input, const char* expected, int records, int calls, const char* original_after = 0)
{
    allocation_count = map_queries = player_queries = controlled_creature_queries = text_loads = types_seen = 0;
    const CCharString script(input);
    CThingScriptUIDReplacer replacer(script);
    return strcmp(NarrowText(replacer.GetReplacedUIDScript()), expected) == 0
        && strcmp(NarrowText(script), original_after ? original_after : input) == 0
        && allocation_count == calls && map_queries == 1 && player_queries == 2
        && controlled_creature_queries == 2 && text_loads == records && types_seen == records;
}

int main()
{
    // Singleton-manager handles are opaque here. Only the fixture boundaries use them.
    static unsigned __int64 game_store[sizeof(CMainGameComponent) / sizeof(unsigned __int64) + 1];
    static unsigned __int64 world_store[sizeof(CWorld) / sizeof(unsigned __int64) + 1];
    static unsigned __int64 player_manager_store[sizeof(CPlayerManager) / sizeof(unsigned __int64) + 1];
    static unsigned __int64 player_store[sizeof(CPlayer) / sizeof(unsigned __int64) + 1];
    static unsigned __int64 hero_store[sizeof(CThingPlayerCreature) / sizeof(unsigned __int64) + 1];
    static unsigned __int64 things_store[sizeof(CThingManager) / sizeof(unsigned __int64) + 1];
    player_manager = reinterpret_cast<CPlayerManager*>(player_manager_store);
    player = reinterpret_cast<CPlayer*>(player_store);
    hero = reinterpret_cast<CThingPlayerCreature*>(hero_store);
    things = reinterpret_cast<CThingManager*>(things_store);
    CWorldMap world_map(*reinterpret_cast<CMainGameComponent*>(game_store),
        *reinterpret_cast<CWorld*>(world_store), C2DBoxI(), 1);
    map = &world_map;

    if (!Check("no records", "no records", 0, 0))
        return 1;
    if (!Check("header NewThing A; DefinitionType=A; UID=0; EndThing",
               "header NewThing A; DefinitionType=A; UID=0; EndThing", 1, 0))
        return 2;
    if (!Check("header NewThing A; DefinitionType=A; Flag=1; EndThing",
               "header NewThing A; DefinitionType=A; Flag=1; EndThing", 1, 0))
        return 3;
    allocations[0] = 475;
    if (!Check("header NewThing A; DefinitionType=A; UID=123; EndThing Ref=123;",
               "header NewThing A; DefinitionType=A; UID=475; EndThing Ref=475;", 1, 1, "header NewThing A; DefinitionType=A; UID="))
        return 4;
    allocations[0] = 200;
    allocations[1] = 300;
    if (!Check("header NewThing A; DefinitionType=A; UID=100; EndThing NewThing B; DefinitionType=B; UID=200; EndThing",
               "header NewThing A; DefinitionType=A; UID=200; EndThing NewThing B; DefinitionType=B; UID=300; EndThing", 2, 2, "header NewThing A; DefinitionType=A; UID="))
        return 5;
    allocations[0] = 5000000000;
    allocations[1] = 9000000000;
    if (!Check("header NewThing A; DefinitionType=A; UID=7000000000; EndThing NewThing B; DefinitionType=B; UID=7000000000; EndThing",
               "header NewThing A; DefinitionType=A; UID=5000000000; EndThing NewThing B; DefinitionType=B; UID=5000000000; EndThing", 2, 2, "header NewThing A; DefinitionType=A; UID="))
        return 6;
    puts("0083e1f0_TEST PASS");
    return 0;
}