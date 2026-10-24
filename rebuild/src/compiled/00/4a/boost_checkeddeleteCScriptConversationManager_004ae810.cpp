// boost::checked_delete<CScriptConversationManager>(CScriptConversationManager* x) -- deletes the world's script conversation manager: clears and frees its conversation map, then the object
// Leaf: ??$checked_delete@VCScriptConversationManager@@@boost@@YIXPAVCScriptConversationManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AE810, 79 bytes, `__fastcall` (x in ecx).
// Identity: NOT LTextTreeWalkThrough::Destroy (propagated label). FableWin.pdb lists
// ??$checked_delete@VCScriptConversationManager@@@boost@@YIXPAVCScriptConversationManager@@@Z (YI = __fastcall). Callers:
// the boost::scoped_ptr<CScriptConversationManager> destructor 0x004AE860 (`mov ecx,[ecx] ; jmp`) and reset 0x004AE870,
// and CWorld (+0x7C = Ego_r.pdb CWorld::PScriptConversationManager) at 0x004A6C55 / 0x004A7458 (the latter right after
// `new` of 0x14 bytes and the CScriptConversationManager constructor 0x006E6150).
// Callees: map<long,CScriptConversation>::_M_erase @ 0x004AE490 (bound as CRetailTree_erase004ae490::Erase by the landed
// tree destructors), free @ 0x00BFEA14 (the map header), operator delete @ 0x00BFE9BC.
// Layout: Ego_r.pdb CScriptConversationManager {map Conversations +0 (retail 12 bytes: header, count, compare),
// long NextConversationIndex +0x0C, bool AlwaysShowText +0x10} = 0x14 bytes, matching the retail allocation.
// Shape: `delete x` = null test + the inline member destructor (STLport clear: when the count is non-zero erase the root,
// relink the header re-read per store, zero the count; free the header) + operator delete.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CRetailTree_erase004ae490 {                 // STLport map<long, CScriptConversation>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _comp;                           // +0x08 (empty less<long> + padding)
    void Erase(node* root);                        // 0x004AE490
    ~CRetailTree_erase004ae490()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
};

class CScriptConversationManager {
public:
    CRetailTree_erase004ae490 Conversations;       // +0x00
    long                      NextConversationIndex;   // +0x0C
    bool                      AlwaysShowText;      // +0x10
};

namespace boost {
template <class T> void __fastcall checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}
template void __fastcall checked_delete<CScriptConversationManager>(CScriptConversationManager* x);
}