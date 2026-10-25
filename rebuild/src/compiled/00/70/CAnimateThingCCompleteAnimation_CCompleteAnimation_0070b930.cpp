// CAnimateThing::CCompleteAnimation::~CCompleteAnimation() -- destroy the two anim-set lists, then the main anim set's two vectors
// Leaf: ??1CCompleteAnimation@CAnimateThing@@QAE@XZ
// Retail: Fable.exe @ 0x0070B930, 89 bytes, plain `ret`. Catalog label "CWound::Kill" is propagated; the only reference is
// the tail `jmp` of CAnimateThing::~CAnimateThing 0x0070BA90 (CurrentAnim is its first member, at +0), and Ego_r.pdb
// lists ??1CCompleteAnimation@CAnimateThing@@QAE@XZ.
// Callees: STLport list<CAnimSet>::clear 0x0070B8A0 (each node's InternalAlphas then Anims buffer freed, the node freed,
// the sentinel relinked; landed as CRetailList_0070b8a0::Clear), free 0x00BFEA14 (malloc allocator).
// Layout (Ego_r.pdb with retail STLport containers: vector 12 bytes, list 4): CCompleteAnimation {MainAnimSet +0 (CAnimSet,
// 0x54: UpdateMode +0, Anims vector<short> +4, InternalAlphas vector<CTransitionBlendAlpha> +0x10), TransitionAnimSets
// list<CAnimSet> +0x54, CombinationAnims list<CAnimSet> +0x58, MainAnimCanTransitionOut +0x5C}.
// Shape: implicit member destructors in reverse order; the list destructor is `clear(); deallocate(sentinel)` with the
// allocator's null test.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

template <class T> struct CRetailVector {           // STLport vector with the malloc allocator: three words
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector() { if (_M_start) free(_M_start); }
};

struct CTransitionBlendAlpha;
struct CAnimSetNode;

struct CAnimSet {                                   // 0x54 bytes retail (PDB 92)
    long                                  UpdateMode;       // +0x00
    CRetailVector<short>                  Anims;            // +0x04
    CRetailVector<CTransitionBlendAlpha>  InternalAlphas;   // +0x10
    unsigned char                         _rest[0x54 - 0x1C];
};

struct CAnimSetList {                               // STLport list<CAnimSet>: the sentinel node pointer
    CAnimSetNode* _M_node;
    void clear();                                   // 0x0070B8A0
    ~CAnimSetList() { clear(); if (_M_node) free(_M_node); }
};

class CAnimateThing {
public:
    class CCompleteAnimation {
    public:
        ~CCompleteAnimation();
        CAnimSet     MainAnimSet;                   // +0x00
        CAnimSetList TransitionAnimSets;            // +0x54
        CAnimSetList CombinationAnims;              // +0x58
        bool         MainAnimCanTransitionOut;      // +0x5C
    };
};

CAnimateThing::CCompleteAnimation::~CCompleteAnimation()
{
}