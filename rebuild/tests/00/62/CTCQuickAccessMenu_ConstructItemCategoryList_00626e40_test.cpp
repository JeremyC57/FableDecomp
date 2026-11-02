#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __forceinline inline
#define __declspec(x)
typedef unsigned long FixtureUInt64;typedef long FixtureInt64;
#else
typedef unsigned __int64 FixtureUInt64;typedef __int64 FixtureInt64;
#endif
#define FABLE_STATIC_ASSERT(x)
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
static const unsigned short fixture_decimal_format[]={37,100,0};

class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
};



#include <stddef.h>

class CCharString;
class CWideStringData;
class CWideString {
public:
    CWideString();
    CWideString(const CWideString&);
    const CWideString& operator+=(const CWideString&);
    void __cdecl Sprintf(const unsigned short*,...);
    ~CWideString();
protected:
    CWideStringData* PStringData;
};










































class CCPPointerInfo {
public:
    unsigned int RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned int refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned int GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

















#include <stddef.h>











class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};


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


class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, int length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};


class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    inline void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    inline const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    inline const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    inline const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    inline bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    inline float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    inline float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    inline const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    inline const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef inline
class CThing;
class CEnginePrimitivePrepareInfo;class CWorld;
class CInterfaceType;
class CPersistContext;
class CTCParamsBase;
namespace NTCMessages { class CMessageBase; }
class CTCBase : public CBaseClassNonCopyable {
public:
    CTCBase(CThing&);
protected:
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
protected:
protected:
    CThing& ParentThing;
private:
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};













#include <stddef.h>

















class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};
class CCharString;
class CClassFactory;
class CClass : public CBaseObject {
public:
    virtual void Save(CCharString&);                                     
    void Unregister(CClassFactory&);                                     
    void PostConstructionInit(CClassFactory&);                           
    unsigned int GetType() const { return Type; }                       
    unsigned char ClassListPos[0x4];                                     
    unsigned char TypeListPos[0x4];                                      
    unsigned char Type;                                                  
};




























#include <stddef.h>







#include <stddef.h>















#include <stddef.h>
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                                 
    virtual void Delete() const;                                         
    void AddRef() const { ++IVRefCount; }                                
    int Release() const { return --IVRefCount; }                        
    int GetRefCount() const { return IVRefCount; }                      
    mutable unsigned int IVRefCount;                                    
};

class CResourceList;
class CResource : public CIVCountedPointeeBase {
public:
    CResource();                                                         
    virtual ~CResource();                                                
    virtual bool UnloadResource();                                       
    virtual unsigned int GetUnloadableMemory() const;                   
    virtual bool IsResourceLoaded() const;                               
    virtual void OnResourceLoaded();                                     
    virtual bool CanAbortResourceLoad(int) const;                       
    virtual void OnResourceLoadAborted();                                
    virtual unsigned int GetDebugMemoryInUse() const;                   
    virtual void ResetDebugMemoryInUse();                                
    CResourceList* ResourceList;                                         
    CResource*     PrevResource;                                         
    CResource*     NextResource;                                         
    unsigned int  ResourceSize;                                         
    unsigned int  LastUsedFrame;                                        
};


class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() { }                                       
};


enum ENullDef { NULL_DEF };                                          

template <class T> class CDefPointer {
public:
    inline CDefPointer(ENullDef = NULL_DEF) : Object(0) {}             
    inline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> inline CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    inline ~CDefPointer() { Release(); }
    inline CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> inline CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> inline void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned int GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      
    inline void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             
private:
    inline void Assign(CDefPointeeBase* object)                                 
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};




#include <stddef.h>
#include <stdlib.h>
#include <new>
#include <string.h>
namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template<class T> inline const T& (max)(const T& a,const T& b) { return a<b?b:a; }
template<class T> inline const T& (min)(const T& a,const T& b) { return b<a?b:a; }
template<class T,class U> inline void _Construct(T* p,const U& v) { new(p) T(v); }
template<class T> inline void _Destroy(T* p) { p->~T(); }
inline void _Destroy(char*,char*) { }
template<class I> inline void _Destroy(I first,I last) { for(;first!=last;++first) _Destroy(&*first); }
template<class I,class O> inline O __uninitialized_copy(I first,I last,O result,const __false_type&) {
    O cur=result;for(;first!=last;++first,++cur) _Construct(&*cur,*first);return cur;
}
template<class I,class N,class T> inline I __uninitialized_fill_n(I first,N n,const T& x,const __false_type&) {
    I cur=first;for(;n>0;--n,++cur) _Construct(&*cur,x);return cur;
}
template<class T> struct __type_traits { typedef __false_type has_trivial_assignment_operator; typedef __false_type has_trivial_destructor;
};
template<> struct __type_traits<char> { typedef __true_type has_trivial_assignment_operator; };
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
template<class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p,size_t) { if(p) free(p); }
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
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
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
    ~vector() { _Destroy(this->_M_start,this->_M_finish); }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _IsPODType;
    void push_back(const _Tp& x) {
        if(this->_M_finish!=this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish,x); ++this->_M_finish;
        } else _M_insert_overflow(this->_M_finish,x,_IsPODType(),1,true);
    }
    iterator erase(iterator first,iterator last) {
        iterator result=first;
        for(iterator source=last;source!=this->_M_finish;++source,++result) *result=*source;
        _Destroy(result,this->_M_finish);this->_M_finish=result;return first;
    }
    void clear() { erase(begin(),end()); }
    void resize(size_type n,_Tp x) {
        if(n<size()) erase(begin()+n,end()); else _M_fill_insert(end(),n-size(),x);
    }
    void resize(size_type n) { resize(n,_Tp()); }
protected:
    void _M_insert_overflow(iterator position,const _Tp& x,const __false_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);_Tp* newFinish=newStart;
        newFinish=__uninitialized_copy(this->_M_start,position,newStart,__false_type());
        if(fillLength==1) {_Construct(newFinish,x);++newFinish;}
        else newFinish=__uninitialized_fill_n(newFinish,fillLength,x,__false_type());
        if(!atEnd) newFinish=__uninitialized_copy(position,this->_M_finish,newFinish,__false_type());
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
    __declspec(noinline)
    void _M_insert_overflow(iterator position,const _Tp& x,const __true_type&,size_type fillLength,bool atEnd) {
        const size_type oldSize=size();const size_type len=oldSize+(max)(oldSize,fillLength);
        _Tp* newStart=this->_M_end_of_storage.allocate(len);
        _Tp* newFinish=(_Tp*)__copy_trivial(this->_M_start,position,newStart);
        newFinish=fill_n(newFinish,fillLength,x);
        if(!atEnd) newFinish=(_Tp*)__copy_trivial(position,this->_M_finish,newFinish);
        _M_clear();_M_set(newStart,newFinish,newStart+len);
    }
protected:
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
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair():first(),second(){}
    pair(const _T1& a,const _T2& b):first(a),second(b) { }
    pair(const pair& p):first(p.first),second(p.second) { }
    template<class U1,class U2> pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
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
    _List_iterator(_List_node<_Tp>* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    ~list();
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
    typedef _Tp value_type;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
    _Alloc get_allocator()const { return _Alloc(); }
    list(const list& x):_List_base<_Tp,_Alloc>(x.get_allocator()) { insert(begin(),x.begin(),x.end()); }
    template<class Input> void insert(iterator position,Input first,Input last) { _M_insert_dispatch(position,first,last,__false_type()); }
    template<class Input> void _M_insert_dispatch(iterator,Input,Input,const __false_type&);
protected:
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
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned int _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned int)-1)/(8*sizeof(unsigned int))> {
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


template<class T> class CArray : public std::vector<T,std::allocator<T> > {
};
template<class K,class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K,V>& a,const std::pair<K,V>& b) const { return a.first<b.first; }
};
template<class K,class V,class CompareType=CKeyPairCompareLess<K,V> >
class CVectorMap : public CArray<std::pair<K,V> > {
public:
    typedef std::pair<K,V> value_type;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    __declspec(noinline) iterator LowerBound(const K& key) {
        iterator first=this->begin();
        int count=this->end()-first;
        while(count>0) {
            int half=count/2;
            iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    __declspec(noinline) const_iterator LowerBound(const K& key) const {
        const_iterator first=this->begin();
        int count=this->end()-first;
        while(count>0) {
            int half=count/2;
            const_iterator middle=first+half;
            if(middle->first<key) { first=middle+1; count-=half+1; }
            else count=half;
        }
        return first;
    }
    iterator Find(const K& key) {
        iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
    const_iterator Find(const K& key) const {
        const_iterator i=LowerBound(key);
        return i==this->end() || key<i->first ? this->end() : i;
    }
private:
    CompareType Compare;
    bool Dirty;
};

class C3DVector; class CTCPhysicsBase; class CTCBase; class CTCDriverBase;
class CThingBaseDef; class CWorld; class CPlayerManager;class CTCParamsBase;
namespace NGraphicAppearance { class CTCGraphicAppearance; }


template<class From,class To> inline To __fastcall GFDebugPDynamicCast(From value,To) { return static_cast<To>(value); }
class CThing : public CClass {

protected:
    CThing(unsigned int);
    FixtureUInt64 UniqueID;
    std::bitset<274> UsingComponents;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> > Components;
    std::list<CTCBase*> UpdateComponentsList;
    CCharString SerialisationSectionName;
    CTCDriverBase* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    int WorldFrameCreatedAt;
    unsigned int ThingTypeFlags;
    CDefPointer<const CThingBaseDef> PBaseDef;
    CCharString ScriptName;
    CCharString ScriptData;
public:
    CThing* RenderListPrev;
    CThing* RenderListNext;
    CThing* UpdateListPrev;
    CThing* UpdateListNext;
protected:
    unsigned short DefGlobalIndex;
    short CurrentMapNumber;
    signed char PlayerNumber;

protected:
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

private:
    unsigned char InitialisationStage : 3;
public:
    void Kill(bool); 
    CTCBase* AddTC(const CCharString&,bool,const CTCParamsBase*);
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
    bool IsDead() const { return Dead!=0; }
    bool IsIncludedInThingScans() const { return IncludeInThingScans && Activated && !Dead && !InLimbo; }
    bool IsAlive() const { return !Dead; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> bool PeekTC(T** result) const {
        bool present=HasTC(T::StatGetInterfaceType());
        if(present) *result=GFDebugPDynamicCast(static_cast<const CTCBase*>(Components.Find(T::StatGetInterfaceType())->second),(T*)0);
        return present;
    }
    CTCBase& GetTC(ETCInterfaceType type) { return *Components.Find(type)->second; }
    const CTCBase& GetTC(ETCInterfaceType type) const { return *Components.Find(type)->second; }
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    static void CheckRetailLayout() {
    }
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const C3DVector& GetPos() const;
    C3DVector GetPhysPos() const;
protected:
    CWorld& GetWorld();
    const CWorld& PeekWorld() const;
    unsigned int* GetPWorldSeed() const;
    CPlayerManager& GetPlayerManager();
};



class CTargetingDef;class CThingCreatureBase;
enum ECreatureInteractionType{CREATURE_INTERACTION_NULL=0,CREATURE_INTERACTION_CONVERSATION=1,CREATURE_INTERACTION_TAG=2,CREATURE_INTERACTION_MULTI_TAG=4,CREATURE_INTERACTION_PURCHASING=8};


class CTCTargetingBase:public CTCBase{
public: CTCTargetingBase(CThing&);
public: static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)8;}
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual void OnInitialActivate();
public: virtual void BulletTimeFrameUpdate();
public: virtual CThing * GetPBestThingForMeleeAttack() const;
public: virtual CThingCreatureBase * GetPBestEnemyCreatureForAttack() const;
public: virtual CThing * GetPBestThingForProjectileTargetingLock() const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float) const;
public: virtual CThingCreatureBase * GetPBestCreatureForInteractionType(ECreatureInteractionType) const;
public: virtual CThing * GetPTargetedThing() const;
public: virtual void SetPTargetedThing(CThing *);
public: virtual CThing * GetPProjectileTargetedThing() const;
public: virtual CThing * GetPTargetLockedThing() const;
public: virtual bool GetTargetedPos(C3DVector &,float) const;
public: virtual bool IsTargetedThingInRange(unsigned int) const;
protected: virtual void UpdateNearestInterestingThing();
public: virtual ~CTCTargetingBase();
protected:bool ScriptOverrideIgnoreEverything;unsigned char PNearestInterestingThing[8],PScriptOverrideInterestingThing[8],PScriptOverrideRangedTarget[8],PTargetedThing[8];CDefPointer<const CTargetingDef> PTargetingDef;bool LookAtCamera;
};

namespace NUISystem {
enum EState {
    UI_STATE_INACTIVE=0,
    UI_STATE_ACTIVE=1,
    UI_STATE_DEAD=2,
    UI_STATE_SELECTED=3,
    UI_STATE_UNSELECTED=4,
    UI_STATE_LIST_ACTIVE=5,
    UI_STATE_LIST_INACTIVE=6,
    UI_STATE_LIST_UNSELECTED=7,
    UI_STATE_CUSTOM=8
};
enum EStateChangeType {
    STATE_CHANGE_SIMULTANEOUS=0,
    STATE_CHANGE_PARENT_FIRST=1,
    STATE_CHANGE_CHILDREN_FIRST=2,
    STATE_CHANGE_PARENT_ONLY=3,
    STATE_CHANGE_CHILDREN_ONLY=4
};
enum EType {
    UI_TYPE_SPRITE=0,
    UI_TYPE_MORPHING_SPRITE=1,
    UI_TYPE_TABLE=2,
    UI_TYPE_MESH=3,
    UI_TYPE_COMPOSITE=4,
    UI_TYPE_CHANGING_STATE_COMPOSITE=5,
    UI_TYPE_TEXT=6,
    UI_TYPE_MENU_ENTRY=7,
    UI_TYPE_LIST=8,
    UI_TYPE_VIEWPORT=9,
    UI_TYPE_FRONTEND_SCREEN=10,
    UI_TYPE_FRONTEND_BUTTON=11,
    UI_TYPE_FRONTEND_LIST=12,
    UI_TYPE_SCROLLING_VIEWPORT=13,
    UI_TYPE_LIST_ARROW=14,
    UI_TYPE_SLIDER=15,
    UI_TYPE_TEXT_SLIDER=16,
    UI_TYPE_MOVIE=17,
    UI_TYPE_SWAPPING_STATE_COMPOSITE=18,
    UI_TYPE_SCROLLING_COMPOSITE=19,
    UI_TYPE_TEXT_CONTAINER=20,
    UI_TYPE_ZOOMING_COMPOSITE=21,
    UI_TYPE_COMPONENT_CONTAINER=22,
    UI_TYPE_SPELL_CONTAINER=23,
    UI_TYPE_SPELL_CONTAINER_LIST=24,
    UI_TYPE_YESNO=25,
    UI_TYPE_OK=26,
    UI_TYPE_PARTICLE_EFFECT=27,
    UI_TYPE_CONTROLLERDISCONNECT=28,
    UI_TYPE_DIRTYDISC=29,
    UI_TYPE_ICON_TEXT=30,
    UI_TYPE_DYNAMIC_LIST=31,
    UI_TYPE_MOUSE_POINTER=32,
    UI_TYPE_HOVERABLE=33,
    UI_TYPE_CLICKABLE=34,
    UI_TYPE_DRAGGABLE=35,
    UI_TYPE_DRAGGABLE_INTO=36,
    UI_TYPE_EDIT_BOX=37,
    UI_TYPE_NAVIGATION_BUTTON=38,
    UI_TYPE_KEY_REDEFINER=39,
    UI_TYPE_REDEFINER_LIST=40,
    UI_TYPE_SCROLLBAR=41,
    UI_TYPE_SCROLLBAR_OUTSIDE=42,
    UI_TYPE_SCROLLABLE_LIST=43
};
enum EActionParamType {
    UI_ACTION_PARAM_TYPE_NONE=0,
    UI_ACTION_PARAM_TYPE_COMPONENT=1,
    UI_ACTION_PARAM_TYPE_STATE=2,
    UI_ACTION_PARAM_TYPE_SLONG=3,
    UI_ACTION_PARAM_TYPE_STRING=4,
    UI_ACTION_PARAM_TYPE_ULONG=5,
    UI_ACTION_PARAM_TYPE_FLOAT=6,
    UI_ACTION_PARAM_TYPE_DATE_AND_TIME=7,
    UI_ACTION_PARAM_TYPE_EBOOL=8,
    UI_ACTION_PARAM_TYPE_COUNTED_POINTER=9,
    UI_ACTION_PARAM_TYPE_2DVECTOR=10
};
enum EEvent {
    UI_EVENT_UP=0,
    UI_EVENT_DOWN=1,
    UI_EVENT_LEFT=2,
    UI_EVENT_RIGHT=3,
    UI_EVENT_SELECT=4,
    UI_EVENT_BACK=5,
    UI_EVENT_RIGHT_STICK_UP=6,
    UI_EVENT_RIGHT_STICK_DOWN=7,
    UI_EVENT_KEYBOARD_ARROWLEFT=8,
    UI_EVENT_KEYBOARD_ARROWRIGHT=9,
    UI_EVENT_KEYBOARD_SPACE=10,
    UI_EVENT_KEYBOARD_BACKSPACE=11,
    UI_EVENT_ZOOM_IN=12,
    UI_EVENT_ZOOM_OUT=13,
    UI_EVENT_SPELL_ROTATE_LEFT=14,
    UI_EVENT_SPELL_ROTATE_RIGHT=15,
    UI_EVENT_SPELL_LIST_UP=16,
    UI_EVENT_SPELL_LIST_DOWN=17,
    UI_EVENT_SPELL_ASSIGN=18,
    UI_EVENT_SPELL_UNASSIGN=19,
    UI_EVENT_SLIDER_LEFT=20,
    UI_EVENT_SLIDER_RIGHT=21,
    UI_EVENT_DELETE=22,
    UI_EVENT_X=23,
    UI_EVENT_Y=24,
    UI_EVENT_MOUSE_MOVEMENT=25,
    UI_EVENT_LEFT_MOUSE_CLICK=26,
    UI_EVENT_RIGHT_MOUSE_CLICK=27,
    UI_EVENT_LEFT_MOUSE_RELEASE=28,
    UI_EVENT_RIGHT_MOUSE_RELEASE=29,
    UI_EVENT_COMPONENT_DROPPED=30,
    UI_EVENT_LEFT_MOUSE_HELD=31,
    UI_EVENT_RIGHT_MOUSE_HELD=32,
    UI_EVENT_KEY_DOWN=33,
    UI_EVENT_CHAR_DOWN=34,
    UI_EVENT_MIDDLE_MOUSE_CLICK=35,
    UI_EVENT_MOUSE_WHEEL_UP=36,
    UI_EVENT_MOUSE_WHEEL_DOWN=37,
    UI_EVENT_MOUSE_BUTTON_4_CLICK=38,
    UI_EVENT_MOUSE_BUTTON_5_CLICK=39,
    UI_EVENT_MOUSE_BUTTON_6_CLICK=40,
    UI_EVENT_MOUSE_BUTTON_7_CLICK=41,
    UI_EVENT_MOUSE_BUTTON_8_CLICK=42
};
}


class C2DVector { public: float X,Y; };

class CRGBColour {
public:
    union {
        #ifdef _MSC_VER
struct { unsigned char B,G,R,A; };
#else
__extension__ struct { unsigned char B,G,R,A; };
#endif
        unsigned int IntValue;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};



class CIEngine;
class CEnginePrimitiveHandle;
class CRGBColour;
class C2DBoxF;
template<class T> class CDefPointer;
namespace std { template<class T> struct less; template<class K,class T,class C,class A> class map; }
namespace NUISystem {
class CComponent;
class CAction;
class CUIDef;
class CUIState;
class CDeletion;

class CUIInterface {
public:
    virtual ~CUIInterface(); 
    virtual void Update(float); 
    virtual void Draw(CIEngine &, CEnginePrimitiveHandle *, int, int *, NUISystem::CComponent *); 
};

class CObserver {
public:
    CObserver();
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent)=0;
    virtual bool IsObservingEvent(EEvent);
    virtual void ObserveEvent(EEvent);
    virtual void IgnoreEvent(EEvent);
    virtual void IgnoreAllEvents();
    virtual void ObserveAllEvents();
    virtual void SetPreventObserving(bool);
protected:
    unsigned char EventsToObserve[12]; 
    bool PreventObservation; 
};

class CActionParam {
public:
    CActionParam();
    virtual ~CActionParam();
    EActionParamType ActionParamType;
};


class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CComponent(int);
    virtual ~CComponent();
    virtual void Update(float);
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,int,int*,CComponent*);
    virtual NUISystem::CComponent * FindChild(CCharString); 
    virtual NUISystem::CComponent * FindChildForward(CCharString); 
    virtual void FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); 
    virtual void FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &); 
    virtual void SetPAcceptButton(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * FindChildNonRecursive(CCharString); 
    virtual NUISystem::CComponent * FindFirstChildOfType(NUISystem::EType); 
    virtual bool HasChild(NUISystem::CComponent *) const; 
    virtual void SetZoom(const C2DVector &); 
    virtual C2DVector GetZoom() const; 
    virtual void SetColour(const CRGBColour &); 
    virtual CRGBColour GetColour() const; 
    virtual void SetPosition(const C2DVector &); 
    virtual C2DVector GetPosition() const; 
    virtual C2DVector GetParentPosition() const; 
    virtual void SetParentPosition(C2DVector &); 
    virtual C2DVector GetParentZoom() const; 
    virtual void SetParentZoom(C2DVector &); 
    virtual CRGBColour GetParentColour() const; 
    virtual void SetParentColour(CRGBColour &); 
    virtual C2DVector GetSize() const; 
    virtual C2DBoxF GetBoundingBox() const; 
    virtual const C2DVector & GetRenderPosition() const; 
    virtual const C2DVector & GetRenderZoom() const; 
    virtual const CRGBColour & GetRenderColour() const; 
    virtual void SetRenderPosition(C2DVector &); 
    virtual void SetRenderZoom(C2DVector &); 
    virtual void SetRenderColour(CRGBColour &); 
    virtual void ChangePosition(const C2DVector &, float, bool); 
    virtual void ChangePositionDelta(const C2DVector &, float, bool); 
    virtual void ChangeTargetPositionDelta(const C2DVector &, float, bool); 
    virtual void UpdatePosition(float); 
    virtual void ChangeZoom(const C2DVector &, float, bool); 
    virtual void ChangeZoomDelta(const C2DVector &, float, bool); 
    virtual void UpdateZoom(float); 
    virtual void ChangeColour(const CRGBColour &, float, bool); 
    virtual void ChangeColourDelta(const CRGBColour &, float, bool); 
    virtual void UpdateColour(float); 
    virtual void ResetState(); 
    virtual void ForceState(unsigned int, bool); 
    virtual void Initialise(); 
    virtual bool HasState(unsigned int); 
    virtual bool HasStateInChildren(unsigned int); 
    virtual void ChangeState(unsigned int); 
    virtual void ChangeState(unsigned int, float); 
    virtual void ChangeState(NUISystem::CUIState &); 
    virtual bool ChangedState(); 
    virtual NUISystem::CUIState * GetCurrentState(); 
    virtual void SetParent(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * GetParent() const; 
    virtual std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & GetChildren(); 
    virtual float GetUpdateTime() const; 
    virtual C2DVector GetTargetPosition() const; 
    virtual CRGBColour GetTargetColour() const; 
    virtual C2DVector GetTargetZoom() const; 
    virtual void ProcessChangeState(); 
    virtual void AddChild(CCountedPointer<NUISystem::CComponent> &); 
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); 
    virtual void AddChildFront(CCountedPointer<NUISystem::CComponent> &); 
    virtual void SetDeletion(NUISystem::CDeletion); 
    virtual NUISystem::CDeletion & GetDeletion(); 
    virtual void RemoveChild(unsigned int); 
    virtual NUISystem::EType GetType() const; 
    virtual void AddAction(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnBack(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddPreAction(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDropped(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnHovered(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &); 
    virtual void AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &); 
    virtual void Die(); 
    virtual std::map<unsigned int,NUISystem::CUIState,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,NUISystem::CUIState> > > & GetStates(); 
    virtual void SetName(const CCharString &); 
    virtual CCharString GetName(); 
    virtual NUISystem::CComponent * FindComponent(NUISystem::CComponent *); 
    virtual unsigned int GetCurrentStateIndex(); 
    virtual unsigned int GetTargetStateIndex() const; 
    virtual float CanScroll(bool, C2DBoxF &); 
    virtual void FakeChildDying(NUISystem::CComponent *); 
    virtual void SetParentViewport(NUISystem::CComponent *); 
    virtual void ClearHandle(); 
    virtual void MakeUnavailable(); 
    virtual void MakeUnavailableDynamically(); 
    virtual void MakeAvailable(); 
    virtual void MakeAvailableDynamically(); 
    virtual void AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &); 
    virtual NUISystem::CComponent * FindFirstChildOfTypeNonRecursive(NUISystem::EType); 
    virtual bool IsAllowedToScroll() const; 
    virtual NUISystem::CComponent * GetParentViewport(); 
    virtual bool IsBastardChild() const; 
    virtual bool IsIndependent() const; 
    virtual bool IsPositionIndependent() const; 
    virtual bool IsZoomIndependent() const; 
    virtual bool IsLayerIndependent() const; 
    virtual bool IsDrawFromViewport() const; 
    virtual bool IsPositionIsCenter() const; 
    virtual int GetDefIndex() const; 
    virtual CDefPointer<NUISystem::CUIDef const > LoadAndReturnDef() const; 
    virtual void OnObserverRemove(); 
    virtual C2DVector CanMove(const C2DVector &); 
    virtual C2DVector GetDynamicSize() const; 
    virtual C2DVector GetTargetRenderPosition() const; 
    virtual bool IsPCStyle() const; 
    virtual void SetRelativeParentPosition(C2DVector &); 
    virtual void SetRelativeParentZoom(C2DVector &); 
    virtual bool IsUsingRelativeZoom() const; 
    virtual bool IsUsingRelativePosition() const; 
    virtual C2DVector GetRelativeRenderPosition(); 
    virtual C2DVector GetRelativeRenderZoom(); 
    virtual void SetRelativeRenderPosition(C2DVector); 
    virtual C2DVector SetRelativeRenderZoom(C2DVector); 
    virtual C2DVector GetTargetRelativeRenderPosition() const; 
    virtual C2DVector GetTargetRelativeRenderZoom() const; 
    virtual void AddPositionChild(NUISystem::CComponent *); 
    virtual void SetPositionParent(NUISystem::CComponent *); 
    virtual NUISystem::CComponent * GetPositionParent(); 
    virtual void PositionChildDying(NUISystem::CComponent *); 
    virtual void PositionParentDying(NUISystem::CComponent *); 
    virtual int GetRenderLayer() const; 
    virtual void AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &); 
    virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *); 
    virtual bool IsClickable() const; 
    virtual void RemoveFromHoveredList(); 
    virtual void ProcessEvent(EEvent);
protected:
    CCharString Name; 
    unsigned char States[12]; 
    float Time; 
    C2DVector Position; 
    C2DVector TargetPosition; 
    C2DVector InitialPosition; 
    C2DVector ParentPosition; 
    C2DVector RenderPosition; 
    C2DVector Zoom; 
    C2DVector TargetZoom; 
    C2DVector InitialZoom; 
    C2DVector ParentZoom; 
    C2DVector RenderZoom; 
    CRGBColour Colour; 
    CRGBColour TargetColour; 
    CRGBColour InitialColour; 
    CRGBColour ParentColour; 
    CRGBColour RenderColour; 
    float PositionTimeElapsed; 
    float PositionTime; 
    float ZoomTimeElapsed; 
    float ZoomTime; 
    float ColourTimeElapsed; 
    float ColourTime; 
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > Children; 
    std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > ChildrenToDelete; 
    NUISystem::CComponent * PParent; 
    C2DVector Size; 
    unsigned char Deletion[8]; 
    NUISystem::CComponent * PParentViewport; 
    unsigned char ActionsIAmParamOf[4]; 
    std::vector<unsigned int,std::allocator<unsigned int> > ShapeChildren; 
    C2DVector RenderPositionSave; 
    C2DVector RelativeRenderPosition; 
    C2DVector RelativeParentPosition; 
    C2DVector RelativeRenderZoom; 
    C2DVector RelativeParentZoom; 
    NUISystem::CComponent * PPositionParent; 
    unsigned char PositionChildren[12]; 
    std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnDestruction; 
    unsigned char Flags; 
    unsigned char Type; 
    unsigned char FlagsSetTwo; 
    char Layer; 
    int DefIndex; 
};
}
namespace NUISystem {

class CChangingStateComponent : public CComponent {
public:
    enum EChangingStateComponentState {UI_NO_CHANGING=0,UI_CHILDREN_CHANGING=1,UI_INTERNAL_CHANGING=2,UI_LAST=4};
    CChangingStateComponent(int);
    virtual ~CChangingStateComponent();
    virtual void Initialise(); 
    virtual void Update(float); 
    virtual void ForceState(unsigned int, bool); 
    virtual void ChangeState(unsigned int, float); 
    virtual void ChangeState(unsigned int); 
    virtual NUISystem::CUIState * GetCurrentState(); 
    virtual bool HasState(unsigned int); 
    virtual bool ChangedState(); 
    virtual unsigned int GetCurrentStateIndex(); 
    virtual unsigned int GetTargetStateIndex() const; 
    virtual float GetUpdateTime() const; 
    virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &); 
    virtual bool IsPositionIndependent() const; 
    virtual bool IsZoomIndependent() const; 
protected:
    virtual void ProcessChangeState(); 
public:
    virtual NUISystem::CUIState * GetState(unsigned int); 
    virtual const NUISystem::CUIState * GetState(unsigned int) const; 
    virtual void UpdateStateChange(); 
    virtual bool ChangedStateLastUpdate(); 
    virtual bool InternalChanged(); 
protected:
    virtual bool AllChildrenChangedState(); 
    virtual void OnActivate(); 
    virtual void OnDeactivate(); 
protected:
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesBeingDone; 
    NUISystem::CChangingStateComponent::EChangingStateComponentState StatesDone; 
    std::list<enum NUISystem::CChangingStateComponent::EChangingStateComponentState,std::allocator<enum NUISystem::CChangingStateComponent::EChangingStateComponentState> > StatesToDo; 
    float ParentUpdateTime; 
    unsigned int CurrentState; 
    unsigned int TargetState; 
    unsigned int RequestedState; 
    float UpdateTime; 
    bool PreviousUpdateChanged; 
    unsigned int PreviousState; 
};
}



namespace NUISystem {
class CUIStateDef;

class CUIState {
public:
    CUIState();
    CUIState(const CUIState&);
    CUIState(const CUIStateDef&);
    bool operator==(const CUIState&) const;
    unsigned int StateChangeFlag; 
    EStateChangeType StateChangeType; 
    C2DVector Position; 
    C2DVector Zoom; 
    CRGBColour Colour; 
    float UpdateTime; 
    bool LinearChange; 
};
}



namespace std {
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
template<class Dummy> class _Rb_global {
public:
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
    _Rb_tree_iterator& operator++(){this->_M_node=_Rb_global_inst::_M_increment(this->_M_node);return *this;}
};
template<class V,class A>struct _Rb_tree_base {
public:
    _Rb_tree_base(const A& a):_M_header(allocator<_Rb_tree_node<V> >(),(_Rb_tree_node<V>*)0){_M_header._M_data=_M_header.allocate(1);}
    ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:
    _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>
class _Rb_tree:public _Rb_tree_base<V,A> {
protected:
    size_t _M_node_count;
    Compare _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
    _Rb_tree(const Compare& c,const A& a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){_M_empty_initialize();}
    _Rb_tree& operator=(const _Rb_tree&);
    size_t size() const { return _M_node_count; }
private:
    void _M_empty_initialize(){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}
public:
    ~_Rb_tree(){clear();}
    iterator begin(){return iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    iterator end(){return iterator(this->_M_header._M_data);}
private:
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
protected:
    typedef _Rb_tree_node<V>* _Link_type;
    _Link_type& _M_root()const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& __fastcall _S_left(_Link_type x){return (_Link_type&)x->_M_left;}
    static _Link_type& __fastcall _S_right(_Link_type x){return (_Link_type&)x->_M_right;}
    static V& __fastcall _S_value(_Link_type x){return x->_M_value_field;}
    static const K& __fastcall _S_key(_Link_type x){return KeyOfValue()(_S_value(x));}
private:
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
    iterator find(const K& key){return iterator(_M_find(key));}
    pair<iterator,bool> insert_unique(const V&);
    iterator insert_unique(iterator,const V&);
    Compare key_comp()const{return _M_key_compare;}
    iterator lower_bound(const K& key) {
        _Link_type y=this->_M_header._M_data;
        _Link_type x=_M_root();
        while(x!=0) {
            if(!_M_key_compare(_S_key(x),key)) {y=x;x=_S_left(x);}
            else x=_S_right(x);
        }
        return iterator(y);
    }
    const_iterator find(const K&) const;
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
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
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >
class map {
public:typedef pair<const K,V> value_type;
private:
    typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::iterator iterator;
    explicit map(const Compare& c=Compare(),const A& a=A()):_M_t(c,a){}
    iterator begin(){return _M_t.begin();}
    const_iterator begin()const{return _M_t.begin();}
    iterator end(){return _M_t.end();}
    V& operator[](const K&);
    size_t size() const { return _M_t.size(); }
    void clear(){_M_t.clear();}
    iterator find(const K& key){return _M_t.find(key);}
    pair<iterator,bool> insert(const value_type& value){return _M_t.insert_unique(value);}
    iterator insert(iterator position,const value_type& value){return _M_t.insert_unique(position,value);}
    iterator lower_bound(const K& key){return _M_t.lower_bound(key);}
    Compare key_comp()const{return _M_t.key_comp();}
    const_iterator find(const K& key)const{return _M_t.find(key);}
    const_iterator end()const{return _M_t.end();}
};
}
namespace NUISystem {
struct CListState;

class CList : public CChangingStateComponent {
public: CList(int); virtual ~CList();
public: virtual void Initialise();
public: virtual void Update(float);
public: virtual void ChangeState(unsigned int);
public: virtual void ProcessEvent(NUISystem::EEvent);
public: virtual int GetSelectedChild();
public: virtual void RemoveChild(unsigned int);
public: virtual void AddChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual void Die();
public: virtual void AddScrollingChild(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddNonScrollingChild(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddNonScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &);
public: virtual C2DVector GetPositionOffset();
public: virtual float GetAlphaOffset();
public: virtual void SetLastInputTimeToBeNow();
public: virtual void SetNextScrollingChildAlpha(float);
public: virtual void SetNextScrollingChildPosition(C2DVector &);
public: virtual void AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<NUISystem::CComponent> &);
public: virtual void AddActionOnInactive(CCountedPointer<NUISystem::CAction> &);
public: virtual void AddActionOnSelected(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetActionOnScrollUp(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetActionOnScrollDown(CCountedPointer<NUISystem::CAction> &);
public: virtual void InitialiseOffsets(unsigned int);
public: virtual void SetDefaultChild(int);
public: virtual void SetDefaultChild(NUISystem::CComponent *);
public: virtual NUISystem::CListState SaveState();
public: virtual void RestoreState(NUISystem::CListState &);
public: virtual void SetSelectedChild(int);
public: virtual bool AllScrollingChildrenDeleting();
public: virtual std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & GetScrollingChildren();
public: virtual std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & GetNonScrollingChildren();
public: virtual void ScrollUp(bool,bool);
protected: virtual void ScrollUp(unsigned int,unsigned int,bool,bool,bool);
public: virtual void ScrollDown(bool,bool);
protected: virtual void ScrollDown(unsigned int,unsigned int,bool,bool,bool);
public: virtual bool AreThereAnyDeletions();
public: virtual bool IsWrapping() const;
public: virtual bool IsScrolling() const;
public: virtual bool IsInverted() const;
public: virtual void ScrollUntilChild(NUISystem::CComponent *);
public: virtual void ScrollUntilChildContaining(NUISystem::CComponent *);
public: virtual C2DVector CanMove(const C2DVector &);
public: virtual void ChangePosition(const C2DVector &,float,bool);
public: virtual NUISystem::CComponent * GetScrollBar();
public: virtual NUISystem::CComponent * GetScrollBarOutside();
public: virtual void SetInitialPositionDummy(NUISystem::CComponent *);
public: virtual void ScrollToFirstChild();
public: virtual void ScrollToLastChild();
protected: virtual void ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *);
protected: virtual void SetNonScrollingChildrenStates();
protected: virtual void SetRecomputeOffsets(bool);
protected: virtual void ScrollUpProcess();
protected: virtual void ScrollDownProcess();
protected: virtual void OnActivate();
protected: virtual void DoRecomputeOffsets();
protected: virtual void ComputeChildrenVisibility();
protected: virtual void ComputeScrollBarPosition();
protected: virtual void ComputeScrollBarZoom();
protected: int SelectedChild;
protected: float LastInputTime;
protected: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > ScrollingChildren;
protected: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > NonScrollingChildren;
protected: std::vector<std::map<unsigned int,NUISystem::CUIState,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,NUISystem::CUIState> > >,std::allocator<std::map<unsigned int,NUISystem::CUIState,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,NUISystem::CUIState> > > > > ScrollingChildrenStates;
protected: C2DVector PositionOffset;
protected: C2DVector PositionOffsetToUse;
protected: float AlphaOffset;
protected: float AlphaOffsetToUse;
protected: float NextScrollingChildAlpha;
protected: C2DVector NextScrollingChildPosition;
protected: std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnInactive;
protected: std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > * PActionsOnSelected;
protected: CCountedPointer<NUISystem::CAction> CPActionOnScrollUp;
protected: CCountedPointer<NUISystem::CAction> CPActionOnScrollDown;
protected: int DefaultChild;
protected: unsigned char ScrollingChildrenVisibility[16]; 
protected: NUISystem::CComponent * PInitialPositionDummy;
protected: NUISystem::CComponent * PScrollBar;
protected: NUISystem::CComponent * PScrollBarOutside;
protected: NUISystem::CComponent * PListUp;
protected: NUISystem::CComponent * PListDown;
protected: bool RecomputeOffsets;
protected: float InputDelay;
protected: C2DVector PDefPositionOffset;
protected: unsigned char PDefAlphaOffset;
};
}

namespace NUISystem {

class CObservable {
public: virtual void SetEvent(NUISystem::EEvent);
public: virtual void SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent);
public: virtual void AddObserver(NUISystem::CObserver *);
public: virtual void AddExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveObserver(NUISystem::CObserver *);
public: virtual void ClearObservers();
public: virtual NUISystem::CObserver * GetExclusiveObserver();
public: virtual bool IsExclusivelyObserved();
public: virtual void AddConcurrentExclusiveObserver(NUISystem::CObserver *);
public: virtual void RemoveConcurrentExclusiveObserver(NUISystem::CObserver *);
protected: std::list<NUISystem::CObserver *,std::allocator<NUISystem::CObserver *> > Observers;
protected: NUISystem::CObserver * m_pExclusiveObserver;
protected: std::list<NUISystem::CObserver *,std::allocator<NUISystem::CObserver *> > ConcurrentExclusiveObservers;
};
}

class CGraphicDataBank;class CMeshDataBank;class CInputManager;class CTCHeroExperience;
class CTCInventoryAbilities;class CTCInventoryWeapons;class CTCInventoryMap;class CTCInventoryQuests;
class CTCInventoryExperience;class CTCInventory;class CTCInventoryClothing;class CTCInventoryTrade;
class CThing;class CDefinitionManager;class CWideString;

enum EEngine2DLayer { ENGINE_2D_RANGE_SIZE=16,ENGINE_2D_RANGE_0_START=0,ENGINE_2D_RANGE_1_START=16,ENGINE_2D_RANGE_2_START=32,ENGINE_2D_RANGE_3_START=48,ENGINE_2D_RANGE_4_START=64,ENGINE_2D_LAYER_M3=64,ENGINE_2D_LAYER_M2=65,ENGINE_2D_LAYER_M1=66,ENGINE_2D_LAYER_0=67,ENGINE_2D_LAYER_1=68,ENGINE_2D_LAYER_2=69,ENGINE_2D_LAYER_3=70,ENGINE_2D_LAYER_4=71,ENGINE_2D_MESH_LAYER=72,ENGINE_2D_LAYER_5=73,ENGINE_2D_LAYER_6=74,ENGINE_2D_LAYER_7=75,ENGINE_2D_LAYER_8=76,ENGINE_2D_LAYER_9=77,ENGINE_2D_LAYER_10=78,ENGINE_2D_LAYER_11=79,ENGINE_2D_LAYER_12=80,ENGINE_2D_LAYER_13=81,ENGINE_2D_LAYER_SPELLS=82,ENGINE_NUM_2D_LAYERS=83,ENGINE_2D_NULL_LAYER=-1,ENGINE_2D_LAYER_FIRST=0 };
namespace NUISystem {
class CList;class CText;class CComponent;class CAction;class CFrontEndData;class CUIMiscThingsDef;class CUIIconsDef;

class CManager : public CObservable {
protected: CManager(); ~CManager();
public:
enum EChange { UI_CHANGE_ACCEPT=0,UI_CHANGE_REJECT=1 };
enum EInputType { UI_INPUT_KEYBOARD=0,UI_INPUT_XBOX=1 };
enum EUsedKeys { UI_KEY_LEFT=0,UI_KEY_RIGHT=1,UI_KEY_DOWN=2,UI_KEY_UP=3,UI_KEY_RIGHT_STICK_DOWN=4,UI_KEY_RIGHT_STICK_UP=5,UI_KEY_A=6,UI_KEY_B=7,UI_KEY_ZOOM_IN=8,UI_KEY_ZOOM_OUT=9,UI_KEY_SPELL_LEFT=10,UI_KEY_SPELL_RIGHT=11,UI_KEY_SPELL_ASSIGN=12,UI_KEY_SPELL_UNASSIGN=13,UI_KEY_SPELL_LIST_UP=14,UI_KEY_SPELL_LIST_DOWN=15 };
public: static NUISystem::CManager * __fastcall GetInstance();
public: virtual bool GetMouseDisabled();
public: NUISystem::CComponent * CreateComponent(const CCharString &,bool);
public: NUISystem::CComponent * CreateComponent(int);
public: virtual void SetKeyPressed(unsigned short);
public: virtual unsigned short GetKeyPressed();
public: virtual void Action(CCountedPointer<NUISystem::CAction> &);
public: virtual void SetWeaponsInventory(CTCInventoryWeapons *);
public: virtual void SetMapInventory(CTCInventoryMap *);
public: virtual void SetQuestInventory(CTCInventoryQuests *);
public: virtual void SetTCHeroExperienceInventory(CTCHeroExperience *);
public: virtual void SetAbilitiesInventory(CTCInventoryAbilities *);
public: virtual void SetExperienceInventory(CTCInventoryExperience *);
public: virtual void SetInventory(CTCInventory *);
public: virtual void SetInventoryClothing(CTCInventoryClothing *);
public: virtual void SetInventoryTrade(CTCInventoryTrade *);
public: virtual void AddThingActionParam(int);
public: virtual void RemoveThingActionParam(int);
public: virtual void SetPlayerThing(CThing *);
public: virtual void AddUIActionParam(NUISystem::CActionParam *);
public: virtual void RemoveUIActionParam(NUISystem::CActionParam *);
public: virtual void ClearActionParams();
public: NUISystem::CComponent * CreateMenuEntryComponent(const CWideString &,float,float,const CCharString &,float,const CCharString,float,CCharString);
public: virtual void SetNumberAugmentations(unsigned int);
public: virtual unsigned int GetNumberAugmentations() const;
public: virtual void SetChange(NUISystem::CManager::EChange);
public: virtual NUISystem::CManager::EChange GetChange();
public: virtual void SetMetaLayer(int);
public: virtual int GetMetaLayer();
public: virtual EEngine2DLayer GetLayer(int);
public: virtual void ClearBastardChild();
public: virtual void SetRightStickForce(C2DVector &);
public: virtual C2DVector & GetRightStickForce();
public: virtual void PlaySoundW(CCharString &,int);
public: virtual unsigned int PlaySample(CCharString &,int);
public: virtual CDefPointer<NUISystem::CUIMiscThingsDef> GetMiscThingsDef();
public: virtual CDefPointer<NUISystem::CUIIconsDef> GetIconsDef();
public: virtual void AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent);
protected: CCountedPointer<CGraphicDataBank> PGraphicsBank;
protected: CCountedPointer<CMeshDataBank> PMeshBank;
protected: CInputManager * PInputManager;
protected: std::map<NUISystem::CManager::EUsedKeys,int,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys const ,int> > > Keys;
protected: NUISystem::CManager::EInputType InputType;
protected: CTCHeroExperience * PTCHeroExperience;
protected: CTCInventoryAbilities * PAbilitiesInventory;
protected: CTCInventoryWeapons * PWeaponsInventory;
protected: CTCInventoryMap * PMapInventory;
protected: CTCInventoryQuests * PQuestInventory;
protected: CTCInventoryExperience * PExperienceInventory;
protected: CTCInventory * PInventory;
protected: CTCInventoryClothing * PInventoryClothing;
protected: CTCInventoryTrade * PInventoryTrade;
protected: CThing * PPlayer;
protected: std::list<int,std::allocator<int> > ThingActionParams;
protected: std::list<NUISystem::CActionParam *,std::allocator<NUISystem::CActionParam *> > UIActionParams;
protected: CDefinitionManager * m_pDefinitionManager;
protected: unsigned int NumberAugmentations;
protected: NUISystem::CManager::EChange Change;
protected: std::map<int,EEngine2DLayer,std::less<int>,std::allocator<std::pair<int const ,EEngine2DLayer> > > Layers;
protected: int MetaLayer;
protected: CCountedPointer<NUISystem::CComponent> BastardChild;
protected: std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString const ,CCountedPointer<NUISystem::CComponent> > > > BastardChildren;
protected: C2DVector RightStickForce;
protected: bool m_bHasErrorMessage;
protected: NUISystem::CComponent * m_pErrorMessage;
protected: bool Restoring;
protected: double m_fLastRenderTime;
protected: C2DVector MouseMovement;
protected: NUISystem::CComponent * PMouseCursor;
protected: NUISystem::CComponent * PDropped;
protected: unsigned short KeyPressed;
protected: float DescriptionScrollingOffset;
protected: bool DescriptionScrollingToMax;
protected: bool MouseDisabled;
protected: bool AssignmentItemsExpressionsSwapping;
protected: bool AssignmentSpellsSwapping;
};
}

namespace NUISystem {

class CDeletion { public:
enum EDeletionMethod { UI_DO_NOT_DELETE=0,UI_DELETE_THIS_INSTANCE=1,UI_DELETE_ALL_INSTANCES=2,UI_DELETE_THIS_INSTANCE_PER_PARENT=3 };
public: NUISystem::CDeletion::EDeletionMethod Method;
public: std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > AssociatedParents;
};
}




class CGraphicDataBank;
namespace NInventory {
class CTCInventoryBase : public CTCBase {
public:
 enum EInventorySelectionState { INVENTORY_SELECTION_STATE_CATEGORIES=0,INVENTORY_SELECTION_STATE_ITEMS=1,INVENTORY_SELECTION_STATE_MAX_STATES=2 };
 enum ESelectionState { SELECTION_STATE_NONE=0,SELECTION_STATE_OFF=1,SELECTION_STATE_ON=2,SELECTION_STATE_SEL=3,SELECTION_STATE_MAX=4 };
protected:
 virtual void OnSerialise(CPersistContext&,bool);
 unsigned char Components[12];
 bool FirstRenderFrame;
 EInventorySelectionState InventorySelectionState;
 unsigned char Categories[12];
 int HighlightedItemNumOwned,HighlightedItemMaxOwned;
 bool CanItemBeQuickAssigned,CanItemBeUsed;
 ESelectionState MenuArrowHighlightsCategoryUp,MenuArrowHighlightsCategoryDn;
 ESelectionState MenuArrowHighlightsItemUp,MenuArrowHighlightsItemDn;
 ESelectionState TriggerStateLeft,TriggerStateRight;
 double LastRenderTime;
 bool InventoryOpen;
 unsigned char _pad_after_InventoryOpen[3];
 unsigned char OldIntervalDesc[8];
 bool OldIntervalDescInitialised;
 unsigned char _pad_after_OldIntervalDescInitialised[3];
private:
 unsigned char PDef[4];
 int GlobalDefIndex,SelectedCategoryIndex;
 const CGraphicDataBank* GraphicManager;
 unsigned char SerialisedItems[8],SerialisedSelectedItems[8];
 bool Dying,CategoryMoving;
 float CategoryMovement,CategoryMovementDirection;
 unsigned char MenuDisplayListItem[12],MenuDisplayListCategory[12],ConfiscatedItems[12];
 CThing* pBuildingThing;
 unsigned char InventoryAnimRenderBox[16];
 unsigned char InventoryAnimCurrentRHSet[24],InventoryAnimLastRHSet[24];
 float InventoryAnimCurrentXRotationAngle,InventoryAnimLastXRotationAngle;
 float InventoryAnimCurrentYRotationAngle,InventoryAnimLastYRotationAngle;
 float InventoryAnimCurrentZRotationAngle,InventoryAnimLastZRotationAngle;
 float InventoryAnimXRotationAngleGain,InventoryAnimYRotationAngleGain,InventoryAnimZRotationAngleGain;
 bool EnableAnimXAxis,EnableAnimZAxis;
 int InventoryAnimThingDefIndex,RHSetInvItemDefIndex,ParentMapNumber;
};
}

namespace NUISystem { class CComponent; }
enum EPlayerMode {  };
namespace NHeroInformationScreens {
enum EScreenType { NULL_SCREEN_TYPE=-1,STANDARD_INVENTORY_SCREEN=0,INVENTORY_QUESTS_SCREEN=1,INVENTORY_EXPERIENCE_SCREEN=2,HERO_ABILITIES_SCREEN=3,INVENTORY_WEAPONS_SCREEN=4,INVENTORY_CLOTHING_SCREEN=5,INVENTORY_TRADE_SCREEN=6,QUEST_CARD_SCREEN=7,INVENTORY_MAP_SCREEN=8,INVENTORY_STATS_SCREEN=9,INVENTORY_MAGIC_SCREEN=10,NUMBER_OF_SCREEN_TYPES=11 };
class CBase {
public: virtual CWideString GetScreenName() const;
public: virtual EPlayerMode GetPlayerMode() const;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const;
public: virtual bool IsInventoryScreen() const;
};
}

class CTCHeroExperience;class CTCHeroStats;

class CTCInventoryStats : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public: virtual ~CTCInventoryStats();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnSerialise(CPersistContext &,bool);
public: virtual void OnKill();
public: virtual void Copy(const CTCBase &);
public: virtual void FrameUpdate();
public: virtual bool HasFrameUpdate() const;
public: virtual void OnCreate();
public: virtual void OnItemSelected(int,bool &);
public: virtual void OpenInventory();
public: virtual void CloseInventory();
public: virtual void GlobalUpdate(bool);
public: virtual void OnChangeCategory();
public: virtual CWideString GetScreenName() const;
public: virtual EPlayerMode GetPlayerMode() const;
public: virtual NHeroInformationScreens::EScreenType GetScreenType() const;
public: static CWideString __fastcall GetXString();
public: virtual NUISystem::CComponent * ConstructScreen(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructPersonalityList(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructExperienceList(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructExperienceListPC(NUISystem::CComponent *);
public: virtual NUISystem::CComponent * ConstructPersonalityListPC(NUISystem::CComponent *);
private: std::map<unsigned int,int,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,int> > > CreaturesKilled;
private: CThing * pMannequin;
private: bool UseMannequin;
private: int lastCategoryIdx;
private: const CTCHeroExperience * pTCHeroExperience;
private: const CTCHeroStats * pTCHeroStats;
};




namespace NUISystem { class CList;class CComponent; }

class CTCQuickAccessMenu : public CTCBase {
public: CTCQuickAccessMenu(CThing&);
public: virtual ~CTCQuickAccessMenu();
public: virtual ETCInterfaceType GetInterfaceType() const;
public: virtual CCharString GetClassTypeName() const;
public: virtual void OnCreate();
public: virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &);
public: virtual void GlobalUpdate(bool);
private: void ConstructItemCategoryList(CWideString &,NUISystem::CList *,std::vector<std::pair<int,int>,std::allocator<std::pair<int,int> > > &,NUISystem::CComponent *,bool);
private: void ContructItemLists(std::vector<std::pair<int,int>,std::allocator<std::pair<int,int> > > &,NUISystem::CList *,NUISystem::CComponent *,CCountedPointer<NUISystem::CComponent> &);
private: void ContructItemLists(std::map<int,int,std::less<int>,std::allocator<std::pair<int const ,int> > > &,NUISystem::CList *,NUISystem::CComponent *,CCountedPointer<NUISystem::CComponent> &);
private: bool Active;
private: std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > Components;
private: bool Quitting;
private: double LastRenderTime;
private: bool FirstRenderFrame;
};
#pragma auto_inline(off)
namespace std {
template<> list<NUISystem::CComponent*>::~list() {}
template<> void _List_base<NUISystem::CComponent*,allocator<NUISystem::CComponent*> >::clear() {
    _List_node<NUISystem::CComponent*>* cur=(_List_node<NUISystem::CComponent*>*)_M_node._M_data->_M_next;
    while(cur!=_M_node._M_data) {
        _List_node<NUISystem::CComponent*>* old=cur;
        cur=(_List_node<NUISystem::CComponent*>*)cur->_M_next;
        _M_node.deallocate(old,1);
    }
    _M_node._M_data->_M_next=_M_node._M_data;
    _M_node._M_data->_M_prev=_M_node._M_data;
}
}
#pragma auto_inline(on)
inline CWideString __fastcall GFIntToWideString(int value)
{
    CWideString text;
    text.Sprintf(fixture_decimal_format,value);
    return text;
}
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }
void CTCQuickAccessMenu::ConstructItemCategoryList(CWideString& menuparam,NUISystem::CList* plist,std::vector<std::pair<int,int> >& items,NUISystem::CComponent* ptop_list,bool is_gifts)
{
    using namespace NUISystem;
    CManager* pmanager=CManager::GetInstance();
    CWideString menu_title(menuparam);
    unsigned int total_amount=0;
    for(unsigned int count=0;count<items.size();++count) total_amount+=items[count].second;
    if(!items.empty()) {
        menu_title+=CTCInventoryStats::GetXString();
        menu_title+=GFIntToWideString(total_amount);
    }
    CCountedPointer<CComponent> pmenu_entry_cat(pmanager->CreateMenuEntryComponent(menu_title,0.0f,1.0f,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS",0.0f,"",0.0f,"PC_BUTTON_BASTARD"));
    plist->AddScrollingChildAndSetOffsets(pmenu_entry_cat);
    if(items.empty()) {
        CComponent* arrow=pmenu_entry_cat->FindChild("UI_SELECTION_ARROW_RIGHT");
        if(arrow) {
            CDeletion delete_arrow;
            delete_arrow.Method=CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(delete_arrow);
        }
        pmenu_entry_cat->MakeUnavailable();
    } else {
        std::map<int,int> thingmap;
        for(int count=0;count<(int)items.size();++count) {
            int key=items[count].first;
            std::map<int,int>::iterator found=thingmap.find(key);
            if(found==thingmap.end()) thingmap.insert(std::pair<int,int>(key,items[count].second));
            else found->second+=items[count].second;
        }
        CCountedPointer<CComponent> plist_cat_component(pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS",false));
        CList* plist_cat=static_cast<CList*>(plist_cat_component.Get());
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(thingmap.size());
        bool usable=true;
        const CTCTargetingBase* targeting;
        if(is_gifts && ParentThing.PeekTC(&targeting) && !IS_THING_ALIVE(targeting->GetPTargetedThing())) usable=false;
        CCharString action_list_component_name(usable?"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS":"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS_CANT_USE");
        CCountedPointer<CComponent> pactions_list(CManager::GetInstance()->CreateComponent(action_list_component_name,false));
        CList* pactions_list_ptr=static_cast<CList*>(pactions_list.Get());
        pactions_list_ptr->SetDefaultChild(0);
        if(!usable) {
            CComponent* unavailable=pactions_list_ptr->FindChild("UI_MENU_ENTRY_QUICK_ACCESS_ITEM_ACTION_NOT_USE");
            CComponent* arrow=unavailable->FindChild("UI_SELECTION_ARROW_RIGHT_FOR_QUICK_ACCESS");
            if(arrow) {
                CDeletion delete_arrow;
                delete_arrow.Method=CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(delete_arrow);
            }
            unavailable->MakeUnavailable();
        }
        ContructItemLists(thingmap,plist_cat,ptop_list,pactions_list);
    }
}


namespace NUISystem {


struct CListState { public: std::list<std::pair<int,int> > Structure; };
}
namespace NUISystem {
void CUIInterface::Update(float) { assert(!"unexpected CUIInterface call: Update"); throw 0; }
void CUIInterface::Draw(CIEngine &, CEnginePrimitiveHandle *, int, int *, NUISystem::CComponent *) { assert(!"unexpected CUIInterface call: Draw"); throw 0; }
void CObserver::ProcessEvent(EEvent) { assert(!"unexpected CObserver call: ProcessEvent"); throw 0; }
bool CObserver::IsObservingEvent(EEvent) { assert(!"unexpected CObserver call: IsObservingEvent"); throw 0; }
void CObserver::ObserveEvent(EEvent) { assert(!"unexpected CObserver call: ObserveEvent"); throw 0; }
void CObserver::IgnoreEvent(EEvent) { assert(!"unexpected CObserver call: IgnoreEvent"); throw 0; }
void CObserver::IgnoreAllEvents() { assert(!"unexpected CObserver call: IgnoreAllEvents"); throw 0; }
void CObserver::ObserveAllEvents() { assert(!"unexpected CObserver call: ObserveAllEvents"); throw 0; }
void CObserver::SetPreventObserving(bool) { assert(!"unexpected CObserver call: SetPreventObserving"); throw 0; }

void CComponent::Update(float) { assert(!"unexpected CComponent call: Update"); throw 0; }
void CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,int,int*,CComponent*) { assert(!"unexpected CComponent call: Draw"); throw 0; }
NUISystem::CComponent * CComponent::FindChild(CCharString) { assert(!"unexpected CComponent call: FindChild"); throw 0; }
NUISystem::CComponent * CComponent::FindChildForward(CCharString) { assert(!"unexpected CComponent call: FindChildForward"); throw 0; }
void CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { assert(!"unexpected CComponent call: FindAllChildren"); throw 0; }
void CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { assert(!"unexpected CComponent call: FindAllChildrenOfType"); throw 0; }
void CComponent::SetPAcceptButton(NUISystem::CComponent *) { assert(!"unexpected CComponent call: SetPAcceptButton"); throw 0; }
NUISystem::CComponent * CComponent::FindChildNonRecursive(CCharString) { assert(!"unexpected CComponent call: FindChildNonRecursive"); throw 0; }
NUISystem::CComponent * CComponent::FindFirstChildOfType(NUISystem::EType) { assert(!"unexpected CComponent call: FindFirstChildOfType"); throw 0; }
bool CComponent::HasChild(NUISystem::CComponent *) const { assert(!"unexpected CComponent call: HasChild"); throw 0; }
void CComponent::SetZoom(const C2DVector &) { assert(!"unexpected CComponent call: SetZoom"); throw 0; }
C2DVector CComponent::GetZoom() const { assert(!"unexpected CComponent call: GetZoom"); throw 0; }
void CComponent::SetColour(const CRGBColour &) { assert(!"unexpected CComponent call: SetColour"); throw 0; }
CRGBColour CComponent::GetColour() const { assert(!"unexpected CComponent call: GetColour"); throw 0; }
void CComponent::SetPosition(const C2DVector &) { assert(!"unexpected CComponent call: SetPosition"); throw 0; }
C2DVector CComponent::GetPosition() const { assert(!"unexpected CComponent call: GetPosition"); throw 0; }
C2DVector CComponent::GetParentPosition() const { assert(!"unexpected CComponent call: GetParentPosition"); throw 0; }
void CComponent::SetParentPosition(C2DVector &) { assert(!"unexpected CComponent call: SetParentPosition"); throw 0; }
C2DVector CComponent::GetParentZoom() const { assert(!"unexpected CComponent call: GetParentZoom"); throw 0; }
void CComponent::SetParentZoom(C2DVector &) { assert(!"unexpected CComponent call: SetParentZoom"); throw 0; }
CRGBColour CComponent::GetParentColour() const { assert(!"unexpected CComponent call: GetParentColour"); throw 0; }
void CComponent::SetParentColour(CRGBColour &) { assert(!"unexpected CComponent call: SetParentColour"); throw 0; }
C2DVector CComponent::GetSize() const { assert(!"unexpected CComponent call: GetSize"); throw 0; }
C2DBoxF CComponent::GetBoundingBox() const { assert(!"unexpected CComponent call: GetBoundingBox"); throw 0; }
const C2DVector & CComponent::GetRenderPosition() const { assert(!"unexpected CComponent call: GetRenderPosition"); throw 0; }
const C2DVector & CComponent::GetRenderZoom() const { assert(!"unexpected CComponent call: GetRenderZoom"); throw 0; }
const CRGBColour & CComponent::GetRenderColour() const { assert(!"unexpected CComponent call: GetRenderColour"); throw 0; }
void CComponent::SetRenderPosition(C2DVector &) { assert(!"unexpected CComponent call: SetRenderPosition"); throw 0; }
void CComponent::SetRenderZoom(C2DVector &) { assert(!"unexpected CComponent call: SetRenderZoom"); throw 0; }
void CComponent::SetRenderColour(CRGBColour &) { assert(!"unexpected CComponent call: SetRenderColour"); throw 0; }
void CComponent::ChangePosition(const C2DVector &, float, bool) { assert(!"unexpected CComponent call: ChangePosition"); throw 0; }
void CComponent::ChangePositionDelta(const C2DVector &, float, bool) { assert(!"unexpected CComponent call: ChangePositionDelta"); throw 0; }
void CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { assert(!"unexpected CComponent call: ChangeTargetPositionDelta"); throw 0; }
void CComponent::UpdatePosition(float) { assert(!"unexpected CComponent call: UpdatePosition"); throw 0; }
void CComponent::ChangeZoom(const C2DVector &, float, bool) { assert(!"unexpected CComponent call: ChangeZoom"); throw 0; }
void CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { assert(!"unexpected CComponent call: ChangeZoomDelta"); throw 0; }
void CComponent::UpdateZoom(float) { assert(!"unexpected CComponent call: UpdateZoom"); throw 0; }
void CComponent::ChangeColour(const CRGBColour &, float, bool) { assert(!"unexpected CComponent call: ChangeColour"); throw 0; }
void CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { assert(!"unexpected CComponent call: ChangeColourDelta"); throw 0; }
void CComponent::UpdateColour(float) { assert(!"unexpected CComponent call: UpdateColour"); throw 0; }
void CComponent::ResetState() { assert(!"unexpected CComponent call: ResetState"); throw 0; }
void CComponent::ForceState(unsigned int, bool) { assert(!"unexpected CComponent call: ForceState"); throw 0; }
void CComponent::Initialise() { assert(!"unexpected CComponent call: Initialise"); throw 0; }
bool CComponent::HasState(unsigned int) { assert(!"unexpected CComponent call: HasState"); throw 0; }
bool CComponent::HasStateInChildren(unsigned int) { assert(!"unexpected CComponent call: HasStateInChildren"); throw 0; }
void CComponent::ChangeState(unsigned int) { assert(!"unexpected CComponent call: ChangeState"); throw 0; }
void CComponent::ChangeState(unsigned int, float) { assert(!"unexpected CComponent call: ChangeState"); throw 0; }
void CComponent::ChangeState(NUISystem::CUIState &) { assert(!"unexpected CComponent call: ChangeState"); throw 0; }
bool CComponent::ChangedState() { assert(!"unexpected CComponent call: ChangedState"); throw 0; }
NUISystem::CUIState * CComponent::GetCurrentState() { assert(!"unexpected CComponent call: GetCurrentState"); throw 0; }
void CComponent::SetParent(NUISystem::CComponent *) { assert(!"unexpected CComponent call: SetParent"); throw 0; }
NUISystem::CComponent * CComponent::GetParent() const { assert(!"unexpected CComponent call: GetParent"); throw 0; }
std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & CComponent::GetChildren() { assert(!"unexpected CComponent call: GetChildren"); throw 0; }
float CComponent::GetUpdateTime() const { assert(!"unexpected CComponent call: GetUpdateTime"); throw 0; }
C2DVector CComponent::GetTargetPosition() const { assert(!"unexpected CComponent call: GetTargetPosition"); throw 0; }
CRGBColour CComponent::GetTargetColour() const { assert(!"unexpected CComponent call: GetTargetColour"); throw 0; }
C2DVector CComponent::GetTargetZoom() const { assert(!"unexpected CComponent call: GetTargetZoom"); throw 0; }
void CComponent::ProcessChangeState() { assert(!"unexpected CComponent call: ProcessChangeState"); throw 0; }
void CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CComponent call: AddChild"); throw 0; }
void CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CComponent call: AddChildDynamically"); throw 0; }
void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CComponent call: AddChildFront"); throw 0; }
void CComponent::SetDeletion(NUISystem::CDeletion) { assert(!"unexpected CComponent call: SetDeletion"); throw 0; }
NUISystem::CDeletion & CComponent::GetDeletion() { assert(!"unexpected CComponent call: GetDeletion"); throw 0; }
void CComponent::RemoveChild(unsigned int) { assert(!"unexpected CComponent call: RemoveChild"); throw 0; }
NUISystem::EType CComponent::GetType() const { assert(!"unexpected CComponent call: GetType"); throw 0; }
void CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddAction"); throw 0; }
void CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnSelected"); throw 0; }
void CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnBack"); throw 0; }
void CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddPreAction"); throw 0; }
void CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnInactive"); throw 0; }
void CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnLeftClicked"); throw 0; }
void CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnLeftHeld"); throw 0; }
void CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnRightClicked"); throw 0; }
void CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnDropped"); throw 0; }
void CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnDroppedNowhere"); throw 0; }
void CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnDraggedUp"); throw 0; }
void CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnDraggedDown"); throw 0; }
void CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnHovered"); throw 0; }
void CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnUnhovered"); throw 0; }
void CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnLeftUnclicked"); throw 0; }
void CComponent::Die() { assert(!"unexpected CComponent call: Die"); throw 0; }
std::map<unsigned int,NUISystem::CUIState,std::less<unsigned int>,std::allocator<std::pair<unsigned int const ,NUISystem::CUIState> > > & CComponent::GetStates() { assert(!"unexpected CComponent call: GetStates"); throw 0; }
void CComponent::SetName(const CCharString &) { assert(!"unexpected CComponent call: SetName"); throw 0; }
CCharString CComponent::GetName() { assert(!"unexpected CComponent call: GetName"); throw 0; }
NUISystem::CComponent * CComponent::FindComponent(NUISystem::CComponent *) { assert(!"unexpected CComponent call: FindComponent"); throw 0; }
unsigned int CComponent::GetCurrentStateIndex() { assert(!"unexpected CComponent call: GetCurrentStateIndex"); throw 0; }
unsigned int CComponent::GetTargetStateIndex() const { assert(!"unexpected CComponent call: GetTargetStateIndex"); throw 0; }
float CComponent::CanScroll(bool, C2DBoxF &) { assert(!"unexpected CComponent call: CanScroll"); throw 0; }
void CComponent::FakeChildDying(NUISystem::CComponent *) { assert(!"unexpected CComponent call: FakeChildDying"); throw 0; }
void CComponent::SetParentViewport(NUISystem::CComponent *) { assert(!"unexpected CComponent call: SetParentViewport"); throw 0; }
void CComponent::ClearHandle() { assert(!"unexpected CComponent call: ClearHandle"); throw 0; }
void CComponent::MakeUnavailable() { assert(!"unexpected CComponent call: MakeUnavailable"); throw 0; }
void CComponent::MakeUnavailableDynamically() { assert(!"unexpected CComponent call: MakeUnavailableDynamically"); throw 0; }
void CComponent::MakeAvailable() { assert(!"unexpected CComponent call: MakeAvailable"); throw 0; }
void CComponent::MakeAvailableDynamically() { assert(!"unexpected CComponent call: MakeAvailableDynamically"); throw 0; }
void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionIAmParamOf"); throw 0; }
NUISystem::CComponent * CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { assert(!"unexpected CComponent call: FindFirstChildOfTypeNonRecursive"); throw 0; }
bool CComponent::IsAllowedToScroll() const { assert(!"unexpected CComponent call: IsAllowedToScroll"); throw 0; }
NUISystem::CComponent * CComponent::GetParentViewport() { assert(!"unexpected CComponent call: GetParentViewport"); throw 0; }
bool CComponent::IsBastardChild() const { assert(!"unexpected CComponent call: IsBastardChild"); throw 0; }
bool CComponent::IsIndependent() const { assert(!"unexpected CComponent call: IsIndependent"); throw 0; }
bool CComponent::IsPositionIndependent() const { assert(!"unexpected CComponent call: IsPositionIndependent"); throw 0; }
bool CComponent::IsZoomIndependent() const { assert(!"unexpected CComponent call: IsZoomIndependent"); throw 0; }
bool CComponent::IsLayerIndependent() const { assert(!"unexpected CComponent call: IsLayerIndependent"); throw 0; }
bool CComponent::IsDrawFromViewport() const { assert(!"unexpected CComponent call: IsDrawFromViewport"); throw 0; }
bool CComponent::IsPositionIsCenter() const { assert(!"unexpected CComponent call: IsPositionIsCenter"); throw 0; }
int CComponent::GetDefIndex() const { assert(!"unexpected CComponent call: GetDefIndex"); throw 0; }
CDefPointer<NUISystem::CUIDef const > CComponent::LoadAndReturnDef() const { assert(!"unexpected CComponent call: LoadAndReturnDef"); throw 0; }
void CComponent::OnObserverRemove() { assert(!"unexpected CComponent call: OnObserverRemove"); throw 0; }
C2DVector CComponent::CanMove(const C2DVector &) { assert(!"unexpected CComponent call: CanMove"); throw 0; }
C2DVector CComponent::GetDynamicSize() const { assert(!"unexpected CComponent call: GetDynamicSize"); throw 0; }
C2DVector CComponent::GetTargetRenderPosition() const { assert(!"unexpected CComponent call: GetTargetRenderPosition"); throw 0; }
bool CComponent::IsPCStyle() const { assert(!"unexpected CComponent call: IsPCStyle"); throw 0; }
void CComponent::SetRelativeParentPosition(C2DVector &) { assert(!"unexpected CComponent call: SetRelativeParentPosition"); throw 0; }
void CComponent::SetRelativeParentZoom(C2DVector &) { assert(!"unexpected CComponent call: SetRelativeParentZoom"); throw 0; }
bool CComponent::IsUsingRelativeZoom() const { assert(!"unexpected CComponent call: IsUsingRelativeZoom"); throw 0; }
bool CComponent::IsUsingRelativePosition() const { assert(!"unexpected CComponent call: IsUsingRelativePosition"); throw 0; }
C2DVector CComponent::GetRelativeRenderPosition() { assert(!"unexpected CComponent call: GetRelativeRenderPosition"); throw 0; }
C2DVector CComponent::GetRelativeRenderZoom() { assert(!"unexpected CComponent call: GetRelativeRenderZoom"); throw 0; }
void CComponent::SetRelativeRenderPosition(C2DVector) { assert(!"unexpected CComponent call: SetRelativeRenderPosition"); throw 0; }
C2DVector CComponent::SetRelativeRenderZoom(C2DVector) { assert(!"unexpected CComponent call: SetRelativeRenderZoom"); throw 0; }
C2DVector CComponent::GetTargetRelativeRenderPosition() const { assert(!"unexpected CComponent call: GetTargetRelativeRenderPosition"); throw 0; }
C2DVector CComponent::GetTargetRelativeRenderZoom() const { assert(!"unexpected CComponent call: GetTargetRelativeRenderZoom"); throw 0; }
void CComponent::AddPositionChild(NUISystem::CComponent *) { assert(!"unexpected CComponent call: AddPositionChild"); throw 0; }
void CComponent::SetPositionParent(NUISystem::CComponent *) { assert(!"unexpected CComponent call: SetPositionParent"); throw 0; }
NUISystem::CComponent * CComponent::GetPositionParent() { assert(!"unexpected CComponent call: GetPositionParent"); throw 0; }
void CComponent::PositionChildDying(NUISystem::CComponent *) { assert(!"unexpected CComponent call: PositionChildDying"); throw 0; }
void CComponent::PositionParentDying(NUISystem::CComponent *) { assert(!"unexpected CComponent call: PositionParentDying"); throw 0; }
int CComponent::GetRenderLayer() const { assert(!"unexpected CComponent call: GetRenderLayer"); throw 0; }
void CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CComponent call: AddActionOnDesctruction"); throw 0; }
void CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { assert(!"unexpected CComponent call: ExecuteActions"); throw 0; }
bool CComponent::IsClickable() const { assert(!"unexpected CComponent call: IsClickable"); throw 0; }
void CComponent::RemoveFromHoveredList() { assert(!"unexpected CComponent call: RemoveFromHoveredList"); throw 0; }
void CComponent::ProcessEvent(EEvent) { assert(!"unexpected CComponent call: ProcessEvent"); throw 0; }
void CChangingStateComponent::Initialise() { assert(!"unexpected CChangingStateComponent call: Initialise"); throw 0; }
void CChangingStateComponent::Update(float) { assert(!"unexpected CChangingStateComponent call: Update"); throw 0; }
void CChangingStateComponent::ForceState(unsigned int, bool) { assert(!"unexpected CChangingStateComponent call: ForceState"); throw 0; }
void CChangingStateComponent::ChangeState(unsigned int, float) { assert(!"unexpected CChangingStateComponent call: ChangeState"); throw 0; }
void CChangingStateComponent::ChangeState(unsigned int) { assert(!"unexpected CChangingStateComponent call: ChangeState"); throw 0; }
NUISystem::CUIState * CChangingStateComponent::GetCurrentState() { assert(!"unexpected CChangingStateComponent call: GetCurrentState"); throw 0; }
bool CChangingStateComponent::HasState(unsigned int) { assert(!"unexpected CChangingStateComponent call: HasState"); throw 0; }
bool CChangingStateComponent::ChangedState() { assert(!"unexpected CChangingStateComponent call: ChangedState"); throw 0; }
unsigned int CChangingStateComponent::GetCurrentStateIndex() { assert(!"unexpected CChangingStateComponent call: GetCurrentStateIndex"); throw 0; }
unsigned int CChangingStateComponent::GetTargetStateIndex() const { assert(!"unexpected CChangingStateComponent call: GetTargetStateIndex"); throw 0; }
float CChangingStateComponent::GetUpdateTime() const { assert(!"unexpected CChangingStateComponent call: GetUpdateTime"); throw 0; }
void CChangingStateComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CChangingStateComponent call: AddChildDynamically"); throw 0; }
bool CChangingStateComponent::IsPositionIndependent() const { assert(!"unexpected CChangingStateComponent call: IsPositionIndependent"); throw 0; }
bool CChangingStateComponent::IsZoomIndependent() const { assert(!"unexpected CChangingStateComponent call: IsZoomIndependent"); throw 0; }
void CChangingStateComponent::ProcessChangeState() { assert(!"unexpected CChangingStateComponent call: ProcessChangeState"); throw 0; }
NUISystem::CUIState * CChangingStateComponent::GetState(unsigned int) { assert(!"unexpected CChangingStateComponent call: GetState"); throw 0; }
const NUISystem::CUIState * CChangingStateComponent::GetState(unsigned int) const { assert(!"unexpected CChangingStateComponent call: GetState"); throw 0; }
void CChangingStateComponent::UpdateStateChange() { assert(!"unexpected CChangingStateComponent call: UpdateStateChange"); throw 0; }
bool CChangingStateComponent::ChangedStateLastUpdate() { assert(!"unexpected CChangingStateComponent call: ChangedStateLastUpdate"); throw 0; }
bool CChangingStateComponent::InternalChanged() { assert(!"unexpected CChangingStateComponent call: InternalChanged"); throw 0; }
bool CChangingStateComponent::AllChildrenChangedState() { assert(!"unexpected CChangingStateComponent call: AllChildrenChangedState"); throw 0; }
void CChangingStateComponent::OnActivate() { assert(!"unexpected CChangingStateComponent call: OnActivate"); throw 0; }
void CChangingStateComponent::OnDeactivate() { assert(!"unexpected CChangingStateComponent call: OnDeactivate"); throw 0; }
void CList::Initialise() { assert(!"unexpected CList call: Initialise"); throw 0; }
void CList::Update(float) { assert(!"unexpected CList call: Update"); throw 0; }
void CList::ChangeState(unsigned int) { assert(!"unexpected CList call: ChangeState"); throw 0; }
void CList::ProcessEvent(NUISystem::EEvent) { assert(!"unexpected CList call: ProcessEvent"); throw 0; }
int CList::GetSelectedChild() { assert(!"unexpected CList call: GetSelectedChild"); throw 0; }
void CList::RemoveChild(unsigned int) { assert(!"unexpected CList call: RemoveChild"); throw 0; }
void CList::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddChildDynamically"); throw 0; }
void CList::AddScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddScrollingChildDynamically"); throw 0; }
void CList::Die() { assert(!"unexpected CList call: Die"); throw 0; }
void CList::AddScrollingChild(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddScrollingChild"); throw 0; }
void CList::AddNonScrollingChild(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddNonScrollingChild"); throw 0; }
void CList::AddNonScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddNonScrollingChildDynamically"); throw 0; }
C2DVector CList::GetPositionOffset() { assert(!"unexpected CList call: GetPositionOffset"); throw 0; }
float CList::GetAlphaOffset() { assert(!"unexpected CList call: GetAlphaOffset"); throw 0; }
void CList::SetLastInputTimeToBeNow() { assert(!"unexpected CList call: SetLastInputTimeToBeNow"); throw 0; }
void CList::SetNextScrollingChildAlpha(float) { assert(!"unexpected CList call: SetNextScrollingChildAlpha"); throw 0; }
void CList::SetNextScrollingChildPosition(C2DVector &) { assert(!"unexpected CList call: SetNextScrollingChildPosition"); throw 0; }
void CList::AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddScrollingChildAndSetOffsets"); throw 0; }
void CList::AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { assert(!"unexpected CList call: AddScrollingChildDynamicallyAndSetOffsets"); throw 0; }
void CList::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CList call: AddActionOnInactive"); throw 0; }
void CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CList call: AddActionOnSelected"); throw 0; }
void CList::SetActionOnScrollUp(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CList call: SetActionOnScrollUp"); throw 0; }
void CList::SetActionOnScrollDown(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CList call: SetActionOnScrollDown"); throw 0; }
void CList::InitialiseOffsets(unsigned int) { assert(!"unexpected CList call: InitialiseOffsets"); throw 0; }
void CList::SetDefaultChild(int) { assert(!"unexpected CList call: SetDefaultChild"); throw 0; }
void CList::SetDefaultChild(NUISystem::CComponent *) { assert(!"unexpected CList call: SetDefaultChild"); throw 0; }
NUISystem::CListState CList::SaveState() { assert(!"unexpected CList call: SaveState"); throw 0; }
void CList::RestoreState(NUISystem::CListState &) { assert(!"unexpected CList call: RestoreState"); throw 0; }
void CList::SetSelectedChild(int) { assert(!"unexpected CList call: SetSelectedChild"); throw 0; }
bool CList::AllScrollingChildrenDeleting() { assert(!"unexpected CList call: AllScrollingChildrenDeleting"); throw 0; }
std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetScrollingChildren() { assert(!"unexpected CList call: GetScrollingChildren"); throw 0; }
std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetNonScrollingChildren() { assert(!"unexpected CList call: GetNonScrollingChildren"); throw 0; }
void CList::ScrollUp(bool,bool) { assert(!"unexpected CList call: ScrollUp"); throw 0; }
void CList::ScrollUp(unsigned int,unsigned int,bool,bool,bool) { assert(!"unexpected CList call: ScrollUp"); throw 0; }
void CList::ScrollDown(bool,bool) { assert(!"unexpected CList call: ScrollDown"); throw 0; }
void CList::ScrollDown(unsigned int,unsigned int,bool,bool,bool) { assert(!"unexpected CList call: ScrollDown"); throw 0; }
bool CList::AreThereAnyDeletions() { assert(!"unexpected CList call: AreThereAnyDeletions"); throw 0; }
bool CList::IsWrapping() const { assert(!"unexpected CList call: IsWrapping"); throw 0; }
bool CList::IsScrolling() const { assert(!"unexpected CList call: IsScrolling"); throw 0; }
bool CList::IsInverted() const { assert(!"unexpected CList call: IsInverted"); throw 0; }
void CList::ScrollUntilChild(NUISystem::CComponent *) { assert(!"unexpected CList call: ScrollUntilChild"); throw 0; }
void CList::ScrollUntilChildContaining(NUISystem::CComponent *) { assert(!"unexpected CList call: ScrollUntilChildContaining"); throw 0; }
C2DVector CList::CanMove(const C2DVector &) { assert(!"unexpected CList call: CanMove"); throw 0; }
void CList::ChangePosition(const C2DVector &,float,bool) { assert(!"unexpected CList call: ChangePosition"); throw 0; }
NUISystem::CComponent * CList::GetScrollBar() { assert(!"unexpected CList call: GetScrollBar"); throw 0; }
NUISystem::CComponent * CList::GetScrollBarOutside() { assert(!"unexpected CList call: GetScrollBarOutside"); throw 0; }
void CList::SetInitialPositionDummy(NUISystem::CComponent *) { assert(!"unexpected CList call: SetInitialPositionDummy"); throw 0; }
void CList::ScrollToFirstChild() { assert(!"unexpected CList call: ScrollToFirstChild"); throw 0; }
void CList::ScrollToLastChild() { assert(!"unexpected CList call: ScrollToLastChild"); throw 0; }
void CList::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { assert(!"unexpected CList call: ExecuteActions"); throw 0; }
void CList::SetNonScrollingChildrenStates() { assert(!"unexpected CList call: SetNonScrollingChildrenStates"); throw 0; }
void CList::SetRecomputeOffsets(bool) { assert(!"unexpected CList call: SetRecomputeOffsets"); throw 0; }
void CList::ScrollUpProcess() { assert(!"unexpected CList call: ScrollUpProcess"); throw 0; }
void CList::ScrollDownProcess() { assert(!"unexpected CList call: ScrollDownProcess"); throw 0; }
void CList::OnActivate() { assert(!"unexpected CList call: OnActivate"); throw 0; }
void CList::DoRecomputeOffsets() { assert(!"unexpected CList call: DoRecomputeOffsets"); throw 0; }
void CList::ComputeChildrenVisibility() { assert(!"unexpected CList call: ComputeChildrenVisibility"); throw 0; }
void CList::ComputeScrollBarPosition() { assert(!"unexpected CList call: ComputeScrollBarPosition"); throw 0; }
void CList::ComputeScrollBarZoom() { assert(!"unexpected CList call: ComputeScrollBarZoom"); throw 0; }
void CObservable::SetEvent(NUISystem::EEvent) { assert(!"unexpected CObservable call: SetEvent"); throw 0; }
void CObservable::SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent) { assert(!"unexpected CObservable call: SetOwnedEvent"); throw 0; }
void CObservable::AddObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: AddObserver"); throw 0; }
void CObservable::AddExclusiveObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: AddExclusiveObserver"); throw 0; }
void CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: RemoveExclusiveObserver"); throw 0; }
void CObservable::RemoveObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: RemoveObserver"); throw 0; }
void CObservable::ClearObservers() { assert(!"unexpected CObservable call: ClearObservers"); throw 0; }
NUISystem::CObserver * CObservable::GetExclusiveObserver() { assert(!"unexpected CObservable call: GetExclusiveObserver"); throw 0; }
bool CObservable::IsExclusivelyObserved() { assert(!"unexpected CObservable call: IsExclusivelyObserved"); throw 0; }
void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: AddConcurrentExclusiveObserver"); throw 0; }
void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { assert(!"unexpected CObservable call: RemoveConcurrentExclusiveObserver"); throw 0; }
bool CManager::GetMouseDisabled() { assert(!"unexpected CManager call: GetMouseDisabled"); throw 0; }
void CManager::SetKeyPressed(unsigned short) { assert(!"unexpected CManager call: SetKeyPressed"); throw 0; }
unsigned short CManager::GetKeyPressed() { assert(!"unexpected CManager call: GetKeyPressed"); throw 0; }
void CManager::Action(CCountedPointer<NUISystem::CAction> &) { assert(!"unexpected CManager call: Action"); throw 0; }
void CManager::SetWeaponsInventory(CTCInventoryWeapons *) { assert(!"unexpected CManager call: SetWeaponsInventory"); throw 0; }
void CManager::SetMapInventory(CTCInventoryMap *) { assert(!"unexpected CManager call: SetMapInventory"); throw 0; }
void CManager::SetQuestInventory(CTCInventoryQuests *) { assert(!"unexpected CManager call: SetQuestInventory"); throw 0; }
void CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { assert(!"unexpected CManager call: SetTCHeroExperienceInventory"); throw 0; }
void CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { assert(!"unexpected CManager call: SetAbilitiesInventory"); throw 0; }
void CManager::SetExperienceInventory(CTCInventoryExperience *) { assert(!"unexpected CManager call: SetExperienceInventory"); throw 0; }
void CManager::SetInventory(CTCInventory *) { assert(!"unexpected CManager call: SetInventory"); throw 0; }
void CManager::SetInventoryClothing(CTCInventoryClothing *) { assert(!"unexpected CManager call: SetInventoryClothing"); throw 0; }
void CManager::SetInventoryTrade(CTCInventoryTrade *) { assert(!"unexpected CManager call: SetInventoryTrade"); throw 0; }
void CManager::AddThingActionParam(int) { assert(!"unexpected CManager call: AddThingActionParam"); throw 0; }
void CManager::RemoveThingActionParam(int) { assert(!"unexpected CManager call: RemoveThingActionParam"); throw 0; }
void CManager::SetPlayerThing(CThing *) { assert(!"unexpected CManager call: SetPlayerThing"); throw 0; }
void CManager::AddUIActionParam(NUISystem::CActionParam *) { assert(!"unexpected CManager call: AddUIActionParam"); throw 0; }
void CManager::RemoveUIActionParam(NUISystem::CActionParam *) { assert(!"unexpected CManager call: RemoveUIActionParam"); throw 0; }
void CManager::ClearActionParams() { assert(!"unexpected CManager call: ClearActionParams"); throw 0; }
void CManager::SetNumberAugmentations(unsigned int) { assert(!"unexpected CManager call: SetNumberAugmentations"); throw 0; }
unsigned int CManager::GetNumberAugmentations() const { assert(!"unexpected CManager call: GetNumberAugmentations"); throw 0; }
void CManager::SetChange(NUISystem::CManager::EChange) { assert(!"unexpected CManager call: SetChange"); throw 0; }
NUISystem::CManager::EChange CManager::GetChange() { assert(!"unexpected CManager call: GetChange"); throw 0; }
void CManager::SetMetaLayer(int) { assert(!"unexpected CManager call: SetMetaLayer"); throw 0; }
int CManager::GetMetaLayer() { assert(!"unexpected CManager call: GetMetaLayer"); throw 0; }
EEngine2DLayer CManager::GetLayer(int) { assert(!"unexpected CManager call: GetLayer"); throw 0; }
void CManager::ClearBastardChild() { assert(!"unexpected CManager call: ClearBastardChild"); throw 0; }
void CManager::SetRightStickForce(C2DVector &) { assert(!"unexpected CManager call: SetRightStickForce"); throw 0; }
C2DVector & CManager::GetRightStickForce() { assert(!"unexpected CManager call: GetRightStickForce"); throw 0; }
void CManager::PlaySoundW(CCharString &,int) { assert(!"unexpected CManager call: PlaySoundW"); throw 0; }
unsigned int CManager::PlaySample(CCharString &,int) { assert(!"unexpected CManager call: PlaySample"); throw 0; }
CDefPointer<NUISystem::CUIMiscThingsDef> CManager::GetMiscThingsDef() { assert(!"unexpected CManager call: GetMiscThingsDef"); throw 0; }
CDefPointer<NUISystem::CUIIconsDef> CManager::GetIconsDef() { assert(!"unexpected CManager call: GetIconsDef"); throw 0; }
void CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent) { assert(!"unexpected CManager call: AddActionsListenIgnoreOnList"); throw 0; }
}
void CClass::Save(CCharString&) { assert(!"unexpected CClass call: Save"); throw 0; }
void CTCBase::OnSerialise(CPersistContext &,bool) { assert(!"unexpected CTCBase call: OnSerialise"); throw 0; }
void CTCBase::OnRemove() { assert(!"unexpected CTCBase call: OnRemove"); throw 0; }
void CTCBase::ConstructFromParams(const CTCParamsBase &) { assert(!"unexpected CTCBase call: ConstructFromParams"); throw 0; }
void CTCBase::OnCreate() { assert(!"unexpected CTCBase call: OnCreate"); throw 0; }
void CTCBase::InitialActivate() { assert(!"unexpected CTCBase call: InitialActivate"); throw 0; }
void CTCBase::OnInitialActivate() { assert(!"unexpected CTCBase call: OnInitialActivate"); throw 0; }
void CTCBase::RenderUpdate(float) { assert(!"unexpected CTCBase call: RenderUpdate"); throw 0; }
bool CTCBase::HasFrameUpdate() const { assert(!"unexpected CTCBase call: HasFrameUpdate"); throw 0; }
void CTCBase::FrameUpdate() { assert(!"unexpected CTCBase call: FrameUpdate"); throw 0; }
void CTCBase::BulletTimeFrameUpdate() { assert(!"unexpected CTCBase call: BulletTimeFrameUpdate"); throw 0; }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &) { assert(!"unexpected CTCBase call: DrawPreparePrimitivesForRendering"); throw 0; }
void CTCBase::Copy(const CTCBase &) { assert(!"unexpected CTCBase call: Copy"); throw 0; }
void CTCBase::OnDie() { assert(!"unexpected CTCBase call: OnDie"); throw 0; }
void CTCBase::OnKill() { assert(!"unexpected CTCBase call: OnKill"); throw 0; }
void CTCBase::OnLevelUnload() { assert(!"unexpected CTCBase call: OnLevelUnload"); throw 0; }
void CTCBase::OnSetInLimbo(bool) { assert(!"unexpected CTCBase call: OnSetInLimbo"); throw 0; }
bool CTCBase::Validate(CCharString &) const { assert(!"unexpected CTCBase call: Validate"); throw 0; }
bool CTCBase::IsFreeForDeletion() const { assert(!"unexpected CTCBase call: IsFreeForDeletion"); throw 0; }
bool CTCBase::IsSerialiseCreatable() const { assert(!"unexpected CTCBase call: IsSerialiseCreatable"); throw 0; }
bool CTCBase::IsSerialisableForHeroState() const { assert(!"unexpected CTCBase call: IsSerialisableForHeroState"); throw 0; }
ETCInterfaceType CTCBase::GetInterfaceType() const { assert(!"unexpected CTCBase call: GetInterfaceType"); throw 0; }
CCharString CTCBase::GetClassTypeName() const { assert(!"unexpected CTCBase call: GetClassTypeName"); throw 0; }
bool CTCBase::IsComponentSerialisable() const { assert(!"unexpected CTCBase call: IsComponentSerialisable"); throw 0; }
bool CTCBase::IsDriver() const { assert(!"unexpected CTCBase call: IsDriver"); throw 0; }
bool CTCBase::IsCopyable() const { assert(!"unexpected CTCBase call: IsCopyable"); throw 0; }
void CTCBase::GlobalUpdate(bool) { assert(!"unexpected CTCBase call: GlobalUpdate"); throw 0; }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase &) { assert(!"unexpected CTCBase call: HandleMessage"); throw 0; }
ETCInterfaceType CTCTargetingBase::GetInterfaceType() const { assert(!"unexpected CTCTargetingBase call: GetInterfaceType"); throw 0; }
void CTCTargetingBase::OnInitialActivate() { assert(!"unexpected CTCTargetingBase call: OnInitialActivate"); throw 0; }
void CTCTargetingBase::BulletTimeFrameUpdate() { assert(!"unexpected CTCTargetingBase call: BulletTimeFrameUpdate"); throw 0; }
CThing * CTCTargetingBase::GetPBestThingForMeleeAttack() const { assert(!"unexpected CTCTargetingBase call: GetPBestThingForMeleeAttack"); throw 0; }
CThingCreatureBase * CTCTargetingBase::GetPBestEnemyCreatureForAttack() const { assert(!"unexpected CTCTargetingBase call: GetPBestEnemyCreatureForAttack"); throw 0; }
CThing * CTCTargetingBase::GetPBestThingForProjectileTargetingLock() const { assert(!"unexpected CTCTargetingBase call: GetPBestThingForProjectileTargetingLock"); throw 0; }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float) const { assert(!"unexpected CTCTargetingBase call: GetPBestCreatureForInteractionTypeWithinRangeLimit"); throw 0; }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionType(ECreatureInteractionType) const { assert(!"unexpected CTCTargetingBase call: GetPBestCreatureForInteractionType"); throw 0; }
CThing * CTCTargetingBase::GetPTargetedThing() const { assert(!"unexpected CTCTargetingBase call: GetPTargetedThing"); throw 0; }
void CTCTargetingBase::SetPTargetedThing(CThing *) { assert(!"unexpected CTCTargetingBase call: SetPTargetedThing"); throw 0; }
CThing * CTCTargetingBase::GetPProjectileTargetedThing() const { assert(!"unexpected CTCTargetingBase call: GetPProjectileTargetedThing"); throw 0; }
CThing * CTCTargetingBase::GetPTargetLockedThing() const { assert(!"unexpected CTCTargetingBase call: GetPTargetLockedThing"); throw 0; }
bool CTCTargetingBase::GetTargetedPos(C3DVector &,float) const { assert(!"unexpected CTCTargetingBase call: GetTargetedPos"); throw 0; }
bool CTCTargetingBase::IsTargetedThingInRange(unsigned int) const { assert(!"unexpected CTCTargetingBase call: IsTargetedThingInRange"); throw 0; }
void CTCTargetingBase::UpdateNearestInterestingThing() { assert(!"unexpected CTCTargetingBase call: UpdateNearestInterestingThing"); throw 0; }
ETCInterfaceType CTCQuickAccessMenu::GetInterfaceType() const { assert(!"unexpected CTCQuickAccessMenu call: GetInterfaceType"); throw 0; }
CCharString CTCQuickAccessMenu::GetClassTypeName() const { assert(!"unexpected CTCQuickAccessMenu call: GetClassTypeName"); throw 0; }
void CTCQuickAccessMenu::OnCreate() { assert(!"unexpected CTCQuickAccessMenu call: OnCreate"); throw 0; }
void CTCQuickAccessMenu::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &) { assert(!"unexpected CTCQuickAccessMenu call: DrawPreparePrimitivesForRendering"); throw 0; }
void CTCQuickAccessMenu::GlobalUpdate(bool) { assert(!"unexpected CTCQuickAccessMenu call: GlobalUpdate"); throw 0; }


static int live_chars=0,live_wide=0,live_components=0,deleted_arrows=0,unavailable_calls=0,constructions=0,selected_calls=0,build_calls=0;
static int item_count=0,target_mode=0,gifts=0,arrows=0,seed=0,expected_unique=0,expected_total=0;static bool expected_usable;
static int keys[12],amounts[12],aggregate_keys[12],aggregate_amounts[12];
static char* copy_text(const char* p){size_t n=strlen(p);char* q=(char*)malloc(n+1);assert(q);memcpy(q,p,n+1);return q;}
static const char* ctext(const CCharString& s){return s.PStringData?(const char*)s.PStringData:"";}
static const char* wtext(const CWideString& s){return s.PStringData?(const char*)s.PStringData:"";}
CCharString::CCharString():PStringData(0){++live_chars;}
CCharString::CCharString(const char* p,int n):PStringData(0){if(n<0)n=(int)strlen(p);char* q=(char*)malloc(n+1);assert(q);memcpy(q,p,n);q[n]=0;PStringData=(CCharStringData*)q;++live_chars;}
CCharString::CCharString(const CCharString& s):PStringData((CCharStringData*)copy_text(ctext(s))){++live_chars;}
CCharString::~CCharString(){free(PStringData);--live_chars;}
CWideString::CWideString():PStringData(0){++live_wide;}
CWideString::CWideString(const CWideString& s):PStringData((CWideStringData*)copy_text(wtext(s))){++live_wide;}
CWideString::~CWideString(){free(PStringData);--live_wide;}
const CWideString& CWideString::operator+=(const CWideString& other){size_t first=strlen(wtext(*this)),second=strlen(wtext(other));char* q=(char*)malloc(first+second+1);assert(q);memcpy(q,wtext(*this),first);memcpy(q+first,wtext(other),second+1);free(PStringData);PStringData=(CWideStringData*)q;return *this;}
void __cdecl CWideString::Sprintf(const unsigned short* format,...){assert(format[0]=='%'&&format[1]=='d'&&format[2]==0);va_list args;va_start(args,format);int n=va_arg(args,int);va_end(args);char buffer[32];sprintf(buffer,"%d",n);free(PStringData);PStringData=(CWideStringData*)copy_text(buffer);}
CWideString __fastcall CTCInventoryStats::GetXString(){CWideString value;value.PStringData=(CWideStringData*)copy_text(" x ");CWideString explicit_copy(value);return explicit_copy;}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){assert(IntelligentPointer==0);}void CBaseObject::Validate()const{}
CThing::CThing(unsigned int number):UniqueID(number){memset(&UsingComponents,0,sizeof(UsingComponents));Dead=0;}
CTCBase::CTCBase(CThing& thing):ParentThing(thing),ToBeRemoved(false),IsInGlobalUpdateTCs(false){}CTCBase::~CTCBase(){}
CTCTargetingBase::CTCTargetingBase(CThing& thing):CTCBase(thing){}CTCTargetingBase::~CTCTargetingBase(){}
CTCQuickAccessMenu::CTCQuickAccessMenu(CThing& thing):CTCBase(thing),Active(false),Quitting(false),LastRenderTime(0),FirstRenderFrame(false){}
CTCQuickAccessMenu::~CTCQuickAccessMenu(){}
namespace std {
void __stl_throw_out_of_range(const char*){assert(false);abort();}
template<class T,class A>list<T,A>::~list(){}
template<class T,class A>void _List_base<T,A>::clear(){_List_node<T>* p=(_List_node<T>*)_M_node._M_data->_M_next;while(p!=_M_node._M_data){_List_node<T>* next=(_List_node<T>*)p->_M_next;p->_M_data.~T();_M_node.deallocate(p,1);p=next;}_M_node._M_data->_M_next=_M_node._M_data;_M_node._M_data->_M_prev=_M_node._M_data;}
template<class T,class A>template<class I>void list<T,A>::_M_insert_dispatch(iterator position,I first,I last,const __false_type&){for(;first!=last;++first)insert(position,*first);}
template<class K,class V,class KeyOfValue,class Compare,class A>pair<typename _Rb_tree<K,V,KeyOfValue,Compare,A>::iterator,bool>_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(const V& value){
 _Rb_tree_node<V>* head=this->_M_header._M_data;_Rb_tree_node_base** link=&head->_M_parent;_Rb_tree_node_base* parent=head;
 while(*link){_Rb_tree_node<V>* node=(_Rb_tree_node<V>*)*link;if(KeyOfValue()(node->_M_value_field)==KeyOfValue()(value))return pair<iterator,bool>(iterator(node),false);parent=node;link=_M_key_compare(KeyOfValue()(value),KeyOfValue()(node->_M_value_field))?&node->_M_left:&node->_M_right;}
 _Rb_tree_node<V>* node=(_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));assert(node);node->_M_parent=parent;node->_M_left=node->_M_right=0;node->_M_color=true;new(&node->_M_value_field)V(value);*link=node;++_M_node_count;
 if(_M_node_count==1)head->_M_left=head->_M_right=node;else{if(_M_key_compare(KeyOfValue()(value),KeyOfValue()(((_Rb_tree_node<V>*)head->_M_left)->_M_value_field)))head->_M_left=node;if(_M_key_compare(KeyOfValue()(((_Rb_tree_node<V>*)head->_M_right)->_M_value_field),KeyOfValue()(value)))head->_M_right=node;}
 return pair<iterator,bool>(iterator(node),true);
}
}
namespace NUISystem {
CUIInterface::~CUIInterface(){}CObserver::CObserver(){memset(EventsToObserve,0,sizeof(EventsToObserve));PreventObservation=false;}CObserver::~CObserver(){}
CActionParam::CActionParam():ActionParamType(UI_ACTION_PARAM_TYPE_NONE){}CActionParam::~CActionParam(){}
CComponent::CComponent(int id):DefIndex(id){++live_components;}CComponent::~CComponent(){--live_components;}
CChangingStateComponent::CChangingStateComponent(int id):CComponent(id){}CChangingStateComponent::~CChangingStateComponent(){}
CList::CList(int id):CChangingStateComponent(id){}CList::~CList(){}
CManager::CManager(){}CManager::~CManager(){}
}
class QuickTarget:public CTCTargetingBase{public:CThing* target;QuickTarget(CThing& thing):CTCTargetingBase(thing),target(0){}CThing* GetPTargetedThing()const{return target;}CCharString GetClassTypeName()const{return CCharString("fixture");}};
class MenuFixture:public NUISystem::CList{
public:int kind;MenuFixture* child;MenuFixture(int id):NUISystem::CList(id),kind(id),child(0){}
 ~MenuFixture(){delete child;}
 NUISystem::CComponent* FindChild(CCharString name){
  if(kind==1){assert(strcmp(ctext(name),"UI_SELECTION_ARROW_RIGHT")==0);return arrows?child:0;}
  if(kind==3){assert(strcmp(ctext(name),"UI_MENU_ENTRY_QUICK_ACCESS_ITEM_ACTION_NOT_USE")==0&&!expected_usable);return child;}
  assert(kind==4&&strcmp(ctext(name),"UI_SELECTION_ARROW_RIGHT_FOR_QUICK_ACCESS")==0);return arrows?child:0;
 }
 void SetDeletion(NUISystem::CDeletion deletion){assert(kind==5&&deletion.Method==NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE&&deletion.AssociatedParents.empty());++deleted_arrows;}
 void MakeUnavailable(){assert(kind==1||kind==4);++unavailable_calls;}
 void AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent>& item){assert(kind==0&&item.Get());Children.push_back(item);}
 void AddChild(CCountedPointer<NUISystem::CComponent>& item){assert(kind==1&&item.Get());Children.push_back(item);}
 void InitialiseOffsets(unsigned int count){assert(kind==2&&count==(unsigned int)expected_unique);++constructions;}
 void SetDefaultChild(int index){assert(kind==3&&index==0);++selected_calls;}
};
static NUISystem::CManager* manager;static MenuFixture* root_list;static MenuFixture* category_list;static NUISystem::CComponent* top_list;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance(){assert(manager);return manager;}
NUISystem::CComponent* NUISystem::CManager::CreateMenuEntryComponent(const CWideString& title,float first,float second,const CCharString& pattern,float third,const CCharString blank,float fourth,CCharString button){
 assert(this==manager&&first==0.0f&&second==1.0f&&third==0.0f&&fourth==0.0f&&strcmp(ctext(pattern),"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW_FOR_QUICK_ACCESS")==0&&strcmp(ctext(blank),"")==0&&strcmp(ctext(button),"PC_BUTTON_BASTARD")==0);
 char expected[80];if(item_count)sprintf(expected,"Category x %d",expected_total);else strcpy(expected,"Category");assert(strcmp(wtext(title),expected)==0);MenuFixture* result=new MenuFixture(1);result->child=new MenuFixture(5);return result;
}
NUISystem::CComponent* NUISystem::CManager::CreateComponent(const CCharString& name,bool flag){assert(this==manager&&!flag);if(strcmp(ctext(name),"UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS")==0){category_list=new MenuFixture(2);return category_list;}
 assert(strcmp(ctext(name),expected_usable?"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS":"UI_QUICK_ACCESS_LIST_ITEMS_ACTIONS_CANT_USE")==0);MenuFixture* result=new MenuFixture(3);result->child=new MenuFixture(4);result->child->child=new MenuFixture(5);return result;
}
void CTCQuickAccessMenu::ContructItemLists(std::map<int,int>& values,NUISystem::CList* category,NUISystem::CComponent* top,CCountedPointer<NUISystem::CComponent>& actions){
 assert(category==category_list&&top==top_list&&values.size()==(unsigned int)expected_unique&&actions.Get()&&selected_calls==1);for(int i=0;i<expected_unique;++i){std::map<int,int>::iterator found=values.find(aggregate_keys[i]);assert(found!=values.end()&&found->second==aggregate_amounts[i]);}category->Children.push_back(actions);++build_calls;
}
int main(){int cases=0;int counts[7]={0,1,2,3,5,8,12};for(int group=0;group<7;++group)for(target_mode=0;target_mode<4;++target_mode)for(gifts=0;gifts<2;++gifts)for(arrows=0;arrows<2;++arrows)for(seed=0;seed<3;++seed){
 item_count=counts[group];expected_unique=expected_total=0;deleted_arrows=unavailable_calls=constructions=selected_calls=build_calls=0;expected_usable=!gifts||target_mode==0||target_mode==2;
 for(int i=0;i<item_count;++i){keys[i]=seed==0?i-5:seed==1?i%3-1:7-i%4;amounts[i]=(i+seed)%5;expected_total+=amounts[i];int at=0;while(at<expected_unique&&aggregate_keys[at]!=keys[i])++at;if(at==expected_unique){aggregate_keys[at]=keys[i];aggregate_amounts[at]=0;++expected_unique;}aggregate_amounts[at]+=amounts[i];}
 {NUISystem::CManager manager_object;manager=&manager_object;CThing owner(1),target(2);target.Dead=target_mode==3;QuickTarget targeting(owner);targeting.target=target_mode==1?0:&target;if(target_mode){owner.UsingComponents._M_w[0]|=1UL<<8;owner.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>((ETCInterfaceType)7,0));owner.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>((ETCInterfaceType)8,&targeting));owner.Components.push_back(std::pair<ETCInterfaceType,CTCBase*>((ETCInterfaceType)9,0));}
  CTCQuickAccessMenu quick(owner);MenuFixture root(0),top(6);root_list=&root;top_list=&top;CWideString title;title.PStringData=(CWideStringData*)copy_text("Category");std::vector<std::pair<int,int> > items;for(int i=0;i<item_count;++i)items.push_back(std::pair<int,int>(keys[i],amounts[i]));
  quick.ConstructItemCategoryList(title,&root,items,&top,gifts!=0);assert(strcmp(wtext(title),"Category")==0&&items.size()==(unsigned int)item_count&&root.Children.size()==1);for(int i=0;i<item_count;++i)assert(items[i].first==keys[i]&&items[i].second==amounts[i]);
  bool disabled=item_count==0||!expected_usable;assert(deleted_arrows==(disabled&&arrows?1:0)&&unavailable_calls==(disabled?1:0)&&constructions==(item_count?1:0)&&selected_calls==(item_count?1:0)&&build_calls==(item_count?1:0));
 }assert(live_chars==0&&live_wide==0&&live_components==0);++cases;
 }assert(cases==336);puts("00626e40_TEST PASS");return 0;}