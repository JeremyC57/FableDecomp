// Grade: DIFFER(227v223) -- the zero register: the lane keeps the CSE'd 0 in ebp to the end (`xor ebp,ebp` after the loop, `cmp edi,ebp` for the _M_start null test, end in eax/ebx), retail zeroes edi for the state test and the vector's zero stores only, keeps end in ebp and tests `test edi,edi`; the mixed inline/out-of-line vector destructor (inline at the fall-through, the 0x00414EA0 call on the found-handle return) and everything else match. Same gap as the landed sibling PlayAttachedCriteria 0x0066BD00.
// CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached) -- expand the criteria into its candidate sound-event names and play the first one that starts from a new emitter at the owner's position; the invalid handle when sound is off or nothing plays
// Leaf: ?PlayUnattachedCriteria@CTCSoundPlayer
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BDF0, 223 bytes (+ int3 padding to 224), thiscall `ret 10h`.
//   Name: Ego_r.pdb `?PlayUnattachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@ABVC3DVector@@_N2@Z`; the position
//   argument is never read in retail (each candidate plays at the owner's CThing::GetPosition()). Callers: 0x004A595F,
//   0x0057C497, 0x007A00B3, 0x007A23C3.
// Callees: CThing::GetSampleBank() const @ 0x004C7A80, GetPotentialCriteriaStrings @ 0x0066BC10, CThing::GetPosition()
//   const @ 0x004C73D0 (out of line here), InternalPlaySoundEventAtPos @ 0x0066A920, ~CCharString @ 0x0099EAE0, free
//   @ 0x00BFEA14, vector<CCharString>::~vector @ 0x00414EA0 (the found-handle exit). Globals: the sound-enabled byte
//   @ 0x0137E974, the game state @ 0x013B8394, the invalid sound handle @ 0x0129D25C.
// Label correction: the catalog "CParticleMorphs_LoadAndProcessMorphs" is a propagated name.
// Shape: the landed PlayAttachedCriteria 0x0066BD00 body with the at-position player; the STLport vector destructors are
//   __forceinline and the found-handle `return` sits in `#pragma inline_depth(0)` (GOTCHAS "mixed inline/out-of-line"
//   recipe), which gives retail's inline fall-through copy and the out-of-line call. No pragma otherwise.
//   tried: plain inline dtors (both exits inline, 267v223); forceinline + inline_depth(0) (227v223); plain _Vector_base
//   dtor under the forceinline vector dtor (227v223); full STLport _Vector_base(const allocator&) / _STLP_alloc_proxy /
//   allocator::deallocate null test (227v223).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~_Vector_base() { if (_M_start != 0) _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    vector() { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
class CASoundBank;
class CThing {
public:
    CASoundBank& GetSampleBank() const;                // 0x004C7A80
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
extern bool          g_bSoundEnabled;                  // 0x0137E974
extern void*         g_pGameState;                     // 0x013B8394
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04
};
class CTCSoundPlayer : public CTCBase {
public:
    void GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEventAtPos(CASoundBank& bank, const C3DVector& pos, const CCharString& name, bool loop, bool attached);   // 0x0066A920
    unsigned long PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached);
};
unsigned long CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached)
{
    if (g_bSoundEnabled && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        std::vector<CCharString> names;
        GetPotentialCriteriaStrings(criteria, names);
        for (std::vector<CCharString>::iterator it = names.begin(); it != names.end(); ++it) {
            unsigned long handle = InternalPlaySoundEventAtPos(bank, ParentThing->GetPosition(), *it, loop, attached);
            if (handle != g_InvalidSoundHandle) {
#pragma inline_depth(0)
                return handle;
#pragma inline_depth()
            }
        }
    }
    return g_InvalidSoundHandle;
}
