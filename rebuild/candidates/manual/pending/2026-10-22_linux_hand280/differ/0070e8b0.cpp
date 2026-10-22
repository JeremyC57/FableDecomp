// CAnimateThing::UpdateFrameLogic(const CAnimSetUpdateInfo& info) -- advance the combination lists and the main anim set, fade the main alpha, update or drop each transition set and share the remaining alpha between them by weight, then roll the combination queue and mark the appearance changed
// Leaf: UpdateFrameLogic
// Layout: CAnimSet / list models as the landed UpdateCombinationAnimList 0x0070D390 and PreloadAnims 0x0070C590 (CAnimateThing
//   starts with the current CCompleteAnimation: CAnimSet 0x54, TransitionAnimSets +0x54, CombinationAnims +0x58; the
//   combination queue list<list<CAnimSet> > +0x60, DelayedCombinationAnims +0x64; ChangeAlphaBy +0x8c, refresh stamp +0x98,
//   owner +0xa0 as the landed UpdateCachedAnims 0x0070E520).
#include <new>
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern const float EPSILON;                            // 0x0129BA3C

struct CAnimSetUpdateInfo {
    float TimePassed;
    float DistanceMoved;
    float PercentageComplete;
    CAnimSetUpdateInfo(float timePassed, float distanceMoved, float percentageComplete)
        : TimePassed(timePassed), DistanceMoved(distanceMoved), PercentageComplete(percentageComplete) { }
};
struct CTransitionBlendAlpha {
    float Current;
    float Last;
};
struct SScalarTag { SScalarTag() { } };
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
    unsigned int size() const { return last - first; }
    T& operator[](unsigned long n) { return first[n]; }
    ~CRetailVector() { if (first) free(first); }
};
struct CRetailVector_float {
    float* first; float* last; float* end_of_storage;
    CRetailVector_float() : first(0), last(0), end_of_storage(0) { }
    ~CRetailVector_float() { if (first) free(first); }
    void InsertGrow(float* where, const float& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x004408A0
    void push_back(const float& value)
    {
        if (last != end_of_storage) {
            new (last) float(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct CAnimSet {
    long UpdateMode;                                     // +0
    CRetailVector<short> Anims;                          // +4
    CRetailVector<CTransitionBlendAlpha> InternalAlphas; // +0x10
    float CurrentPercentageComplete;                     // +0x1c
    float LastPercentageComplete;                        // +0x20
    float Speed;                                         // +0x24
    unsigned long FrameInternalAlphasUpdated;            // +0x28
    float CurrentAlpha;                                  // +0x2c
    float LastAlpha;                                     // +0x30
    float LastUpdateChange;                              // +0x34
    float ChangeAlphaBy;                                 // +0x38
    void* PAnimator;                                     // +0x3c
    unsigned long Flags;                                 // +0x40
    unsigned long TypeFlags;                             // +0x44
    long AnimDelay;                                      // +0x48
    long Priority;                                       // +0x4c
    unsigned char Type;                                  // +0x50
    bool Looping : 1;                                    // +0x51 bit 0
    bool Finished : 1;                                   // bit 1
    bool Forwards : 1;                                   // bit 2
    bool HadUpdate : 1;                                  // bit 3
    bool OwnedByMainAnim : 1;                            // bit 4
    void Update(const CAnimSetUpdateInfo& info);         // 0x0070D030
    void UpdateInternalAlphas();                         // 0x0070C2C0
};
inline float GFClamp01(float a)
{
    if (a < 0.0f)
        a = 0.0f;
    else if (a > 1.0f)
        a = 1.0f;
    return a;
}
struct CAnimSetNode {
    CAnimSetNode* next;
    CAnimSetNode* prev;
    CAnimSet      value;
};
struct SListTag { SListTag() { } };
struct CAnimSetListIterator {
    CAnimSetNode* node;
    CAnimSetListIterator(CAnimSetNode* n) : node(n) { }
    CAnimSetListIterator(const CAnimSetListIterator& other) : node(other.node) { }
};
struct CAnimSetList {
    CAnimSetNode* header;
    CAnimSetNode* begin() const { return header->next; }
    CAnimSetNode* end() const { return header; }
    CAnimSetList(const CAnimSetList& other) : header(0)
    {
        header = (CAnimSetNode*)malloc(sizeof(CAnimSetNode));
        header->next = header;
        header->prev = header;
        InsertRange(CAnimSetListIterator(begin()), other.begin(), other.end(), SListTag());
    }
    ~CAnimSetList()
    {
        clear();
        if (header)
            free(header);
    }
    void clear();                                        // 0x0070B8A0
    void InsertRange(CAnimSetListIterator pos, CAnimSetNode* first, CAnimSetNode* last, const SListTag& tag);   // 0x007119C0
    CAnimSetNode* erase(CAnimSetNode* node)
    {
        CAnimSetNode* next = node->next;
        CAnimSetNode* prev = node->prev;
        prev->next = next;
        next->prev = prev;
        node->value.~CAnimSet();
        free(node);
        return next;
    }
};
struct CAnimSetListNode {
    CAnimSetListNode* next;
    CAnimSetListNode* prev;
    CAnimSetList      value;
};
struct CAnimSetListList {                                // list<list<CAnimSet> >
    CAnimSetListNode* header;
    void pop_front()
    {
        CAnimSetListNode* node = header->next;
        CAnimSetListNode* next = node->next;
        CAnimSetListNode* prev = node->prev;
        prev->next = next;
        next->prev = prev;
        node->value.~CAnimSetList();
        free(node);
    }
    void push_back(const CAnimSetList& x)
    {
        CAnimSetListNode* pos = header;
        CAnimSetListNode* node = (CAnimSetListNode*)malloc(sizeof(CAnimSetListNode));
        new (&node->value) CAnimSetList(x);
        node->next = pos;
        node->prev = pos->prev;
        pos->prev->next = node;
        pos->prev = node;
    }
};
struct CTCGraphicAppearance { void SetAsChanged(unsigned long flags); };   // 0x004BF9E0
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;          // +0x64
};
struct C3DAnimationBlendState { void* VTable; void* Data; };
struct CCompleteAnimation : CAnimSet {
    CAnimSetList* PTransitionAnimSets_dummy() const;
};
class CAnimateThing : public CAnimSet {
public:
    CAnimSetList TransitionAnimSets;                     // +0x54
    CAnimSetList CombinationAnims;                       // +0x58
    unsigned char _pad5c[4];
    CAnimSetListList CombinationQueue;                   // +0x60
    CAnimSetList DelayedCombinationAnims;                // +0x64
    C3DAnimationBlendState StartAnimState, EndAnimState, LastStartAnimState, LastEndAnimState;   // +0x68
    float Scale;                                         // +0x88
    float ChangeAlphaBy2;                                // +0x8c
    long AnimDelay2;
    unsigned long Interruptable;
    unsigned long CurrentAnimRefreshStamp;               // +0x98
    unsigned long LastAnimRefreshStamp;
    CThing* PThing;                                      // +0xa0
    void UpdateCombinationAnimList(CAnimSetList& anims, const CAnimSetUpdateInfo& info);   // 0x0070D390
    void PreloadAnims(const CAnimSet& animation);        // 0x0070C590
    void UpdateFrameLogic(const CAnimSetUpdateInfo& info);
    void SetAlpha(float a) { LastAlpha = CurrentAlpha; CurrentAlpha = a; }
};

void CAnimateThing::UpdateFrameLogic(const CAnimSetUpdateInfo& info)
{
    UpdateCombinationAnimList(CombinationAnims, info);
    UpdateCombinationAnimList(DelayedCombinationAnims, info);
    Update(info);
    CAnimSetUpdateInfo setInfo(info.TimePassed, info.DistanceMoved, CurrentPercentageComplete);
    {
        float alpha = CurrentAlpha + ChangeAlphaBy2;
        if (alpha < 0.0f)
            alpha = 0.0f;
        else if (alpha > 1.0f)
            alpha = 1.0f;
        SetAlpha(GFClamp01(alpha));
        CRetailVector_float weights;
        float total = 0.0f;
        CAnimSetNode* node = TransitionAnimSets.begin();
        while (node != TransitionAnimSets.end()) {
            CAnimSet& set = node->value;
            unsigned int count;
            if (set.CurrentAlpha <= EPSILON
                || !((count = set.Anims.size()) == 1 || (count == 2 && set.Anims[1] > 0))
                || set.Anims[0] <= 0
                || count != set.InternalAlphas.size())
                node = TransitionAnimSets.erase(node);
            else {
                set.Update(setInfo);
                if (!set.HadUpdate) {
                    set.UpdateInternalAlphas();
                    set.HadUpdate = true;
                }
                float weight = set.CurrentAlpha - set.ChangeAlphaBy;
                if (weight < 0.0f)
                    weight = 0.0f;
                else if (weight > 1.0f)
                    weight = 1.0f;
                weights.push_back(weight);
                total += weight;
                node = node->next;
            }
        }
        float remaining = 1.0f - alpha;
        float scale;
        if (total > EPSILON)
            scale = remaining / total;
        else
            scale = 1.0f;
        float* weight = weights.first;
        for (CAnimSetNode* it = TransitionAnimSets.begin(); it != TransitionAnimSets.end(); it = it->next) {
            float value = GFClamp01(scale * *weight);
            float last = it->value.CurrentAlpha;
            it->value.CurrentAlpha = value;
            it->value.LastAlpha = last;
            ++weight;
        }
        CombinationQueue.pop_front();
        CombinationQueue.push_back(DelayedCombinationAnims);
        PreloadAnims(*this);
        ++CurrentAnimRefreshStamp;
        PThing->PTCGraphicAppearance->SetAsChanged(2);
    }
}
