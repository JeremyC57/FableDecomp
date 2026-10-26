// CTCQuestCompletionUI::CTCQuestCompletionUI(CThing* parent) -- quest-completion UI thing component ctor: flags cleared, empty component vector, the boast list and the three start-quest maps built, strings default-constructed, counters zeroed, CanTeleportToGuildMember set; registered for updates
// Leaf: ??0CTCQuestCompletionUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E4030, length 432 bytes (body 0x1a4 + int3 padding), `ret 4`.
// Label correction: the catalog row says CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite (propagated); the vtable
//   stored, 0x0125589C, is CTCQuestCompletionUI (rtti-vtables.tsv), the only caller is 0x004D6302 and the members match the PDB class.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, malloc @ 0x00BFEA0E (STLport node allocator), CWideString::CWideString()
//   @ 0x0099AED0, CCharString::CCharString() @ 0x0099E4B0, CTCBase::AddToGlobalTCs @ 0x00686ED0.
// Layout: Ego_r.pdb CTCQuestCompletionUI; retail STLport containers are 12 bytes (PDB 16), so past Components (+0x10) every member
//   sits 4 (then 8, 12, 16, 20) bytes lower: Quitting +0x1c, Frame +0x24, Boasts +0x30, LastRenderTime +0x48, ScriptName +0x50,
//   GoldReward +0x58, the maps +0x84/+0x90/+0x9c, PRenownText +0xa8, QuittingFadeOut +0xc0, CanTeleportToGuildMember +0xd4.
// Shape: the landed CTCBoastUI (0x0056AC10) / CTCWound (0x00757640) container ctors: the list sentinel malloc'd and linked to itself
//   before the store, each map header nulled, malloc'd, count zeroed, header red/null parent/self-linked; members without an
//   initialiser (WaitingForBoastProcessing, Completion) are left alone.
class CThing;
struct CComponent;
struct CText;
class CQuestCardDef;
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    CTCBase(CThing* parent);                        // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

class CWideString { public: void* PStringData; CWideString(); };   // 0x0099AED0
class CCharString { public: void* PStringData; CCharString(); };   // 0x0099E4B0

template <class T> class CDefPointer { public:
    T* PItem;
    CDefPointer() : PItem(0) { }
};

template <class T> struct CRetailList {             // STLport list: one sentinel-node pointer
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct _Rb_tree_node_base {
    char                 _M_color;                  // +0
    _Rb_tree_node_base*  _M_parent;                 // +4
    _Rb_tree_node_base*  _M_left;                   // +8
    _Rb_tree_node_base*  _M_right;                  // +0xc
};
struct SLongULongNode : _Rb_tree_node_base {        // 0x18 bytes
    long          first;
    unsigned long second;
};
struct CLongULongMap {                              // STLport map<long, unsigned long>
    SLongULongNode* _M_header;                      // +0
    unsigned long   _M_node_count;                  // +4
    unsigned char   _M_key_compare;                 // +8 less<> (empty)
    CLongULongMap()
        : _M_header(0)
    {
        _M_header = (SLongULongNode*)malloc(sizeof(SLongULongNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CComponentVector {                           // STLport vector<CComponent*>
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CBoast { unsigned char _body[0x20]; };       // CQuestManager::CBoast

class CTCQuestCompletionUI : public CTCBase { public:             // vtable 0x0125589C
    bool                       Active;                      // +0x0c
    bool                       ReallyActive;                // +0x0d
    CComponentVector           Components;                  // +0x10
    bool                       Quitting;                    // +0x1c
    bool                       WaitingForRegionLoad;        // +0x1d
    bool                       WaitingForPlayerControl;     // +0x1e
    bool                       WaitingForBoastProcessing;   // +0x1f
    long                       Completion;                  // +0x20
    unsigned long              Frame;                       // +0x24
    long                       FrameStartedBoastProcessing; // +0x28
    CDefPointer<CQuestCardDef> PCardDef;                    // +0x2c
    CRetailList<CBoast>        Boasts;                      // +0x30
    unsigned long              RenderedObjectDef;           // +0x34
    CWideString                QuestSummary;                // +0x38
    CWideString                SuccessSummary;              // +0x3c
    CWideString                QuestObjective;              // +0x40
    double                     LastRenderTime;              // +0x48
    CCharString                ScriptName;                  // +0x50
    CWideString                FailureReason;               // +0x54
    long                       GoldReward;                  // +0x58
    long                       RenownReward;                // +0x5c
    long                       TargetGoldGained;            // +0x60
    long                       InitialGold;                 // +0x64
    long                       TargetExperienceGained;      // +0x68
    long                       InitialExperience;           // +0x6c
    long                       TargetRenownGained;          // +0x70
    long                       InitialRenown;               // +0x74
    float                      ExperienceGainedDisplayed;   // +0x78
    float                      RenownGainedDisplayed;       // +0x7c
    float                      GoldGainedDisplayed;         // +0x80
    CLongULongMap              StartQuestExperience;        // +0x84
    CLongULongMap              StartQuestGold;              // +0x90
    CLongULongMap              StartQuestRenown;            // +0x9c
    CText*                     PRenownText;                 // +0xa8
    CText*                     PGoldText;                   // +0xac
    CText*                     PExperienceText;             // +0xb0
    float                      AccumFadeTime;               // +0xb4
    CComponent*                PFrame;                      // +0xb8
    CComponent*                PBlack;                      // +0xbc
    bool                       QuittingFadeOut;             // +0xc0
    long                       QuestCardDef;                // +0xc4
    unsigned long              ExperienceCountingSound;     // +0xc8
    unsigned long              GoldCountingSound;           // +0xcc
    unsigned long              RenownCountingSound;         // +0xd0
    bool                       CanTeleportToGuildMember;    // +0xd4
    virtual void v00();
    CTCQuestCompletionUI(CThing* parent);
};

CTCQuestCompletionUI::CTCQuestCompletionUI(CThing* parent)
    : CTCBase(parent)
    , Active(false)
    , ReallyActive(false)
    , Quitting(false)
    , WaitingForRegionLoad(false)
    , WaitingForPlayerControl(false)
    , Frame(0)
    , FrameStartedBoastProcessing(-1)
    , RenderedObjectDef(0)
    , LastRenderTime(0.0)
    , GoldReward(0)
    , RenownReward(0)
    , TargetGoldGained(0)
    , InitialGold(0)
    , TargetExperienceGained(0)
    , InitialExperience(0)
    , TargetRenownGained(0)
    , InitialRenown(0)
    , ExperienceGainedDisplayed(0)
    , RenownGainedDisplayed(0)
    , GoldGainedDisplayed(0)
    , PRenownText(0)
    , PGoldText(0)
    , PExperienceText(0)
    , AccumFadeTime(0)
    , PFrame(0)
    , PBlack(0)
    , QuittingFadeOut(false)
    , QuestCardDef(0)
    , ExperienceCountingSound(0)
    , GoldCountingSound(0)
    , RenownCountingSound(0)
    , CanTeleportToGuildMember(true)
{
    AddToGlobalTCs();
}