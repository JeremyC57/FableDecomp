// CQ_FireHeartScript::CQ_FireHeartScript(void* data, CGameScriptInterface* game) -- quest-script constructor: script base, data/interface pair, vtable 0x012CCB90, the two empty cutscene maps, empty effect handle and FX strings, the empty bool/thing/handle/pattern vectors, three empty scripted-thing handles, an empty comment string and escape marker
// Leaf: ??0CQ_FireHeartScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D34470, 489 bytes (496 with padding), `ret 8`; the 0x1A4-byte object is built by
//   the factory at 0x00D3AFC2 (operator new 0x1A4).
// Label: manifest "CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript" is propagated; the member layout (two maps, a
//   CScriptThing at +0xa8, two strings, nine vectors, a vector of handles, three scripted-thing handles, a string and a
//   CScriptThing, 0x1A4 bytes) is Ego_r.pdb CQ_FireHeartScript (retail = PDB - 0x14 with 12-byte maps and vectors).
// Callees: CScriptBase::CScriptBase 0x00CB8110, malloc 0x00BFEA0E (map headers, 0x18 bytes), CBaseClass::CBaseClass
//   0x0099A2D0 (CScriptThing root, vtable 0x01238C8C), CCharString::CCharString() 0x0099E4B0, CBaseObject::CBaseObject
//   0x0099A380 (CScriptGameResourceObjectScriptedThingBase, vtable 0x0127094C).
// Shape: the novtable script-object base of the landed CQ_HangingTreeGoodScript ctor 0x00D6EF20 (arguments stored
//   before the vtable); the STLport map model of the landed CTCHeroOnlineScoreboard ctor 0x00562EC0; the scripted-
//   thing handle of the landed CRockTroll::Main 0x00DA5F40; every zero word shares ebx.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
class CGameScriptInterface;

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};
struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pData;                                   // +0x44
    CScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pData(data) { }
};
class CBaseClass {
public:
    CBaseClass();                                         // 0x0099A2D0
    virtual ~CBaseClass();                                // 0x0099A2E0
};
class CScriptThing;
class CCPPointerInfo;
struct CCountedPointer_CScriptThing {                // CScriptThing::PImp
    CScriptThing* Data;                              // +0
    CCPPointerInfo* Info;                            // +4
    CCountedPointer_CScriptThing() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:                        // vtable 0x01238C8C
    CCountedPointer_CScriptThing PImp;               // +0x04
    CScriptThing() { }
    virtual ~CScriptThing();
};
class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};
class CBaseObject { public:                                 // CBaseIntelligentPointer root (vtable 0x0129A7D8, +4 nulled)
    virtual void b00();
    void* m_pIntelligentPointer;                     // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointerWords PImp;                       // +0x08
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
};
template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;                          // +0x00
    SRbTreeNodeBase* _M_parent;                         // +0x04
    SRbTreeNodeBase* _M_left;                           // +0x08
    SRbTreeNodeBase* _M_right;                          // +0x0c
};
template <int SIZE> struct STreeNode : SRbTreeNodeBase {
    unsigned char _M_value_field[SIZE - 0x10];
};
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)malloc(n * sizeof(N)) : 0; }
};
template <int SIZE> struct CTreeBase {                  // STLport _Rb_tree_base
    CNodeAllocProxy< STreeNode<SIZE> > _M_header;
    CTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
template <int SIZE> struct CMap : CTreeBase<SIZE> {     // map<...>, 12 bytes
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    CMap() : _M_node_count(0)
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};

class CFireHeartPatternDef;

namespace NScript {
class CQ_FireHeartScript : public CScriptObject {
public:          // vtable 0x012CCB90, 0x1A4 bytes
    bool  QuestStartScreenShown;                     // +0x48
    bool  MissionSucceeded;                          // +0x49
    bool  MissionFailed;                             // +0x4a
    CMap<0x18> CutsceneFlags;                        // +0x4c map<CCharString, bool>
    CMap<0x18> csargs;                               // +0x58 map<CCharString, CCharString>
    long  FreedProphets;                             // +0x64
    long  KilledProphets;                            // +0x68
    long  StartNumberOfSuns;                         // +0x6c
    long  NumberOfRounds;                            // +0x70
    long  NumberOfPatterns;                          // +0x74
    long  NumberOfProphets;                          // +0x78
    long  NumberOfPads;                              // +0x7c
    long  NumberOfSuns;                              // +0x80
    long  NumberOfVillagers;                         // +0x84
    long  PuzzleRound;                               // +0x88
    bool  ResetBlocks;                               // +0x8c
    long  NumBlocksReset;                            // +0x90
    bool  RoundEnded;                                // +0x94
    bool  RoundBeginning;                            // +0x95
    bool  PreventRoundEnd;                           // +0x96
    bool  HasHeroHitVillagersGroupOne;               // +0x97
    bool  HasHeroHitVillagersGroupTwo;               // +0x98
    bool  InfoDisplayed;                             // +0x99
    long  EvilRounds;                                // +0x9c
    long  GoodRounds;                                // +0xa0
    float PadRadius;                                 // +0xa4
    CScriptThing Effect;                             // +0xa8
    CCharString  PadSunFX;                           // +0xb4
    CCharString  PadMoonFX;                          // +0xb8
    vector<bool> StartingPattern;                    // +0xbc (retail vector<bool> is a plain 12-byte vector)
    vector<bool> ProphetsKilled;                     // +0xc8
    vector<bool> ProphetsReleased;                   // +0xd4
    vector<bool> ShouldProphetSpeak;                 // +0xe0
    vector<bool> ShouldVillagerSpeak;                // +0xec
    vector<bool> CommentGivenThisRound;              // +0xf8
    vector<CScriptThing> Prophets;                   // +0x104
    vector<CScriptThing> Villagers;                  // +0x110
    vector<CScriptThing> Followers;                  // +0x11c
    vector<CScriptGameResourceObjectScriptedThingBase> FollowerHandles;   // +0x128
    vector<CFireHeartPatternDef> Pattern;            // +0x134
    long  Guest;                                     // +0x140
    long  ProphetsScripted;                          // +0x144
    vector<CScriptGameResourceObjectScriptedThingBase> seh_Prophet;       // +0x148
    CScriptGameResourceObjectScriptedThingBase seh_BriarRose;             // +0x154
    CScriptGameResourceObjectScriptedThingBase seh_Scythe;                // +0x164
    CScriptGameResourceObjectScriptedThingBase seh_GuildMaster;           // +0x174
    long  MissionHistory;                            // +0x184 EMissionHistory
    CCharString  ProphetComment;                     // +0x188
    CScriptThing VillagerEscapeMarker;               // +0x18c
    float MoralityChangeOnKilled;                    // +0x198
    float MoralityChangeOnFreed;                     // +0x19c
    bool  HasDemonDoorBeenEnabled;                   // +0x1a0
    bool  HasScytheCutscenePlayed;                   // +0x1a1
    bool  KilledAllPrisoners;                        // +0x1a2
    bool  FreedAllPrisoners;                         // +0x1a3
    virtual void v00();
    CQ_FireHeartScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_FireHeartScript;

NScript::CQ_FireHeartScript::CQ_FireHeartScript(void* data, CGameScriptInterface* game)
    : CScriptObject(data, game)
{
}