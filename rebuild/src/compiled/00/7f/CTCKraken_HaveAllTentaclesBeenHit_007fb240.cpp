// CTCKraken::HaveAllTentaclesBeenHit() const -- true once the boss fight has started (Phase non-zero) and no tentacle is left in the Tentacles list
// Leaf: ?HaveAllTentaclesBeenHit@CTCKraken@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FB240, 36 bytes (+ int3 padding). Ego_r.pdb / FableWin.pdb
//   `?HaveAllTentaclesBeenHit@CTCKraken@@QBE_NXZ` (public const thiscall; 0x20 bytes in Ego_r, whose list keeps a size
//   member); called by CAIStateGroup_KrakenAttack::Update 0x00924C70. Catalog label `CAIStateGroup_KrakenAttack_IsQueueEmpty`.
// Layout (retail, as the landed SetPhase 0x007FC820): Tentacles +0x44 (STLport head node), Phase +0x60.
// Shape: `if (!Phase) return false; return Tentacles.size() == 0;` -- STLport's walking size (`xor edx ; ... inc edx`),
//   then `xor eax,eax ; test edx,edx ; sete al`.
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {                         // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    unsigned int size() const
    {
        unsigned int n = 0;
        for (SListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};

class CThing;
struct CIntelligentPointer_CThing {
    void*   __vfptr;
    CThing* m_pItem;
};

typedef long EKrakenBossPhase;

class CTCKraken {
public:
    void*                                    __vfptr;           // +0x00
    CThing*                                  ParentThing;       // +0x04 (CTCBase)
    unsigned char                            _pad08[0x44 - 0x08];
    CRetailList<CIntelligentPointer_CThing>  Tentacles;         // +0x44
    unsigned char                            _pad48[0x60 - 0x48];
    EKrakenBossPhase                         Phase;             // +0x60
    long                                     Mode;              // +0x64 EKrakenMode
    bool HaveAllTentaclesBeenHit() const;
};

bool CTCKraken::HaveAllTentaclesBeenHit() const
{
    if (!Phase)
        return false;
    return Tentacles.size() == 0;
}