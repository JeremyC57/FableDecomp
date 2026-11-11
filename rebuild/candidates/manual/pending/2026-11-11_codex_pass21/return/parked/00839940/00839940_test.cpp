#ifndef FABLE_ENGINE_ETCINTERFACETYPE_H
#define FABLE_ENGINE_ETCINTERFACETYPE_H
// Ego_r original enum names and values; native HasTC masks corroborate used values.
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
    TCI_PARTICLE_EMITTER = 75,
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

#ifndef FABLE_ENGINE_EHEROEXPERIENCESTATCATEGORY_H
#define FABLE_ENGINE_EHEROEXPERIENCESTATCATEGORY_H
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH=0,HERO_STAT_SKILL=1,HERO_STAT_WILL=2,NUMBER_OF_HERO_STAT_CATEGORIES=3 };
#endif

#ifndef FABLE_ENGINE_EDAMAGEATTRIBUTE_H
#define FABLE_ENGINE_EDAMAGEATTRIBUTE_H
// Original enumerators from both raw donor PDBs and native map-key/damage argument uses.
enum EDamageAttribute {
    DAMAGE_NULL=-1,
    DAMAGE_MELEE=0,
    DAMAGE_MELEE_UNARMED=1,
    DAMAGE_LIGHTNING=2,
    DAMAGE_FIRE=3,
    DAMAGE_PROJECTILE=4,
    DAMAGE_EXPLOSION=5,
    DAMAGE_DRAIN=6,
    DAMAGE_DRAIN_HEAL=7,
    DAMAGE_GENERIC_WILL=8,
    DAMAGE_DIVINE_WRATH=9,
    DAMAGE_UNHOLY_POWER=10
};
#endif

// Full production OnHit body with controlled behavior-test dependencies.
// This fixture exercises caller logic; it does not claim to test engine ABI or pointer ownership.
#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
static void Check(bool v,int line){if(!v){std::fprintf(stderr,"FAIL line %d\n",line);std::exit(1);}}
#define CHECK(x) Check(!!(x),__LINE__)
static std::vector<std::string> Trace;
static void Record(const char* x){Trace.push_back(x);}
static int Count(const char* x){int n=0;for(unsigned i=0;i<Trace.size();++i)if(Trace[i]==x)++n;return n;}
class CCharString {public:std::string Value;CCharString(const char* s="",long=-1):Value(s){}bool operator==(const CCharString& x)const{return Value==x.Value;}};
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector& r)const{return C3DVector(X+r.X,Y+r.Y,Z+r.Z);}};
class CRGBColour {public:unsigned char R,G,B,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b):R(r),G(g),B(b),A(255){} };
template<class T>class CDefPointer {public:T* Data;CDefPointer(T* p=0):Data(p){}T* Get()const{return Data;}T* operator->()const{CHECK(Data);return Data;}T& operator*()const{CHECK(Data);return *Data;}};
class CMaterialDef {public:enum{Kind=1};float OnHitRumbleLevel,OnHitRumbleFalloff;CMaterialDef():OnHitRumbleLevel(0),OnHitRumbleFalloff(0.5f){} };
class CArmourDef {public:long DamageMaterial,BlockMaterial;CArmourDef():DamageMaterial(0),BlockMaterial(0){} };
class CCreatureStatsDef {};
class CCreatureDef {public:enum{Kind=2};CCreatureStatsDef Stats;const CCreatureStatsDef& GetStatsDef()const{return Stats;}};
class CHeroStatIncreaseData {public:std::map<EHeroExperienceStatCategory,float> StatIncreases;};
class CHeroExperienceDef {public:enum{Kind=3};std::map<EDamageAttribute,CHeroStatIncreaseData> StatIncreasePerHitType;};
class CThingBaseDef {public:std::map<int,void*> Sub;
 template<class T>bool GetSubDef(CDefPointer<T>* out)const{std::map<int,void*>::const_iterator i=Sub.find(T::Kind);if(i==Sub.end())return false;out->Data=static_cast<T*>(i->second);return true;}
};
class CDefinitionManager {public:std::map<long,CMaterialDef*> Materials;std::map<std::string,CMaterialDef*> Named;
 bool GetDef(long n,CDefPointer<const CMaterialDef>* p)const{Record("material-index");std::map<long,CMaterialDef*>::const_iterator i=Materials.find(n);if(i==Materials.end())return false;p->Data=i->second;return true;}
 bool GetDef(const CCharString& n,CDefPointer<const CMaterialDef>* p)const{Record(n.Value.c_str());std::map<std::string,CMaterialDef*>::const_iterator i=Named.find(n.Value);if(i==Named.end())return false;p->Data=i->second;return true;}
};
class CThing;class CThingGameObject;class CThingCreatureBase;class CHitParameters;
class CTCBase {public:virtual ~CTCBase(){}virtual CCharString GetClassTypeName()const{return CCharString("component");}};
#define TEST_TC(cls,id) class cls:public CTCBase {public:static ETCInterfaceType StatGetInterfaceType(){return id;}
class CThingManager {public:bool Draw;CThingManager():Draw(false){}bool IsDrawCombatCollisionDebug()const{return Draw;}};
class CWorldMap {};
class CPlayer {public:long Number;CPlayer():Number(0){}long GetNumber()const{return Number;}};
class CPlayerManager {public:CThing* Main;CPlayer Player;CPlayerManager():Main(0){}bool IsThingMainPlayer(const CThing& t)const{return &t==Main;}const CPlayer& GetMainPlayer()const{return Player;}};
class CCombatManager {public:void DeregisterCombatant(CThingCreatureBase&){Record("deregister-combatant");}};
static CThingManager Manager;static CWorldMap World;static CDefinitionManager Definitions;static CPlayerManager Players;static CCombatManager Combat;
class CThing {public:std::map<int,CTCBase*> Components;bool Dead;unsigned long Flags;long Type;C3DVector Pos;CPlayer Player;CThingBaseDef Base;
 CThing():Dead(false),Flags(0),Type(1){}virtual ~CThing(){}
 bool IsDead()const{return Dead;}unsigned long GetTypeFlags()const{return Flags;}long GetType()const{return Type;}const C3DVector& GetPos()const{return Pos;}
 bool HasTC(ETCInterfaceType n)const{return Components.find(n)!=Components.end();}
 template<class T>bool HasTC()const{return HasTC(T::StatGetInterfaceType())&&Components.find(T::StatGetInterfaceType())->second->GetClassTypeName()==T::StatGetClassTypeName();}
 template<class T>bool GetTC(T** p){typename std::map<int,CTCBase*>::iterator i=Components.find(T::StatGetInterfaceType());if(i==Components.end())return false;*p=static_cast<T*>(i->second);return true;}
 template<class T>bool PeekTC(T** p)const{typename std::map<int,CTCBase*>::const_iterator i=Components.find(T::StatGetInterfaceType());if(i==Components.end())return false;*p=static_cast<T*>(i->second);return true;}
 template<class T>bool GetTCDriver(T** p){return GetTC(p);}void RemoveTC(ETCInterfaceType n){Record("remove-flourish");Components.erase(n);}
 const CDefinitionManager& PeekDefinitionManager()const{return Definitions;}CThingManager& GetThingManager(){return Manager;}
 const CWorldMap& PeekWorldMap()const{return World;}CPlayerManager& GetPlayerManager(){return Players;}const CPlayerManager& PeekPlayerManager()const{return Players;}
 const CPlayer& PeekPlayer()const{return Player;}long GetJoystickDeviceNumber()const{return 0;}CDefPointer<const CThingBaseDef> GetPBaseDef()const{return CDefPointer<const CThingBaseDef>(&Base);}
 template<class T>void Attach(T& t){Components[T::StatGetInterfaceType()]=&t;}
};
template<class T>bool IS_THING_ALIVE(const T& t){return t!=0&&!t->IsDead();}
class CThingPhysical:public CThing {public:CMaterialDef* Material;float Health;bool Damageable,Negate,LastAllow;
 CThingPhysical():Material(0),Health(16),Damageable(true),Negate(false),LastAllow(true){Flags=4;}
 virtual CDefPointer<const CMaterialDef> PeekPMaterial()const{Record("peek-material");return CDefPointer<const CMaterialDef>(Material);}
 virtual void ModifyHealth(float d,bool allowed){Record("modify-health");LastAllow=allowed;if(Damageable){Health+=d;if(!allowed&&Health<1)Health=1;}}
 bool IsDamageable()const{return Damageable;}bool IsToNegateAllHits()const{return Negate;}float GetHealth()const{return Health;}
};
class CThingGameObject:public CThingPhysical {public:bool GiveStats,AddCombo;CThingGameObject():GiveStats(false),AddCombo(false){}
 bool IsGivingHeroStatChangesOnBeingHit()const{return GiveStats;}bool IsAddToComboMultiplierOnHit()const{return AddCombo;}virtual void OnHit(CHitParameters&);
};
class CThingCreatureBase:public CThingGameObject {public:bool InMelee;CThingCreatureBase():InMelee(false){Flags=12;}
 void MakeNoise(const CCharString& s){CHECK(s.Value=="SIMPLE_NOISE");Record("noise");}bool IsInMeleeAsAttacker()const{return InMelee;}CCombatManager& GetCombatManager(){return Combat;}
 void PerformBlockedReaction(CThingCreatureBase* p,bool yes){CHECK(!p&&yes);Record("recoil");}
};
namespace NHitLocations {class CHitLocation {public:CDefPointer<const CArmourDef> Armour;bool Response;CHitLocation():Response(true){}const CDefPointer<const CArmourDef>& GetArmourDef()const{return Armour;}bool IsToPlayGetHitResponse()const{return Response;}void ProcessHitLocationCode(CThing&,const CHitParameters&){Record("hit-location");}};}
class CHitParameters {public:CThing* Hitter;CThing* Weapon;NHitLocations::CHitLocation* Location;CDefPointer<const CMaterialDef> Material;
 bool Local,Directional,Melee,Blocked,Negated,Recoil,Response,Lightning,Divine,Unholy;float Damage;C3DVector Pos,Direction;std::map<EDamageAttribute,float> DamageMap;
 CHitParameters():Hitter(0),Weapon(0),Location(0),Local(false),Directional(false),Melee(false),Blocked(false),Negated(false),Recoil(false),Response(true),Lightning(false),Divine(false),Unholy(false),Damage(-4),Pos(2,4,8),Direction(1,2,4){}
 CThing* GetPWeapon()const{return Weapon;}CThing* GetPThingHitting()const{return Hitter;}NHitLocations::CHitLocation* GetPHitLocation()const{return Location;}
 void SetHitLocation(NHitLocations::CHitLocation& x){Location=&x;Record("set-location");}void SetPHitMaterialDef(CDefPointer<const CMaterialDef> x){Material=x;Record("set-material");}CDefPointer<const CMaterialDef> GetPHitMaterialDef()const{return Material;}
 bool IsLocalised()const{return Local;}bool IsDirectional()const{return Directional;}const C3DVector& GetHitPosition()const{return Pos;}const C3DVector& GetHitDirection()const{return Direction;}
 bool IsMeleeStrike()const{return Melee;}bool IsBlocked()const{return Blocked;}bool IsHitNegated()const{return Negated;}void SetHitNegated(bool x){Negated=x;Record("negate");}
 void SetPlayHitResponse(bool x){Response=x;Record("response");}void SetToCauseRecoil(bool x){Recoil=x;}bool IsToCauseRecoil()const{return Recoil;}
 void SetAsModifiedByArmour(const CThing&,CDefPointer<const CArmourDef>&){Record("armour");}float GetDamageToThing(const CThingGameObject&)const{return Damage;}
 const std::map<EDamageAttribute,float>& GetDamageMap()const{return DamageMap;}bool IsLightningStrike()const{return Lightning;}bool IsDivineWrathStrike()const{return Divine;}bool IsUnholyPowerStrike()const{return Unholy;}
};
TEST_TC(CTCHitLocations,TCI_HIT_LOCATIONS)NHitLocations::CHitLocation* Location;CTCHitLocations():Location(0){}NHitLocations::CHitLocation* GetPDefaultHitLocation(){Record("default-location");return Location;}};
TEST_TC(CTCPerceiveThing,TCI_PERCEIVE_THING)void OnHit(const CThing&){Record("perceive");}};
TEST_TC(CTCElectrocuteOnTouch,TCI_ELECTROCUTE_ON_TOUCH)void OnHit(CThing&){Record("electrocute");}};
TEST_TC(CTCHeroExperience,TCI_HERO_EXPERIENCE)bool Break;CTCHeroExperience():Break(true){}bool IsAbleToBreakDownDoors()const{return Break;}void AddSuccessfulBlockParry(){Record("parry");}void SetAsHit(){Record("hero-hit");}void AddSuccessfulStrikeWithHitParams(const CHitParameters&){Record("combo");}};
TEST_TC(CTCActionUseWindow,TCI_ON_ACTION_USE)static CCharString StatGetClassTypeName(){return CCharString("CTCActionUseWindow");}virtual CCharString GetClassTypeName()const{return StatGetClassTypeName();}};
TEST_TC(CTCWeapon,TCI_WEAPON)long Explosion;CTCWeapon():Explosion(0){}long GetExplosionType()const{return Explosion;}void IncrementNumThingsKilled(CThing*){Record("kill-credit");}};
TEST_TC(CTCDExplosion,TCI_EXPLOSION)static CThing* Created;static CThing* Create(long n,const C3DVector&){CHECK(n==7);Record("explosion");return Created;}void SetSpecificThingToHit(CThing&){Record("explosion-target");}};CThing* CTCDExplosion::Created=0;
TEST_TC(CTCFlammable,TCI_FLAMMABLE)bool Fire;CTCFlammable():Fire(false){}bool IsOnFire()const{return Fire;}void SetOnFire(){Fire=true;Record("set-fire");}};
TEST_TC(CTCBerserk,TCI_BERSERK)void IncreaseBerserkDamageToMeMultiplier(){Record("berserk");}};
TEST_TC(CTCCombat,TCI_COMBAT)bool Blocking;CTCCombat():Blocking(false){}void SetStruck(const NHitLocations::CHitLocation&,CHitParameters&){Record("struck");}bool IsBlocking()const{return Blocking;}void UnsetBlocking(){Blocking=false;Record("unblock");}void OnHit(const CThing&){Record("combat-hit");}};
TEST_TC(CTCEmoteIcon,TCI_EMOTEICON)enum EEmotePriority{EMOTE_PRIORITY_NONE=8};void SetEmoteIcon(EEmotePriority p,long n,float f,float scale,unsigned char a){CHECK(p==EMOTE_PRIORITY_NONE&&n==0&&f==0&&scale==1&&a==255);Record("emote");}};
TEST_TC(CTCMultiStrike,TCI_MULTI_STRIKE)bool Started;CTCMultiStrike():Started(false){}bool IsStrikeStarted()const{return Started;}void StartMultiHit(const CHitParameters&,CThing*){Started=true;Record("multistrike");}};
TEST_TC(CTCGuard,TCI_GUARD)bool Fine;CTCGuard():Fine(false){}bool IsAttackingToFineRatherThanKill()const{return Fine;}};
TEST_TC(CTCHero,TCI_HERO)bool Weapons;CTCHero():Weapons(true){}bool IsWeaponUseAllowed()const{return Weapons;}};
class CTCVillage:public CTCBase {public:bool Limbo;CTCVillage():Limbo(false){}bool GetVillageLimbo()const{return Limbo;}};
TEST_TC(CTCVillageMember,TCI_VILLAGE_MEMBER)CTCVillage* Village;CTCVillageMember():Village(0){}CTCVillage* GetPVillage(){return Village;}};
TEST_TC(CTCOwnedEntity,TCI_OWNED_ENTITY)CThing* Owner;CTCOwnedEntity():Owner(0){}CThing* GetPOwner(){return Owner;}};
TEST_TC(CTCAbsorbHits,TCI_ABSORB_HITS)long Remaining;CTCAbsorbHits():Remaining(0){}long DecrementHitsRemaining(){return --Remaining;}};
TEST_TC(CTCWound,TCI_WOUND)bool AddPossibleWoundGivenStrikePoint(const C3DVector&){Record("wound");return true;}};
TEST_TC(CTCDegradableBase,TCI_DEGRADABLE)virtual void OnHit(const C3DVector&,const C3DVector&){Record("degrade");}};
enum EOpinionDeedType{OPINION_DEED_TYPE_GENERIC_INCOMPREHENSION=73};enum EOpinionPostDeedType{OPINION_POST_DEED_TYPE_WITNESSES=2};
TEST_TC(CTCHeroOpinionDeedLog,TCI_HERO_OPINION_DEED_LOG)long PostDeed(EOpinionDeedType p,const C3DVector&,const CThing*,CThingCreatureBase*,EOpinionPostDeedType t,bool a,bool b,const CThing*,bool c,long d){CHECK(p==73&&t==2&&!a&&!b&&!c&&d==0);Record("deed");return 0;}};
namespace NThingShot {TEST_TC(CTCShotDefinitionBase,TCI_SHOT_DEFINITION)CThing* Weapon;CTCShotDefinitionBase():Weapon(0){}CThing* GetPWeapon()const{return Weapon;}};}
TEST_TC(CTCSummonableCreature,TCI_SUMMONABLE_CREATURE)bool Summoned;CTCSummonableCreature():Summoned(false){}bool GetHasBeenSummoned()const{return Summoned;}};
class CTCGhostSword {public:static const CThing* Ghost;static bool IsThingGhostSword(const CThing* p){return p==Ghost;}};const CThing* CTCGhostSword::Ghost=0;
namespace NHeroStatModifierEvent {
class CBase {public:virtual ~CBase(){}virtual void ModifyHeroStats()const{}};
class CHitThing:public CBase {public:bool Unarmed;EHeroExperienceStatCategory Stat;float Amount;CHitThing(const CCreatureStatsDef&):Unarmed(false),Stat(HERO_STAT_STRENGTH),Amount(0){}void SetAsUnarmed(bool b){Unarmed=b;}void SetStatToIncrement(EHeroExperienceStatCategory s){Stat=s;}void SetStatIncrementAmount(float x){Amount=x;}};
}
TEST_TC(CTCHeroStats,TCI_HERO_STATS)std::vector<NHeroStatModifierEvent::CHitThing> Seen;void InformOfEvent(const NHeroStatModifierEvent::CBase& b){Seen.push_back(static_cast<const NHeroStatModifierEvent::CHitThing&>(b));Record("stat");}};
enum EHeroAbility{HERO_ABILITY_BERSERK=8};
namespace NEntityEvents {class CEventBase {public:int Kind;explicit CEventBase(int n):Kind(n){}virtual ~CEventBase(){}};class CEventHitBy:public CEventBase {public:CEventHitBy(const CHitParameters&):CEventBase(1){}};class CEventHitBySpecialAbility:public CEventBase{public:CEventHitBySpecialAbility(const CHitParameters&,EHeroAbility a):CEventBase(2){CHECK(a==8);}};}
TEST_TC(CTCEntityEvents,TCI_ENTITY_EVENTS)std::vector<int> Seen;void AddEvent(NEntityEvents::CEventBase* e){Seen.push_back(e->Kind);delete e;Record("entity-event");}};
TEST_TC(CTCSoundPlayer,TCI_SOUND_PLAYER)const CMaterialDef* Mine;const CMaterialDef* Other;CTCSoundPlayer():Mine(0),Other(0){}void PlayCollisionSound(const C3DVector&,const CDefPointer<const CMaterialDef>& m,const CDefPointer<const CMaterialDef>& o){Mine=m.Get();Other=o.Get();Record("sound");}};
class CPlayerGui {public:static CPlayerGui Instance;float Damage;bool Main;CPlayerGui():Damage(0),Main(false){}static CPlayerGui& Get(){return Instance;}void AddDamageChange(float d,bool main){Damage=d;Main=main;Record("display-damage");}};CPlayerGui CPlayerGui::Instance;
class CJoystick {public:class CInitRumble {public:float MaxLevel,Smoothness,AttackTime,DecayTime;bool Cumulative;CInitRumble(float a,float b,float c,float d,bool e):MaxLevel(a),Smoothness(b),AttackTime(c),DecayTime(d),Cumulative(e){}};
 float Level;CJoystick():Level(0){}void AddRumble(long n,const CInitRumble& r,bool replace){CHECK(n==3&&replace&&r.Smoothness==0.5f&&r.AttackTime==0&&!r.Cumulative);Level=r.MaxLevel;Record("rumble");}
};
class CInputManager {public:CJoystick Joystick;CJoystick& GetJoystick(long n){CHECK(n==0);return Joystick;}};static CInputManager Inputs;static CInputManager* GPInputManager=&Inputs;
class CEnginePrimitiveHandle {};
enum EEnginePrimitiveBlendMode{ENGINE_PRIMITIVE_BLEND_SOLID=0};enum EEngineInterpolationSet{ENGINE_INTERPOLATION_SET_DEFAULT=0};enum EEnginePrimitiveBlendOp{ENGINE_PRIMITIVE_BLEND_OP_ADD=0};
class CEnginePrimitiveLine {public:C3DVector Start,End;CRGBColour Colour;CEnginePrimitiveLine(unsigned long,const C3DVector& a,const C3DVector& b,const CRGBColour& c,float width,EEnginePrimitiveBlendMode,unsigned long,bool depth,EEngineInterpolationSet,EEnginePrimitiveBlendOp):Start(a),End(b),Colour(c){CHECK(width>0&&depth);}};
class CIEngine {public:std::vector<CEnginePrimitiveLine> Lines;void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitiveLine& p,unsigned long n){CHECK(n==0);Lines.push_back(p);Record("debug-line");}};
class CDisplayEngine {public:CIEngine Engine;CIEngine& Get3DEngine(){return Engine;}};
class CMainGameComponent {public:CDisplayEngine Display;static CMainGameComponent Instance;static CMainGameComponent* Get(){return &Instance;}CDisplayEngine& GetDisplayEngine(){return Display;}};CMainGameComponent CMainGameComponent::Instance;
static CEnginePrimitiveHandle hit_pos,hit_dir;
static const float DELTA_NOTIONAL_ZERO=0.0009765625f;
namespace NThingHelpers {static float LastDamage;static int Effects;void CreateEffectOnHit(const CThing&,const CHitParameters&,const CWorldMap&,float d){LastDamage=d;++Effects;Record("effect");}}

void CThingGameObject::OnHit(CHitParameters& hit_params)
{
    // Debug: a red line up from the hit_params pos, a green one along the hit_params dir.
    if (GetThingManager().IsDrawCombatCollisionDebug() && hit_params.IsLocalised()) {
        C3DVector pos = hit_params.GetHitPosition();
        {
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(hit_pos,
                CEnginePrimitiveLine(0, pos, pos + C3DVector(0.0f, 0.0f, 0.1f), CRGBColour(255, 0, 0),
                                     0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD), 0);
        }
        if (hit_params.IsDirectional()) {
            C3DVector dir = hit_params.GetHitDirection();
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(hit_dir,
                CEnginePrimitiveLine(0, pos, dir + pos, CRGBColour(0, 255, 0),
                                     0.05f, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD), 0);
        }
    }
    RemoveTC(TCI_FLOURISH_TARGET);
    CThing* pweapon = hit_params.GetPWeapon();
    CThing* phitter = hit_params.GetPThingHitting();
    NHitLocations::CHitLocation* phit_location = hit_params.GetPHitLocation();
    CTCHitLocations* ptc_hit_locations;
    if (!phit_location) {
        if (!GetTC(&ptc_hit_locations))
            return;
        phit_location = ptc_hit_locations->GetPDefaultHitLocation();
        if (!phit_location)
            return;
        hit_params.SetHitLocation(*phit_location);
    }
    hit_params.SetHitLocation(*phit_location);
    CDefPointer<const CMaterialDef> pmat_def = PeekPMaterial();
    if (GetTypeFlags() & 8) {
        long mat_def = phit_location->GetArmourDef()->DamageMaterial;
        if (mat_def > 0)
            PeekDefinitionManager().GetDef(mat_def, &pmat_def);
    }
    hit_params.SetPHitMaterialDef(pmat_def);
    CTCPerceiveThing* ptc_perceive;
    if (IS_THING_ALIVE(phitter) && GetTC(&ptc_perceive))
        ptc_perceive->OnHit(*phitter);
    if (hit_params.IsMeleeStrike()) {
        if (IS_THING_ALIVE(phitter)) {
            if (phitter->GetTypeFlags() & 8) {
                CThingCreatureBase& attacker_creature = *static_cast<CThingCreatureBase*>(phitter);
                attacker_creature.MakeNoise(CCharString("SIMPLE_NOISE", -1));
                CTCElectrocuteOnTouch* ptc_electrocute;
                if (GetTC(&ptc_electrocute)) {
                    ptc_electrocute->OnHit(*phitter);
                    hit_params.SetHitNegated(true);
                    return;
                }
            }
            if (IS_THING_ALIVE(phitter)) {
                const CTCHeroExperience* ptc_hero_experience;
                if ((HasTC(TCI_DOOR) || HasTC<CTCActionUseWindow>()) && phitter->PeekTC(&ptc_hero_experience) && !ptc_hero_experience->IsAbleToBreakDownDoors())
                    hit_params.SetHitNegated(true);
            }
        }
        if (IS_THING_ALIVE(pweapon)) {
            CTCWeapon* ptc_weapon;
            if (IS_THING_ALIVE(phitter) && pweapon->GetTC(&ptc_weapon)) {
                long explosion_index = ptc_weapon->GetExplosionType();
                if (explosion_index > 0) {
                    CThing* pexplosion = CTCDExplosion::Create(explosion_index, phitter->GetPos());
                    if (IS_THING_ALIVE(pexplosion)) {
                        CTCDExplosion* ptcd_explosion;
                        pexplosion->GetTCDriver(&ptcd_explosion);
                        ptcd_explosion->SetSpecificThingToHit(*this);
                    }
                }
            }
            CTCFlammable* ptc_my_flammable;
            CTCFlammable* ptc_weapon_flammable;
            if (IS_THING_ALIVE(pweapon) && GetTC(&ptc_my_flammable) && pweapon->GetTC(&ptc_weapon_flammable)) {
                if (ptc_my_flammable->IsOnFire() && !ptc_weapon_flammable->IsOnFire())
                    ptc_weapon_flammable->SetOnFire();
                else if (!ptc_my_flammable->IsOnFire() && ptc_weapon_flammable->IsOnFire())
                    ptc_my_flammable->SetOnFire();
            }
        }
    }
    if (!HasTC(TCI_BATTLE_CHARGE) && !HasTC(TCI_ASSASSIN_RUSH)) {
    CTCBerserk* ptc_berserk;
    if (GetTC(&ptc_berserk))
        ptc_berserk->IncreaseBerserkDamageToMeMultiplier();
    CTCCombat* ptc_combat = 0;
    if (GetTC(&ptc_combat))
        ptc_combat->SetStruck(*phit_location, hit_params);
    float damage_absorbed = 0.0f;
    if (hit_params.IsBlocked()) {
        CTCHeroExperience* ptc_hero_experience;
        if (GetTC(&ptc_hero_experience))
            ptc_hero_experience->AddSuccessfulBlockParry();
    } else {
        if (IsToNegateAllHits()) {
            hit_params.SetHitNegated(true);
            if (GetTypeFlags() & 8) {
                long mat_def = phit_location->GetArmourDef()->BlockMaterial;
                if (mat_def > 0) {
                    CDefPointer<const CMaterialDef> pmaterial_def;
                    PeekDefinitionManager().GetDef(mat_def, &pmaterial_def);
                    hit_params.SetPHitMaterialDef(pmaterial_def);
                }
            }
        }
        if (ptc_combat && ptc_combat->IsBlocking())
            ptc_combat->UnsetBlocking();
        CTCEmoteIcon* ptc_emote;
        if (GetTC(&ptc_emote))
            ptc_emote->SetEmoteIcon(CTCEmoteIcon::EMOTE_PRIORITY_NONE, 0, 0.0f, 1.0f, 0xFF);
        if (!phit_location->IsToPlayGetHitResponse())
            hit_params.SetPlayHitResponse(false);
        CDefPointer<const CArmourDef> armour_def = phit_location->GetArmourDef();
        hit_params.SetAsModifiedByArmour(*this, armour_def);
        if (hit_params.IsHitNegated()) {
            hit_params.SetPlayHitResponse(false);
            hit_params.SetToCauseRecoil(true);
        }
        damage_absorbed = hit_params.GetDamageToThing(*this);
        {
        const CHitParameters& nonse_params = hit_params;
        const CHitParameters& hit_params = nonse_params;
        if (!hit_params.IsHitNegated()) {
            CTCMultiStrike* ptc_multistrike;
            if (hit_params.IsMeleeStrike() && ((GetTypeFlags() & 8) || IsDamageable()) && IS_THING_ALIVE(pweapon) && pweapon != phitter
                && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_multistrike) && !ptc_multistrike->IsStrikeStarted())
                ptc_multistrike->StartMultiHit(hit_params, this);
            bool allowed_to_kill = true;
            CTCGuard* ptc_guard;
            const CTCHero* ptc_hero;
            if (GetType() == 2 && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_guard)) {
                if (ptc_guard->IsAttackingToFineRatherThanKill())
                    allowed_to_kill = false;
            } else if ((GetTypeFlags() & 8) && IS_THING_ALIVE(phitter) && phitter->PeekTC(&ptc_hero) && !ptc_hero->IsWeaponUseAllowed())
                allowed_to_kill = false;
            CTCOwnedEntity* tc_owned_entity;
            if (HasTC(TCI_DOOR) && GetTC(&tc_owned_entity)) {
                CThing* powner = tc_owned_entity->GetPOwner();
                CTCVillageMember* tc_village_member;
                if (powner && powner->GetTC(&tc_village_member)) {
                    CTCVillage* ptc_village = tc_village_member->GetPVillage();
                    if (ptc_village && ptc_village->GetVillageLimbo())
                        allowed_to_kill = false;
                }
            }
            bool already_dead = GetHealth() < DELTA_NOTIONAL_ZERO;
            CTCAbsorbHits* ptc_absorb_hits;
            if (!GetTC(&ptc_absorb_hits) || ptc_absorb_hits->DecrementHitsRemaining() < 0)
                ModifyHealth(damage_absorbed, allowed_to_kill);
            if (!already_dead && GetHealth() < DELTA_NOTIONAL_ZERO) {
                CThing* pweapon_here = pweapon;
                const NThingShot::CTCShotDefinitionBase* ptc_shot_base;
                if (IS_THING_ALIVE(pweapon_here) && pweapon_here->PeekTC(&ptc_shot_base))
                    pweapon_here = ptc_shot_base->GetPWeapon();
                CTCWeapon* ptc_weapon;
                if (IS_THING_ALIVE(pweapon_here) && pweapon_here->GetTC(&ptc_weapon))
                    ptc_weapon->IncrementNumThingsKilled(this);
            }
            if (GetTypeFlags() & 8) {
                CThingCreatureBase& creature = *static_cast<CThingCreatureBase*>(this);
                if (creature.IsInMeleeAsAttacker())
                    creature.GetCombatManager().DeregisterCombatant(creature);
            }
            if ((GetTypeFlags() & 8) || HasTC(TCI_CREATURE_GENERATOR)) {
                float display_damage = -damage_absorbed;
                if (IsDamageable() && display_damage > DELTA_NOTIONAL_ZERO) {
                    if (GetPlayerManager().IsThingMainPlayer(*this))
                        CPlayerGui::Get().AddDamageChange(display_damage, true);
                    else if (IS_THING_ALIVE(phitter) && GetPlayerManager().IsThingMainPlayer(*phitter))
                        CPlayerGui::Get().AddDamageChange(display_damage, false);
                }
            }
            if (damage_absorbed < 0.0f) {
                CTCHeroExperience* ptc_hero_experience;
                if (GetTC(&ptc_hero_experience))
                    ptc_hero_experience->SetAsHit();
                if (hit_params.IsLocalised()) {
                    CTCWound* ptc_wound;
                    if (GetTC(&ptc_wound))
                        ptc_wound->AddPossibleWoundGivenStrikePoint(hit_params.GetHitPosition());
                    CTCDegradableBase* tc_degradable;
                    if (hit_params.IsDirectional() && GetTC(&tc_degradable))
                        tc_degradable->OnHit(hit_params.GetHitPosition(), hit_params.GetHitDirection());
                }
            }
        } else {
            CTCHeroOpinionDeedLog* ptc_deed_log;
            if (hit_params.IsMeleeStrike() && IS_THING_ALIVE(phitter) && !(GetTypeFlags() & 8) && phitter->GetTC(&ptc_deed_log))
                ptc_deed_log->PostDeed((EOpinionDeedType)0x49, GetPos(), this, 0, (EOpinionPostDeedType)2,
                                false, false, 0, false, 0);
        }
        if (IS_THING_ALIVE(phitter) && ptc_combat)
            ptc_combat->OnHit(*phitter);
        phit_location->ProcessHitLocationCode(*this, hit_params);
        }
    }
    {
    const CHitParameters& nonse_params = hit_params;
    const CHitParameters& hit_params = nonse_params;
    CTCHeroStats* ptc_stats;
    if (!hit_params.IsHitNegated() && !hit_params.IsBlocked() && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_stats) && IsGivingHeroStatChangesOnBeingHit()) {
        CDefPointer<const CCreatureDef> pcreature_def;
        if (GetPBaseDef()->GetSubDef(&pcreature_def)) {
            bool unarmed = true;
            if (IS_THING_ALIVE(pweapon) && pweapon != phitter)
                unarmed = false;
            CDefPointer<const CHeroExperienceDef> phero_exp_def;
            if (phitter->GetPBaseDef()->GetSubDef(&phero_exp_def)) {
                for (std::map<EDamageAttribute,float>::const_iterator it = hit_params.GetDamageMap().begin(); it != hit_params.GetDamageMap().end(); ++it) {
                    const EDamageAttribute att = it->first;
                    std::map<EDamageAttribute,CHeroStatIncreaseData>::const_iterator it_lookup = phero_exp_def->StatIncreasePerHitType.find(att);
                    if (it_lookup != phero_exp_def->StatIncreasePerHitType.end()) {
                        for (std::map<EHeroExperienceStatCategory,float>::const_iterator it2 = it_lookup->second.StatIncreases.begin();
                             it2 != it_lookup->second.StatIncreases.end(); ++it2) {
                            NHeroStatModifierEvent::CHitThing hit_event(pcreature_def->GetStatsDef());
                            hit_event.SetAsUnarmed(unarmed);
                            hit_event.SetStatToIncrement(it2->first);
                            hit_event.SetStatIncrementAmount(it2->second);
                            bool summoned_creature = false;
                            CTCSummonableCreature* ptc_summonable;
                            if (GetTC(&ptc_summonable) && ptc_summonable->GetHasBeenSummoned())
                                summoned_creature = true;
                            bool ghost_sword = CTCGhostSword::IsThingGhostSword(this);
                            if (!summoned_creature && !ghost_sword)
                                ptc_stats->InformOfEvent(hit_event);
                        }
                    }
                }
            }
        }
    }
    CTCHeroExperience* ptc_hero_experience;
    if (!hit_params.IsHitNegated() && !hit_params.IsBlocked() && IS_THING_ALIVE(phitter) && phitter->GetTC(&ptc_hero_experience) && IsAddToComboMultiplierOnHit())
        ptc_hero_experience->AddSuccessfulStrikeWithHitParams(hit_params);
    CTCEntityEvents* ptc_entity_events;
    if (GetTC(&ptc_entity_events) && IS_THING_ALIVE(phitter)) {
        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBy(hit_params));
        if (phitter->HasTC(TCI_BERSERK))
            ptc_entity_events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit_params, HERO_ABILITY_BERSERK));
    }
    if (hit_params.IsToCauseRecoil() && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 8)
        && !(IS_THING_ALIVE(pweapon) && pweapon->HasTC(TCI_SHOT_DEFINITION)))
    {
        CThingCreatureBase& attacker_creature = *static_cast<CThingCreatureBase*>(phitter);
        attacker_creature.PerformBlockedReaction(0, true);
    }
    if (hit_params.IsMeleeStrike()) {
        CDefPointer<const CMaterialDef> pmaterial = hit_params.GetPHitMaterialDef();
        if (pmaterial.Get() && pmaterial->OnHitRumbleLevel != 0.0f && IS_THING_ALIVE(phitter) && phitter->GetType() == 2
            && phitter->PeekPlayer().GetNumber() == PeekPlayerManager().GetMainPlayer().GetNumber())
            GPInputManager->GetJoystick(phitter->GetJoystickDeviceNumber()).AddRumble(3,
                CJoystick::CInitRumble(pmaterial->OnHitRumbleLevel, 0.5f, 0.0f, pmaterial->OnHitRumbleFalloff, false), true);
    }
    CTCSoundPlayer* ptc_sound;
    if (GetTC(&ptc_sound) && hit_params.IsLocalised()) {
        CDefPointer<const CMaterialDef> phitting_material;
        if (IS_THING_ALIVE(pweapon) && (pweapon->GetTypeFlags() & 4))
            phitting_material = static_cast<CThingPhysical*>(pweapon)->PeekPMaterial();
        if (!phitting_material.Get() && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 4)) {
            phitting_material = static_cast<CThingPhysical*>(phitter)->PeekPMaterial();
            if (hit_params.IsLightningStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_LIGHTNING", -1), &phitting_material);
            else if (hit_params.IsDivineWrathStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_DIVINE_WRATH", -1), &phitting_material);
            else if (hit_params.IsUnholyPowerStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_UNHOLY_POWER", -1), &phitting_material);
            else {
                CTCHitLocations* ptc_hit_locations;
                if ((phitter->GetTypeFlags() & 8) && phitter->GetTC(&ptc_hit_locations))
                    PeekDefinitionManager().GetDef(ptc_hit_locations->GetPDefaultHitLocation()->GetArmourDef()->DamageMaterial, &phitting_material);
            }
        }
        CDefPointer<const CMaterialDef> phit_material = hit_params.GetPHitMaterialDef();
        if (phit_material.Get())
            ptc_sound->PlayCollisionSound(hit_params.GetHitPosition(), phit_material, phitting_material);
    }
    NThingHelpers::CreateEffectOnHit(*this, hit_params, PeekWorldMap(), damage_absorbed);
    }
    return;
    }
    hit_params.SetPlayHitResponse(false);
}

class Scene {public:CThingCreatureBase Target,Hitter;CThingGameObject Weapon;CMaterialDef Material,WeaponMaterial,HitterMaterial;CArmourDef Armour;NHitLocations::CHitLocation Location;CHitParameters Hit;CTCHitLocations Locations;
 Scene(){Trace.clear();Manager.Draw=false;Definitions.Materials.clear();Definitions.Named.clear();Players.Main=0;Players.Player.Number=0;Inputs.Joystick.Level=0;CPlayerGui::Instance.Damage=0;CPlayerGui::Instance.Main=false;CMainGameComponent::Instance.Display.Engine.Lines.clear();NThingHelpers::Effects=0;NThingHelpers::LastDamage=0;CTCDExplosion::Created=0;CTCGhostSword::Ghost=0;Target.Flags=4;Target.Material=&Material;Weapon.Material=&WeaponMaterial;Hitter.Material=&HitterMaterial;Location.Armour.Data=&Armour;Locations.Location=&Location;Hit.Location=&Location;Hit.Hitter=&Hitter;Hit.Weapon=&Weapon;}
 void Run(){Target.OnHit(Hit);}
};
int main(){int scenarios=0;
 {Scene s;s.Run();CHECK(s.Target.Health==12&&Count("modify-health")==1&&Count("hit-location")==1);CHECK(NThingHelpers::Effects==1&&NThingHelpers::LastDamage==-4);++scenarios;}
 {Scene s;s.Hit.Location=0;s.Run();CHECK(Count("default-location")==0&&Count("peek-material")==0&&NThingHelpers::Effects==0);++scenarios;}
 {Scene s;s.Hit.Location=0;s.Target.Attach(s.Locations);s.Run();CHECK(s.Hit.Location==&s.Location&&Count("set-location")==2&&NThingHelpers::Effects==1);++scenarios;}
 {Scene s;s.Hit.Location=0;s.Locations.Location=0;s.Target.Attach(s.Locations);s.Run();CHECK(Count("default-location")==1&&Count("peek-material")==0);++scenarios;}
 {Scene s;CTCHeroExperience exp;s.Target.Attach(exp);s.Hit.Blocked=true;s.Run();CHECK(s.Target.Health==16&&Count("parry")==1&&Count("armour")==0&&NThingHelpers::LastDamage==0);++scenarios;}
 {Scene s;s.Hit.Negated=true;s.Run();CHECK(s.Target.Health==16&&s.Hit.Recoil&&!s.Hit.Response&&Count("hit-location")==1);++scenarios;}
 {Scene s;CTCBase marker;s.Target.Components[TCI_BATTLE_CHARGE]=&marker;s.Run();CHECK(!s.Hit.Response&&Count("armour")==0&&NThingHelpers::Effects==0);++scenarios;}
 {Scene s;CTCBase marker;s.Target.Components[TCI_ASSASSIN_RUSH]=&marker;s.Run();CHECK(!s.Hit.Response&&Count("armour")==0&&NThingHelpers::Effects==0);++scenarios;}
 {Scene s;CTCElectrocuteOnTouch x;s.Target.Attach(x);s.Hit.Melee=true;s.Run();CHECK(s.Hit.Negated&&Count("electrocute")==1&&Count("noise")==1&&NThingHelpers::Effects==0);++scenarios;}
 {Scene s;CTCPerceiveThing x;s.Target.Attach(x);s.Run();CHECK(Count("perceive")==1);++scenarios;}
 {Scene s;CTCPerceiveThing x;s.Target.Attach(x);s.Hitter.Dead=true;s.Run();CHECK(Count("perceive")==0);++scenarios;}
 {Scene s;CTCHeroExperience exp;exp.Break=false;s.Hitter.Attach(exp);CTCBase marker;s.Target.Components[TCI_DOOR]=&marker;s.Hit.Melee=true;s.Run();CHECK(s.Hit.Negated&&s.Target.Health==16);++scenarios;}
 {Scene s;CTCHeroExperience exp;exp.Break=false;s.Hitter.Attach(exp);CTCActionUseWindow window;s.Target.Attach(window);s.Hit.Melee=true;s.Run();CHECK(s.Hit.Negated&&s.Target.Health==16);++scenarios;}
 {Scene s;CTCHeroExperience exp;exp.Break=false;s.Hitter.Attach(exp);CTCBase other;s.Target.Components[TCI_ON_ACTION_USE]=&other;s.Hit.Melee=true;s.Run();CHECK(!s.Hit.Negated&&s.Target.Health==12);++scenarios;}
 {Scene s;CTCWeapon w;w.Explosion=7;s.Weapon.Attach(w);CThingGameObject explosion;CTCDExplosion driver;explosion.Attach(driver);CTCDExplosion::Created=&explosion;s.Hit.Melee=true;s.Run();CHECK(Count("explosion")==1&&Count("explosion-target")==1);++scenarios;}
 {Scene s;CTCFlammable a,b;a.Fire=true;s.Target.Attach(a);s.Weapon.Attach(b);s.Hit.Melee=true;s.Run();CHECK(b.Fire&&Count("set-fire")==1);++scenarios;}
 {Scene s;CTCFlammable a,b;b.Fire=true;s.Target.Attach(a);s.Weapon.Attach(b);s.Hit.Melee=true;s.Run();CHECK(a.Fire&&Count("set-fire")==1);++scenarios;}
 {Scene s;CTCBerserk x;s.Target.Attach(x);s.Run();CHECK(Count("berserk")==1);++scenarios;}
 {Scene s;CTCCombat c;c.Blocking=true;s.Target.Attach(c);s.Run();CHECK(!c.Blocking&&Count("struck")==1&&Count("unblock")==1&&Count("combat-hit")==1);++scenarios;}
 {Scene s;CTCEmoteIcon e;s.Target.Attach(e);s.Location.Response=false;s.Run();CHECK(Count("emote")==1&&!s.Hit.Response);++scenarios;}
 {Scene s;CMaterialDef replacement;s.Target.Flags=12;s.Armour.DamageMaterial=7;Definitions.Materials[7]=&replacement;s.Run();CHECK(s.Hit.Material.Get()==&replacement&&Count("material-index")==1);++scenarios;}
 {Scene s;CMaterialDef damage,block;s.Target.Flags=12;s.Target.Negate=true;s.Armour.DamageMaterial=7;s.Armour.BlockMaterial=9;Definitions.Materials[7]=&damage;Definitions.Materials[9]=&block;s.Run();CHECK(s.Hit.Material.Get()==&block&&s.Hit.Negated&&Count("material-index")==2);++scenarios;}
 {Scene s;CTCMultiStrike multi;s.Hitter.Attach(multi);s.Hit.Melee=true;s.Run();CHECK(multi.Started&&Count("multistrike")==1);++scenarios;}
 {Scene s;CTCGuard g;g.Fine=true;s.Hitter.Attach(g);s.Target.Type=2;s.Hit.Damage=-32;s.Run();CHECK(!s.Target.LastAllow&&s.Target.Health==1);++scenarios;}
 {Scene s;CTCHero hero;hero.Weapons=false;s.Hitter.Attach(hero);s.Target.Flags=12;s.Hit.Damage=-32;s.Run();CHECK(!s.Target.LastAllow&&s.Target.Health==1);++scenarios;}
 {Scene s;CTCBase door;CTCOwnedEntity owned;CTCVillageMember member;CTCVillage village;CThing owner;village.Limbo=true;member.Village=&village;owner.Attach(member);owned.Owner=&owner;s.Target.Attach(owned);s.Target.Components[TCI_DOOR]=&door;s.Hit.Damage=-32;s.Run();CHECK(!s.Target.LastAllow&&s.Target.Health==1);++scenarios;}
 {Scene s;CTCAbsorbHits a;a.Remaining=1;s.Target.Attach(a);s.Run();CHECK(s.Target.Health==16&&a.Remaining==0&&Count("modify-health")==0);++scenarios;}
 {Scene s;CTCAbsorbHits a;a.Remaining=0;s.Target.Attach(a);s.Run();CHECK(s.Target.Health==12&&a.Remaining==-1&&Count("modify-health")==1);++scenarios;}
 {Scene s;CTCWeapon w;s.Weapon.Attach(w);s.Target.Health=2;s.Run();CHECK(s.Target.Health==-2&&Count("kill-credit")==1);++scenarios;}
 {Scene s;CTCWeapon w;s.Weapon.Attach(w);s.Target.Health=-1;s.Run();CHECK(Count("kill-credit")==0);++scenarios;}
 {Scene s;CThingGameObject firingWeapon;CTCWeapon w;firingWeapon.Attach(w);NThingShot::CTCShotDefinitionBase shot;shot.Weapon=&firingWeapon;s.Weapon.Attach(shot);s.Target.Health=2;s.Run();CHECK(Count("kill-credit")==1);++scenarios;}
 {Scene s;s.Target.Flags=12;s.Target.InMelee=true;s.Run();CHECK(Count("deregister-combatant")==1);++scenarios;}
 {Scene s;s.Target.Flags=12;Players.Main=&s.Target;s.Run();CHECK(CPlayerGui::Instance.Damage==4&&CPlayerGui::Instance.Main);++scenarios;}
 {Scene s;s.Target.Flags=12;Players.Main=&s.Hitter;s.Run();CHECK(CPlayerGui::Instance.Damage==4&&!CPlayerGui::Instance.Main);++scenarios;}
 {Scene s;CTCHeroExperience exp;CTCWound wound;CTCDegradableBase d;s.Target.Attach(exp);s.Target.Attach(wound);s.Target.Attach(d);s.Hit.Local=true;s.Hit.Directional=true;s.Run();CHECK(Count("hero-hit")==1&&Count("wound")==1&&Count("degrade")==1);++scenarios;}
 {Scene s;CTCHeroOpinionDeedLog log;s.Hitter.Attach(log);s.Hit.Melee=true;s.Hit.Negated=true;s.Run();CHECK(Count("deed")==1&&Count("modify-health")==0);++scenarios;}
 {Scene s;CTCHeroStats stats;CCreatureDef creature;CHeroExperienceDef hero;s.Hitter.Attach(stats);s.Target.GiveStats=true;s.Target.Base.Sub[CCreatureDef::Kind]=&creature;s.Hitter.Base.Sub[CHeroExperienceDef::Kind]=&hero;hero.StatIncreasePerHitType[DAMAGE_MELEE].StatIncreases[HERO_STAT_STRENGTH]=0.5f;hero.StatIncreasePerHitType[DAMAGE_MELEE].StatIncreases[HERO_STAT_SKILL]=0.25f;s.Hit.DamageMap[DAMAGE_MELEE]=4;s.Run();CHECK(stats.Seen.size()==2&&!stats.Seen[0].Unarmed&&stats.Seen[0].Amount==0.5f&&stats.Seen[1].Amount==0.25f);++scenarios;}
 {Scene s;CTCHeroStats stats;CCreatureDef creature;CHeroExperienceDef hero;CTCSummonableCreature summon;summon.Summoned=true;s.Target.Attach(summon);s.Hitter.Attach(stats);s.Target.GiveStats=true;s.Target.Base.Sub[CCreatureDef::Kind]=&creature;s.Hitter.Base.Sub[CHeroExperienceDef::Kind]=&hero;hero.StatIncreasePerHitType[DAMAGE_MELEE].StatIncreases[HERO_STAT_WILL]=1;s.Hit.DamageMap[DAMAGE_MELEE]=4;s.Run();CHECK(stats.Seen.empty());++scenarios;}
 {Scene s;CTCHeroExperience exp;s.Hitter.Attach(exp);s.Target.AddCombo=true;s.Run();CHECK(Count("combo")==1);++scenarios;}
 {Scene s;CTCEntityEvents events;CTCBerserk berserk;s.Target.Attach(events);s.Hitter.Attach(berserk);s.Run();CHECK(events.Seen.size()==2&&events.Seen[0]==1&&events.Seen[1]==2);++scenarios;}
 {Scene s;s.Hit.Recoil=true;s.Run();CHECK(Count("recoil")==1);++scenarios;}
 {Scene s;NThingShot::CTCShotDefinitionBase shot;s.Weapon.Attach(shot);s.Hit.Recoil=true;s.Run();CHECK(Count("recoil")==0);++scenarios;}
 {Scene s;s.Hitter.Type=2;s.Hit.Melee=true;s.Material.OnHitRumbleLevel=0.5f;s.Run();CHECK(Count("rumble")==1&&Inputs.Joystick.Level==0.5f);++scenarios;}
 {Scene s;CTCSoundPlayer sound;s.Target.Attach(sound);s.Hit.Local=true;s.Run();CHECK(sound.Mine==&s.Material&&sound.Other==&s.WeaponMaterial&&Count("sound")==1);++scenarios;}
 {Scene s;CTCSoundPlayer sound;CMaterialDef lightning;s.Target.Attach(sound);s.Hit.Local=true;s.Hit.Weapon=0;s.Hit.Lightning=true;Definitions.Named["MATERIAL_LIGHTNING"]=&lightning;s.Run();CHECK(sound.Other==&lightning&&Count("MATERIAL_LIGHTNING")==1);++scenarios;}
 {Scene s;CTCSoundPlayer sound;CMaterialDef divine;s.Target.Attach(sound);s.Hit.Local=true;s.Hit.Weapon=0;s.Hit.Divine=true;Definitions.Named["MATERIAL_DIVINE_WRATH"]=&divine;s.Run();CHECK(sound.Other==&divine);++scenarios;}
 {Scene s;CTCSoundPlayer sound;CMaterialDef unholy;s.Target.Attach(sound);s.Hit.Local=true;s.Hit.Weapon=0;s.Hit.Unholy=true;Definitions.Named["MATERIAL_UNHOLY_POWER"]=&unholy;s.Run();CHECK(sound.Other==&unholy);++scenarios;}
 {Scene s;CTCSoundPlayer sound;CMaterialDef armourMaterial;CTCHitLocations locations;NHitLocations::CHitLocation location;CArmourDef armour;armour.DamageMaterial=7;location.Armour.Data=&armour;locations.Location=&location;s.Hitter.Attach(locations);s.Target.Attach(sound);s.Hit.Local=true;s.Hit.Weapon=0;Definitions.Materials[7]=&armourMaterial;s.Run();CHECK(sound.Other==&armourMaterial);++scenarios;}
 {Scene s;Manager.Draw=true;s.Hit.Local=true;s.Hit.Directional=true;s.Run();const std::vector<CEnginePrimitiveLine>& lines=CMainGameComponent::Instance.Display.Engine.Lines;CHECK(lines.size()==2&&lines[0].Start.X==2&&lines[0].Colour.R==255&&lines[1].Colour.G==255);CHECK(lines[1].End.X==3&&lines[1].End.Y==6&&lines[1].End.Z==12);++scenarios;}
 CHECK(scenarios==49);std::puts("00839940_TEST PASS");return 0;
}
