// CBulletTimeManager::PrepareForLoad() -- before a load: clears the override flag, restores the default multiplier, resets the last strike tick and empties the bullet-time info map
// Leaf: PrepareForLoad
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C6080, 57 bytes. FableWin.pdb ?PrepareForLoad@CBulletTimeManager@@QAEXXZ.
// Identity: NOT LTextTreeWalkThrough::PrepareForLoad (propagated label). Its only caller is CWorld::PrepareForLoad 0x004A0220
// (`mov ecx,[esi+68h] ; call`), and CWorld +0x68 is Ego_r.pdb CWorld::PBulletTimeManager (scoped_ptr<CBulletTimeManager>; the
// next scoped_ptr, +0x7C PScriptConversationManager, is confirmed by its checked_delete 0x004AE810).
// Callee: map<void*,CBulletTimeInfo>::_M_erase @ 0x004AA650 (bound as CRetailTree_erase004aa650::Erase by the landed tree
// destructors 0x004AC4E0 / 0x004AD1D0 / 0x004AD960).
// Layout: Ego_r.pdb CBulletTimeManager with the 12-byte retail STLport map: BulletTimeInfoMap +0 (header, count, compare),
// Override +0x0C, UseDefaultMultiplier +0x0D, LastStrikeActionTick +0x10, World +0x14, PlayerManager +0x18.
// Shape: the three scalar stores share the zero register and run before the inline STLport clear() (erase the root when
// the count is non-zero, relink the header re-read per store, zero the count).
struct CRetailTree_erase004aa650 {                 // STLport map<void*, CBulletTimeManager::CBulletTimeInfo>
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; };
    node*         head;                            // +0x00
    unsigned long size;                            // +0x04
    unsigned long _comp;                           // +0x08
    void Erase(node* root);                        // 0x004AA650
    void clear()
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
    }
};

class CWorld;
class CPlayerManager;

class CBulletTimeManager {
public:
    CRetailTree_erase004aa650 BulletTimeInfoMap;   // +0x00
    bool            Override;                      // +0x0C
    bool            UseDefaultMultiplier;          // +0x0D
    long            LastStrikeActionTick;          // +0x10
    CWorld*         World;                         // +0x14
    CPlayerManager* PlayerManager;                 // +0x18
    void PrepareForLoad();
};

void CBulletTimeManager::PrepareForLoad()
{
    Override = false;
    UseDefaultMultiplier = true;
    LastStrikeActionTick = 0;
    BulletTimeInfoMap.clear();
}