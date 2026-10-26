// CAnimateThing::AnimsCanBeIgnored() -- false when any transition or combination anim set, or a plain one/two-anim main set, carries the "must not be ignored" flag (8); true otherwise
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070C100, 109 bytes (+3 int3), __fastcall, returns bool.
// Layout: CAnimateThing::CurrentAnim (CCompleteAnimation) sits at +0, so `this` is also the main
// CAnimSet. Retail CAnimSet is 0x54 bytes (12-byte vectors: Anims +4, InternalAlphas +0x10, Flags +0x40;
// see the landed CAnimSet rows, e.g. 0x0070C2C0); the two VC6-era lists (allocator word, head, size)
// follow at +0x54 (TransitionAnimSets, head +0x58) and +0x60 (CombinationAnims, head +0x64); a list
// node is {next, prev, CAnimSet}, so a set's Flags are read at node+0x48.
// Shape: the flag constant is hoisted into bl for both list walks and the final test; the size of the
// short-element Anims vector is `(last-first) >> 1` (sar), compared with 1 and 2; the second anim must
// be positive in the two-anim case, then the first anim must be positive, the internal alpha count
// (`sar 3`, 8-byte elements) must equal the anim count and the main set's own flag must be set. The
// compare is written `count == alphas.size()`: the other operand order gives `cmp edx,eax`.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    long size() const { return last - first; }
    const T& operator[](long n) const { return first[n]; }
};

class CTransitionBlendAlpha { public:
    float Current;
    float Last;
};

class CAnimSet { public:
    long UpdateMode;                                     // +0x00
    CRetailVector<short> Anims;                          // +0x04
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    void* PAnimator;                                     // +0x3c CAnimateThing*
    unsigned long Flags;                                 // +0x40
    unsigned long TypeFlags;                             // +0x44
    long AnimDelay;                                      // +0x48
    long Priority;                                       // +0x4c
    unsigned char Type;                                  // +0x50
    unsigned char m_bits;                                // +0x51 Looping/Finished/Forwards/HadUpdate/OwnedByMainAnim
};

enum { ANIM_SET_FLAG_CANNOT_BE_IGNORED = 8 };

template<class T> struct CRetailList {                   // VC6-era Dinkumware list: allocator word, head, size
    struct SNode {
        SNode* next;
        SNode* prev;
        T value;
    };
    long m_allocator;
    SNode* m_head;
    unsigned long m_size;
};

struct CCompleteAnimation {
    CAnimSet MainAnimSet;                                // +0x00
    CRetailList<CAnimSet> TransitionAnimSets;            // +0x54
    CRetailList<CAnimSet> CombinationAnims;              // +0x60
    bool MainAnimCanTransitionOut;                       // +0x6c
};

class CAnimateThing { public:
    CCompleteAnimation CurrentAnim;                      // +0x00
    bool AnimsCanBeIgnored();
};

bool CAnimateThing::AnimsCanBeIgnored()
{
    CRetailList<CAnimSet>::SNode* node;
    for (node = CurrentAnim.TransitionAnimSets.m_head->next; node != CurrentAnim.TransitionAnimSets.m_head; node = node->next)
        if (node->value.Flags & ANIM_SET_FLAG_CANNOT_BE_IGNORED)
            return false;
    for (node = CurrentAnim.CombinationAnims.m_head->next; node != CurrentAnim.CombinationAnims.m_head; node = node->next)
        if (node->value.Flags & ANIM_SET_FLAG_CANNOT_BE_IGNORED)
            return false;
    const CAnimSet& main = CurrentAnim.MainAnimSet;
    long count = main.Anims.size();
    if ((count == 1 || (count == 2 && main.Anims[1] > 0))
        && main.Anims[0] > 0
        && count == main.InternalAlphas.size()
        && (main.Flags & ANIM_SET_FLAG_CANNOT_BE_IGNORED))
        return false;
    return true;
}