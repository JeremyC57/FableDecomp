// CTCCreatureOpinionOfHero::ClearAllKnownDeeds() -- empty the pending-deed list and each of the 92 per-deed-type known-deed lists
// Leaf: ClearAllKnownDeeds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681A70, length 110 bytes (+2 int3 padding).
// Label correction: the catalog row says "CTCHero::ClearMarriageAndRelationshipData"; the body clears CTCCreatureOpinionOfHero
// lists (PendingDeeds and the KnownDeeds[92] array; it sits right before KnowsOfDeed 0x00681AE0 and is called from the
// CTCWife divorce code on the opinion component). The PDB name of the member that clears exactly these lists is
// ?ClearAllKnownDeeds@CTCCreatureOpinionOfHero@@AAEXXZ (private); ClearPendingDeeds clears only the first list.
// Callees: free @ 0x00BFEA14 (STLport malloc allocator node release).
// Layout: Ego_r.pdb CTCCreatureOpinionOfHero with retail 4-byte STLport lists (head node pointer): PendingDeeds +0x54,
// LastFrameAppliedDeedEffect long[92] +0x58, KnownDeeds list<long>[92] +0x1C8.
// Shape: the inline STLport list::clear (walk from the head's next, free each node, relink the head to itself, re-reading the
// head member after each free); the array loop counts down from 92 because the index is otherwise unused.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {         // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    void clear()
    {
        SListNode<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SListNode<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
class CTCCreatureOpinionOfHero {
public:
    virtual ~CTCCreatureOpinionOfHero();
    unsigned char      _pad04[0x54 - 0x04];
    CRetailList<long>  PendingDeeds;                    // +0x54
    long               LastFrameAppliedDeedEffect[92];  // +0x58
    CRetailList<long>  KnownDeeds[92];                  // +0x1c8
    void ClearAllKnownDeeds();
};

void CTCCreatureOpinionOfHero::ClearAllKnownDeeds()
{
    PendingDeeds.clear();
    for (long i = 0; i < 92; ++i)
        KnownDeeds[i].clear();
}
