// Grade: DIFFER(185v226) -- retail keeps an ebp frame but zeroes with xor ebx/mov (speed code), lays the saving arm first, homes the set's pred/alloc temporaries in the dead context slot ([ebp+0Bh]), caches the vector end in that slot and shares one inline vector free tail for both arms; ours (/Os) zeroes with and, inverts the arms and builds the range-ctor temporaries in the frame | tried: optimize(s), default /O2 (no frame, 222v226), optimize(s) + user-ctor tags | next: default speed with #pragma optimize("y", off) (frame pointer kept), one vector declared before the if
// CCombatAbilityBlockDefBase::Transfer(CPersistContext& context) -- persist the block def: three scalars, the blocking data, then the set of ids through a temporary vector (saving: vector(set.begin(), set.end()); loading: vector read back, set rebuilt from it and assigned)
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00465C54, 226 bytes (ret 4); size-optimised TU with an ebp frame.
// Callees (incremental-link thunks, manifest labels propagated): context Transfer(long*) @ 0x00431061, Transfer(bool*)
//          @ 0x0043314A (twice), Transfer(CCombatAbilityBlockingData*) @ 0x00452B4D, Transfer(vector<long>&) @ 0x00463F3C,
//          vector<long>::_Construct(first, last, tag) @ 0x00465E44, set<long>::set(pred, alloc) @ 0x00451ADD,
//          set<long>::insert(const long&) @ 0x00465D36 (pair returned in the frame), set<long>::operator= @ 0x00461BF0
//          (landed), set<long>::~set @ 0x00451B07, free @ 0x00BFEA14 (the vector's inline destructor).
// Layout (Ego_r.pdb CCombatAbilityBlockDefBase, 92 bytes in the PDB): CombatAbility +0x28, a long +0x40, two bools +0x44/+0x45,
//          the id set +0x48 (retail tree {head, size}; head->left = begin at +8). Context mode +0x18 (1 or 3 = saving).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct CTreeNode {
    unsigned char color;
    CTreeNode*    parent;                       // +4
    CTreeNode*    left;                         // +8
    CTreeNode*    right;                        // +0xC
};
struct CTreeIterator { CTreeNode* node; };
struct CIteratorTag { CIteratorTag() { } };
struct CLessLong { CLessLong() { } };
struct CAllocLong { CAllocLong() { } };
struct CInsertResult { CTreeIterator it; bool inserted; };

struct CLongVector {
    long* first;
    long* last;
    long* end_;
    CLongVector() : first(0), last(0), end_(0) { }
    CLongVector(CTreeIterator b, CTreeIterator e) : first(0), last(0), end_(0) { _Construct(b, e, CIteratorTag()); }
    ~CLongVector() { if (first) free(first); }
    void _Construct(CTreeIterator b, CTreeIterator e, const CIteratorTag& tag);   // 0x00465E44
    long* begin() { return first; }
    long* end() { return last; }
};

struct CLongSet {
    CTreeNode*    head;                         // +0
    unsigned long size;                         // +4
    CLongSet(const CLessLong& pred = CLessLong(), const CAllocLong& alloc = CAllocLong());   // 0x00451ADD
    CLongSet(long* b, long* e, const CLessLong& pred = CLessLong(), const CAllocLong& alloc = CAllocLong())
    {
        Init(pred, alloc);
        for (; b != e; ++b)
            insert(*b);
    }
    void Init(const CLessLong& pred, const CAllocLong& alloc);   // 0x00451ADD (the out-of-line base ctor)
    ~CLongSet();                                // 0x00451B07
    CInsertResult insert(const long& value);    // 0x00465D36
    CLongSet& operator=(const CLongSet& other); // 0x00461BF0
    CTreeIterator begin() const { CTreeIterator i = { head->left }; return i; }
    CTreeIterator end() const { CTreeIterator i = { head }; return i; }
};

struct CCombatAbilityBlockingData { unsigned char _body[0x18]; };

struct CPersistContext {
    unsigned char _pad[0x18];
    long          Mode;                         // +0x18
    void Transfer(long* value);                 // 0x00431061
    void Transfer(bool* value);                 // 0x0043314A
    void Transfer(CCombatAbilityBlockingData* value);   // 0x00452B4D
    void Transfer(CLongVector& value);          // 0x00463F3C
};

class CCombatAbilityBlockDefBase {
public:
    unsigned char              _pad[0x28];
    CCombatAbilityBlockingData CombatAbility;   // +0x28
    long                       m_value40;       // +0x40
    bool                       m_flag44;        // +0x44
    bool                       m_flag45;        // +0x45
    CLongSet                   m_ids;           // +0x48
    void Transfer(CPersistContext& context);
};

void CCombatAbilityBlockDefBase::Transfer(CPersistContext& context)
{
    context.Transfer(&m_value40);
    context.Transfer(&m_flag44);
    context.Transfer(&m_flag45);
    context.Transfer(&CombatAbility);
    if (context.Mode == 1 || context.Mode == 3) {
        CLongVector ids(m_ids.begin(), m_ids.end());
        context.Transfer(ids);
    } else {
        CLongVector ids;
        context.Transfer(ids);
        CLongSet loaded(ids.begin(), ids.end());
        m_ids = loaded;
    }
}
