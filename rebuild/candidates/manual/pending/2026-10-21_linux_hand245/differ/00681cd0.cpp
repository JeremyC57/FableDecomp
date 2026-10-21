// CTCCreatureOpinionOfHero::SetOpinionReactionMask(long maskDefIndex) -- reset the reaction mask (first 0x39 reactions on, the rest off); when an OPINION_REACTION_MASK def index is given, start every base reaction at the def's default and apply its per-reaction overrides
// Grade: DIFFER(367v351) -- the prologue, filler loop, class-name test and GetDef call match; the def is then held in ebx with a spilled copy and the default-value loop strength-reduces &Def->DefaultValue (retail keeps the def in esi and reloads [esi+39h]); the map walk follows
// Leaf: SetOpinionReactionMask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681CD0, length 349 bytes (`ret 4`). The manifest owner
//   "CGameScriptInterface" is propagated: `this` carries the reaction mask vector at +0x30 (Ego_r.pdb CTCCreatureOpinionOfHero
//   ReactionMask, retail 12-byte vectors) and the neighbours 0x006819D0 / 0x00681A70 are CTCCreatureOpinionOfHero members.
// Callees: the component's game accessor @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine" propagated; definition manager
//   at +0x10), CDefinitionManager::GetDefClassTypeNameFromGlobalIndex @ 0x009ACCE0 (CDefString by value),
//   CDefStringTable::GetStringAsCharArray @ 0x009D49F0 on the global table @ 0x013CA828, the strcmp intrinsic,
//   CDefinitionManager::GetDef<COpinionReactionMaskDef> @ 0x00685D90, malloc / free @ 0x00BFEA0E / 0x00BFEA14, the def's
//   slot 1 (destroy on the last release).
// Layout: COpinionReactionMaskDef default +0x39 (bool), overrides map<long, bool> +0x3C (STLport tree header; nodes key +0x10,
//   value +0x14).
// Shape: the mask's start pointer is re-read for every store (bool stores may alias it); the "overridden" scratch vector<bool>
//   (0x4F zeroed bytes, malloc allocator) is filled but never read; the map walk inlines the tree increment.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(strcmp, memset)
struct CDefString { long m_index; CDefString(const CDefString& o) : m_index(o.m_index) {} };
struct CDefStringTable_Retail { const char* GetStringAsCharArray(long index); };   // 0x009D49F0
extern CDefStringTable_Retail g_defStringTable;         // 0x013CA828
struct SOverrideNode {
    long           color;
    SOverrideNode* parent;
    SOverrideNode* left;
    SOverrideNode* right;
    long           key;                                 // +0x10
    bool           value;                               // +0x14
};
struct COpinionReactionMaskDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long           RefCount;                            // +0x04
    unsigned char  _pad08[0x31];
    bool           DefaultValue;                        // +0x39
    unsigned char  _pad3a[2];
    SOverrideNode* Overrides;                           // +0x3C (map header)
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CDefPointerReactionMask {                       // the GetDef out slot (released through a holder below)
    COpinionReactionMaskDef* Object;
    CDefPointerReactionMask() : Object(0) {}
};
struct CReactionMaskDefHolder {                         // keeps the def in a register and releases it at scope end
    COpinionReactionMaskDef* Def;
    CReactionMaskDefHolder(COpinionReactionMaskDef* def) : Def(def) {}
    ~CReactionMaskDefHolder() { if (Def) Def->Release(); }
};
struct CDefinitionManager_Retail {
    CDefString GetDefClassTypeNameFromGlobalIndex(long index);                  // 0x009ACCE0
    bool GetDef(long index, CDefPointerReactionMask* out);                      // 0x00685D90
};
struct CGameComponent_Retail { unsigned char _pad[0x10]; CDefinitionManager_Retail* DefinitionManager; };
struct CBoolVector {
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    bool& operator[](unsigned long i) { return _M_start[i]; }
};
struct CScratchBoolVector {                             // vector<bool>(n, false) on the malloc allocator
    bool* _M_start; bool* _M_finish; bool* _M_end_of_storage;
    CScratchBoolVector(unsigned long n)
    {
        _M_start = (bool*)malloc(n);
        memset(_M_start, 0, n);
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    ~CScratchBoolVector() { if (_M_start) free(_M_start); }
    bool& operator[](unsigned long i) { return _M_start[i]; }
};
__forceinline SOverrideNode* Increment(SOverrideNode* node)
{
    if (node->right) {
        node = node->right;
        while (node->left)
            node = node->left;
    } else {
        SOverrideNode* up = node->parent;
        while (node == up->right) {
            node = up;
            up = up->parent;
        }
        if (node->right != up)
            node = up;
    }
    return node;
}
struct CTCCreatureOpinionOfHero {
    unsigned char _pad00[0x30];
    CBoolVector   ReactionMask;                         // +0x30
    CGameComponent_Retail* GetGameComponent();          // 0x00686A70
    void SetOpinionReactionMask(long maskDefIndex);
};

void CTCCreatureOpinionOfHero::SetOpinionReactionMask(long maskDefIndex)
{
    for (long i = 0; i < 0x4f; ++i)
        ReactionMask[i] = i < 0x39;
    if (!maskDefIndex)
        return;
    CDefinitionManager_Retail* definitions = GetGameComponent()->DefinitionManager;
    CDefPointerReactionMask def;
    if (strcmp(g_defStringTable.GetStringAsCharArray(definitions->GetDefClassTypeNameFromGlobalIndex(maskDefIndex).m_index), "OPINION_REACTION_MASK") != 0)
        return;
    bool found = definitions->GetDef(maskDefIndex, &def);
    CReactionMaskDefHolder holder(def.Object);
    if (found) {
        for (long j = 0; j < 0x39; ++j)
            ReactionMask[j] = holder.Def->DefaultValue;
        CScratchBoolVector overridden(0x4f);
        SOverrideNode* end = holder.Def->Overrides;
        for (SOverrideNode* it = end->left; it != end; it = Increment(it)) {
            if (it->key < 0x4f) {
                overridden[it->key] = true;
                ReactionMask[it->key] = it->value;
            }
        }
    }
}
