// COpinionSourceDef::Transfer(CPersistContext&) -- persist an opinion source def: the produced-opinion and reaction-flag maps and the flags; when saving or comparing, the flattened arrays too, otherwise rebuild the flattened opinion set and the 79 reaction flags from the maps
// Leaf: Transfer
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045F178 (unaligned: a /Os TU), length 370 bytes, ebp frame with the /GS cookie, `ret 4`.
// Callees: CPersistContext::Transfer overloads -- map<EOpinion,float> 0x0045F2EA, bool 0x0043314A, map<EOpinionReactionType,bool> 0x0045DEE9, long 0x00431102, float 0x00431061 (all labelled CEngineLightingManager::UpdateShadowScene); STLport rb-tree increment 0x004292C0 (labelled GetNoAllocatedAreas); __security_check_cookie 0x00BFE9F9.
// Layout: Ego_r.pdb COpinionSourceDef less 4 (retail CDefClassBase 0x3C) with 12-byte STLport maps: ProducedOpinion +0x3C, five bools +0x48, ReactionFlag +0x50, five bools +0x5C, RestrictToFactionAndAllies +0x64, RestrictToFactionIsReversed +0x68, OpinionProduced (float* first) +0x6C, ReactionFlags[79] +0x78; the context mode +0x18 (2/3 = saving/comparing).
// Shape: the save test is a NAMED bool (`saving = mode == 2 || mode == 3`), which puts the save block first with `je`/`jne` (an inline `if`, an early return or an inverted test put the load block first; a switch gives jl/jg); the map ends are cached in locals before the loops; the opinion values are zeroed by memset (five `stosd` under /Os); the per-key "set" bool arrays are written but never read (kept: they size the frame).
#pragma optimize("s", on)
#include <string.h>
struct SRbNode {
    long     Color;
    SRbNode* Parent;
    SRbNode* Left;
    SRbNode* Right;
};
SRbNode* __cdecl RbIncrement(SRbNode* node);   // 0x004292C0
template <class K, class V> struct SMapNode : SRbNode {
    K first;                                    // +0x10
    V second;                                   // +0x14
};
template <class K, class V> struct CRetailMap {   // STLport map (12 bytes): header node, count, compare
    SRbNode* Header;
    long     Count;
    long     Compare;
    struct iterator {
        SRbNode* Node;
        iterator(SRbNode* node) : Node(node) { }
        bool operator!=(const iterator& o) const { return Node != o.Node; }
        void operator++() { Node = RbIncrement(Node); }
        SMapNode<K, V>* operator->() const { return (SMapNode<K, V>*)Node; }
    };
    iterator begin() const { return iterator(Header->Left); }
    iterator end() const { return iterator(Header); }
};
enum EOpinion { OPINION_COUNT = 5 };
enum EOpinionReactionType { REACTION_COUNT = 0x4f };
typedef CRetailMap<EOpinion, float> COpinionMap;
typedef CRetailMap<EOpinionReactionType, bool> CReactionMap;
struct CPersistContext {
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18
    void Transfer(COpinionMap& value);          // 0x0045F2EA
    void Transfer(bool& value);                 // 0x0043314A
    void Transfer(CReactionMap& value);         // 0x0045DEE9
    void Transfer(long& value);                 // 0x00431102
    void Transfer(float& value);                // 0x00431061
};
struct COpinionSet {
    float* Values;
    long   _pad[2];
};
struct CDefClassBase {
    virtual void v00();
    unsigned char _pad04[0x38];
};
struct COpinionSourceDef : CDefClassBase {
    COpinionMap   ProducedOpinion;              // +0x3c
    bool          AlwaysAwareOf;                // +0x48
    bool          UnknownThreatSource;          // +0x49
    bool          ReactNonstop;                 // +0x4a
    bool          ReactToSelfPermitted;         // +0x4b
    bool          ReactionFlagDefault;          // +0x4c
    CReactionMap  ReactionFlag;                 // +0x50
    bool          UseNavLineOfSight;            // +0x5c
    bool          WitnessesWillNotAttack;       // +0x5d
    bool          WitnessesWillOverrideFactionEnemy;   // +0x5e
    bool          BypassesPersonality;          // +0x5f
    bool          RelativeEffect;               // +0x60
    long          RestrictToFactionAndAllies;   // +0x64
    bool          RestrictToFactionIsReversed;  // +0x68
    COpinionSet   OpinionProduced;              // +0x6c
    bool          ReactionFlags[0x4f];          // +0x78
    void Transfer(CPersistContext& context);
};

void COpinionSourceDef::Transfer(CPersistContext& context)
{
    context.Transfer(ProducedOpinion);
    context.Transfer(AlwaysAwareOf);
    context.Transfer(UnknownThreatSource);
    context.Transfer(ReactNonstop);
    context.Transfer(ReactToSelfPermitted);
    context.Transfer(ReactionFlagDefault);
    context.Transfer(ReactionFlag);
    context.Transfer(UseNavLineOfSight);
    context.Transfer(WitnessesWillNotAttack);
    context.Transfer(WitnessesWillOverrideFactionEnemy);
    context.Transfer(BypassesPersonality);
    context.Transfer(RelativeEffect);
    context.Transfer(RestrictToFactionAndAllies);
    context.Transfer(RestrictToFactionIsReversed);
    bool saving = context.Mode == 2 || context.Mode == 3;
    if (saving) {
        unsigned long i;
        for (i = 0; i < REACTION_COUNT; ++i)
            context.Transfer(ReactionFlags[i]);
        for (i = 0; i < OPINION_COUNT; ++i)
            context.Transfer(OpinionProduced.Values[i]);
    } else {
        float values[OPINION_COUNT];
        memset(values, 0, sizeof(values));
        bool opinionSet[OPINION_COUNT];
        COpinionMap::iterator end = ProducedOpinion.end();
        for (COpinionMap::iterator it = ProducedOpinion.begin(); it != end; ++it) {
            values[it->first] = it->second;
            opinionSet[it->first] = true;
        }
        unsigned long i;
        for (i = 0; i < OPINION_COUNT; ++i)
            OpinionProduced.Values[i] = values[i];
        for (i = 0; i < REACTION_COUNT; ++i)
            ReactionFlags[i] = i < 0x12 ? false : ReactionFlagDefault;
        bool reactionSet[REACTION_COUNT];
        CReactionMap::iterator rend = ReactionFlag.end();
        for (CReactionMap::iterator r = ReactionFlag.begin(); r != rend; ++r) {
            reactionSet[r->first] = true;
            ReactionFlags[r->first] = r->second;
        }
    }
}