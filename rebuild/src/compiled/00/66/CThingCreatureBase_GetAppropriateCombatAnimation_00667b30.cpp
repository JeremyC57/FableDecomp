// CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name) -- collect every appearance animation called `name` and return the one GetAppropriateCombatAnimationFromList picks
// Leaf: GetAppropriateCombatAnimation
// Naming wave 28 (family d4): 0x00666B50 is ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@..@Z
//   (Ego_r.pdb / FableWin.pdb const member, DECISIONS_W27 D4) over the STLport vector<const CAnimationEntry*>. Was CThingCreatureBase::..(const CRetailVector_dword&) (QAE); the candidate
//   list's grow-insert is the own row 0x005E1420's ?_M_insert_overflow@?$vector@PBVCAnimationEntry@@V?$allocator@..@std@@@std@@IAEX..@Z
//   (same-TU body kept; was CRetailVector_dword::InsertGrow_005e1420).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00667B30, length 204 bytes, `ret 4`, const CAnimationEntry* in eax.
// Label correction: the catalog/engine_api name "GetCombatAnimationSet" (returning CAnimationSet*) does not fit the body,
//   which returns the chosen vector element; the name used here is descriptive.
// Callees: CThingBaseDef::GetSubDef<CAppearanceDef> @ 0x0042B0A2, the animation set's lower bound by name @ 0x005DC020
//   (manifest "CVectorMap_LookupOrGetEnd"), the vector's insert-overflow @ 0x005E1420 (declared exactly as the landed
//   CRetailVector_dword::push_back 0x005E1EB0 declares it: the element is a pointer-sized dword), CThingCreatureBase::
//   GetAppropriateCombatAnimationFromList(const vector&) @ 0x00666B50, free @ 0x00BFEA14, the def's virtual Destroy.
// Layout: retail CThing PBaseDef +0x70; CAppearanceDef Animation +0x34 (a sorted vector of 0x14-byte {name key,
//   CAnimationEntry} records: first +0, last +4).
// Shape: the appearance def is released right after its animation set is addressed (null-tested local); the candidate
//   vector is a local STLport vector filled by the inline push_back with the entry address as an rvalue (homed in the
//   dead name slot, which the insert-overflow's scalar tag shares); the pick is `*(begin() + index)`.
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString;
class CAnimationEntry;
struct SAnimationSetEntry {                     // 0x14 bytes
    long Key;                                   // +0x00
    unsigned char Entry[0x10];                  // +0x04 CAnimationEntry
    const CAnimationEntry* GetEntryAddress() const { return (const CAnimationEntry*)Entry; }
};
class CAnimationSet { public:
    SAnimationSetEntry* first;                  // +0x00
    SAnimationSetEntry* last;                   // +0x04
    SAnimationSetEntry* LowerBound_005dc020(const CCharString& name);   // 0x005DC020
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector<const CAnimationEntry*>, 12 bytes
public:
    _Tp* first;
    _Tp* last;
    _Tp* end;
    vector() : first(0), last(0), end(0) { }
    ~vector() { if (first) free(first); }
protected:
    void _M_insert_overflow(_Tp* where, const _Tp& value, const __true_type& tag, unsigned int count, bool noTail);   // 0x005E1420
public:
    void push_back(const _Tp& value)
    {
        if (last != end) {
            new (last) _Tp(value);
            ++last;
        }
        else
            _M_insert_overflow(last, value, __true_type(), 1, true);
    }
    const _Tp& operator[](unsigned int n) const { return *(first + n); }
};
}
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its
// const reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* where, const _Tp& value, const std::__true_type& tag, unsigned int count, bool noTail)
{
    _Tp* p = (_Tp*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAppearanceDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingCreatureBase {
public:
    unsigned char _pad00[0x70];
    CThingBaseDef* PBaseDef;                    // +0x70
    long GetAppropriateCombatAnimationFromList(const std::vector<const CAnimationEntry*>& animations) const;   // 0x00666B50
    const CAnimationEntry* GetAppropriateCombatAnimation(const CCharString& name);
};

const CAnimationEntry* CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name)
{
    CDefPointer<CAppearanceDef> out;
    PBaseDef->GetSubDef(&out);
    CAppearanceDef* def = out.Object;
    CAnimationSet& animations = def->Animation;
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
    std::vector<const CAnimationEntry*> candidates;
    SAnimationSetEntry* found = animations.LowerBound_005dc020(name);
    for (SAnimationSetEntry* it = found; it != animations.last && it->Key == found->Key; ++it)
        candidates.push_back(it->GetEntryAddress());
    long index = GetAppropriateCombatAnimationFromList(candidates);
    return candidates[index];
}