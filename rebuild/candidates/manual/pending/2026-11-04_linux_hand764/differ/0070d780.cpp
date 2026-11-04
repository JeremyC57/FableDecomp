// CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim) -- unless the main anim set blocks combinations (Flags bit 0) and the add is not forced, build a CAnimSet from the init (OwnedByMainAnim, LastAlpha, AnimDelay; fading in over fadeInFrames from internal alpha 0, else fully on) and append it to the combination list (or the delayed list when delay != 0)
// Grade: DIFFER(305v305) -- register allocation: retail pushes ebp at entry and keeps the set's InternalAlphas start in it from right after Init through the inlined SetInternalAlpha stores and the final free (VC7.1 treats the set as not escaping); ours re-reads it after each store/call. Same-TU copy ctor and SetInternalAlpha bodies give retail's length.
// Leaf: ?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070D780, 305 bytes (`ret 14h`). Ego_r.pdb ?AddCombinationAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ_N1@Z.
// Callees: CAnimSet::Init(CAnimateThing&, const CAnimInit&, float) 0x0070D1A0, CWorld::GetFrame 0x0049D870 (through the inlined
//   CAnimSet::SetInternalAlpha 0x0070C280, the landed body), malloc 0x00BFEA0E (the 0x5C-byte list node), CAnimSet(const CAnimSet&)
//   0x00710B80 (the landed body, in the TU: VC7.1 then keeps the set's InternalAlphas start in ebp across it), free 0x00BFEA14.
// Layout: Ego_r.pdb CAnimSet / CAnimInit / CTransitionBlendAlpha with 12-byte retail vectors (CAnimSet 0x54); CAnimateThing's main anim
//   set at +0 (Flags +0x40) and the two STLport lists CombinationAnims +0x58 / DelayedCombinationAnims +0x64.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
enum EAnimUpdateMode { };
class CAnimInit { public:                             // Ego_r.pdb, 44 bytes
    EAnimUpdateMode UpdateMode;                       // +0x00
    unsigned char Type;                               // +0x04
    bool          Looping;                            // +0x05
    long          Anim;                               // +0x08
    float         Speed;                              // +0x0c
    float         Alpha;                              // +0x10
    float         LastAlpha;                          // +0x14
};
class CTransitionBlendAlpha { public:                 // Ego_r.pdb, 8 bytes
    float CurrentAlpha;
    float LastAlpha;
};
struct CVectorShort {                                 // vector<short> (STLport, 12 bytes)
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    CVectorShort() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CVectorShort(const CVectorShort& other);          // 0x00710700
    ~CVectorShort() { if (_M_start) free(_M_start); }
};
struct CVectorTransitionBlendAlpha {                  // vector<CTransitionBlendAlpha> (STLport, 12 bytes)
    CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage;
    CVectorTransitionBlendAlpha() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CVectorTransitionBlendAlpha(const CVectorTransitionBlendAlpha& other);   // 0x007101A0
    ~CVectorTransitionBlendAlpha() { if (_M_start) free(_M_start); }
    CTransitionBlendAlpha& operator[](long n) { return _M_start[n]; }
};
class CAnimateThing;
class CAnimSet { public:                              // retail 0x54 bytes (Ego_r.pdb 0x5c)
    EAnimUpdateMode UpdateMode;                       // +0x00
    CVectorShort Anims;                               // +0x04
    CVectorTransitionBlendAlpha InternalAlphas;       // +0x10
    float CurrentPercentageComplete;                  // +0x1c
    float LastPercentageComplete;                     // +0x20
    float Speed;                                      // +0x24
    unsigned long FrameInternalAlphasUpdated;         // +0x28
    float CurrentAlpha;                               // +0x2c
    float LastAlpha;                                  // +0x30
    float LastUpdateChange;                           // +0x34
    float ChangeAlphaBy;                              // +0x38
    CAnimateThing* PAnimator;                         // +0x3c
    unsigned long Flags;                              // +0x40
    unsigned long TypeFlags;                          // +0x44
    long AnimDelay;                                   // +0x48
    long Priority;                                    // +0x4c
    unsigned char Type;                               // +0x50
    bool Looping : 1;                                 // +0x51 bit 0
    bool Finished : 1;
    bool Forwards : 1;
    bool HadUpdate : 1;
    bool OwnedByMainAnim : 1;                         // bit 4
    CAnimSet() { }
    CAnimSet(const CAnimSet& other);                  // 0x00710B80
    void SetInternalAlpha(long index, float alpha, bool setLastToo);   // 0x0070C280
    void Init(CAnimateThing& animator, const CAnimInit& init, float alpha);   // 0x0070D1A0
};
// The landed bodies of SetInternalAlpha 0x0070C280 and the copy constructor 0x00710B80 (same TU in retail).
void CAnimSet::SetInternalAlpha(long index, float alpha, bool setLastToo)
{
    InternalAlphas[index].LastAlpha = InternalAlphas[index].CurrentAlpha;
    InternalAlphas[index].CurrentAlpha = alpha;
    if (setLastToo)
        InternalAlphas[index].LastAlpha = alpha;
    FrameInternalAlphasUpdated = CWorld::GetFrame();
}
#pragma auto_inline(off)
__declspec(noinline) CAnimSet::CAnimSet(const CAnimSet& other)
    : UpdateMode(other.UpdateMode), Anims(other.Anims), InternalAlphas(other.InternalAlphas),
      CurrentPercentageComplete(other.CurrentPercentageComplete), LastPercentageComplete(other.LastPercentageComplete),
      Speed(other.Speed), FrameInternalAlphasUpdated(other.FrameInternalAlphasUpdated), CurrentAlpha(other.CurrentAlpha),
      LastAlpha(other.LastAlpha), LastUpdateChange(other.LastUpdateChange), ChangeAlphaBy(other.ChangeAlphaBy),
      PAnimator(other.PAnimator), Flags(other.Flags), TypeFlags(other.TypeFlags), AnimDelay(other.AnimDelay),
      Priority(other.Priority), Type(other.Type)
{
    Looping = other.Looping;
    Finished = other.Finished;
    Forwards = other.Forwards;
    HadUpdate = other.HadUpdate;
    OwnedByMainAnim = other.OwnedByMainAnim;
}
#pragma auto_inline(on)

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct CRetailList {               // STLport 4.6 list<T>: the sentinel node pointer
    _List_node_base* _M_node;
    void push_back(const T& x)
    {
        _List_node<T>* tmp = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        new ((void*)&tmp->_M_data) T(x);
        _List_node_base* n = _M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};
class CAnimateThing { public:
    CAnimSet      MainAnimSet;                        // +0x00 (CurrentAnim's main set)
    unsigned char _pad54[4];                          // +0x54
    CRetailList<CAnimSet> CombinationAnims;           // +0x58
    unsigned char _pad5c[8];
    CRetailList<CAnimSet> DelayedCombinationAnims;    // +0x64
    void AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim);
};

void CAnimateThing::AddCombinationAnim(const CAnimInit& init, long fadeInFrames, long delay, bool force, bool ownedByMainAnim)
{
    if (!force && (MainAnimSet.Flags & 1))
        return;
    if (!init.Anim)
        return;
    CAnimSet set;
    set.Init(*this, init, init.Alpha);
    set.OwnedByMainAnim = ownedByMainAnim;
    set.LastAlpha = init.LastAlpha;
    set.AnimDelay = delay;
    if (fadeInFrames > 0) {
        set.SetInternalAlpha(0, 0.0f, true);
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
