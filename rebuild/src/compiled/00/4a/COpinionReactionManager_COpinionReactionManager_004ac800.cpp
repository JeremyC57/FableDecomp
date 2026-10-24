// COpinionReactionManager::~COpinionReactionManager() -- destroy the opinion reaction manager: its debug creature pointer, the opinion-source list, the opinion-source pointer, the reaction list, then release its definition
// Leaf: ??1COpinionReactionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AC800, length 87 bytes, plain `ret`. Callers: the non-virtual ??_G 0x004ACA70
// (manifest "CLandscapeBackgroundPatch::`vector deleting destructor'", propagated) and the CCountedPointer per-type deleter
// 0x004AD600 (manifest "CTCMapwho::OnDie", propagated), stored by CCountedPointer<COpinionReactionManager>::Reset 0x004ADAE0,
// which the world setup calls at 0x004A740A on `new` (0x728 bytes) + COpinionReactionManager::COpinionReactionManager
// 0x007004B0 (landed). The manifest label (_Dest_val<allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>>>) is
// propagated and wrong.
// Layout (Ego_r.pdb COpinionReactionManager 0x738, retail 4-byte list heads, as the landed ctor): PDef +0x000, World +0x004,
// ReactionFlagsTemp +0x008, Reactions (list<CReactionMatch*>) +0x058, the per-reaction tables +0x05c..+0x714,
// PThingOpinionSource +0x714, OpinionSourcesList (list<CIntelligentPointer<const CThing> >) +0x71c, DebugCreature +0x720.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10 (x2), the opinion-source list's clear @ 0x0043F230
// (thiscall; each node's element destroyed through its virtual dtor, node freed), free @ 0x00BFEA14 (its header node), the
// reaction list's destructor @ 0x004AB420, CDefPointeeBase slot 1 (DeleteThis) through the def.
// The reaction list's whole destructor is out of line @ 0x004AB420 (clear loop + header free; also reached by `jmp` from
// 0x004AC000 and 0x0067BC90, i.e. folded with other list<T*> destructors); the opinion-source list's destructor is inline
// (`clear(); if (_M_node) free(_M_node);`).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

class CWorld;
class CThing;
class CThingAICreature;
struct CReactionMatch;
class COpinionReactionManagerDef;

struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void DeleteThis();                               // slot 1 (+4)
    unsigned long RefCount;                                  // +0x04
};

template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();                      // 0x00A01C10
    void* PItem;                                             // +0x04
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };

struct SReactionListNode { SReactionListNode* _M_next; SReactionListNode* _M_prev; CReactionMatch* _M_data; };
struct CReactionMatchList {                                  // STLport list<CReactionMatch*>: the header-node pointer
    SReactionListNode* _M_node;
    ~CReactionMatchList();                                   // 0x004AB420
};

struct SOpinionSourceNode { SOpinionSourceNode* _M_next; SOpinionSourceNode* _M_prev; CIntelligentPointer<const CThing> _M_data; };
struct COpinionSourceList {                                  // STLport list<CIntelligentPointer<const CThing> >
    SOpinionSourceNode* _M_node;
    void clear();                                            // 0x0043F230
    ~COpinionSourceList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

class COpinionReactionManager {
public:
    CDefPointer<const COpinionReactionManagerDef> PDef;      // +0x000
    CWorld*                World;                            // +0x004
    bool                   ReactionFlagsTemp[79];            // +0x008
    CReactionMatchList     Reactions;                        // +0x058
    long                   AttitudeCondition[79];            // +0x05c
    long                   TargetingCondition[79];           // +0x198
    unsigned long          PreReactionDelaySeconds[79];      // +0x2d4
    bool                   BlockFurtherReactionsFlags[79];   // +0x410
    bool                   AllowSpeechOnNonPureAISpeaker[79];   // +0x45f
    bool                   AllowWhileCarrying[79];           // +0x4ae
    bool                   AllowWhileFollowingPlayer[79];    // +0x4fd
    bool                   FurtherReactionsBlocked;          // +0x54c
    unsigned long          ReactionReenableFrame;            // +0x550
    bool                   AttitudeMask[79];                 // +0x554
    long                   DeedReactionSpeechPlaying[92];    // +0x5a4
    CIntelligentPointer<CThing>                 PThingOpinionSource;   // +0x714
    COpinionSourceList                          OpinionSourcesList;    // +0x71c
    CIntelligentPointer<const CThingAICreature> DebugCreature;         // +0x720
    ~COpinionReactionManager();
};

COpinionReactionManager::~COpinionReactionManager()
{
}