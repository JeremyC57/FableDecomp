// CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached) -- expand the criteria into its candidate sound-event names and play the first one that starts from a new emitter at the owner's position; the invalid handle when sound is off or nothing plays
// Grade: RELOCATION_MATCH (agent902, 1 grade this pass) -- a same-TU noinline stand-in for GetPotentialCriteriaStrings 0x0066BC10 (this TU; it fills `names` but never keeps its address -- chunk-422 recipe) on top of the parked forceinline vector dtors + inline_depth(0) return: VC7.1 then zeroes edi only for the state test / the zero stores, keeps end in ebp and tests `test edi,edi` like retail (was 227v223: the CSE'd zero kept in ebp).
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
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (stand-in only)
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
namespace _STL {
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
    void GetPotentialCriteriaStrings(const CCharString& criteria, _STL::vector<CCharString>& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEventAtPos(CASoundBank& bank, const C3DVector& pos, const CCharString& name, bool loop, bool attached);   // 0x0066A920
    unsigned long PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached);
};
#pragma auto_inline(off)
// 0x0066BC10 (this TU): same-TU noinline stand-in -- it fills `names` but never keeps its address (chunk-422 recipe)
__declspec(noinline) void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, _STL::vector<CCharString>& names)
{
    CCharString* block = (CCharString*)malloc(sizeof(CCharString) * 2);
    block[0].PStringData = criteria.PStringData;
    names._M_start = block;
    names._M_finish = block + 1;
    names._M_end_of_storage = block + 2;
}
#pragma auto_inline(on)

unsigned long CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached)
{
    if (g_bSoundEnabled && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        _STL::vector<CCharString> names;
        GetPotentialCriteriaStrings(criteria, names);
        for (_STL::vector<CCharString>::iterator it = names.begin(); it != names.end(); ++it) {
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