#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long
#endif
#include <new>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CTHING_CONSTRUCTORS
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_CCHARSTRING_DATA
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGBASEDEF
#pragma once

#include <stddef.h>

#pragma once



#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifdef _MSC_VER
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]
#else
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_host_abi_ignored_, __LINE__)[1]
#endif


#ifndef FABLE_RETAIL_CCHARSTRING_DATA
class CWideStringData
{ public:
    wchar_t* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_i32 owners;
};

#endif

#pragma pack(push, 1)
#ifdef FABLE_RETAIL_CCHARSTRING_DATA



template<class T> class CBasicString {
public:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    static long __fastcall Compare(const T*,const T*);
#endif
public:
    const T* PeekData() const { return PData; }
#ifdef FABLE_RETAIL_FACTIONS
    bool operator<(const CBasicString&)const;
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    unsigned long GetLength() const { return StringLength; }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_REGION_GRAPH)
    bool operator==(const T* text) const;
#endif
};
class CCharStringData {
public:
    CBasicString<char> Data;
public:
    long NoRefs;
};
#else
class CCharStringData
{ public:
    char* text;
    fable_u32 unknown04;
    fable_u32 unknown08;
    fable_u8 flags0C;
    fable_i32 owners;
};
#endif
#pragma pack(pop)

#ifndef FABLE_RETAIL_CCHARSTRING_DATA

class CCharString;

class CWideString
{
public:
    CWideString();
    CWideString(const CWideString& other);
    explicit CWideString(const wchar_t* text);
    ~CWideString();
    operator const wchar_t*() const;
    const wchar_t* GetAsCharArray() const;                     
    long GetLength() const;
    long FindCharPos(wchar_t c) const;                         
    void __cdecl Sprintf(const wchar_t* format, ...);          
    void SetLength(long length);                               
    const CWideString& operator=(const CWideString& other);
    const CWideString& operator=(const wchar_t* text);
    const CWideString& operator+=(const CWideString& other);
    const CWideString& operator+=(const wchar_t* text);
    bool operator!=(const wchar_t* other) const;

    static CWideString FABLE_FASTCALL CreateFromCharString(
        const CCharString& text);

public:
    explicit CWideString(const CCharString& text);
    void UnassignString();

public:
    CWideStringData* AllocStringData(                          
        const wchar_t* text,
        long length);

public:

    CWideStringData* storage_;
};

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const CWideString& right);
CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right);
CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right);

class CCharString
{
public:
    CCharString();
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);                     
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString& other);
    const CCharString& operator=(const char* text);
    CWideString ToWideString() const;

public:
    void UnassignString();                                     
    CCharStringData* AllocStringData(                          
        const char* text,
        long length);

public:
    CCharStringData* storage_;
};

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right);



extern fable_i32 g_CWideStringInstanceCount_013BCA20;
extern fable_i32 g_CCharStringInstanceCount_013BD800;
extern wchar_t g_FableEmptyWideString_0129A8E0[1];

FABLE_STATIC_ASSERT(sizeof(CWideStringData) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCharStringData) == 0x11);
FABLE_STATIC_ASSERT(sizeof(CWideString) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CCharString) == 0x04);

#endif








#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SHOP
#define FABLE_RETAIL_SHOP_TREE_BODY
#define FABLE_RETAIL_SHOP_MAP_BODY
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#ifndef FABLE_ENGINE_CTCSHOP_H
#define FABLE_ENGINE_CTCSHOP_H
#ifndef FABLE_ENGINE_CTCBASE_H
#define FABLE_ENGINE_CTCBASE_H
















#ifdef FABLE_RETAIL_CTCBASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CBASECLASSNONCOPYABLE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H




#ifndef FABLE_ENGINE_CBASECLASS_H
#define FABLE_ENGINE_CBASECLASS_H









class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);

#endif 


class CBaseClassNonCopyable : public CBaseClass {
public:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
public:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

#endif 

#ifndef FABLE_ENGINE_ETCINTERFACETYPE_H
#define FABLE_ENGINE_ETCINTERFACETYPE_H

enum ETCInterfaceType {
    TCI_NULL = 0,
    TCI_PERFORM_ACTION = 1,
    TCI_PHYSICS = 2,
    TCI_APPEARANCE_MORPH = 3,
    TCI_HERO_STATS = 4,
    TCI_WOUND = 5,
    TCI_SMOKE_GENERATOR = 6,
    TCI_NAVIGATION = 7,
    TCI_TARGETING = 8,
    TCI_TARGETED = 9,
    TCI_CREATURE_STATS = 10,
    TCI_CREATURE_AI = 11,
    TCI_CREATURE_EXPRESSION = 12,
    TCI_ON_ACTION_USE = 13,
    TCI_CARRIED_ON_ACTION_USE = 14,
    TCI_TALK = 15,
    TCI_SOUND_PLAYER = 16,
    TCI_INVENTORY = 17,
    TCI_INVENTORY_CLOTHING = 18,
    TCI_INVENTORY_WEAPONS = 19,
    TCI_INVENTORY_STATS = 20,
    TCI_INVENTORY_MAGIC = 21,
    TCI_INVENTORY_EXPERIENCE = 22,
    TCI_INVENTORY_TRADE = 23,
    TCI_INVENTORY_AUGMENT = 24,
    TCI_INVENTORY_QUESTS = 25,
    TCI_INVENTORY_ITEM = 26,
    TCI_INVENTORY_MAP = 27,
    TCI_EDITOR = 28,
    TCI_EDITOR_ANIMATION_THING = 29,
    TCI_EDITOR_ANIMATION_CREATURE = 30,
    TCI_SCRIPTED_CONTROL = 31,
    TCI_MESH_AUTOMATIC_ENTITY_CREATOR = 32,
    TCI_THING_OWNER = 33,
    TCI_VILLAGE = 34,
    TCI_VILLAGE_MEMBER = 35,
    TCI_JACK_SHIELD = 36,
    TCI_JACK_PILLAR = 37,
    TCI_DOOR = 38,
    TCI_LIGHT = 39,
    TCI_CLOCK = 40,
    TCI_HERO = 41,
    TCI_ROTATE = 42,
    TCI_RESPAWN = 43,
    TCI_WEATHERVANE = 44,
    TCI_SINGLE_USER_ACTION_POINTS = 45,
    TCI_NAMED_USER_ACTION_POINTS = 46,
    TCI_EXCLUSIVE_USE = 47,
    TCI_COMBAT = 48,
    TCI_ENTITY_MODE_MANAGER = 49,
    TCI_CREATED_ENTITY = 50,
    TCI_OWNED_ENTITY = 51,
    TCI_PERCEIVE_THING = 52,
    TCI_BED = 53,
    TCI_STEALTH = 54,
    TCI_NOISE = 55,
    TCI_CUPBOARD = 56,
    TCI_TROPHY = 57,
    TCI_MAPWHO = 58,
    TCI_DRIVER_QUEST = 59,
    TCI_CONTAINER = 60,
    TCI_CREATURE_GENERATOR = 61,
    TCI_DRIVER_CAMERA_POINTS = 62,
    TCI_CAMERA_POINT = 63,
    TCI_DISCOVERY = 64,
    TCI_EXPLOSION = 65,
    TCI_ENTITY_EVENTS = 66,
    TCI_LOOK = 67,
    TCI_SHAPE_MANAGER = 68,
    TCI_EMOTEICON = 69,
    TCI_CARRYING = 70,
    TCI_CARRYABLE = 71,
    TCI_WEAPON = 72,
    TCI_ENEMY = 73,
    TCI_BANDIT = 74,
    TCI_WEAPON_TRAIL = 76,
    TCI_SOLID_TO_THING = 77,
    TCI_SMASHABLE = 78,
    TCI_CREATURE_OPINION_OF_HERO = 79,
    TCI_OPINION_SOURCE = 80,
    TCI_HERO_OPINION_DEED_LOG = 81,
    TCI_HERO_PODIUM = 82,
    TCI_SHOP = 83,
    TCI_STOCK_ITEM = 84,
    TCI_BLACKSMITHS = 85,
    TCI_TIME_APPEARANCE_FADE = 86,
    TCI_FAMILY_MEMBER = 87,
    TCI_NAVIGATION_SEED = 88,
    TCI_GIFT = 89,
    TCI_ANIMATION = 90,
    TCI_GRAPHIC_APPEARANCE_NEW = 91,
    TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT = 92,
    TCI_HERO_SUIT = 93,
    TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS = 94,
    TCI_HIGHLIGHT_ITEM = 95,
    TCI_EMPTY_DRIVER = 96,
    TCI_CREATURE_ITEM_LEVITATION = 97,
    TCI_CREATURE_HIVE = 98,
    TCI_REGION_ENTRANCE = 99,
    TCI_REGION_EXIT = 100,
    TCI_ACTIVATION_RECEPTOR = 101,
    TCI_ACTIVATION_TRIGGER = 102,
    TCI_DEGRADABLE = 103,
    TCI_HERO_EXPERIENCE = 104,
    TCI_EXPERIENCE_ORB = 105,
    TCI_HERO_CENTRE = 106,
    TCI_HERO_GUIDE = 107,
    TCI_QUEST_CARD = 108,
    TCI_FLAMMABLE = 109,
    TCI_HERO_CENTRE_DOOR_MARKER = 110,
    TCI_HERO_ABILITIES = 111,
    TCI_BOASTING_PODIUM = 112,
    TCI_BOASTING_POSITION = 113,
    TCI_BOASTING_AREA = 114,
    TCI_BOASTING_CROWD_CONTROL = 115,
    TCI_CREATURE_GROUP_BOASTING_CROWD = 116,
    TCI_VOLUME_CONTAINMENT_TRACKER = 117,
    TCI_DEAD_SHOT_IN_THING_TRACKER = 118,
    TCI_SHOT_DEFINITION = 119,
    TCI_SKELETAL_MORPH = 120,
    TCI_SWITCHABLE_NAVIGATION = 121,
    TCI_REGION_FOLLOWER = 122,
    TCI_GATHER_POINT = 123,
    TCI_DAY_OR_NIGHT_ONLY_SUPPORT = 124,
    TCI_TRAP = 125,
    TCI_UNDEAD_SOUL = 126,
    TCI_HIT_LOCATIONS = 127,
    TCI_MONEY_RECEIVER = 128,
    TCI_HERO_RANGED_ATTACKS = 129,
    TCI_PARTICLE_ATTACHER = 130,
    TCI_ANIMATING_OBJECT = 131,
    TCI_BERSERK = 132,
    TCI_DIVINE_WRATH = 133,
    TCI_UNHOLY_POWER = 134,
    TCI_MULTI_STRIKE = 135,
    TCI_MULTI_ARROW = 136,
    TCI_BURNT_EFFECT = 137,
    TCI_ELECTROCUTED_EFFECT = 138,
    TCI_CREATURE_TINT_EFFECT = 139,
    TCI_HASTE = 140,
    TCI_FORCE_PUSHED = 141,
    TCI_REGION_DISPLAY = 142,
    TCI_EXPRESSION = 143,
    TCI_SHOP_KEEPER = 144,
    TCI_SUMMON_SPELL = 145,
    TCI_BULLET_TIME = 146,
    TCI_PHYSICAL_SHIELD_SPELL = 147,
    TCI_WILL_LIGHTNING = 148,
    TCI_WILL_ENFLAME = 149,
    TCI_FORCE_PUSH_POWER = 150,
    TCI_DRAIN_LIFE = 151,
    TCI_LIFE_DRAINABLE = 152,
    TCI_HEAL_LIFE = 153,
    TCI_GHOST_SWORD = 154,
    TCI_EFFECT_ON_DIE = 155,
    TCI_WILL_RESPONSE = 156,
    TCI_FIREBALL_SPELL = 157,
    TCI_TURNCOAT_SPELL = 158,
    TCI_TURNCOAT = 159,
    TCI_SUMMONABLE_CREATURE = 160,
    TCI_SUMMONNED_CREATURE = 161,
    TCI_INTERNAL_MARKER = 162,
    TCI_CREATURE_GROUP = 163,
    TCI_CREATURE_GROUP_TAG = 164,
    TCI_MESH_REPLACER = 165,
    TCI_PRE_CALCULATED_NAVIGATION_ROUTE = 166,
    TCI_SPECIAL_ABILITIES = 167,
    TCI_BATTLE_CHARGE = 168,
    TCI_ASSASSIN_RUSH = 169,
    TCI_AI_SCRATCHPAD = 170,
    TCI_TAVERN = 171,
    TCI_SINGLE_PERSON_OCCUPIABLE = 172,
    TCI_BOSS = 173,
    TCI_ROCK_TROLL_SHIELD = 174,
    TCI_COMBAT_LEADER = 175,
    TCI_FISHING_SPOT = 176,
    TCI_FISHING = 177,
    TCI_STEALABLE_ITEM_LOCATION = 178,
    TCI_INTERESTING_TO_VILLAGERS = 179,
    TCI_GUARD = 180,
    TCI_CRATE_STACK = 181,
    TCI_WATER_WADER = 182,
    TCI_OVERHEAD_DISPLAY = 183,
    TCI_TAVERN_TABLE = 184,
    TCI_TAVERN_BAR = 185,
    TCI_TAVERN_TANKARD = 186,
    TCI_TAVERN_JUG = 187,
    TCI_FOLLOWED = 188,
    TCI_TRESPASSABLE = 189,
    TCI_TRESPASSER = 190,
    TCI_OBJECT_AUGMENTATIONS = 191,
    TCI_AUGMENTATION = 192,
    TCI_GOSSIP = 193,
    TCI_ACTION_USE_SCRIPTED_HOOK = 194,
    TCI_DRUNKENNESS = 195,
    TCI_ON_HERO_PUSH = 196,
    TCI_VILLAGE_CRIME = 197,
    TCI_IN_GAME_MENU = 198,
    TCI_COOP_SPIRIT = 199,
    TCI_HIT_ON_COLLISION = 200,
    TCI_GENERATES_EXPERIENCE_ON_KILLING = 201,
    TCI_CREATURE_GENERATOR_CREATOR = 202,
    TCI_HERO_RECEIVE_ITEMS = 203,
    TCI_GOLD = 204,
    TCI_CREATURE_HIT_NOTIFICATION = 205,
    TCI_AICREATURE_WILL_POWER_INDICATOR = 206,
    TCI_TAVERN_GAME = 207,
    TCI_GAME_PLAYER = 208,
    TCI_DECAPITATION = 209,
    TCI_TAVERN_GAME_MASTER = 210,
    TCI_IDLE_SCHEDULER = 211,
    TCI_SLEEP = 212,
    TCI_BUYABLE_HOUSE = 213,
    TCI_DUMMY_VILLAGER = 214,
    TCI_WIFE = 215,
    TCI_PARTICLE_ATTACHMENT_TRACKER = 216,
    TCI_CREATURE_FLY_ON_SPLINE = 217,
    TCI_WALL_MOUNT = 218,
    TCI_WALL_MOUNT_EFFECTS = 219,
    TCI_FISH = 220,
    TCI_TELEPORTER = 221,
    TCI_EXPLODING_OBJECT = 222,
    TCI_HERO_SPECIAL_MOVEMENT = 223,
    TCI_HERO_REMEMBER_CRIMES = 224,
    TCI_RESURRECTION_ITEM = 225,
    TCI_KRAKEN = 226,
    TCI_KRAKEN_TENTACLE = 227,
    TCI_CREATURE_FLY_IN_CIRCLES = 228,
    TCI_INFO_DISPLAY = 229,
    TCI_SPOT_LIGHT = 230,
    TCI_AMBIENT_BIRD = 231,
    TCI_BOAST_UI = 232,
    TCI_TEXTURE_DECAL = 233,
    TCI_FISHING_ROD = 234,
    TCI_THING_UID_DUMMY_FOR_SERIALISATION = 235,
    TCI_RUMBLE = 236,
    TCI_PHYSICAL_OBSTRUCTION = 237,
    TCI_ELECTROCUTE_ON_TOUCH = 238,
    TCI_DIGGING_SPOT = 239,
    TCI_DICE_GAME = 240,
    TCI_PLOT = 241,
    TCI_BOB = 242,
    TCI_FINE_DIALOGUE = 243,
    TCI_SOUND_ATMOSPHERE = 244,
    TCI_NYMPH = 245,
    TCI_QUICK_ACCESS_MENU = 246,
    TCI_WOBBLE = 247,
    TCI_CAMERA_COLLISION = 248,
    TCI_FADE_OUT_AND_IN = 249,
    TCI_AI_SUMMON = 250,
    TCI_FLASH_ON_HIT = 251,
    TCI_QUEST_COMPLETION_UI = 252,
    TCI_DESTROY_THING_ON_ACTION_FINISH = 253,
    TCI_OBSTACLE = 254,
    TCI_ATMOS_PLAYER = 255,
    TCI_FLOURISH_TARGET = 256,
    TCI_CREDITS_UI = 257,
    TCI_BETTING = 258,
    TCI_LIFE_TIMER = 259,
    TCI_FIRE_RANGED_WEAPON_ON_LEAVING_STATE_GROUP = 260,
    TCI_AVOID_REGION_EXIT = 261,
    TCI_GRAPHIC_APPEARANCE_STATIC_MESH_FADE_OUT_ON_PROXIMITY = 262,
    TCI_ORACLE_MINIGAME = 263,
    TCI_FIREHEART_MINIGAME = 264,
    TCI_DESTROY_THING_ON_SPECIAL_ABILITY_FINISH = 265,
    TCI_LIGHTNING_ORB = 266,
    TCI_LIGHTNING_NODE = 267,
    TCI_HAS_NAME = 268,
    TCI_HERO_ONLINE_SCOREBOARD = 269,
    TCI_PHOTOJOURNAL_CAPTURE = 270,
    TCI_EXPLOSIVE_TRAIL = 271,
    TCI_ABSORB_HITS = 272,
    TCI_GENERATED_CREATURE = 273,
    MAX_NO_TCI_TYPES = 274
};
#endif

#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H


#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#define FABLE_RETAIL_CCHARSTRING_DATA

#endif
class CCharStringData;
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWideString;
#endif
#ifdef FABLE_RETAIL_FACTIONS
class CCharString;
#ifndef FABLE_ENGINE_CCRC_H
#define FABLE_ENGINE_CCRC_H


class CCharString;
extern const unsigned long CRC32TAB[];
class CCRC {
public:
#ifndef FABLE_RETAIL_FACTIONS
    static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);
#else
    static unsigned long __fastcall Calc(unsigned long seed,const void* data,unsigned long length) {
        unsigned long result=seed;
        if(length!=0)for(unsigned long index=0;index<length;++index)
            result=(result>>8)^CRC32TAB[((const unsigned char*)data)[index]^(result&255)];
        return result;
    }
 #endif
    static unsigned long __fastcall Calc(const void* data,unsigned long length){return Calc(0,data,length);}
    static unsigned long __fastcall CalcFromString(const CCharString&);
    static unsigned long __fastcall CalcFromPathname(const CCharString&);
};
#endif

#endif
class CCharString {
public:
#ifdef FABLE_RETAIL_FACTIONS
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():&NullChar;}
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
    bool operator<(const CCharString& other)const{
        if(PStringData==other.PStringData)return false;
        if(PStringData&&other.PStringData)return PStringData->Data<other.PStringData->Data;
        return PStringData==0;
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWideString ToWideString() const;
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    CCharString GetSubString(unsigned long,unsigned long)const;
    bool operator==(const char* text)const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
    bool operator==(const CCharString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&)const;
    const CCharString& operator=(const CCharString&);
#endif
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#if defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned long GetCRC() const; 
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#endif
    ~CCharString();
public:
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    void UnassignString();
    static long NoStrings;
#endif
#if defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifndef FABLE_ENGINE_C3DVECTOR_H
#define FABLE_ENGINE_C3DVECTOR_H


#ifdef _MSC_VER
#define FABLE_VECTOR_INLINE __forceinline
#else
#define FABLE_VECTOR_INLINE inline
#endif
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
#ifndef FABLE_ENGINE_GFMATHS_H
#define FABLE_ENGINE_GFMATHS_H
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_HIT_EFFECT)
inline float __fastcall GFSqr(float n){return n*n;}
#endif
extern const float DELTA_NOTIONAL_ZERO;
extern unsigned long GFastOneOverSqrtTable[128];
inline float __fastcall GFFloatAbs(float n){
#ifdef _MSC_VER
 *(unsigned long*)&n &= 0x7fffffffUL;
#else
 *(unsigned int*)&n &= 0x7fffffffU;
#endif
 return n;
}
#ifdef _MSC_VER
__forceinline
#else
inline
#endif
float __fastcall GFFastOneOverSqrt(float n){
#ifdef _MSC_VER
 typedef unsigned long word_type;
#else
 typedef unsigned int word_type;
#endif
 word_type bits=*(word_type*)&n;
 word_type exponent=(0x17c-((bits>>23)&0xff))<<22;
 word_type mantissa=(word_type)GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15;
 float estimate;
 *(word_type*)&estimate=(exponent&0xff800000)|mantissa;
 return (3.0f-estimate*estimate*n)*estimate*0.5f;
}
#endif

#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
#ifndef FABLE_ENGINE_C2DCOORDI_H
#define FABLE_ENGINE_C2DCOORDI_H


class C2DCoordI { public: long X,Y;C2DCoordI(){}C2DCoordI(long x,long y):X(x),Y(y){} };
FABLE_STATIC_ASSERT(sizeof(C2DCoordI)==8);
#endif

#endif
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
#ifdef FABLE_RETAIL_VECTOR_SPLINE_CTOR
    C3DVector(float x,float y,float z){X=x;Y=y;Z=z;}
#else
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    C3DVector(float x,float y,float z){Initialise(x,y,z);}
#else
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
#endif
#endif
#ifdef FABLE_RETAIL_VECTOR_COPY_INITIALISE
    C3DVector(const C3DVector& p){Initialise(p.X,p.Y,p.Z);}
#elif defined(FABLE_RETAIL_VECTOR_GEOMETRY) || defined(FABLE_RETAIL_VECTOR_SPLINE_COPY)
    C3DVector(const C3DVector& p){*this=p;}
#else
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
#endif
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return Z*Z+Y*Y+X*X;}
#else
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
#endif
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
#ifdef FABLE_RETAIL_VECTOR_GEOMETRY
    FABLE_VECTOR_INLINE void Clear(){X=Y=Z=0.0f;}
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
    FABLE_VECTOR_INLINE void LimitLower(const C3DVector& v){if(X<v.X)X=v.X;if(Y<v.Y)Y=v.Y;if(Z<v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE void LimitUpper(const C3DVector& v){if(X>v.X)X=v.X;if(Y>v.Y)Y=v.Y;if(Z>v.Z)Z=v.Z;}
    FABLE_VECTOR_INLINE bool IsNonZero()const{return DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO<X*X+Y*Y+Z*Z;}
    FABLE_VECTOR_INLINE C3DVector& Normalise(){float one_on_magnitude=GFFastOneOverSqrt(GetSquaredMagnitude());X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
#endif
#ifdef FABLE_RETAIL_VECTOR_SPLINE
    FABLE_VECTOR_INLINE const C3DVector& operator+=(const C3DVector& v){X+=v.X;Y+=v.Y;Z+=v.Z;return *this;}
    FABLE_VECTOR_INLINE const C3DVector& operator*=(float v){X*=v;Y*=v;Z*=v;return *this;}
    FABLE_VECTOR_INLINE const C3DVector operator/(float v)const{float reciprocal=1.0f/v;return C3DVector(X*reciprocal,Y*reciprocal,Z*reciprocal);}
    FABLE_VECTOR_INLINE const C3DVector& operator/=(float v){float reciprocal=1.0f/v;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
    FABLE_VECTOR_INLINE float GetMagnitude()const{return (float)sqrt(GetSquaredMagnitude());}
#endif
#ifdef FABLE_RETAIL_VALIDPOSITIONS
    inline C2DCoordI ToC2DCoordI() const { return C2DCoordI((long)X,(long)Y); }
#endif
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE
#endif

class CThing;
class CEnginePrimitivePrepareInfo;class CWorld;
class CInterfaceType;
class CPersistContext;
#ifdef FABLE_RETAIL_SHOP
class CGameDefinitionManager;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWorldMap;class CThingManager;
#endif
class CTCParamsBase;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable {
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CTCBase(CThing&);
#endif
public:
    virtual void OnSerialise(CPersistContext &,bool);
public:
    virtual ~CTCBase();
public:
    virtual void OnRemove();
public:
    virtual void ConstructFromParams(const CTCParamsBase &);
public:
    virtual void OnCreate();
public:
    virtual void InitialActivate();
public:
    virtual void OnInitialActivate();
public:
    virtual void RenderUpdate(float);
public:
    virtual bool HasFrameUpdate() const;
public:
    virtual void FrameUpdate();
public:
    virtual void BulletTimeFrameUpdate();
public:
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public:
    virtual void Copy(const CTCBase &);
public:
    virtual void OnDie();
public:
    virtual void OnKill();
public:
    virtual void OnLevelUnload();
public:
    virtual void OnSetInLimbo(bool);
public:
    virtual bool Validate(CCharString &) const;
public:
    virtual bool IsFreeForDeletion() const;
public:
    virtual bool IsSerialiseCreatable() const;
public:
    virtual bool IsSerialisableForHeroState() const;
public:
    virtual ETCInterfaceType GetInterfaceType() const=0;
public:
    virtual CCharString GetClassTypeName() const=0;
public:
    virtual bool IsComponentSerialisable() const;
public:
    virtual bool IsDriver() const;
public:
    virtual bool IsCopyable() const;
public:
    virtual void GlobalUpdate(bool);
public:
    virtual void HandleMessage(const NTCMessages::CMessageBase &);
public:
#ifdef FABLE_RETAIL_SHOP
    const CGameDefinitionManager& GetDefinitionManager() const;
#endif
public:
#ifdef FABLE_RETAIL_TARGET_RANDOM_WALK
    CWorld& GetWorld() const;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    const CWorld& PeekWorld() const;
    CWorldMap& GetWorldMap();
    CThingManager& GetThingManager();
#endif
    CThing& ParentThing;
public:
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
FABLE_STATIC_ASSERT(sizeof(CTCBase)==12);
#else 
#include <stddef.h>


class CThing;

#pragma pack(push, 1)
class CTCBase {
public:
    void*         __vftable;                                 
    CThing*       ParentThing;                               
    bool          ToBeRemoved;                               
    bool          IsInGlobalUpdateTCs;                       
    unsigned char _pad_0x0a[0x2];                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCBase) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ParentThing) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CTCBase, ToBeRemoved) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CTCBase, IsInGlobalUpdateTCs) == 0x9);

#endif 

#endif 

#ifndef FABLE_ENGINE_CSHOPITEM_H
#define FABLE_ENGINE_CSHOPITEM_H


class CShopItem {
public:
    CShopItem();
    float Quantity,PriceMultiplier,MaxStock;
    long RestockPeriod;
    float RestockPeriodRandomness,SalesPerDay,SalesPerDayRandomness;
};
FABLE_STATIC_ASSERT(sizeof(CShopItem)==28);
#endif

#ifndef FABLE_ENGINE_CINTELLIGENTPOINTER_H
#define FABLE_ENGINE_CINTELLIGENTPOINTER_H

class CBaseObject;
class CBaseObjectPointer;


class CBaseIntelligentPointer {
public:
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CBaseIntelligentPointer();
#endif
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    void SetPItem(CBaseObject*);
#endif
public:
    CBaseObjectPointer* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_CAMERA_TRANSITIONS)
    CIntelligentPointer() { }
#endif
    #ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    CIntelligentPointer(T* pointer) { SetPointer(pointer); }
    void Set(T* pointer) { SetPointer(pointer); }
public:
    void SetPointer(T* pointer) { SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(pointer))); }
public:
#endif
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);
#endif

#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H



class CRightHandedSet { public: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#endif

#ifndef FABLE_STLPORT_MAP_H
#define FABLE_STLPORT_MAP_H
#ifndef FABLE_STLPORT_CONTAINERS_H
#define FABLE_STLPORT_CONTAINERS_H



#include <stddef.h>
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CREATURE_POSITIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_ENTITY_PERSIST)
#include <new>
#endif
#include <stdlib.h>
#ifdef FABLE_RETAIL_SHOP
#include <stdio.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY)
#include <string.h>
#endif
#if defined(FABLE_STLPORT_VISIBLE_PUSH_BACK) || defined(FABLE_STLPORT_HTTP)
#include <new>
#include <string.h>
#endif
namespace std {
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
inline void fill(unsigned char* first,unsigned char* last,const unsigned char& value) {
    unsigned char tmp=value;memset(first,tmp,last-first);
}
template<class Size> inline unsigned char* fill_n(unsigned char* first,Size n,const unsigned char& value) {
    fill(first,first+n,value);return first+n;
}
#endif
struct __true_type { };
#ifdef FABLE_STLPORT_CAMERA_POINTS
struct __false_type { };
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
struct __false_type { };
template<class T> struct __type_traits { typedef __false_type is_POD_type; };
template<> struct __type_traits<unsigned char> { typedef __true_type is_POD_type; };
template<class Iterator> struct iterator_traits;
template<class T> struct iterator_traits<T*> { typedef T value_type; };
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,const T& value,const __true_type&) {
    return std::fill_n(first,n,value);
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,const T& value) {
    typedef typename iterator_traits<ForwardIterator>::value_type Value;
    typedef typename __type_traits<Value>::is_POD_type IsPOD;
    return __uninitialized_fill_n(first,n,value,IsPOD());
}
#endif
#ifdef FABLE_STLPORT_HTTP
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<class I,class O,class D> inline O __fastcall __copy(I first,I last,O result,const random_access_iterator_tag&,D*) {
    for(D n=last-first;n>0;--n) { *result=*first;++first;++result; } return result;
}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I,class O,class D>O __fastcall __copy(I,I,O,const random_access_iterator_tag&,D*);
template<class I,class C>void __fastcall sort(I,I,C);
#endif
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __false_type&) {return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class T> inline T* __copy_ptrs(T* first,T* last,T* result,const __true_type&) {return last==first?result:(T*)((char*)memmove(result,first,(char*)last-(char*)first)+((char*)last-(char*)first));}
#endif
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class T> struct __type_traits;
template<class I> struct iterator_traits;
template<class T> struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_REGION_GRAPH)
template<class I> void __fastcall __destroy_aux(I,I,const __false_type&);
#else
template<class I> inline void __destroy_aux(I first,I last,const __false_type&) {
    for(;first!=last;++first) _Destroy(&*first);
}
#endif
template<class I> inline void __destroy_aux(I,I,const __true_type&) { }
template<class I,class T> inline void __destroy(I first,I last,T*) {
    typedef typename __type_traits<T>::has_trivial_destructor Trivial;
    __destroy_aux(first,last,Trivial());
}
template<class I> inline void _Destroy(I first,I last) {
    __destroy(first,last,(typename iterator_traits<I>::value_type*)0);
}
#else
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
#endif
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
#endif
#ifdef FABLE_STLPORT_HTTP
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
#ifdef FABLE_RETAIL_SHOP
    typedef __false_type is_POD_type;
#endif
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
template<> struct __type_traits<unsigned char> { typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor; };
#endif
#ifdef FABLE_RETAIL_QUEST_COMPLETION
#ifdef FABLE_RETAIL_HERO_LOGBOOK
template<> struct __type_traits<unsigned short> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
template<> struct __type_traits<long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
template<> struct __type_traits<unsigned long> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
template<> struct __type_traits<unsigned __int64> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#endif
#ifdef FABLE_RETAIL_SHOP
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; typedef __true_type is_POD_type; };
#endif
#endif
inline void* __copy_trivial(const void* first,const void* last,void* result) {
    return last==first?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class T> inline T* __uninitialized_copy(const T* first,const T* last,T* result,const __true_type&) {
    return (T*)__copy_trivial(first,last,result);
}
template<class I,class T> inline void fill(I first,I last,const T& x) { for(;first!=last;++first) *first=x; }
template<class I,class N,class T> inline I fill_n(I first,N n,const T& x) { for(;n>0;--n,++first) *first=x;return first; }
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __true_type&) {
    ptrdiff_t n=last-first;return (T*)memmove(result-n,first,n*sizeof(T));
}
template<class T> inline T* __copy_backward_ptrs(const T* first,const T* last,T* result,const __false_type&) {
    while(first!=last) *--result=*--last;return result;
}
#endif
#ifdef FABLE_STLPORT_CREATURE_POSITIONS
#ifndef FABLE_STLPORT_HTTP
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
struct __false_type { };
#else
struct __false_type { __false_type() { } };
#endif
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
#endif
template<class Input,class Output,class Distance>
Output __fastcall __copy(Input,Input,Output,const random_access_iterator_tag&,Distance*);
template<class Input,class Output> inline Output __copy_ptrs(Input first,Input last,Output result,const __false_type&) {
    return __copy(first,last,result,random_access_iterator_tag(),(ptrdiff_t*)0);
}
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
template<class Input,class Output>
Output __fastcall __uninitialized_copy(Input,Input,Output,const __false_type&);
template<class Input,class Size,class T>
Input __fastcall __uninitialized_fill_n(Input,Size,const T&,const __false_type&);
template<class T,class U> inline void _Construct(T* p,const U& value) { new(p) T(value); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
#endif
template<class _Tp> class allocator {
public:
#ifdef FABLE_STLPORT_HTTP
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
#else
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
#endif
    void deallocate(_Tp* p, size_t) {
#ifdef FABLE_RETAIL_ENTITY_PERSIST
        if(p) free(p);
#else
        free(p);
#endif
    }
#endif
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHOP)
    _Vector_base(size_t n,const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) {
        _M_start=n?(_Tp*)malloc(n*sizeof(_Tp)):0;
        _M_finish=_M_start;
        _M_end_of_storage._M_data=_M_start+n;
    }
#endif
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    vector& operator=(const vector&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    explicit vector(size_type n) : _Vector_base<_Tp,_Alloc>(n,_Alloc()) {
        this->_M_finish=uninitialized_fill_n(this->_M_start,n,_Tp());
    }
    _Tp& front() { return *this->_M_start; }
#endif
#ifdef FABLE_RETAIL_SHOP
    vector(const vector& x) : _Vector_base<_Tp,_Alloc>(x.size(),x.get_allocator()) {
        this->_M_finish=__uninitialized_copy((const _Tp*)x._M_start,(const _Tp*)x._M_finish,this->_M_start,typename __type_traits<_Tp>::is_POD_type());
    }
#elif defined(FABLE_STLPORT_UI)
    vector(const vector&);
#endif
#if defined(FABLE_STLPORT_UI) || defined(FABLE_RETAIL_SHOP)
    _Alloc get_allocator() const { return _Alloc(this->_M_end_of_storage); }
#endif
#ifdef FABLE_STLPORT_HTTP
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
#endif
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x);++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
public:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fill_len,bool at_end=false) {
        const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
        _Tp* new_start=this->_M_end_of_storage.allocate(len);
        _Tp* new_finish=new_start;
        new_finish=__uninitialized_copy(this->_M_start,position,new_start,__false_type());
        if(fill_len==1) { _Construct(new_finish,x);++new_finish; }
        else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,__false_type());
        if(!at_end) new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,__false_type());
        _M_clear();_M_set(new_start,new_finish,new_start+len);
    }
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(_Tp* start,_Tp* finish,_Tp* end) {
        this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=end;
    }
public:
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
    void reserve(size_type);
    vector& operator=(const vector&);
    _Tp& back(){return *(this->_M_finish-1);}
#endif
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
#ifdef FABLE_STLPORT_HTTP
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
#ifdef FABLE_RETAIL_QUEST_COMPLETION
public:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
public:
#endif
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
#ifdef FABLE_RETAIL_QUEST_COMPLETION
    iterator insert(iterator position,const _Tp& x) {
        size_type n=position-begin();
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            if(position==end()) {
                _Construct(this->_M_finish,x);
                ++this->_M_finish;
            } else {
                _Construct(this->_M_finish,*(this->_M_finish-1));
                ++this->_M_finish;
                _Tp x_copy=x;
                __copy_backward_ptrs(position,this->_M_finish-2,this->_M_finish-1,_IsPODType());
                *position=x_copy;
            }
        } else _M_insert_overflow(position,x,_IsPODType(),1UL,false);
        return begin()+n;
    }
#endif
    iterator erase(iterator first,iterator last) {
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
        iterator result=__copy_ptrs(last,this->_M_finish,first,_IsPODType());
#else
        iterator result=std::__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
#endif
#else
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
#endif
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    void resize(size_type n,const _Tp& x) {if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
#elif defined(FABLE_RETAIL_SCOREBOARD)
    __declspec(noinline) void resize(size_type n,const _Tp& x);
#elif defined(FABLE_RETAIL_OAKVALE) || defined(FABLE_RETAIL_HANGING_TREE)
    void resize(size_type n,const _Tp& x);
#else
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
#endif
    void resize(size_type n) { resize(n,_Tp()); }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
public:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#ifndef FABLE_RETAIL_QUEST_COMPLETION
    __declspec(noinline)
#endif
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
public:
    void _M_clear() {
        _Destroy(this->_M_start,this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
    }
    void _M_set(iterator s,iterator f,iterator e) {
        this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;
    }
public:
    __declspec(noinline) void _M_fill_insert(iterator position,size_type n,const _Tp& x) {
        if(n!=0) {
            if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n) {
                _Tp copy=x;const size_type after=this->_M_finish-position;iterator oldFinish=this->_M_finish;
                if(after>n) {
                    __uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());
                    this->_M_finish+=n;
                    __copy_backward_ptrs(position,oldFinish-n,oldFinish,_IsPODType());
                    std::fill(position,position+n,copy);
                } else {
                    __uninitialized_fill_n(this->_M_finish,n-after,copy,__false_type());
                    this->_M_finish+=n-after;
                    __uninitialized_copy(position,oldFinish,this->_M_finish,_IsPODType());
                    this->_M_finish+=after;std::fill(position,oldFinish,copy);
                }
            } else _M_insert_overflow(position,x,_IsPODType(),n,false);
        }
    }
#elif defined(FABLE_STLPORT_CREATURE_POSITIONS)
    
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            new((void*)this->_M_finish) _Tp(x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,__false_type(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator i=std::__copy_ptrs(last,this->_M_finish,first,__false_type());
        for(iterator d=i;d!=this->_M_finish;++d) d->~_Tp();
        this->_M_finish=i;
        return first;
    }
    void clear() { erase(begin(),end()); }
public:
#ifdef FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& value,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type length=oldSize+(oldSize<fillLength?fillLength:oldSize);
        _Tp* newStart=(_Tp*)malloc(length*sizeof(_Tp));
        _Tp* newFinish=newStart;
        for(iterator first=this->_M_start;first!=position;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(size_type count=fillLength;count>0;--count,++newFinish) new((void*)newFinish) _Tp(value);
        if(!atEnd) for(iterator first=position;first!=this->_M_finish;++first,++newFinish) new((void*)newFinish) _Tp(*first);
        for(iterator first=this->_M_start;first!=this->_M_finish;++first) first->~_Tp();
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+length;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __false_type&,size_type,bool);
#endif
#elif defined(FABLE_STLPORT_VISIBLE_PUSH_BACK)
    void push_back(const _Tp& x) {
        if (this->_M_finish!=this->_M_end_of_storage._M_data) {
            new ((void*)this->_M_finish) _Tp(x);
            ++this->_M_finish;
        } else {
            _M_insert_overflow(end(),x,__true_type(),size_type(1),true);
        }
    }
public:
#ifdef FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
    
    void _M_insert_overflow(iterator position,const _Tp& x,
                                                const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();
        const size_type len=oldSize+(oldSize<fillLength ? fillLength : oldSize);
        _Tp* newStart=(_Tp*)malloc(len*sizeof(_Tp));
        size_type before=position-this->_M_start;
        memmove(newStart,this->_M_start,before*sizeof(_Tp));
        _Tp* newFinish=newStart+before;
        for(size_type n=fillLength;n>0;--n,++newFinish) *newFinish=x;
        if(!atEnd) {
            size_type after=this->_M_finish-position;
            memmove(newFinish,position,after*sizeof(_Tp));
            newFinish+=after;
        }
        if(this->_M_start) free(this->_M_start);
        this->_M_start=newStart;
        this->_M_finish=newFinish;
        this->_M_end_of_storage._M_data=newStart+len;
    }
#else
    void _M_insert_overflow(iterator,const _Tp&,const __true_type&,size_type,bool);
#endif
#else
#ifndef FABLE_RETAIL_ENTITY_PERSIST
    void push_back(const _Tp&);
#endif
#endif
};
#ifdef FABLE_RETAIL_QUEST_COMPLETION
template<class _Tp,class _Alloc> inline void vector<_Tp,_Alloc>::_M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
#endif
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_QUICK_ACCESS)
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
#if defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_COMBAT_TYPE)
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
#endif
#endif
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
#else
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
#endif
    _List_iterator() { }
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
#endif
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _List_iterator operator++(int) { _List_iterator tmp=*this; this->_M_incr(); return tmp; }
#endif
};
template<class _Tp,class _Alloc> class _List_base {
public:
#ifdef FABLE_STLPORT_CONSTRUCTORS
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
#endif
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_PARTICLE_SERIALISE)
    void clear() {
        _List_node<_Tp>* cur=(_List_node<_Tp>*)_M_node._M_data->_M_next;
        while(cur!=_M_node._M_data) {
            _List_node<_Tp>* tmp=cur;
            cur=(_List_node<_Tp>*)cur->_M_next;
            tmp->_M_data.~_Tp();
            _M_node.deallocate(tmp,1);
        }
        _M_node._M_data->_M_next=_M_node._M_data;
        _M_node._M_data->_M_prev=_M_node._M_data;
    }
#else
    void clear();
#endif
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
#ifdef FABLE_RETAIL_REGION_GRAPH
    ~list();
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~list();
#endif
#ifdef FABLE_STLPORT_CONSTRUCTORS
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
#endif
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_STLPORT_UI_CHANGE_STATE) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
#else
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
#endif
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
#ifdef FABLE_RETAIL_CAMERA_TRANSITIONS
    _Tp& front() { return *begin(); }
    const _Tp& front() const { return *begin(); }
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Alloc get_allocator()const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator,Input,Input,const __false_type&);
#endif
#ifdef FABLE_STLPORT_CAMERA_POINTS
    _Alloc get_allocator() const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) {
        insert(begin(),x.begin(),x.end());
    }
    ~list() { }
    template<class Input> void insert(iterator p,Input first,Input last) {
        _M_insert_dispatch(p,first,last,__false_type());
    }
    template<class Input> __declspec(noinline) void _M_insert_dispatch(iterator position,Input first,Input last,const __false_type&) {
        for(;first!=last;++first) insert(position,*first);
    }
#endif
#if defined(FABLE_STLPORT_UI_ACTIONS) || defined(FABLE_STLPORT_CAMERA_POINTS) || defined(FABLE_RETAIL_QUICK_ACCESS)
public:
    _List_node<_Tp>* _M_create_node(const _Tp& x) {
        _List_node<_Tp>* p=this->_M_node.allocate(1);
        new((void*)&p->_M_data) _Tp(x);
        return p;
    }
public:
    iterator insert(iterator position,const _Tp& x) {
        _List_node<_Tp>* tmp=_M_create_node(x);
        _List_node_base* n=position._M_node;
        _List_node_base* p=n->_M_prev;
        tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x) { insert(end(),x); }
#endif
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
#ifdef FABLE_RETAIL_SHOP

inline void __cdecl __stl_throw_out_of_range(const char* message) { puts(message); abort(); }
#else
void __stl_throw_out_of_range(const char*);
#endif
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
#endif



namespace std {
#ifdef FABLE_RETAIL_FACTIONS
template<class I>inline ptrdiff_t __fastcall __distance(const I& first,const I& last,const bidirectional_iterator_tag&){
    ptrdiff_t n=0;I it(first);while(it!=last){++it;++n;}return n;
}
template<class I>inline ptrdiff_t __fastcall distance(const I& first,const I& last){return __distance(first,last,bidirectional_iterator_tag());}
#endif
template<class A,class B,class R> struct binary_function { typedef A first_argument_type;typedef B second_argument_type;typedef R result_type; };
template<class T> struct less:public binary_function<T,T,bool>{bool operator()(const T& a,const T& b)const{return a<b;}};
template<class P> struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type& operator()(const P& p)const{return p.first;}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class V> struct _Rb_tree_node:public _Rb_tree_node_base { V _M_value_field; };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
template<class Dummy> class _Rb_global {
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    typedef _Rb_tree_node_base* _Base_ptr;
#endif
public:
#if defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_SHOP_TREE_BODY)
    static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static void __cdecl _Rotate_left(_Rb_tree_node_base*,_Rb_tree_node_base*&);
    static void __cdecl _Rotate_right(_Rb_tree_node_base*,_Rb_tree_node_base*&);
#endif
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* _M_node) {
  if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
    _M_node = _M_node->_M_right;
  else if (_M_node->_M_left != 0) {
    _Base_ptr __y = _M_node->_M_left;
    while (__y->_M_right != 0)
      __y = __y->_M_right;
    _M_node = __y;
  }
  else {
    _Base_ptr __y = _M_node->_M_parent;
    while (_M_node == __y->_M_left) {
      _M_node = __y;
      __y = __y->_M_parent;
    }
    _M_node = __y;
  }
  return _M_node;
}
#endif
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node) {
        if(node->_M_right!=0) {
            node=node->_M_right;
            while(node->_M_left!=0) node=node->_M_left;
        } else {
            _Rb_tree_node_base* parent=node->_M_parent;
            while(node==parent->_M_right) {node=parent;parent=parent->_M_parent;}
            if(node->_M_right!=parent) node=parent;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
#endif
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
    bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator {
    typedef typename Traits::reference reference;
    typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(){_M_node=0;}
    _Rb_tree_iterator(_Rb_tree_node<V>* p){_M_node=p;}
    _Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >& p){_M_node=p._M_node;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &operator*();}
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    _Rb_tree_iterator& operator--(){this->_M_node=_Rb_global_inst::_M_decrement(this->_M_node);return *this;}
#endif
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
#endif
};
template<class V,class A>struct _Rb_tree_base {
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
public:
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
#else
    _Rb_tree_base(const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
#else
    ~_Rb_tree_base();
#endif
#endif
public:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
public:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
#ifdef FABLE_RETAIL_COMBAT_TYPE
    iterator insert_equal(const V&);
public:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
#endif
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
public:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
#else
    _Rb_tree(const Compare&,const A&);
#endif
#if defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    ~_Rb_tree(){clear();}
#else
    ~_Rb_tree();
#endif
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
public:
    void _M_erase(_Rb_tree_node<V>*);
public:
    void clear() {
        if(_M_node_count!=0) {
            _M_erase(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_parent));
            this->_M_header._M_data->_M_left=this->_M_header._M_data;
            this->_M_header._M_data->_M_parent=0;
            this->_M_header._M_data->_M_right=this->_M_header._M_data;
            _M_node_count=0;
        }
    }
#ifndef FABLE_RETAIL_QUICK_ACCESS
    iterator find(const K&);
#endif
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS) || defined(FABLE_RETAIL_FACTIONS)
public:
    typedef _Rb_tree_node<V>* _Link_type;
#ifdef FABLE_RETAIL_SHOP_TREE_BODY
    _Link_type& _M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
    _Link_type& _M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
    static _Link_type& __fastcall _S_parent(_Link_type x){return (_Link_type&)x->_M_parent;}
    static _Link_type& __fastcall _S_left(_Rb_tree_node_base* x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Rb_tree_node_base* x){return (_Link_type&)x->_M_right;}
    static const K& __fastcall _S_key(_Rb_tree_node_base* x){return KeyOfValue()(static_cast<_Link_type>(x)->_M_value_field);}
    _Link_type _M_create_node(const V& x){_Link_type p=this->_M_header.allocate(1);std::_Construct(&p->_M_value_field,x);return p;}
public:
    iterator _M_insert(_Rb_tree_node_base*,_Rb_tree_node_base*,const V&,_Rb_tree_node_base* =0);
public:
    size_t size()const{return _M_node_count;}
public:
#endif
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
public:
    template<class Key> _Rb_tree_node<V>* _M_find(const Key& key)const {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        if(y==this->_M_header._M_data || _M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
        return y;
    }
public:
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_QUICK_ACCESS)
    iterator find(const K& key){return iterator(_M_find(key));}
#endif
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
#ifdef FABLE_RETAIL_FACTIONS
    iterator lower_bound(const K& key){return iterator(_M_lower_bound(key));}
    iterator upper_bound(const K& key){return iterator(_M_upper_bound(key));}
#else
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
#endif
#endif
#ifdef FABLE_RETAIL_FACTIONS
public:
    _Link_type _M_lower_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
    _Link_type _M_upper_bound(const K& key)const{
        _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
        while(x!=0){if(_M_key_compare(key,_S_key(x))){y=x;x=_S_left(x);}else x=_S_right(x);}return y;
    }
public:
    pair<iterator,iterator> equal_range(const K& key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
    void erase(iterator,iterator);
    size_t erase(const K& key){pair<iterator,iterator> p=equal_range(key);size_t n=distance(p.first,p.second);erase(p.first,p.second);return n;}
#endif
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
#ifdef FABLE_RETAIL_QUICK_ACCESS
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Rb_tree_node<V>* x) {
    while(x!=0) {
        _M_erase(static_cast<_Rb_tree_node<V>*>(x->_M_right));
        _Rb_tree_node<V>* y=static_cast<_Rb_tree_node<V>*>(x->_M_left);
        _Destroy(&x->_M_value_field);
        this->_M_header.deallocate(x,1);
        x=y;
    }
}
#endif
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
public:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_ENTITY_PERSIST) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
#ifdef FABLE_RETAIL_WASP
    ~map();
#endif
#if defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    size_t size() const { return _M_t.size(); }
#endif
#endif
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_SCRIPT_QUEST) || defined(FABLE_RETAIL_COMBAT_TYPE) || defined(FABLE_RETAIL_QUICK_ACCESS)
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
#endif
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_SHOP_MAP_BODY) || defined(FABLE_RETAIL_QUICK_ACCESS)
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    iterator find(const K& key){return _M_t.find(key);}
#endif
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
#endif
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
FABLE_STATIC_ASSERT(sizeof(std::map<unsigned long,long>)==12);
#endif

class CThingCreatureBase;


class CTCShop : public CTCBase {
public:
    enum ESTOCK_STATE { STOCKED_YES=0,STOCKED_NO=1,STOCKED_DONTKNOW=2 };
    struct GoodsPlace { C3DVector pos; CRightHandedSet rhset; };
    ESTOCK_STATE StockState;
    CTCShop(CThing&);
    virtual ~CTCShop();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void OnCreate();
    virtual void OnInitialActivate();
    virtual bool IsComponentSerialisable() const;
    virtual void OnSerialise(CPersistContext&,bool);
    const CShopItem* FindDefaultStockItem(long) const;
    void SellItem(long);
    float GetCurrentBasicPrice(long,long) const;
    long GetCurrentBuyPrice(long,long) const;
    std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool);
    void AddSaleItem(long,const GoodsPlace&);
public:
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;
    std::vector<CIntelligentPointer<CThingCreatureBase> > Shoppers;
    bool ShopKeeperAvailable;
    std::vector<GoodsPlace> GoodsPlaces;
    std::map<long,CShopItem> Stock;
    long SpecialItem,DaysSinceSpecialItemPlaced,DayLastChecked,GoldDonated;
    bool StallLike;
    unsigned long Alarm;
};
FABLE_STATIC_ASSERT(sizeof(CTCShop)==0x58);
FABLE_STATIC_ASSERT(sizeof(CTCShop::GoodsPlace)==36);
#endif

#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#ifdef FABLE_RETAIL_DEFINITION_MANAGER
#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H
#ifdef FABLE_RETAIL_DEFINITION_MANAGER
#ifndef FABLE_ENGINE_CRESOURCELIST_H
#define FABLE_ENGINE_CRESOURCELIST_H
#ifndef FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H
#define FABLE_ENGINE_CFAILEDALLOCATIONHANDLER_H


class CFailedAllocationHandler {
public:
    virtual ~CFailedAllocationHandler();
    virtual bool HandleFailedAllocation(unsigned long)=0;
};
FABLE_STATIC_ASSERT(sizeof(CFailedAllocationHandler)==4);
#endif

#ifndef FABLE_ENGINE_CRESOURCE_H
#define FABLE_ENGINE_CRESOURCE_H







#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTEEBASE_H















#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>
class CIVCountedPointeeBase {
public:
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
    CIVCountedPointeeBase();
#endif
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    long Release() const { return --IVRefCount; }                        
    long GetRefCount() const { return IVRefCount; }                      
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE
public:
#endif
    mutable unsigned long IVRefCount;                                    
};
FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    void*         __vftable;                                 
    unsigned long IVRefCount;                                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CIVCountedPointeeBase) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CIVCountedPointeeBase, IVRefCount) == 0x4);

#endif 

#endif 

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         
    virtual ~CResource();                                                
    virtual bool UnloadResource();                                       
    virtual unsigned long GetUnloadableMemory() const;                   
    virtual bool IsResourceLoaded() const;                               
    virtual void OnResourceLoaded();                                     
    virtual bool CanAbortResourceLoad(long) const;                       
    virtual void OnResourceLoadAborted();                                
    virtual unsigned long GetDebugMemoryInUse() const;                   
    virtual void ResetDebugMemoryInUse();                                
    CResourceList* ResourceList;                                         
    CResource*     PrevResource;                                         
    CResource*     NextResource;                                         
    unsigned long  ResourceSize;                                         
    unsigned long  LastUsedFrame;                                        
};
FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
#else 
#include <stddef.h>


class CResourceList;

#pragma pack(push, 1)
class CResource {
public:
    void*          __vftable;                                
    unsigned char  _pad_0x04[0x4];                           
    CResourceList* ResourceList;                             
    CResource*     PrevResource;                             
    CResource*     NextResource;                             
    unsigned long  ResourceSize;                             
    unsigned long  LastUsedFrame;                            
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CResource) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CResource, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceList) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CResource, PrevResource) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CResource, NextResource) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CResource, ResourceSize) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CResource, LastUsedFrame) == 0x18);

#endif 

#endif 



class CResourceList : public CFailedAllocationHandler {
public:
    virtual ~CResourceList();
    virtual bool HandleFailedAllocation(unsigned long);
public:
    CResource Head;
    unsigned long ResourceCount;
    long AllocatedMemory,MaximumMemory;
    unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
};
FABLE_STATIC_ASSERT(sizeof(CResourceList)==0x3c);
#endif
#else
#ifndef FABLE_ENGINE_CRESOURCELIST_H
#define FABLE_ENGINE_CRESOURCELIST_H





#include <stddef.h>


#pragma pack(push, 1)
class CResourceList {
public:
    void*         __vftable;                                 
    unsigned char Head[0x1c];                                
    unsigned long ResourceCount;                             
    long          AllocatedMemory;                           
    long          MaximumMemory;                             
    unsigned long CurrentFrame;                              
    unsigned long DebugStatsFrame;                           
    unsigned long UnloadDelay;                               
    unsigned long UnloadedThisFrame;                         
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CResourceList) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CResourceList, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CResourceList, Head) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CResourceList, ResourceCount) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CResourceList, AllocatedMemory) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CResourceList, MaximumMemory) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CResourceList, CurrentFrame) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CResourceList, DebugStatsFrame) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CResourceList, UnloadDelay) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CResourceList, UnloadedThisFrame) == 0x38);

#endif 

#endif

#ifndef FABLE_ENGINE_CDEFPOINTER_H
#define FABLE_ENGINE_CDEFPOINTER_H























#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CDefPointer.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else

#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H




#include <stddef.h>



class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1c);

#endif 

#if defined(_MSC_VER)
#define FABLE_DEFPTR_INLINE __forceinline
#else
#define FABLE_DEFPTR_INLINE inline
#endif

enum ENullDef { NULL_DEF };                                          

template <class T> class CDefPointer {
public:
    FABLE_DEFPTR_INLINE CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
#ifdef FABLE_RETAIL_COMBAT_TYPE
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) {
        if(Object!=other.Object){Release();Object=other.Object;if(Object)Object->AddRef();}
        return *this;
    }
#else
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
#endif
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
#ifdef FABLE_RETAIL_FACTIONS
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) {
        if(Object!=other.InternalGetObject()){Release();Object=other.InternalGetObject();if(Object)Object->AddRef();}
    }
#else
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
#endif
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             
public:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointer<CDefPointeeBase>) == 0x4);

#endif 
#endif 



class CDefClassBase;
#ifdef FABLE_RETAIL_SHOP
class CDefString;
#endif
class CDefinitionManager : public CResourceList {
    friend class CDefClassBase;
public:
    virtual ~CDefinitionManager();
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_FACTIONS)
    long GetGlobalIndexFromClassIndex(const CCharString&,long) const;
    template<class T> bool GetDef(long,CDefPointer<const T>*) const;
#endif
#ifdef FABLE_RETAIL_SHOP
    CDefString GetDefNameFromGlobalIndex(long) const;
    long GetDefGlobalIndexFromName(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    long GetDefGlobalIndexFromName(const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_FACTIONS
    long GetNoInstantiatedDefsOfType(const CCharString&)const;
#endif
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long) const;
public:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long);
    unsigned char PDefTypeDebugInfoMap[4];
    unsigned char SymbolPathList[12],CompilePathList[12];
    unsigned char PSymbolMap[8],PDefinitionSymbolMap[8];
    unsigned char NameCRCToIndexMap[16],DefClassInfo[16];
    unsigned char CompressedDefData[12],CompressedOffsets[16],InstantiatedDefs[12];
    unsigned char ErrorCallback[4],CurrentFilename[4],CompiledFileName[4];
    unsigned long DependencyCheckCRC;
    bool UseSafeBinaryFormat;
    unsigned char _pad_0xc1[3];
    unsigned char RecompiledDefs[12];
};
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager)==0xd0);
#endif
#else
#ifndef FABLE_ENGINE_CDEFINITIONMANAGER_H
#define FABLE_ENGINE_CDEFINITIONMANAGER_H






#include <stddef.h>


class CCPPointerInfo;
class CCharStringData;
class CSymbolMap;
class CWideStringData;
struct _func___cdecl_void_CCharString_ptr;

#pragma pack(push, 1)
class CDefinitionManager {
public:
    void*                               __vftable;           
    unsigned char                       _pad_0x04[0x4];      
    unsigned char                       _pad_0x08[0x4];      
    unsigned char                       _pad_0x0c[0x4];      
    unsigned char                       _pad_0x10[0x4];      
    unsigned char                       _pad_0x14[0x4];      
    void*                               m_classArray;        
    unsigned char                       _pad_0x1c[0x4];      
    unsigned char                       _pad_0x20[0x4];      
    unsigned char                       _pad_0x24[0x4];      
    unsigned char                       _pad_0x28[0x4];      
    unsigned char                       _pad_0x2c[0x4];      
    unsigned char                       _pad_0x30[0x4];      
    unsigned char                       _pad_0x34[0x4];      
    unsigned char                       _pad_0x38[0x4];      
    void*                               PDefTypeDebugInfoMap; 
    unsigned char                       SymbolPathList[0xc]; 
    unsigned char                       CompilePathList[0xc]; 
    CSymbolMap*                         PSymbolMap_Data;     
    CCPPointerInfo*                     PSymbolMap_Info;     
    CSymbolMap*                         PDefinitionSymbolMap_Data; 
    CCPPointerInfo*                     PDefinitionSymbolMap_Info; 
    unsigned char                       NameCRCToIndexMap[0x10]; 
    unsigned char                       DefClassInfo[0x10];  
    unsigned char                       CompressedDefData[0xc]; 
    unsigned char                       CompressedOffsets[0x10]; 
    unsigned char                       InstantiatedDefs[0xc]; 
    _func___cdecl_void_CCharString_ptr* ErrorCallback;       
    CCharStringData*                    CurrentFilename_PStringData; 
    CWideStringData*                    CompiledFileName_PStringData; 
    unsigned long                       DependencyCheckCRC;  
    bool                                UseSafeBinaryFormat; 
    unsigned char                       _pad_0xc1[0x1];      
    unsigned char                       _pad_0xc2[0x2];      
    unsigned char                       RecompiledDefs[0xc]; 
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDefinitionManager) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, m_classArray) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefTypeDebugInfoMap) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, SymbolPathList) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompilePathList) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Data) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PSymbolMap_Info) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Data) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, PDefinitionSymbolMap_Info) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, NameCRCToIndexMap) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DefClassInfo) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedDefData) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompressedOffsets) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, InstantiatedDefs) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, ErrorCallback) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CurrentFilename_PStringData) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, CompiledFileName_PStringData) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, DependencyCheckCRC) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, UseSafeBinaryFormat) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CDefinitionManager, RecompiledDefs) == 0xc4);

#endif 

#endif





class CGameDefinitionManager : public CDefinitionManager {
public:
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    static CGameDefinitionManager& __fastcall Get();
#endif
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
#ifdef FABLE_RETAIL_FACTIONS
    using CDefinitionManager::GetDef;
#endif
public:
    unsigned char _pad_0xd0[16];
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
#endif

#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H


class CCharString;
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#ifndef FABLE_ENGINE_CDEFSTRINGTABLE_H
#define FABLE_ENGINE_CDEFSTRINGTABLE_H




class CDefStringTable {
public:
    CCharString GetString(long) const;
#ifdef FABLE_RETAIL_FACTIONS
    unsigned long GetStringCRC(long)const;
#endif
public:
    unsigned char StringMap[12]; 
    unsigned char StringStream[24]; 
    unsigned long StringCount,SharedCount,RandomID;
    unsigned char TableFilename[4]; 
    bool Locked,ReadOnly;
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable)==0x38);
extern CDefStringTable GDefStringTable;
#endif

#endif
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#ifdef FABLE_RETAIL_FACTIONS
 unsigned long GetCRC()const{return GDefStringTable.GetStringCRC(TablePos);}
#endif
 operator CCharString()const{return GDefStringTable.GetString(TablePos);}
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
 CCharString GetString() const;
#endif
public:
 long TablePos;
};
#endif

#ifndef FABLE_ENGINE_CPERSISTCONTEXT_H
#define FABLE_ENGINE_CPERSISTCONTEXT_H

class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
namespace std { template<class T> class allocator; template<class A,class B> struct pair; template<class T> struct less; template<class K,class V,class C,class A> class map; template<class T,class A> class vector; }


class CPersistContext_TransferableVectorGenericType;
class CPersistContext_TransferableComponentGenericType;
class CPersistContextWriteSection;
template<class T> class CPersistTraits;
class CPersistContext {
public:
#ifdef FABLE_RETAIL_PERSIST_OPINION
    void PersistError(const CStringParser&,const CCharString&) const;
#endif
#ifdef FABLE_RETAIL_SHAPE_PERSIST
    template<class T> void Transfer(const char*,T&,const T& =T());
#elif defined(FABLE_RETAIL_SHOP)
    template<class T> void Transfer(const char*,T&,const T&);
#endif
#ifdef FABLE_RETAIL_SHOP
    template<class T> void Transfer(const char*,std::vector<T,std::allocator<T> >&);
#endif
#ifdef FABLE_RETAIL_PARTICLE_SERIALISE
    CPersistContext(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    bool IsSaving()const{return Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY;}
    CStringParser* GetPParser(){return PParser;}
    void WriteString(const CCharString&);
    template<class T>void Transfer(const char*,T&);
    template<class T>void Transfer(const char*,T&,const T&);
#endif
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    bool IsSaving() const { return Mode==MODE_SAVE_TEXT || Mode==MODE_SAVE_BINARY; }
    template<class T> void TransferVector(const char*,std::vector<T,std::allocator<T> >&);
#ifdef FABLE_RETAIL_QUEST_TRANSFER_BODY
public:
#ifndef FABLE_RETAIL_CTHINGMANAGER_BINARY
    void CheckCRC(const char*);
#endif
    void TransferVectorSaveText(const char*,CPersistContext_TransferableVectorGenericType&);
    void TransferVectorLoadText(const char*,CPersistContext_TransferableVectorGenericType&);
public:
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    CPersistContext(CMemoryDataInputStream&,bool);
    CMemoryDataInputStream* GetPLoadStream() { return PLoadStream; }
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    CPersistContext(CDataOutputStream&,bool);
    CDataOutputStream* GetPSaveStream() { return PSaveStream; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool IsBinary() const { return Mode==MODE_LOAD_BINARY || Mode==MODE_SAVE_BINARY; }
    void StartSection(CPersistContextWriteSection&,const CCharString&);
    void EndSection(CPersistContextWriteSection&);
    void WriteString(const CCharString&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    template<class T> void Transfer(T& val) {
        CheckCRC("");
        switch(Mode) {
        case MODE_LOAD_BINARY: { CPersistTraits<T> traits;traits.TransferBinaryIn(*PLoadStream,val);break; }
        case MODE_SAVE_BINARY: { CPersistTraits<T> traits;traits.TransferBinaryOut(*PSaveStream,val);break; }
        }
    }
#else
    template<class T> void Transfer(T&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    template<class T> void Transfer(const char*,T&);
#endif
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
public:
#if defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    void CheckCRC(const char*);
#endif
#ifdef FABLE_RETAIL_SHAPE_PERSIST
    void TransferObjectSaveText(const char*,CPersistContext_TransferableComponentGenericType&);
    void TransferObjectLoadText(const char*,CPersistContext_TransferableComponentGenericType&);
#endif
    const CDefinitionManager* PDefinitionManager;
    const CSymbolMap* PSymbolMap;
    CStringParser* PParser;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCharString* PStringToClean;
    bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;
    EMode Mode;
    bool Binary;
    CCharString* PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
};
FABLE_STATIC_ASSERT(sizeof(CPersistContext)==44);
#endif

#pragma auto_inline(off)
namespace std {
template<> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::_M_insert(_Rb_tree_node_base* __x_,_Rb_tree_node_base* __y_,const pair<const long,CShopItem>& __v,_Rb_tree_node_base* __w_)
{
  _Link_type __w = (_Link_type) __w_;
  _Link_type __x = (_Link_type) __x_;
  _Link_type __y = (_Link_type) __y_;
  _Link_type __z;

  if ( __y == this->_M_header._M_data ||
       ( __w == 0 && 
         ( __x != 0 ||     
           _M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__y) ) )
	 )
       ) {
    
    __z = _M_create_node(__v);
    _S_left(__y) = __z;               
                                      
    if (__y == this->_M_header._M_data) {
      _M_root() = __z;
      _M_rightmost() = __z;
    }
    else if (__y == _M_leftmost())
      _M_leftmost() = __z;   
  }
  else {
    __z = _M_create_node(__v);
    _S_right(__y) = __z;
    if (__y == _M_rightmost())
      _M_rightmost() = __z;  
  }
  _S_parent(__z) = __y;
  _S_left(__z) = 0;
  _S_right(__z) = 0;
  _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
  ++_M_node_count;
  return iterator(__z);
}
template<> pair<_Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator,bool> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::insert_unique(const pair<const long,CShopItem>& __v)
{
  _Link_type __y = this->_M_header._M_data;
  _Link_type __x = _M_root();
  bool __comp = true;
  while (__x != 0) {
    __y = __x;
    __comp = _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__x));
    __x = __comp ? _S_left(__x) : _S_right(__x);
  }
  iterator __j = iterator(__y);   
  if (__comp)
    if (__j == begin())     
      return pair<iterator,bool>(_M_insert( __y, __y, __v), true);
    else
      --__j;
  if (_M_key_compare(_S_key(__j._M_node), _Select1st<pair<const long,CShopItem> >()(__v)))
    return pair<iterator,bool>(_M_insert(__x, __y, __v), true);
  return pair<iterator,bool>(__j, false);
}
template<> _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::iterator _Rb_tree<long,pair<const long,CShopItem>,_Select1st<pair<const long,CShopItem> >,less<long>,allocator<pair<const long,CShopItem> > >::insert_unique(iterator __position,const pair<const long,CShopItem>& __v)
{
  if (__position._M_node == this->_M_header._M_data->_M_left) { 

    
    if (size() <= 0)
      return insert_unique(__v).first;

    if ( _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__position._M_node)))
      return _M_insert(__position._M_node, __position._M_node, __v);
    
    else
      {
	bool __comp_pos_v = _M_key_compare( _S_key(__position._M_node), _Select1st<pair<const long,CShopItem> >()(__v) );
	
	if (__comp_pos_v == false)  
	  return __position;
	

	
	
	iterator __after = __position;
	++__after;

	
	
	if (__after._M_node == this->_M_header._M_data)
	  
	  
	  
	  return _M_insert(0, __position._M_node, __v, __position._M_node);
		
	
	
	
	
	
	if(_M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__after._M_node) )) {
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	    return insert_unique(__v).first;
	}
      }

  } else if (__position._M_node == this->_M_header._M_data) { 
    if (_M_key_compare(_S_key(_M_rightmost()), _Select1st<pair<const long,CShopItem> >()(__v)))
      
      
      return _M_insert(0, _M_rightmost(), __v, __position._M_node); 
    else
      return insert_unique(__v).first;
  } else {
    iterator __before = __position;
    --__before;
    
    bool __comp_v_pos = _M_key_compare(_Select1st<pair<const long,CShopItem> >()(__v), _S_key(__position._M_node));

    if (__comp_v_pos
      && _M_key_compare( _S_key(__before._M_node), _Select1st<pair<const long,CShopItem> >()(__v) )) {

      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v, __before._M_node); 
      else
        return _M_insert(__position._M_node, __position._M_node, __v);
    
    } else
      {
	
	iterator __after = __position;
	++__after;
	
	
	bool __comp_pos_v = !__comp_v_pos;  
	
	
	
	
	if (!__comp_v_pos) __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _Select1st<pair<const long,CShopItem> >()(__v));
	
	if ( (!__comp_v_pos) 
	     && __comp_pos_v
	     && (__after._M_node == this->_M_header._M_data ||
	        _M_key_compare( _Select1st<pair<const long,CShopItem> >()(__v), _S_key(__after._M_node) ))) {
	  
	  if (_S_right(__position._M_node) == 0)
	    return _M_insert(0, __position._M_node, __v, __position._M_node);
	  else
	    return _M_insert(__after._M_node, __after._M_node, __v);
	} else {
	  
	  if (__comp_v_pos == __comp_pos_v)
	    return __position;
	  else
	    return insert_unique(__v).first;
	}
      }
  }
}
template<> CShopItem& map<long,CShopItem>::operator[](const long& key){iterator it=lower_bound(key);if(it==end()||key_comp()(key,(*it).first))it=insert(it,value_type(key,CShopItem()));return (*it).second;}
}
#pragma auto_inline(on)
template<>void CPersistContext::TransferVector<CCharString>(const char*,std::vector<CCharString>&);
template<>void CPersistContext::Transfer<float>(const char*,std::vector<float>&);
template<>void CPersistContext::Transfer<long>(const char*,long&,const long&);
void CTCShop::OnSerialise(CPersistContext& context,bool in_game)
{
    CTCBase::OnSerialise(context,in_game);
    if(context.IsSaving()) {
        if(in_game) {
            std::vector<CCharString> current_stock_item_defs_keys;
            std::vector<float> current_stock_item_defs_quantity;
            std::vector<float> current_stock_item_defs_max_stock;
            std::map<long,CShopItem>::iterator end_it=Stock.end();
            std::map<long,CShopItem>::iterator it=Stock.begin();
            for(;it!=end_it;++it) {
                current_stock_item_defs_keys.push_back(GetDefinitionManager().GetDefNameFromGlobalIndex(it->first));
                current_stock_item_defs_quantity.push_back(it->second.Quantity);
                current_stock_item_defs_max_stock.push_back(it->second.MaxStock);
            }
            context.TransferVector("StockItemDefsKeys",current_stock_item_defs_keys);
            context.Transfer("StockItemDefsQuantity",current_stock_item_defs_quantity);
            context.Transfer("StockItemDefsMaxStock",current_stock_item_defs_max_stock);
            context.Transfer("DayLastChecked",DayLastChecked,0L);
            context.Transfer("GoldDonated",GoldDonated,0L);
            context.Transfer("SpecialItem",SpecialItem,-1L);
            context.Transfer("DaysSinceSpecialItemPlaced",DaysSinceSpecialItemPlaced,0L);
        }
    } else {
        std::vector<CCharString> current_stock_item_defs_keys;
        std::vector<float> current_stock_item_defs_quantity;
        std::vector<float> current_stock_item_defs_max_stock;
        context.TransferVector("StockItemDefsKeys",current_stock_item_defs_keys);
        context.Transfer("StockItemDefsQuantity",current_stock_item_defs_quantity);
        context.Transfer("StockItemDefsMaxStock",current_stock_item_defs_max_stock);
        context.Transfer("DayLastChecked",DayLastChecked,0L);
        context.Transfer("GoldDonated",GoldDonated,0L);
        context.Transfer("SpecialItem",SpecialItem,-1L);
        context.Transfer("DaysSinceSpecialItemPlaced",DaysSinceSpecialItemPlaced,0L);
        if(current_stock_item_defs_keys.size()>0) {
            Stock.clear();
            for(long n=0;n<current_stock_item_defs_keys.size();++n) {
                long def_index=GetDefinitionManager().GetDefGlobalIndexFromName(current_stock_item_defs_keys[n]);
                if(def_index>0 && FindDefaultStockItem(def_index)) {
                    CShopItem item=*FindDefaultStockItem(def_index);
                    item.Quantity=current_stock_item_defs_quantity[n];
                    item.MaxStock=current_stock_item_defs_max_stock[n];
                    Stock[def_index]=item;
                }
            }
        }
    }
}

#ifdef FABLE_RETAIL_CTHINGBASEDEF
#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H
#ifndef FABLE_ENGINE_CPARENTDEFCLASSBASE_H
#define FABLE_ENGINE_CPARENTDEFCLASSBASE_H

























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>

#ifndef FABLE_ENGINE_CDEFCLASSBASE_H
#define FABLE_ENGINE_CDEFCLASSBASE_H























#ifdef FABLE_RETAIL_DEF_CLASS_BASE
#include <stddef.h>


class CCharString;
class CDefString;
class CDefinitionManager;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <class T> class CDefPointer;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
#ifdef FABLE_RETAIL_DEF_CONSTRUCTORS
public:
    CDefClassBase(); 
    friend class CParentDefClassBase;
#endif
public:
    class CSubDefInfo {                                                  
    public:
        long DefIndex;                                                   
        long OriginalParentDefIndex;                                     
    };
    CDefClassBase(const CDefClassBase&);                                 
    CDefClassBase& operator=(const CDefClassBase&);                      
    template <class T> bool GetSubDef(CDefPointer<const T>*) const;      
    bool AreDefaultValsApplied() const { return DefaultValsApplied; }    
    void SetDefaultValsApplied(bool value) { DefaultValsApplied = value; }   
    virtual ~CDefClassBase() { }                                         
    virtual bool UnloadResource();                                       
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void PostTransfer();                                         
    virtual void Transfer(CPersistContext&) = 0;                         
    virtual void Copy(const CDefClassBase*) = 0;                         
    virtual unsigned long GetSizeofClass() = 0;                          
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
    CDefinitionManager* PDefManager;                                     
    unsigned long       GlobalIndex;                                     
    unsigned char       Setup : 1;                                       
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
#else 
#include <stddef.h>


class CDefinitionManager;

#pragma pack(push, 1)
class CDefClassBase {
public:
    void*               __vftable;                           
    unsigned char       _base_0x04[0x18];                    
    CDefinitionManager* PDefManager;                         
    unsigned long       GlobalIndex;                         
    unsigned char       DefaultValsApplied[0x1];             
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, PDefManager) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, GlobalIndex) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CDefClassBase, DefaultValsApplied) == 0x24);

#endif 

#endif 

class CDataInputStream;
class CDataOutputStream;
#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase {
public:
    CParentDefClassBase();                                               
    CParentDefClassBase(const CParentDefClassBase&);                     
    CParentDefClassBase& operator=(const CParentDefClassBase&);          
#ifdef FABLE_RETAIL_UI_DEF_DTOR
    
#else
    virtual ~CParentDefClassBase();                                      
#endif
    virtual CDefString GetInstantiationName() const;                     
    virtual void SetInstantiationName(CDefString);                       
    virtual void AddSubDef(const CCharString&, long, long);              
    virtual void RemoveSubDef(const CCharString&);                       
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; 
    virtual bool IsParentDef() const;                                    
    virtual void InternalSaveBinary(CDataOutputStream&);                 
    virtual void InternalLoadBinary(CDataInputStream&);                  
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             
#ifdef FABLE_PDCB_TYPED_MEMBERS
    CDefString InstantiationName;                                        
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   
#else
    long          InstantiationName_TablePos;                            
    unsigned char SubDefInfoMap[0x10];                                   
#endif
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
#else 
#include <stddef.h>


#pragma pack(push, 1)
class CParentDefClassBase {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x21];                          
    long          InstantiationName_TablePos;                
    unsigned char SubDefInfoMap[0x10];                       
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, InstantiationName_TablePos) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CParentDefClassBase, SubDefInfoMap) == 0x29);

#endif 

#endif 



class CThingBaseDef : public CParentDefClassBase {
#ifdef FABLE_RETAIL_HIT_EFFECT
    unsigned char _pad_0x39[15];
public:float RenderFadeDistance;
public:unsigned char _pad_0x4c[36];
#else
    unsigned char _pad_0x39[0x37];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef)==0x70);

#endif
#else
#ifndef FABLE_ENGINE_CTHINGBASEDEF_H
#define FABLE_ENGINE_CTHINGBASEDEF_H






#include <stddef.h>


#pragma pack(push, 1)
class CThingBaseDef {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x25];                          
    unsigned char _pad_0x29[0x10];                           
    unsigned char Components[0x8];                           
    unsigned char InMapSearches[0x1];                        
    unsigned char AlwaysIncludeInObjectStrikeScans[0x1];     
    unsigned char GiveHeroStatChangesOnBeingHit[0x1];        
    long          GroupDef;                                  
    float         RenderFadeDistance;                        
    long          PersistenceFlags;                          
    float         Health;                                    
    float         MinHealth;                                 
    float         MeshHeightOffset;                          
    float         MeshHeight;                                
    float         MeshRadius;                                
    float         ApproxMaxMeshHeight;                       
    long          DefaultScriptName_TablePos;                
    long          DefaultScriptData_TablePos;                
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingBaseDef) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, Components) == 0x39);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, InMapSearches) == 0x41);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, AlwaysIncludeInObjectStrikeScans) == 0x42);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, GiveHeroStatChangesOnBeingHit) == 0x43);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, GroupDef) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, RenderFadeDistance) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, PersistenceFlags) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, Health) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MinHealth) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshHeightOffset) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshHeight) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, MeshRadius) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, ApproxMaxMeshHeight) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, DefaultScriptName_TablePos) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThingBaseDef, DefaultScriptData_TablePos) == 0x6c);

#endif 

#endif

#ifdef FABLE_RETAIL_CTHING
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H


#ifndef FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASS
#endif
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_ENGINE_CCLASS_H
#define FABLE_ENGINE_CCLASS_H










#ifdef FABLE_RETAIL_CCLASS
#include <stddef.h>

#ifndef FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_BASE_OBJECT
#endif
#ifndef FABLE_ENGINE_CBASEOBJECT_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#ifdef FABLE_RETAIL_BASE_OBJECT


class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
public:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
public:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
public:
#endif
    CBaseObjectPointer* IntelligentPointer;                              
};
FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
#else 




#include <stddef.h>


struct CBaseObjectPointer;

#pragma pack(push, 1)
class CBaseObject {
public:
    void*               __vftable;                           
#if defined(FABLE_RETAIL_WASP) || defined(FABLE_RETAIL_SCRIPT_QUEST)
public:
#endif
    CBaseObjectPointer* IntelligentPointer;                  
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CBaseObject) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CBaseObject, IntelligentPointer) == 0x4);

#endif 
#endif 

class CCharString;
class CClassFactory;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);                                     
    void Unregister(CClassFactory&);                                     
    void PostConstructionInit(CClassFactory&);                           
    unsigned long GetType() const { return Type; }                       
    unsigned char ClassListPos[0x4];                                     
    unsigned char TypeListPos[0x4];                                      
    unsigned char Type;                                                  
};
FABLE_STATIC_ASSERT(sizeof(CClass) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);
#else 





#include <stddef.h>


#pragma pack(push, 1)
class CClass {
public:
    void*         __vftable;                                 
    unsigned char _base_0x04[0x4];                           
    unsigned char ClassListPos[0x4];                         
    unsigned char TypeListPos[0x4];                          
    unsigned char Type;                                      
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CClass) == 0x11);
FABLE_STATIC_ASSERT(offsetof(CClass, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CClass, ClassListPos) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CClass, TypeListPos) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CClass, Type) == 0x10);

#endif 
#endif 



#ifndef FABLE_ENGINE_CVECTORMAP_H
#define FABLE_ENGINE_CVECTORMAP_H
#ifdef FABLE_VECTOR_MAP_GEOMETRY


#ifndef FABLE_STLPORT46_GEOMETRY_H
#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t,void* p){return p;}
#else
#include <new>
#endif
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
public:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T* s,T* f,T* e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 template<class I>T* _M_allocate_and_copy(size_type n,I first,I last){T* result=this->_M_end_of_storage.allocate(n);T* out=result;for(;first!=last;++first,++out)_Construct(out,*first);return result;}
#endif
};
#ifdef FABLE_GEOMETRY_VECTOR_BODIES
template<class T,class A>
__declspec(noinline) void vector<T,A>::reserve(size_type n){
 if(capacity()<n){const size_type old_size=size();T* temp;if(this->_M_start){temp=_M_allocate_and_copy(n,this->_M_start,this->_M_finish);_M_clear();}else temp=this->_M_end_of_storage.allocate(n);_M_set(temp,temp+old_size,temp+n);}
}
template<class T,class A>
__declspec(noinline) void vector<T,A>::_M_insert_overflow(T* position,const T& value,const __false_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type length=old_size+(max)(old_size,fill_len);
 T* new_start=this->_M_end_of_storage.allocate(length);T* new_finish=new_start;
 for(T* first=this->_M_start;first!=position;++first,++new_finish)_Construct(new_finish,*first);
 if(fill_len==1){_Construct(new_finish,value);++new_finish;}else for(size_type count=0;count<fill_len;++count,++new_finish)_Construct(new_finish,value);
 if(!at_end)for(T* first=position;first!=this->_M_finish;++first,++new_finish)_Construct(new_finish,*first);
 _M_clear();_M_set(new_start,new_finish,new_start+length);
}
#endif
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
public:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
public:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
public:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
public:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}
#endif

#else

#endif


#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
#if defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



#ifndef FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT46_ANIMATION_H
#define FABLE_STLPORT_CONTAINERS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_assignment_operator;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<unsigned char>{typedef __true_type is_POD_type;typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T>class allocator{public:allocator(){} T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};struct random_access_iterator_tag:public bidirectional_iterator_tag{};
template<class I,class O,class D>inline O __copy(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n){*d=*a;++a;++d;}return d;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __false_type&){return __copy(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O,class D>inline O __copy_backward(I a,I b,O d,const random_access_iterator_tag&,D*){for(D n=b-a;n>0;--n)*--d=*--b;return d;}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __false_type&){return __copy_backward(a,b,d,random_access_iterator_tag(),(ptrdiff_t*)0);}
template<class I,class O>inline O __copy_backward_ptrs(I a,I b,O d,const __true_type&){ptrdiff_t n=b-a;return n?(O)memmove(d-n,a,n*sizeof(*a)):d;}
template<class I,class T>inline void fill(I a,I b,const T&x){for(;a!=b;++a)*a=x;}
template<class I,class O>inline O __copy_ptrs(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*copy(const T*a,const T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class T>inline T*copy(T*a,T*b,T*d){return __copy_ptrs(a,b,d,typename __type_traits<T>::has_trivial_assignment_operator());}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __false_type&){O cur=d;for(;a!=b;++a,++cur)_Construct(&*cur,*a);return cur;}
template<class I,class O>inline O __uninitialized_copy(I a,I b,O d,const __true_type&){return(O)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class I,class N,class T>inline I __uninitialized_fill_n(I p,N n,const T&x,const __false_type&){I cur=p;for(;n>0;--n,++cur)_Construct(&*cur,x);return cur;}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T value_type;typedef T*pointer;typedef const T*const_pointer;typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
public:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false);
 void _M_insert_overflow(T*position,const T&x,const __false_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=new_start;
 new_finish=__uninitialized_copy(this->_M_start,position,new_start,_IsPODType());
 if(fill_len==1){_Construct(new_finish,x);++new_finish;}else new_finish=__uninitialized_fill_n(new_finish,fill_len,x,_IsPODType());
 if(!at_end)new_finish=__uninitialized_copy(position,this->_M_finish,new_finish,_IsPODType());
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
 template<class I>pointer _M_allocate_and_copy(size_type n,I first,I last){pointer result=this->_M_end_of_storage.allocate(n);__uninitialized_copy(first,last,result,_IsPODType());return result;}

public:
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 explicit vector(size_type n):_Vector_base<T,A>(n,A()){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,T(),_IsPODType());}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 vector&operator=(const vector&);
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void _M_fill_insert(iterator,size_type,const T&);
 void resize(size_type n,T x){if(n<size())erase(begin()+n,end());else _M_fill_insert(end(),n-size(),x);}
 void resize(size_type n){resize(n,T());}
};
template<class T,class A>vector<T,A>&vector<T,A>::operator=(const vector& x){
 if(&x!=this){const size_type xlen=x.size();if(xlen>capacity()){pointer tmp=_M_allocate_and_copy(xlen,(const_pointer)x._M_start+0,(const_pointer)x._M_finish+0);_M_clear();this->_M_start=tmp;this->_M_end_of_storage._M_data=this->_M_start+xlen;}
 else if(size()>=xlen){pointer i=__copy_ptrs((const_pointer)x._M_start+0,(const_pointer)x._M_finish+0,(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());_Destroy(i,this->_M_finish);}
 else{__copy_ptrs((const_pointer)x._M_start,(const_pointer)x._M_start+size(),(pointer)this->_M_start,typename __type_traits<T>::has_trivial_assignment_operator());__uninitialized_copy((const_pointer)x._M_start+size(),(const_pointer)x._M_finish+0,this->_M_finish,_IsPODType());}this->_M_finish=this->_M_start+xlen;}return *this;
}
template<class T,class A>void vector<T,A>::_M_fill_insert(iterator position,size_type n,const T&x){
 if(n!=0){if(size_type(this->_M_end_of_storage._M_data-this->_M_finish)>=n){T x_copy=x;const size_type elems_after=this->_M_finish-position;pointer old_finish=this->_M_finish;
 if(elems_after>n){__uninitialized_copy(this->_M_finish-n,this->_M_finish,this->_M_finish,_IsPODType());this->_M_finish+=n;__copy_backward_ptrs(position,old_finish-n,old_finish,typename __type_traits<T>::has_trivial_assignment_operator());fill(position,position+n,x_copy);}
 else{__uninitialized_fill_n(this->_M_finish,n-elems_after,x_copy,_IsPODType());this->_M_finish+=n-elems_after;__uninitialized_copy(position,old_finish,this->_M_finish,_IsPODType());this->_M_finish+=elems_after;fill(position,old_finish,x_copy);}}
 else _M_insert_overflow(position,x,_IsPODType(),n);}
}
template<class T,class A>void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);pointer new_start=this->_M_end_of_storage.allocate(len);pointer new_finish=(pointer)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(pointer)__copy_trivial(position,this->_M_finish,new_finish);_M_clear();_M_set(new_start,new_finish,new_start+len);
}

}
#endif


template<class T>class CArray:public std::vector<T>{public:
 CArray():std::vector<T>(){} explicit CArray(unsigned int n):std::vector<T>(n){}
 CArray(unsigned int n,const T& x):std::vector<T>(n,x){}
 void push_back(const T& _Val){if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);}
 T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_ANIMATION_SHARED)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}const T*GetAsCArray()const{return this->size()>0?this->_M_start:(const T*)this;}};
#endif
#else
#ifdef FABLE_RETAIL_NAV_REGIONS
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



#ifndef FABLE_STLPORT46_NAVIGATION_H
#define FABLE_STLPORT46_NAVIGATION_H



#ifndef FABLE_STLPORT46_VECTOR_ROOTS_H
#define FABLE_STLPORT46_VECTOR_ROOTS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
inline void* __cdecl operator new(size_t, void* p) { return p; }
#else
#include <new>
#endif
namespace std {
struct __true_type {};
struct __false_type {};
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor; };
template<> struct __type_traits<float> { typedef __true_type has_trivial_assignment_operator; typedef __true_type has_trivial_destructor; };
#ifdef FABLE_RETAIL_NAV_OUTPUT
template<class T>struct __type_traits<T*> {typedef __true_type has_trivial_assignment_operator;typedef __true_type has_trivial_destructor;};
#endif
template<class T> inline const T& max(const T& a,const T& b) { return a < b ? b : a; }
template<class A,class B> struct pair {
    typedef A first_type; typedef B second_type;
    A first; B second;
    pair():first(A()),second(B()){}
    pair(const A& a,const B& b):first(a),second(b){}
};
template<class A,class B> inline pair<A,B> make_pair(const A& a,const B& b) {return pair<A,B>(a,b);}
template<class T> class allocator {
public:
    allocator() {}
#ifdef FABLE_RETAIL_NAV_REGIONS
    allocator(const allocator&){}
    template<class U>allocator(const allocator<U>&){}
#endif
    T* allocate(size_t n,const void* = 0) { return n ? (T*)malloc(n*sizeof(T)) : 0; }
    void deallocate(T* p,size_t) { if(p) free(p); }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A {
public: V _M_data;
    _STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class T,class U> inline void _Construct(T* p,const U& x) { new(p) T(x); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> inline void __destroy_aux(T* first,T* last,const __false_type&) { for(;first!=last;++first) _Destroy(&*first); }
template<class T> inline void __destroy_aux(T*,T*,const __true_type&) {}
template<class T> inline void _Destroy(T* first,T* last) { __destroy_aux(first,last,typename __type_traits<T>::has_trivial_destructor()); }
inline void* __copy_trivial(const void* first,const void* last,void* out) {
 return last==first ? out : (char*)memmove(out,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __false_type&) {
 O cur=out; for(;first!=last;++first,++cur) _Construct(&*cur,*first); return cur;
}
template<class I,class O> inline O __uninitialized_copy(I first,I last,O out,const __true_type&) {
 return (O)__copy_trivial(first,last,out);
}
template<class T> inline T* __uninitialized_fill_n(T* first,size_t n,const T& value,const __false_type&) { T* cur=first;for(;n>0;--n,++cur)_Construct(&*cur,value);return cur;}
template<class T> inline T* fill_n(T* first,size_t n,const T& value) { for(;n>0;--n,++first)*first=value;return first; }
template<class T,class A> class _Vector_base {
public: typedef A allocator_type;
 _Vector_base(const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A& a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0) {
  _M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;
 }
 ~_Vector_base(){if(_M_start!=0)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
public:
 T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {
public:
 typedef T value_type;typedef T* pointer;typedef const T* const_pointer;typedef T* iterator;typedef const T* const_iterator;typedef size_t size_type;typedef A allocator_type;
 typedef typename __type_traits<T>::has_trivial_assignment_operator _IsPODType;
 explicit vector(const A& a=A()):_Vector_base<T,A>(a){}
 vector(const vector& x):_Vector_base<T,A>(x.size(),x.get_allocator()){
 this->_M_finish=__uninitialized_copy((const_pointer)x._M_start,(const_pointer)x._M_finish,this->_M_start,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 A get_allocator()const{return (const A&)this->_M_end_of_storage;}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
#ifdef FABLE_RETAIL_NAV_OUTPUT
 bool empty()const{return begin()==end();}
 T& back(){return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
#endif
#ifdef FABLE_STLPORT_CYLINDER
 void push_back(const T& x);
#else
 void push_back(const T& x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
#endif
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_REGIONS)
 vector& operator=(const vector&);
 iterator insert(iterator,const T&);
 T& operator[](size_type n){return this->_M_start[n];}
 const T& operator[](size_type n)const{return this->_M_start[n];}
#endif
 void clear(){erase(begin(),end());}
 iterator erase(iterator pos){if(pos+1!=end()) __copy_trivial(pos+1,this->_M_finish,pos);--this->_M_finish;_Destroy(this->_M_finish);return pos;}
 iterator erase(iterator first,iterator last){pointer i=first;for(;last!=this->_M_finish;++last,++i)*i=*last;_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
public:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(pointer s,pointer f,pointer e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
#if defined(FABLE_STLPORT_SPLINE) || defined(FABLE_RETAIL_NAV_OUTPUT)
 void _M_insert_overflow(pointer,const T&,const __false_type&,size_type,bool=false);
#else
 void _M_insert_overflow(pointer pos,const T& x,const __false_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=start;
 finish=__uninitialized_copy(this->_M_start,pos,start,__false_type());
 if(fill_len==1){_Construct(finish,x);++finish;}else finish=__uninitialized_fill_n(finish,fill_len,x,__false_type());
 if(!atend)finish=__uninitialized_copy(pos,this->_M_finish,finish,__false_type());
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
#ifdef FABLE_RETAIL_NAV_OUTPUT
 void _M_insert_overflow(pointer,const T&,const __true_type&,size_type,bool=false);
#else
 void _M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
 }
#endif
};
#ifdef FABLE_RETAIL_NAV_OUTPUT
#pragma auto_inline(off)
template<class T,class A>void vector<T,A>::_M_insert_overflow(pointer pos,const T& x,const __true_type&,size_type fill_len,bool atend){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 pointer start=this->_M_end_of_storage.allocate(len);pointer finish=(pointer)__copy_trivial(this->_M_start,pos,start);
 finish=fill_n(finish,fill_len,x);if(!atend)finish=(pointer)__copy_trivial(pos,this->_M_finish,finish);
 _M_clear();_M_set(start,finish,start+len);
}
#pragma auto_inline(on)
#endif
#ifdef FABLE_STLPORT_CYLINDER
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class I,class C>void sort(I,I,C);
#endif
}
#endif

namespace std {
template<class T>struct less;
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map;
struct _List_node_base {_List_node_base* _M_next;_List_node_base* _M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T>struct _Nonconst_traits{typedef T* pointer;typedef T& reference;};
template<class T>struct _Const_traits{typedef const T* pointer;typedef const T& reference;};
struct _List_iterator_base{
 _List_node_base* _M_node;
 _List_iterator_base(_List_node_base* x):_M_node(x){} _List_iterator_base(){}
 void _M_incr(){_M_node=_M_node->_M_next;} void _M_decr(){_M_node=_M_node->_M_prev;}
 bool operator==(const _List_iterator_base& x)const{return _M_node==x._M_node;}
 bool operator!=(const _List_iterator_base& x)const{return _M_node!=x._M_node;}
};
template<class T,class Traits>struct _List_iterator:public _List_iterator_base{
 typedef _List_iterator<T,_Nonconst_traits<T> > iterator;
 _List_iterator(_List_node<T>* x):_List_iterator_base(x){} _List_iterator(){}
 _List_iterator(const iterator& x):_List_iterator_base(x._M_node){}
 typename Traits::reference operator*()const{return ((_List_node<T>*)_M_node)->_M_data;}
 typename Traits::pointer operator->()const{return &operator*();}
 _List_iterator& operator++(){_M_incr();return *this;}
 _List_iterator& operator--(){_M_decr();return *this;}
};
template<class T,class A>class _List_base {
public:
 typedef _List_node<T> _Node;
 _List_base(const A& a):_M_node(allocator<_Node>(a),(_Node*)0){_Node* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;}
 ~_List_base(){clear();_M_node.deallocate(_M_node._M_data,1);}
 void clear();
 _STLP_alloc_proxy<_Node*,_Node,allocator<_Node> > _M_node;
};
template<class T,class A>void _List_base<T,A>::clear(){_List_node<T>* cur=(_List_node<T>*)this->_M_node._M_data->_M_next;while(cur!=this->_M_node._M_data){_List_node<T>* tmp=cur;cur=(_List_node<T>*)cur->_M_next;_Destroy(&tmp->_M_data);this->_M_node.deallocate(tmp,1);}this->_M_node._M_data->_M_next=this->_M_node._M_data;this->_M_node._M_data->_M_prev=this->_M_node._M_data;}
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{
public:
 typedef _List_node<T> _Node;typedef _List_iterator<T,_Nonconst_traits<T> > iterator;typedef _List_iterator<T,_Const_traits<T> > const_iterator;
 explicit list(const A& a=A()):_List_base<T,A>(a){}
 list(const list& x):_List_base<T,A>(x.get_allocator()){insert(begin(),x.begin(),x.end());}
 ~list(){}
 A get_allocator()const{return A((const allocator<_Node>&)this->_M_node);}
 iterator begin(){return (_Node*)this->_M_node._M_data->_M_next;}const_iterator begin()const{return (_Node*)this->_M_node._M_data->_M_next;}
 iterator end(){return this->_M_node._M_data;}const_iterator end()const{return this->_M_node._M_data;}
 T& back(){return *--end();}const T& back()const{return *--end();}
public:
 _Node* _M_create_node(const T& x){_Node* p=this->_M_node.allocate(1);_Construct(&p->_M_data,x);return p;}
public:
 iterator insert(iterator position,const T& x){_Node* tmp=_M_create_node(x);_List_node_base* n=position._M_node;_List_node_base* p=n->_M_prev;tmp->_M_next=n;tmp->_M_prev=p;p->_M_next=tmp;n->_M_prev=tmp;return tmp;}
 void push_back(const T& x){insert(end(),x);}
 template<class I>void insert(iterator pos,I first,I last){_M_insert_dispatch(pos,first,last,__false_type());}
 template<class I>void _M_insert_dispatch(iterator pos,I first,I last,const __false_type&){for(;first!=last;++first)insert(pos,*first);}
};
}
#endif


template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return this->_M_start[n];}const T&operator[](unsigned int n)const{return this->_M_start[n];}};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_STRIPIFIER)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

#ifndef FABLE_STLPORT46_STRIPIFIER_H
#define FABLE_STLPORT46_STRIPIFIER_H


#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <new>
namespace std {
struct __true_type{};struct __false_type{};
template<class T>struct __type_traits{typedef __false_type is_POD_type;typedef __false_type has_trivial_destructor;};
template<>struct __type_traits<long>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<unsigned short>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<>struct __type_traits<bool>{typedef __true_type is_POD_type;typedef __true_type has_trivial_destructor;};
template<class T>class allocator{public:allocator(){};T*allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){if(p)free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void __destroy_aux(I a,I b,const __false_type&){for(;a!=b;++a)_Destroy(&*a);}
template<class I>inline void __destroy_aux(I,I,const __true_type&){}
template<class T>inline void _Destroy(T*a,T*b){__destroy_aux(a,b,typename __type_traits<T>::has_trivial_destructor());}
inline void*__copy_trivial(const void*a,const void*b,void*d){return b==a?d:(char*)memmove(d,a,(const char*)b-(const char*)a)+((const char*)b-(const char*)a);}
template<class T>inline T*__copy_ptrs(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*__uninitialized_copy(const T*a,const T*b,T*d,const __true_type&){return(T*)__copy_trivial(a,b,d);}
template<class T>inline T*fill_n(T*p,size_t n,const T&x){for(;n>0;--n,++p)*p=x;return p;}
template<class T>inline T*__uninitialized_fill_n(T*p,size_t n,const T&x,const __true_type&){return fill_n(p,n,x);}
template<class T,class A>class _Vector_base{public:
 explicit _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 _Vector_base(size_t n,const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){_M_start=_M_end_of_storage.allocate(n);_M_finish=_M_start;_M_end_of_storage._M_data=_M_start+n;}
 ~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
public:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;typedef typename __type_traits<T>::is_POD_type _IsPODType;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 vector(size_type n,const T&x,const A&a=A()):_Vector_base<T,A>(n,a){this->_M_finish=__uninitialized_fill_n(this->_M_start,n,x,_IsPODType());}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 T&back(){return *(end()-1);}const T&back()const{return *(end()-1);}
 void pop_back(){--this->_M_finish;_Destroy(this->_M_finish);}
 T&operator[](size_type n){return this->_M_start[n];}const T&operator[](size_type n)const{return this->_M_start[n];}
 void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);}
 iterator erase(iterator first,iterator last){iterator i=__copy_ptrs(last,this->_M_finish,first,_IsPODType());_Destroy(i,this->_M_finish);this->_M_finish=i;return first;}
 void clear(){erase(begin(),end());}
 void resize(size_type,const T&);
public:
 void _M_clear(){_Destroy(this->_M_start,this->_M_finish);this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);}
 void _M_set(T*s,T*f,T*e){this->_M_start=s;this->_M_finish=f;this->_M_end_of_storage._M_data=e;}
 void _M_insert_overflow(T*position,const T&x,const __true_type&,size_type fill_len,bool at_end=false){
 const size_type old_size=size();const size_type len=old_size+(max)(old_size,fill_len);
 T*new_start=this->_M_end_of_storage.allocate(len);T*new_finish=(T*)__copy_trivial(this->_M_start,position,new_start);
 new_finish=fill_n(new_finish,fill_len,x);if(!at_end)new_finish=(T*)__copy_trivial(position,this->_M_finish,new_finish);
 _M_clear();_M_set(new_start,new_finish,new_start+len);
 }
};
}
#endif



template<class T>class CArray:public std::vector<T>{
public:
 CArray():std::vector<T>(){}
 explicit CArray(unsigned int n):std::vector<T>(n,T()){}
 CArray(unsigned int n,const T&x):std::vector<T>(n,x){}
 T&operator[](unsigned int n){return this->_M_start[n];}
 const T&operator[](unsigned int n)const{return this->_M_start[n];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_FONT)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T> class CArray:public std::vector<T> {
public:
 T& operator[](unsigned int i){return this->_M_start[i];}
 T* GetAsCArray(){return this->size()>0?&this->front():GetZeroSizeArray();}
public:T* GetZeroSizeArray(){return (T*)this;}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_GEOMETRY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H


template<class T>class CArray:public std::vector<T>{
public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}
};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY_DISPLAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H

template<class T> class CArray : public std::vector<T> {};
#endif
#else
#if defined(FABLE_RETAIL_CARRAY)
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



template<class T> class CArray : public std::vector<T> {
public:
    explicit CArray(unsigned int count) : std::vector<T>(count) {}
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    CArray() : std::vector<T>() { }
    T& operator[](unsigned int n) { return this->_M_start[n]; }
    const T& operator[](unsigned int n) const { return this->_M_start[n]; }
    const T* GetAsCArray() const { return this->size()>0 ? &*this->begin() : (const T*)this; }
#endif
    T* GetAsCArray() { return this->size()>0 ? &this->front() : GetZeroSizeArray(); }
public:
    T* GetZeroSizeArray() { return (T*)this; }
};
#endif
#else
#ifndef FABLE_ENGINE_CARRAY_H
#define FABLE_ENGINE_CARRAY_H



template<class T> class CArray : public std::vector<T> {
public:
    T& operator[](unsigned int i) { return this->_M_start[i]; }
    const T& operator[](unsigned int i) const { return this->_M_start[i]; }
};
#endif

#endif

#endif

#endif

#endif

#endif

#endif

#endif

#endif

#else
template<class T> class CArray : public std::vector<T,std::allocator<T> > {
#ifdef FABLE_RETAIL_FACTIONS
public:
    void push_back(const T& value){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,value);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,value,std::__false_type(),1,true);
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
public:void push_back(const T&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
public:
    
    void push_back(const T& _Val){
        if(this->_M_finish!=this->_M_end_of_storage._M_data){std::_Construct(this->_M_finish,_Val);++this->_M_finish;}
        else this->_M_insert_overflow(this->_M_finish,_Val,typename std::__type_traits<T>::has_trivial_assignment_operator(),1,true);
    }
#endif
};
#endif
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
#ifdef FABLE_RETAIL_FACTIONS
namespace std {template<class Iterator,class Compare>void __fastcall sort(Iterator,Iterator,Compare);}
#endif
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
#ifdef FABLE_RETAIL_FACTIONS
    CVectorMap():Dirty(false){}
#endif
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
#ifdef FABLE_RETAIL_VISIBLE_LOWERBOUND
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        long count=this->end()-first;
        while(count>0) {
            long half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
#else
    iterator LowerBound(const K&);
    const_iterator LowerBound(const K&) const;
#endif
#if defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_FACTIONS)
    void Insert(const K& key,const V& value){this->push_back(value_type(key,value));Dirty=true;}
    void Sort(){std::sort(this->begin(),this->end(),Compare);Dirty=false;}
    void Update(){if(Dirty)Sort();}
#endif
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
public:
    CompareType Compare;
    bool Dirty;
};
#endif


class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }
#if defined(FABLE_RETAIL_TYPED_GETTC) || defined(FABLE_RETAIL_QUICK_ACCESS)


template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
#endif
class CThing : public CClass {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
public:
#ifdef FABLE_RETAIL_CTHING_CONSTRUCTORS
    CThing(unsigned long);
#endif
    unsigned __int64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
public:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

public:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;
    unsigned char AddingTCsPermitted : 1;

public:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); 
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
#ifdef FABLE_RETAIL_SHOP
    const CThingBaseDef& PeekBaseDef()const{return *PBaseDef;}
    unsigned long GetDefGlobalIndex()const{return DefGlobalIndex;}
#endif
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_TARGET_RANDOM_WALK) || defined(FABLE_RETAIL_HIT_EFFECT)
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    unsigned __int64 GetUniqueID() const { return UniqueID; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    bool IsLevelLess() const;
    template<class T> bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=static_cast<T*>(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second));
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    unsigned __int64 GetUniqueID()const{return UniqueID;}
    const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()const{return PTCGraphicAppearance;}
#endif
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
#ifdef FABLE_RETAIL_FORCE_HASTC
    __forceinline bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#else
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
#endif
#ifdef FABLE_RETAIL_TYPED_GETTC
    template<class T> __forceinline bool GetTC(T** result) {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(Components.Find(T::StatGetInterfaceType())->second,(T*)0);
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool PeekTC(T** result) const {
        if(HasTC(T::StatGetInterfaceType())) {
            *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
            return true;
        }
        return false;
    }
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    template<class T> bool PeekTC(T** result) const {
        bool present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_HIT_EFFECT
    template<class T> bool GetTCDriver(T**);
#endif
#ifdef FABLE_RETAIL_TYPED_GETTCDRIVER
    
    
    template<class T> __forceinline bool GetTCDriver(T** result) {
        const bool& present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(&GetTC(T::StatGetInterfaceType()),(T*)0);
        return present;
    }
#endif
#ifdef FABLE_RETAIL_FORCE_GETTC
    __forceinline CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    __forceinline const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#else
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
#endif
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThing,UniqueID)==0x18);
        FABLE_STATIC_ASSERT(offsetof(CThing,UsingComponents)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CThing,Components)==0x44);
        FABLE_STATIC_ASSERT(offsetof(CThing,PhysicsTC)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CThing,PBaseDef)==0x70);
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
public:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned long* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};
FABLE_STATIC_ASSERT(sizeof(CThing)==0x98);

#endif
#else
#ifndef FABLE_ENGINE_CTHING_H
#define FABLE_ENGINE_CTHING_H






#include <stddef.h>


class CCharStringData;
class CDefPointeeBase;
class CTCDriverBase;
struct CTCGraphicAppearance;
class CTCPhysicsBase;

#pragma pack(push, 1)
class CThing {
public:
    void*                 __vftable;                         
    unsigned char         _pad_0x04[0x4];                    
    unsigned char         _pad_0x08[0x4];                    
    unsigned char         _pad_0x0c[0x4];                    
    unsigned char         _pad_0x10[0x1];                    
    unsigned char         _pad_0x11[0x7];                    
    unsigned __int64      UniqueID;                          
    unsigned char         UsingComponents[0x24];             
    unsigned char         Components[0x10];                  
    unsigned char         UpdateComponentsList[0x4];         
    CCharStringData*      SerialisationSectionName_PStringData; 
    CTCDriverBase*        PTCDriver;                         
    CTCPhysicsBase*       PhysicsTC;                         
    CTCGraphicAppearance* PTCGraphicAppearance;              
    long                  WorldFrameCreatedAt;               
    unsigned long         ThingTypeFlags;                    
    CDefPointeeBase*      PBaseDef_Object;                   
    CCharStringData*      ScriptName_PStringData;            
    CCharStringData*      ScriptData_PStringData;            
    CThing*               RenderListPrev;                    
    CThing*               RenderListNext;                    
    CThing*               UpdateListPrev;                    
    CThing*               UpdateListNext;                    
    unsigned short        DefGlobalIndex;                    
    short                 CurrentMapNumber;                  
    signed char           PlayerNumber;                      
    unsigned char         CreatedBySerialisation[0x1];       
    unsigned char         PausedDuringCutscenes[0x1];        
    unsigned char         AddingTCsPermitted[0x1];           
    unsigned char         InitialisationStage[0x4];          
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CThing, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThing, UniqueID) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CThing, UsingComponents) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CThing, Components) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateComponentsList) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CThing, SerialisationSectionName_PStringData) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCDriver) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CThing, PhysicsTC) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThing, PTCGraphicAppearance) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CThing, WorldFrameCreatedAt) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CThing, ThingTypeFlags) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CThing, PBaseDef_Object) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptName_PStringData) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CThing, ScriptData_PStringData) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListPrev) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CThing, RenderListNext) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListPrev) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CThing, UpdateListNext) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CThing, DefGlobalIndex) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CThing, CurrentMapNumber) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CThing, PlayerNumber) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CThing, CreatedBySerialisation) == 0x91);
FABLE_STATIC_ASSERT(offsetof(CThing, PausedDuringCutscenes) == 0x92);
FABLE_STATIC_ASSERT(offsetof(CThing, AddingTCsPermitted) == 0x93);
FABLE_STATIC_ASSERT(offsetof(CThing, InitialisationStage) == 0x94);

#endif 

#endif



CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
void CBaseObject::Validate()const{}void CClass::Save(CCharString&){}
CThing::CThing(unsigned long type):UniqueID(0),PhysicsTC(0),PTCDriver(0),PTCGraphicAppearance(0),WorldFrameCreatedAt(0),ThingTypeFlags(type){Type=(unsigned char)type;Dead=0;DefGlobalIndex=100;memset(&UsingComponents,0,sizeof(UsingComponents));}
namespace std { template<class T,class A>void _List_base<T,A>::clear(){assert(_M_node._M_data->_M_next==_M_node._M_data);} }

namespace std {
template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rotate_left(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_right;
  __x->_M_right = __y->_M_left;
  if (__y->_M_left !=0)
    __y->_M_left->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_left)
    __x->_M_parent->_M_left = __y;
  else
    __x->_M_parent->_M_right = __y;
  __y->_M_left = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void __cdecl 
_Rb_global<_Dummy>::_Rotate_right(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root)
{
  _Rb_tree_node_base* __y = __x->_M_left;
  __x->_M_left = __y->_M_right;
  if (__y->_M_right != 0)
    __y->_M_right->_M_parent = __x;
  __y->_M_parent = __x->_M_parent;

  if (__x == __root)
    __root = __y;
  else if (__x == __x->_M_parent->_M_right)
    __x->_M_parent->_M_right = __y;
  else
    __x->_M_parent->_M_left = __y;
  __y->_M_right = __x;
  __x->_M_parent = __y;
}

template <class _Dummy> void __cdecl
_Rb_global<_Dummy>::_Rebalance(_Rb_tree_node_base* __x, 
			       _Rb_tree_node_base*& __root)
{
  __x->_M_color = false;
  while (__x != __root && __x->_M_parent->_M_color == false) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == false) {
        __x->_M_parent->_M_color = true;
        __y->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _Rotate_left(__x, __root);
        }
        __x->_M_parent->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        _Rotate_right(__x->_M_parent->_M_parent, __root);
      }
    }
    else {
      _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == false) {
        __x->_M_parent->_M_color = true;
        __y->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        __x = __x->_M_parent->_M_parent;
      }
      else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _Rotate_right(__x, __root);
        }
        __x->_M_parent->_M_color = true;
        __x->_M_parent->_M_parent->_M_color = false;
        _Rotate_left(__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = true;
}


}
#include <stdio.h>
#include <assert.h>
#include <string.h>



static int StringObjects,StringDataObjects,BaseCalls,Transfers,DefaultQueries,MapErases;
static long InputIDs[4],SavedIDs[4],InputCounters[4];
static float InputQuantity[4],InputMaximum[4],SavedQuantity[4],SavedMaximum[4];
static unsigned int InputCount,SavedCount;
static const char* FieldNames[7]={"StockItemDefsKeys","StockItemDefsQuantity","StockItemDefsMaxStock","DayLastChecked","GoldDonated","SpecialItem","DaysSinceSpecialItemPlaced"};
static CShopItem Defaults[4];
static bool DefaultAvailable[4];
static const char* ItemNames[4]={"Unknown","Potion","Apple","Sword"};
static CCharStringData* MakeData(const char* text){
 CCharStringData* d=new CCharStringData;
 size_t size=strlen(text);d->Data.PData=(char*)malloc(size+1);strcpy(d->Data.PData,text);
 d->Data.StringLength=(unsigned long)size;d->Data.DataLength=(unsigned long)size+1;d->Data.UseFastExtend=false;d->NoRefs=1;++StringDataObjects;return d;
}
static const char* Text(const CCharString& s){return s.PStringData?s.PStringData->Data.PData:"";}
static long ID(const CCharString& s){for(long i=1;i!=4;++i)if(strcmp(Text(s),ItemNames[i])==0)return i;return 0;}
CCharString::CCharString():PStringData(0){++StringObjects;}
CCharString::CCharString(const char* text,long):PStringData(MakeData(text)){++StringObjects;}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){if(PStringData)++PStringData->NoRefs;++StringObjects;}
CCharString::~CCharString(){if(PStringData&&--PStringData->NoRefs==0){free(PStringData->Data.PData);delete PStringData;--StringDataObjects;}--StringObjects;}
CShopItem::CShopItem(){Quantity=0;PriceMultiplier=1;MaxStock=0;RestockPeriod=0;RestockPeriodRandomness=0;SalesPerDay=0;SalesPerDayRandomness=0;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){} CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CTCBase::CTCBase(CThing& p):ParentThing(p),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}
CTCBase::~CTCBase(){}
void CTCBase::OnSerialise(CPersistContext&,bool){++BaseCalls;}
CTCShop::CTCShop(CThing& p):CTCBase(p),StockState(STOCKED_DONTKNOW),ShopKeeperAvailable(false),SpecialItem(-1),DaysSinceSpecialItemPlaced(0),DayLastChecked(0),GoldDonated(0),StallLike(false),Alarm(0){}
CTCShop::~CTCShop(){}
ETCInterfaceType CTCShop::GetInterfaceType()const{return TCI_SHOP;}
CCharString CTCShop::GetClassTypeName()const{return CCharString("CTCShop");}
bool CTCShop::HasFrameUpdate()const{return true;}
void CTCShop::FrameUpdate(){}void CTCShop::OnCreate(){}void CTCShop::OnInitialActivate(){}
bool CTCShop::IsComponentSerialisable()const{return true;}
const CShopItem* CTCShop::FindDefaultStockItem(long id)const{++DefaultQueries;return id>0&&id<4&&DefaultAvailable[id]?&Defaults[id]:0;}
#define VOID_BOUNDARY(name,args) void CTCBase::name args {assert(false);}
VOID_BOUNDARY(OnRemove,())
VOID_BOUNDARY(ConstructFromParams,(const CTCParamsBase&))
VOID_BOUNDARY(OnCreate,())
VOID_BOUNDARY(InitialActivate,())
VOID_BOUNDARY(OnInitialActivate,())
VOID_BOUNDARY(RenderUpdate,(float))
bool CTCBase::HasFrameUpdate()const{return false;}
VOID_BOUNDARY(FrameUpdate,())
VOID_BOUNDARY(BulletTimeFrameUpdate,())
VOID_BOUNDARY(DrawPreparePrimitivesForRendering,(const CEnginePrimitivePrepareInfo&))
VOID_BOUNDARY(Copy,(const CTCBase&))
VOID_BOUNDARY(OnDie,())
VOID_BOUNDARY(OnKill,())
VOID_BOUNDARY(OnLevelUnload,())
VOID_BOUNDARY(OnSetInLimbo,(bool))
bool CTCBase::Validate(CCharString&)const{return true;}
bool CTCBase::IsFreeForDeletion()const{return true;}bool CTCBase::IsSerialiseCreatable()const{return true;}
bool CTCBase::IsSerialisableForHeroState()const{return true;}bool CTCBase::IsComponentSerialisable()const{return true;}
bool CTCBase::IsDriver()const{return false;}bool CTCBase::IsCopyable()const{return false;}
VOID_BOUNDARY(GlobalUpdate,(bool))
VOID_BOUNDARY(HandleMessage,(const NTCMessages::CMessageBase&))
#undef VOID_BOUNDARY
CFailedAllocationHandler::~CFailedAllocationHandler(){}
CResourceList::~CResourceList(){}bool CResourceList::HandleFailedAllocation(unsigned long){return false;}
CDefinitionManager::~CDefinitionManager(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}
void CGameDefinitionManager::InitAndCompile(){assert(false);}
void CIVCountedPointeeBase::Delete()const{assert(false);}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=0;}
CResource::~CResource(){}bool CResource::UnloadResource(){return true;}unsigned long CResource::GetUnloadableMemory()const{return 0;}
bool CResource::IsResourceLoaded()const{return true;}void CResource::OnResourceLoaded(){}
bool CResource::CanAbortResourceLoad(long)const{return false;}void CResource::OnResourceLoadAborted(){}
unsigned long CResource::GetDebugMemoryInUse()const{return 0;}void CResource::ResetDebugMemoryInUse(){}
static CGameDefinitionManager DefinitionManager;
const CGameDefinitionManager& CTCBase::GetDefinitionManager()const{return DefinitionManager;}
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long n)const{CDefString s;s.TablePos=n;return s;}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& s)const{return ID(s);}
CDefStringTable GDefStringTable;
CCharString CDefStringTable::GetString(long n)const{assert(n>=0&&n<4);return CCharString(ItemNames[n]);}
namespace std {
template<class V,class A>_Rb_tree_base<V,A>::_Rb_tree_base(const A&):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=(_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));memset(_M_header._M_data,0,sizeof(_Rb_tree_node<V>));}
template<class V,class A>_Rb_tree_base<V,A>::~_Rb_tree_base(){free(_M_header._M_data);}
template<class K,class V,class F,class C,class A>_Rb_tree<K,V,F,C,A>::_Rb_tree(const C& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
template<class K,class V,class F,class C,class A>_Rb_tree<K,V,F,C,A>::~_Rb_tree(){clear();}
template<class K,class V,class F,class C,class A>void _Rb_tree<K,V,F,C,A>::_M_erase(_Rb_tree_node<V>* p){if(p){_M_erase((_Rb_tree_node<V>*)p->_M_left);_M_erase((_Rb_tree_node<V>*)p->_M_right);p->_M_value_field.~V();free(p);++MapErases;}}

}
static void CheckTransfer(const char* name){assert(Transfers<7);assert(strcmp(name,FieldNames[Transfers])==0);++Transfers;}
template<>void CPersistContext::TransferVector<CCharString>(const char* name,std::vector<CCharString>& values){
 CheckTransfer(name);if(IsSaving()){SavedCount=(unsigned int)values.size();for(unsigned int i=0;i<SavedCount;++i)SavedIDs[i]=ID(values[i]);}
 else for(unsigned int i=0;i<InputCount;++i)values.push_back(CCharString(ItemNames[InputIDs[i]]));
}
template<>void CPersistContext::Transfer<float>(const char* name,std::vector<float>& values){
 CheckTransfer(name);float* input=Transfers==2?InputQuantity:InputMaximum;float* output=Transfers==2?SavedQuantity:SavedMaximum;
 if(IsSaving()){assert(values.size()==SavedCount);for(unsigned int i=0;i<SavedCount;++i)output[i]=values[i];}
 else for(unsigned int i=0;i<InputCount;++i)values.push_back(input[i]);
}
template<>void CPersistContext::Transfer<long>(const char* name,long& val,const long& default_value){
 CheckTransfer(name);assert(default_value==(Transfers==6?-1:0));if(!IsSaving())val=InputCounters[Transfers-4];
}
static void Reset(){BaseCalls=Transfers=DefaultQueries=MapErases=0;InputCount=SavedCount=0;for(int i=0;i<4;++i){InputIDs[i]=SavedIDs[i]=0;InputQuantity[i]=InputMaximum[i]=SavedQuantity[i]=SavedMaximum[i]=0;InputCounters[i]=i+7;DefaultAvailable[i]=true;Defaults[i].PriceMultiplier=2;Defaults[i].RestockPeriod=4;Defaults[i].SalesPerDay=8;}}
static void RunSaving(CPersistContext::EMode mode,bool in_game,bool populated){
 Reset();{CThing thing(1);CTCShop shop(thing);CPersistContext context;context.Mode=mode;
 if(populated){shop.Stock[2].Quantity=2.5f;shop.Stock[2].MaxStock=4;shop.Stock[1].Quantity=1.25f;shop.Stock[1].MaxStock=8;}
 shop.OnSerialise(context,in_game);assert(BaseCalls==1);assert(Transfers==(in_game?7:0));assert(DefaultQueries==0);
 if(in_game){assert(SavedCount==(populated?2u:0u));if(populated){assert(SavedIDs[0]==1&&SavedIDs[1]==2);assert(SavedQuantity[0]==1.25f&&SavedQuantity[1]==2.5f);assert(SavedMaximum[0]==8&&SavedMaximum[1]==4);}}
 }assert(StringObjects==0&&StringDataObjects==0);
}
static void RunLoading(int scenario,bool in_game){
 Reset();{CThing thing(1);CTCShop shop(thing);CPersistContext context;context.Mode=CPersistContext::MODE_LOAD_BINARY;shop.Stock[3].Quantity=99;
 if(scenario){InputCount=3;InputIDs[0]=2;InputIDs[1]=1;InputIDs[2]=scenario==2?0:2;InputQuantity[0]=2.5f;InputQuantity[1]=4;InputQuantity[2]=8;InputMaximum[0]=4;InputMaximum[1]=8;InputMaximum[2]=16;}
 if(scenario==3)DefaultAvailable[1]=false;
 shop.OnSerialise(context,in_game);assert(BaseCalls==1&&Transfers==7);assert(shop.DayLastChecked==7&&shop.GoldDonated==8&&shop.SpecialItem==9&&shop.DaysSinceSpecialItemPlaced==10);
 if(!scenario){assert(shop.Stock._M_t._M_node_count==1);assert(shop.Stock[3].Quantity==99);assert(DefaultQueries==0);}
 else {assert(MapErases==1);assert(shop.Stock._M_t._M_node_count==(scenario==3?1u:2u));assert(shop.Stock[2].Quantity==(scenario==2?2.5f:8.0f));assert(shop.Stock[2].MaxStock==(scenario==2?4.0f:16.0f));assert(shop.Stock[2].PriceMultiplier==2&&shop.Stock[2].RestockPeriod==4&&shop.Stock[2].SalesPerDay==8);if(scenario!=3)assert(shop.Stock[1].Quantity==4&&shop.Stock[1].MaxStock==8);}
 }assert(StringObjects==0&&StringDataObjects==0);
}
int main(){RunSaving(CPersistContext::MODE_SAVE_TEXT,true,false);RunSaving(CPersistContext::MODE_SAVE_BINARY,true,true);RunSaving(CPersistContext::MODE_SAVE_TEXT,false,true);RunSaving(CPersistContext::MODE_SAVE_BINARY,false,false);RunLoading(0,true);RunLoading(0,false);RunLoading(1,true);RunLoading(1,false);RunLoading(2,true);RunLoading(2,false);RunLoading(3,true);RunLoading(3,false);puts("00775c60_TEST PASS");return 0;}