// CombatDialogueManager::~CombatDialogueManager() -- release the current talker pointer and the dialogue def, then clear and free the speech-group map
// Leaf: ??1CombatDialogueManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F2F60, length 98 bytes (+ int3 padding).
// Label correction: the manifest row "CCreatureAction_PerformExpressionExtended::~CCreatureAction_PerformExpressionExtended" is propagated;
// the only caller is CCombatManager's destructor (0x006ED480, vtable 0x01261EB4) destroying its member at +0x38, which is the PDB's
// CCombatManager::DialogueManager (PDB +0x40; the two retail maps before it are 12 bytes, not 16) -- so this is ~CombatDialogueManager.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, the def's virtual slot 1 (destroy on the last release),
// the STLport tree's _M_erase(root) @ 0x006EFC30, free @ 0x00BFEA14.
// Layout (PDB CombatDialogueManager with the retail 12-byte map): SpeechGroupInfoMap +0x00 (header pointer, node count, empty
// compare), PDialogueDef +0x0c (CDefPointer<CCombatDialogueDef const>), PCurrentTalker +0x10 (CIntelligentPointer<CThingCreatureBase>).
// Shape: members destroyed in reverse order with no vtable store (the class is not polymorphic); the CDefPointer destructor reads
// the pointer into a local (the decrement and the virtual call share ecx) and clears the member inside the null test; the map is
// STLport `_Rb_tree::~_Rb_tree()` = `clear()` (erase the root, leftmost = header, root = 0, rightmost = header, count = 0, each
// store re-reading the header pointer) then the header node's deallocation behind a null test.
#include <stdlib.h>

struct CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;            // +4
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
};

struct CCombatDialogueDef {                 // CDefPointee: vptr + count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
};
struct CCombatDialogueDefPointer {          // CDefPointer<CCombatDialogueDef const>
    CCombatDialogueDef* m_pDef;
    ~CCombatDialogueDefPointer()
    {
        CCombatDialogueDef* def = m_pDef;
        if (def) {
            if (--def->m_refCount == 0)
                def->Destroy();
            m_pDef = 0;
        }
    }
};

struct SRbTreeNode {                        // _Rb_tree_node_base
    unsigned char m_color;                  // +0x00
    SRbTreeNode*  m_parent;                 // +0x04
    SRbTreeNode*  m_left;                   // +0x08
    SRbTreeNode*  m_right;                  // +0x0c
};
struct CSpeechGroupInfoMap {                // map<unsigned long, TSpeechGroupInfo> (STLport _Rb_tree, 12 bytes in retail)
    SRbTreeNode*  m_header;                 // +0x00
    unsigned long m_nodeCount;              // +0x04
    unsigned char m_compare;                // +0x08 std::less (empty)
    void EraseSubtree(SRbTreeNode* root);   // 0x006EFC30 _M_erase
    void Clear()
    {
        if (m_nodeCount != 0) {
            EraseSubtree(m_header->m_parent);
            m_header->m_left = m_header;
            m_header->m_parent = 0;
            m_header->m_right = m_header;
            m_nodeCount = 0;
        }
    }
    ~CSpeechGroupInfoMap()
    {
        Clear();
        SRbTreeNode* header = m_header;
        if (header)
            free(header);
    }
};

struct CombatDialogueManager {
    CSpeechGroupInfoMap       SpeechGroupInfoMap;   // +0x00
    CCombatDialogueDefPointer PDialogueDef;         // +0x0c
    CIntelligentPointer       PCurrentTalker;       // +0x10
    unsigned long             CurrentTalkerTextGroupIndex;   // +0x18
    long                      LastSpeechFrame;      // +0x1c
    long                      LastBattleCryFrame;   // +0x20
    ~CombatDialogueManager();
};

CombatDialogueManager::~CombatDialogueManager()
{
}