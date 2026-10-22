// COpinionReactionManager::COpinionReactionManager(CWorld* world) -- construct the opinion reaction manager: fetch its definition, then copy every reaction type's attitude condition, targeting condition, pre-reaction delay and flags out of the definition's maps (defaults when absent), and clear the blocking state and the speech table
// Leaf: COpinionReactionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007004B0, 704 bytes (`ret 4`).
// Callees: malloc 0x00BFEA0E (the two list header nodes), CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10,
//   CDefinitionManager::GetDef<COpinionReactionManagerDef>(const CCharString&, CDefPointer*) 0x00701C90, __ftol2 0x00BFEA70.
// Data: the def name CCharString 0x013BABC8; 0.0f 0x0122DEDC; vtables CIntelligentPointer<CThing> 0x01238C6C,
//   CIntelligentPointer<const CThingAICreature> 0x01264BAC.
// Layout: Ego_r.pdb COpinionReactionManager (0x738) with retail 4-byte list heads: PDef +0, World +4, ReactionFlagsTemp +8,
//   Reactions +0x58, AttitudeCondition +0x5C, TargetingCondition +0x198, PreReactionDelaySeconds +0x2D4,
//   BlockFurtherReactionsFlags +0x410, AllowSpeechOnNonPureAISpeaker +0x45F, AllowWhileCarrying +0x4AE,
//   AllowWhileFollowingPlayer +0x4FD, FurtherReactionsBlocked +0x54C, ReactionReenableFrame +0x550, AttitudeMask +0x554,
//   DeedReactionSpeechPlaying +0x5A4, PThingOpinionSource +0x714, OpinionSourcesList +0x71C, DebugCreature +0x720.
//   COpinionReactionManagerDef (PDB minus 4, 12-byte maps): AttitudeCondition +0x48, TargetingCondition +0x54,
//   PreReactionDelay +0x60, BlockFurtherReactions +0x78, AllowSpeechOnNonPureAISpeaker +0x84, AllowWhileCarrying +0x90,
//   AllowWhileFollowingPlayer +0x9C. CWorld DefinitionManager +0x10.
// Shape: each lookup is an inline STLport find (lower-bound walk returning the node, the header when absent) on
//   `PDef->Map` and a `it != PDef->Map.end()` test (VC reloads the header for end()); TargetingCondition stores in
//   both arms (if/else), the others select first; the speech table is cleared with an element loop (`rep stosd` after
//   the two scalar stores; memset puts `xor eax,eax` first).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

template <class V> struct SMapNode {
    bool      _M_color;
    SMapNode* _M_parent;
    SMapNode* _M_left;
    SMapNode* _M_right;
    long      first;                                 // +0x10
    V         second;                                // +0x14
};
template <class V> struct CRetailMap {               // STLport map<long-sized enum, V>
    SMapNode<V>*  _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    SMapNode<V>* end() const { return _M_header; }
    SMapNode<V>* find(long key) const
    {
        SMapNode<V>* y = _M_header;
        SMapNode<V>* x = _M_header->_M_parent;
        while (x != 0) {
            if (!(x->first < key)) {
                y = x;
                x = x->_M_left;
            } else
                x = x->_M_right;
        }
        if (y == _M_header || key < y->first)
            y = _M_header;
        return y;
    }
};
struct SListNode { SListNode* _M_next; SListNode* _M_prev; void* _M_data; };
struct CRetailList {                                 // STLport list<T*>: the header-node pointer
    SListNode* _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct SListNode2 { SListNode2* _M_next; SListNode2* _M_prev; void* _M_vptr; void* _M_item; };
struct CRetailList2 {                                // list<CIntelligentPointer<const CThing> >
    SListNode2* _M_node;
    CRetailList2() : _M_node(0)
    {
        SListNode2* node = (SListNode2*)malloc(sizeof(SListNode2));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

struct CBaseIntelligentPointerReal {
    virtual void ip00();
    void* m_pItem;
    CBaseIntelligentPointerReal();                   // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointerReal {          // vtable 0x01238C6C
    virtual void ip00();
};
struct CIntelligentPointerConstCreature : CBaseIntelligentPointerReal {  // vtable 0x01264BAC
    virtual void ip00();
};

struct CCharString { void* PStringData; };
struct COpinionReactionManagerDef {
    unsigned char       _pad00[0x48];
    CRetailMap<long>    AttitudeCondition;           // +0x48
    CRetailMap<long>    TargetingCondition;          // +0x54
    CRetailMap<float>   PreReactionDelay;            // +0x60
    CRetailMap<float>   ToleranceToBeingHit;         // +0x6c
    CRetailMap<bool>    BlockFurtherReactions;       // +0x78
    CRetailMap<bool>    AllowSpeechOnNonPureAISpeaker;   // +0x84
    CRetailMap<bool>    AllowWhileCarrying;          // +0x90
    CRetailMap<bool>    AllowWhileFollowingPlayer;   // +0x9c
};
struct CDefPointerReactionDef {
    const COpinionReactionManagerDef* Data;
    CDefPointerReactionDef() : Data(0) {}
    const COpinionReactionManagerDef* operator->() const { return Data; }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointerReactionDef* out);   // 0x00701C90
};
struct CWorld {
    unsigned char       _pad00[0x10];
    CDefinitionManager* DefinitionManager;           // +0x10
};
extern CCharString g_OpinionReactionManagerDefName;  // 0x013BABC8

class COpinionReactionManager {
public:
    CDefPointerReactionDef PDef;                     // +0x000
    CWorld*                World;                    // +0x004
    bool                   ReactionFlagsTemp[79];    // +0x008
    CRetailList            Reactions;                // +0x058
    long                   AttitudeCondition[79];    // +0x05c
    long                   TargetingCondition[79];   // +0x198
    unsigned long          PreReactionDelaySeconds[79];   // +0x2d4
    bool                   BlockFurtherReactionsFlags[79];   // +0x410
    bool                   AllowSpeechOnNonPureAISpeaker[79];   // +0x45f
    bool                   AllowWhileCarrying[79];   // +0x4ae
    bool                   AllowWhileFollowingPlayer[79];   // +0x4fd
    bool                   FurtherReactionsBlocked;  // +0x54c
    unsigned long          ReactionReenableFrame;    // +0x550
    bool                   AttitudeMask[79];         // +0x554
    long                   DeedReactionSpeechPlaying[92];   // +0x5a4
    CIntelligentPointerThing PThingOpinionSource;    // +0x714
    CRetailList2           OpinionSourcesList;       // +0x71c
    CIntelligentPointerConstCreature DebugCreature;  // +0x720
    COpinionReactionManager(CWorld* world);
};

COpinionReactionManager::COpinionReactionManager(CWorld* world)
    : World(world)
{
    World->DefinitionManager->GetDef(g_OpinionReactionManagerDefName, &PDef);
    for (long i = 0; i < 79; ++i) {
        {
            SMapNode<long>* it = PDef->AttitudeCondition.find(i);
            AttitudeCondition[i] = (it != PDef->AttitudeCondition.end() ? it->second : 0x12);
        }
        {
            SMapNode<long>* it = PDef->TargetingCondition.find(i);
            if (it != PDef->TargetingCondition.end())
                TargetingCondition[i] = it->second;
            else
                TargetingCondition[i] = 0;
        }
        {
            SMapNode<float>* it = PDef->PreReactionDelay.find(i);
            PreReactionDelaySeconds[i] = (long)(it != PDef->PreReactionDelay.end() ? it->second : 0.0f);
        }
        {
            SMapNode<bool>* it = PDef->BlockFurtherReactions.find(i);
            BlockFurtherReactionsFlags[i] = (it != PDef->BlockFurtherReactions.end() ? it->second : false);
        }
        {
            SMapNode<bool>* it = PDef->AllowSpeechOnNonPureAISpeaker.find(i);
            AllowSpeechOnNonPureAISpeaker[i] = (it != PDef->AllowSpeechOnNonPureAISpeaker.end() ? it->second : false);
        }
        {
            SMapNode<bool>* it = PDef->AllowWhileCarrying.find(i);
            AllowWhileCarrying[i] = (it != PDef->AllowWhileCarrying.end() ? it->second : false);
        }
        {
            SMapNode<bool>* it = PDef->AllowWhileFollowingPlayer.find(i);
            AllowWhileFollowingPlayer[i] = (it != PDef->AllowWhileFollowingPlayer.end() ? it->second : false);
        }
        AttitudeMask[i] = i < 0x12;
    }
    ReactionReenableFrame = 0;
    FurtherReactionsBlocked = false;
    for (long j = 0; j < 92; ++j)
        DeedReactionSpeechPlaying[j] = 0;
}
