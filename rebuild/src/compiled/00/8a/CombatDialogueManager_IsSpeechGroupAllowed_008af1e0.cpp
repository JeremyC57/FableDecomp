// CombatDialogueManager::IsSpeechGroupAllowed(unsigned long group) const -- whether the world frame has reached the group's NextAllowedFrame in the speech-group map
// Leaf: IsSpeechGroupAllowed
// Retail: Fable.exe @ 0x008AF1E0, 51 bytes, `__fastcall`, `ret 4` (Ego_r.pdb ?IsSpeechGroupAllowed@CombatDialogueManager@@QBE_NK@Z).
// Callees: the world's GetFrameNumber 0x004167C8 on g_pWorld 0x013B86A0; map<unsigned long,TSpeechGroupInfo>::find(const
// unsigned long&) const 0x008AEF20 (iterator returned through a hidden pointer; manifest label "BinaryTree_Search").
// Layout: SpeechGroupInfoMap +0x00 (retail STLport map, 12 bytes); tree node value at +0x10, TSpeechGroupInfo::NextAllowedFrame
// at node +0x14. The group must be present (the caller found it); find() is dereferenced unchecked.
// Shape: the iterator is a named local read back from its frame slot; the result is a named `bool` local over a `const` frame
// local -- the bare `return frame >= x;` adds `xor eax,eax` and loads x into ecx.
struct TSpeechGroupInfo {                   // CombatDialogueManager::TSpeechGroupInfo, Ego_r.pdb 4 bytes
    long NextAllowedFrame;                  // +0x00
};

struct SSpeechGroupNode {                   // STLport _Rb_tree_node<pair<const unsigned long, TSpeechGroupInfo> >
    bool              _M_color;             // +0x00
    SSpeechGroupNode* _M_parent;            // +0x04
    SSpeechGroupNode* _M_left;              // +0x08
    SSpeechGroupNode* _M_right;             // +0x0C
    unsigned long     first;                // +0x10 key
    TSpeechGroupInfo  second;               // +0x14
};

struct CSpeechGroupMapIterator {            // STLport _Rb_tree_iterator (non-POD: returned through a hidden pointer)
    SSpeechGroupNode* _M_node;
    CSpeechGroupMapIterator() { }
    SSpeechGroupNode* operator->() const { return _M_node; }
};

struct CSpeechGroupInfoMap {                // STLport map<unsigned long, TSpeechGroupInfo>, 12 bytes retail
    SSpeechGroupNode* _M_header;            // +0x00
    unsigned long     _M_node_count;        // +0x04
    unsigned char     _M_key_compare;       // +0x08 (empty less<unsigned long>)
    CSpeechGroupMapIterator find(const unsigned long& key) const;   // 0x008AEF20
};

struct CWorld {
    long GetFrameNumber();                  // 0x004167C8 (manifest label "GFGetPreMainMemoryUsed" is propagated)
};
extern CWorld* g_pWorld;                    // 0x013B86A0
class CombatDialogueManager {
public:
    CSpeechGroupInfoMap SpeechGroupInfoMap;            // +0x00
    bool IsSpeechGroupAllowed(unsigned long group) const;
};

bool CombatDialogueManager::IsSpeechGroupAllowed(unsigned long group) const
{
    const long frame = g_pWorld->GetFrameNumber();
    CSpeechGroupMapIterator it = SpeechGroupInfoMap.find(group);
    bool allowed = frame >= it->second.NextAllowedFrame;
    return allowed;
}