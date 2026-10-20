// CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim) -- unless the main anim set blocks combinations (Flags bit 0) and the add is not forced, build a CAnimSet from the init (OwnedByMainAnim, LastAlpha, AnimDelay; fading in over fadeInFrames from alpha 0, else fully on) and append it to the combination list (or the delayed list when delay != 0)
// Grade: DIFFER(291v305) -- register allocation across the list insert: retail pushes ebp at entry and keeps the set's InternalAlphas begin in it from right after Init through the fade-in writes AND the final free (VC trusts it unchanged across the out-of-line copy ctor), with the bitfield merge in dl/al; ours reloads it for the destructor and pushes ebp only around the fade-in block (frame offsets -4 until then); tried an inline front() reference, a cached begin pointer (this file) and a __forceinline STLport push_back (needed: the plain inline one is emitted out of line)
// Leaf: ?AddCombinationAnim@CAnimateThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D780, length 305 bytes (`ret 14h`).
// Callees: CAnimSet::Init(CAnimateThing*, const CAnimInit&, float) 0x0070D1A0, GFGetCurrentFrame 0x0049D870, malloc
//          0x00BFEA0E (0x5c-byte list node), CAnimSet copy ctor 0x00710B80, free 0x00BFEA14 (the two vectors).
// Layout: Ego_r.pdb CAnimSet/CAnimInit with 12-byte retail vectors (CAnimSet 0x54: InternalAlphas +0x10, FrameInternal-
//          AlphasUpdated +0x28, CurrentAlpha +0x2c, LastAlpha +0x30, ChangeAlphaBy +0x38, Flags +0x40, AnimDelay +0x48,
//          bits +0x51 with OwnedByMainAnim bit 4); CAnimateThing's main anim set at +0 and the two lists at +0x58/+0x64.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
long __fastcall GFGetCurrentFrame();                  // 0x0049D870

struct CAnimInit {                                   // Ego_r.pdb, 44 bytes
    long          UpdateMode;                        // +0x00
    unsigned char Type;                              // +0x04
    bool          Looping;                           // +0x05
    long          Anim;                              // +0x08
    float         Speed;                             // +0x0c
    float         Alpha;                             // +0x10
    float         LastAlpha;                         // +0x14
};
struct CTransitionBlendAlpha {
    float CurrentAlpha;
    float LastAlpha;
};
template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T& front() { return *_M_start; }
};
class CAnimateThing;
struct CAnimSet {                                    // retail 0x54 bytes (PDB 0x5c)
    long          UpdateMode;                        // +0x00
    CRetailVector<short> Anims;                      // +0x04
    CRetailVector<CTransitionBlendAlpha> InternalAlphas;   // +0x10
    float         CurrentPercentageComplete;         // +0x1c
    float         LastPercentageComplete;            // +0x20
    float         Speed;                             // +0x24
    unsigned long FrameInternalAlphasUpdated;        // +0x28
    float         CurrentAlpha;                      // +0x2c
    float         LastAlpha;                         // +0x30
    float         LastUpdateChange;                  // +0x34
    float         ChangeAlphaBy;                     // +0x38
    CAnimateThing* PAnimator;                        // +0x3c
    unsigned long Flags;                             // +0x40
    unsigned long TypeFlags;                         // +0x44
    long          AnimDelay;                         // +0x48
    long          Priority;                          // +0x4c
    unsigned char Type;                              // +0x50
    unsigned char Looping : 1;                       // +0x51
    unsigned char Finished : 1;
    unsigned char Forwards : 1;
    unsigned char HadUpdate : 1;
    unsigned char OwnedByMainAnim : 1;
    CAnimSet() { }
    CAnimSet(const CAnimSet& other);                 // 0x00710B80
    void Init(CAnimateThing* animator, const CAnimInit& init, float alpha);   // 0x0070D1A0
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct CAnimSetNode : _List_node_base { CAnimSet _M_data; };
struct CAnimSetList {                                // STLport list<CAnimSet>: the sentinel pointer
    _List_node_base* _M_node;
    __forceinline void push_back(const CAnimSet& x)
    {
        _List_node_base* pos = _M_node;
        CAnimSetNode* tmp = (CAnimSetNode*)malloc(sizeof(CAnimSetNode));
        new (&tmp->_M_data) CAnimSet(x);
        tmp->_M_next = pos;
        tmp->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = tmp;
        pos->_M_prev = tmp;
    }
};
class CAnimateThing {
public:
    CAnimSet      MainAnimSet;                       // +0x00 (CurrentAnim.MainAnimSet)
    unsigned char _pad54[4];                         // +0x54 TransitionAnimSets
    CAnimSetList  CombinationAnims;                  // +0x58
    unsigned char _pad5c[8];
    CAnimSetList  DelayedCombinationAnims;           // +0x64
    void AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim);
};

void CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim)
{
    if (!force && (MainAnimSet.Flags & 1))
        return;
    if (!init.Anim)
        return;
    CAnimSet set;
    set.Init(this, init, init.Alpha);
    CTransitionBlendAlpha* alphas = set.InternalAlphas._M_start;
    set.OwnedByMainAnim = ownedByMainAnim;
    set.LastAlpha = init.LastAlpha;
    set.AnimDelay = delay;
    if (fadeInFrames > 0) {
        alphas->LastAlpha = alphas->CurrentAlpha;
        alphas->CurrentAlpha = 0.0f;
        alphas->LastAlpha = 0.0f;
        set.FrameInternalAlphasUpdated = GFGetCurrentFrame();
        set.ChangeAlphaBy = 1.0f / fadeInFrames;
    } else {
        set.CurrentAlpha = 1.0f;
        set.LastAlpha = 1.0f;
        set.ChangeAlphaBy = 0.0f;
    }
    if (delay == 0)
        CombinationAnims.push_back(set);
    else
        DelayedCombinationAnims.push_back(set);
}
