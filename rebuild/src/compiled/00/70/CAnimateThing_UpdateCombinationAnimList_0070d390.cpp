// CAnimateThing::UpdateCombinationAnimList(list<CAnimSet>& anims, const CAnimSetUpdateInfo& info) -- two passes over the combination anim sets: first drop every finished set whose blend has faded out (or that has no fade), fading the others out by their ChangeAlphaBy; then update each set from the frame info and fade the unfinished fading ones in
// Leaf: UpdateCombinationAnimList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D390, length 370 bytes (+ int3 padding), ret 8; `this` is unused.
// Callees: CWorld::GetFrame @ 0x0049D870 (__fastcall; catalog "GFGetPreMainMemoryUsed" is propagated), CAnimSet::Update
//          @ 0x0070D030 (landed), free @ 0x00BFEA14 (the inline CAnimSet destructor frees InternalAlphas then Anims, then
//          the list node). EPSILON 0.0001f @ 0x0129BA3C, 0.0f @ 0x0122DEDC, 1.0f @ 0x0122DED8.
// Layout: the landed CAnimSet model (0x0070C280 / 0x0070D030: Anims +4, InternalAlphas +0x10, FrameInternalAlphasUpdated
//          +0x28, ChangeAlphaBy +0x38, flag bits +0x51: Finished = 2) in STLport list nodes {next, prev, value}; the list
//          object is the header-node pointer.
// Shape: the first pass walks node pointers (the value's offsets folded), `if (Finished && !FadeOut(set)) erase else next`
//          with the erase block laid after the loops; the second pass names the set (`lea esi,[edi+8]`) because Update
//          takes it as `this`. Each fade is a __forceinline helper: the blend element through a named reference (one
//          vector-start load serves the test, the value and the Last = Current roll; the final Current store re-reads it),
//          GetValue through float locals (`last + (current - last)`: Last kept on the x87 stack across the test's compare),
//          the fade-in sum through a named value local (`value + ChangeAlphaBy`, else VC reassociates), the clamp
//          `if (a < 0) a = 0; else if (a > 1) a = 1;`.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CWorld { public: static long __fastcall GetFrame(); };          // 0x0049D870
extern const float EPSILON;                            // 0x0129BA3C

struct CAnimSetUpdateInfo {
    float TimePassed;
    float DistanceMoved;
    float PercentageComplete;
};
class CTransitionBlendAlpha { public:
    float Current;
    float Last;
    float GetValue() const
    {
        float last = Last;
        float current = Current;
        return last + (current - last);
    }
};
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    T& operator[](unsigned long n) { return first[n]; }
    ~CRetailVector() { if (first) free(first); }
};
class CAnimSet { public:
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
};
inline float GFClamp01(float a)
{
    if (a < 0.0f)
        a = 0.0f;
    else if (a > 1.0f)
        a = 1.0f;
    return a;
}

struct CAnimSetNode {                                    // STLport list node
    CAnimSetNode* next;
    CAnimSetNode* prev;
    CAnimSet      value;
};
struct CAnimSetList {                                    // STLport list: the header node
    CAnimSetNode* header;
    CAnimSetNode* begin() const { return header->next; }
    CAnimSetNode* end() const { return header; }
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

class CAnimateThing {
public:
    void UpdateCombinationAnimList(CAnimSetList& anims, const CAnimSetUpdateInfo& info);
};

__forceinline bool FadeOutCombinationAnim(CAnimSet& set)
{
    if (set.ChangeAlphaBy > EPSILON) {
        CTransitionBlendAlpha& alpha = set.InternalAlphas[0];
        if (alpha.GetValue() > EPSILON) {
            float a = GFClamp01(alpha.GetValue() - set.ChangeAlphaBy);
            alpha.Last = alpha.Current;
            set.InternalAlphas[0].Current = a;
            set.FrameInternalAlphasUpdated = CWorld::GetFrame();
            return true;
        }
    }
    return false;
}
__forceinline void FadeInCombinationAnim(CAnimSet& set)
{
    if (!set.Finished && set.ChangeAlphaBy > EPSILON) {
        CTransitionBlendAlpha& alpha = set.InternalAlphas[0];
        float value = alpha.GetValue();
        float a = GFClamp01(value + set.ChangeAlphaBy);
        alpha.Last = alpha.Current;
        set.InternalAlphas[0].Current = a;
        set.FrameInternalAlphasUpdated = CWorld::GetFrame();
    }
}

void CAnimateThing::UpdateCombinationAnimList(CAnimSetList& anims, const CAnimSetUpdateInfo& info)
{
    CAnimSetNode* it = anims.begin();
    while (it != anims.end()) {
        if (it->value.Finished && !FadeOutCombinationAnim(it->value))
            it = anims.erase(it);
        else
            it = it->next;
    }
    for (it = anims.begin(); it != anims.end(); it = it->next) {
        CAnimSet& set = it->value;
        set.Update(info);
        FadeInCombinationAnim(set);
    }
}