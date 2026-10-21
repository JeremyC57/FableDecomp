// Grade: DIFFER(512v504) -- first-pass model: the argument's flag word sits at +0x20 and +8 is tested for null (argument type not CCompleteAnimation as modelled); for_each(0x007100A0) and float-slot reuse unmodelled
// CAnimateThing::SetMainAnim(const CCompleteAnimation&, long, long) -- fade out the anims the main animation owned, then install the new complete animation, keeping a transition from the old main set when blending
// Layout: retail CAnimSet is 0x54 (STLport vector 12 bytes), CCompleteAnimation 0x60 (TransitionAnimSets +0x54, CombinationAnims +0x58, StartBlending flag +0x5C);
// CAnimateThing: CurrentAnim +0, DelayedAnim list +0x64, BlendAlphaStep +0x8C, AnimDelay +0x90, LastAnimRefreshStamp +0x9C, flag +0xA4.
#include <stdlib.h>

struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
struct CTransitionBlendAlpha { float CurrentAlpha; float LastAlpha; };
struct CAlphaVector {
    CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};
class CAnimSet {
public:
    long UpdateMode;
    CShortVector Anims;
    CAlphaVector InternalAlphas;
    float CurrentPercentageComplete;
    float LastPercentageComplete;
    float Speed;
    unsigned long FrameInternalAlphasUpdated;
    float CurrentAlpha;                 // +0x2C
    float LastAlpha;
    float LastUpdateChange;
    float ChangeAlphaBy;                // +0x38
    void* PAnimator;
    unsigned long Flags;
    unsigned long TypeFlags;
    long AnimDelay;
    long Priority;
    unsigned char Type;
    bool Looping : 1;
    bool Finished : 1;
    bool Forwards : 1;
    bool HadUpdate : 1;
    bool OwnedByMainAnim : 1;
    bool IsValid() const
    {
        long count = Anims._M_finish - Anims._M_start;
        return (count == 1 || (count == 2 && Anims._M_start[1] > 0)) && Anims._M_start[0] > 0 && count == (long)(InternalAlphas._M_finish - InternalAlphas._M_start);
    }
};
struct CAnimSetNode { CAnimSetNode* _M_next; CAnimSetNode* _M_prev; CAnimSet _M_data; };
struct CAnimSetList {
    CAnimSetNode* _M_node;
    void clear();                                  // 0x0070B8A0
    void push_back(const CAnimSet& set);           // 0x00711A10
};
struct CAnimSetPtrVector {
    CAnimSet** _M_start; CAnimSet** _M_finish; CAnimSet** _M_end_of_storage;
    CAnimSetPtrVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CAnimSetPtrVector() { if (_M_start) free(_M_start); }
};
struct CScaleAlpha { float Scale; };
CScaleAlpha __fastcall ForEachScaleAlpha(CAnimSetNode* first, CAnimSetNode* last, CScaleAlpha f);   // 0x007100A0

struct CCompleteAnimation {
    CAnimSet MainAnimSet;                          // +0x00
    CAnimSetList TransitionAnimSets;               // +0x54
    CAnimSetList CombinationAnims;                 // +0x58
    bool StartBlending;                            // +0x5C
};

class CAnimateThing {
public:
    CCompleteAnimation CurrentAnim;                // +0x00
    CAnimSetList DelayedAnimQueue;                 // +0x60
    CAnimSetList DelayedAnim;                      // +0x64
    char _pad68[0x8c - 0x68];
    float BlendAlphaStep;                          // +0x8C
    long AnimDelay;                                // +0x90
    long _94;
    long CurrentAnimRefreshStamp;                  // +0x98
    long LastAnimRefreshStamp;                     // +0x9C
    void* ParentThing;                             // +0xA0
    bool AnimChanged;                              // +0xA4
    void GetAllAnimSets(CAnimSetPtrVector* sets);  // 0x0070D100
    void CopyCompleteAnimation(CCompleteAnimation* destination, const CCompleteAnimation& source, float alpha);   // 0x0070D1A0
    void SetMainAnim(const CCompleteAnimation& animation, long blendFrames, long animDelay);
};

void CAnimateThing::SetMainAnim(const CCompleteAnimation& animation, long blendFrames, long animDelay)
{
    LastAnimRefreshStamp = 0;
    AnimChanged = false;
    CAnimSetPtrVector sets;
    GetAllAnimSets(&sets);
    for (CAnimSet** it = sets._M_start; it != sets._M_finish; ++it) {
        CAnimSet* set = *it;
        if (set->OwnedByMainAnim && !set->Finished) {
            set->Finished = true;
            long frames = blendFrames;
            if (frames < 1)
                frames = 1;
            set->ChangeAlphaBy = set->CurrentAlpha / (float)frames;
        }
    }
    if (animation.MainAnimSet.Anims._M_finish == 0)
        return;
    if (animation.MainAnimSet.Flags & 1) {
        CurrentAnim.CombinationAnims.clear();
        DelayedAnim.clear();
    }
    AnimDelay = animDelay;
    float alpha = 0.0f;
    if (blendFrames > 0 && CurrentAnim.StartBlending && CurrentAnim.MainAnimSet.IsValid()) {
        BlendAlphaStep = 1.0f / (float)blendFrames;
        CurrentAnim.TransitionAnimSets.push_back(CurrentAnim.MainAnimSet);
        CAnimSetNode* head = CurrentAnim.TransitionAnimSets._M_node;
        for (CAnimSetNode* n = head->_M_next; n != CurrentAnim.TransitionAnimSets._M_node; n = n->_M_next)
            n->_M_data.ChangeAlphaBy += BlendAlphaStep;
        float total = 0.0f;
        for (CAnimSetNode* n = CurrentAnim.TransitionAnimSets._M_node->_M_next; n != CurrentAnim.TransitionAnimSets._M_node; n = n->_M_next)
            total += n->_M_data.LastAlpha;
        if (total > 0.0001f && total < 1.0f - 0.0001f) {
            CScaleAlpha f;
            f.Scale = 1.0f / total;
            ForEachScaleAlpha(CurrentAnim.TransitionAnimSets._M_node->_M_next, CurrentAnim.TransitionAnimSets._M_node, f);
        }
    } else {
        CurrentAnim.TransitionAnimSets.clear();
        BlendAlphaStep = 0.0f;
        alpha = 1.0f;
    }
    CopyCompleteAnimation(&CurrentAnim, animation, alpha);
    CurrentAnim.MainAnimSet.ChangeAlphaBy = -BlendAlphaStep;
    CurrentAnim.StartBlending = !(animation.MainAnimSet.Flags & 2);
}
