// CThingCreatureBase::GetAppropriateCombatAnimation(const CCharString& name) -- collect every appearance animation called `name` and return the one GetAppropriateCombatAnimationFromList picks
// Leaf: GetAppropriateCombatAnimation
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

struct CCharString;
struct CAnimationEntry;
struct SAnimationSetEntry {                     // 0x14 bytes
    long Key;                                   // +0x00
    unsigned char Entry[0x10];                  // +0x04 CAnimationEntry
    long GetEntryAddress() const { return (long)Entry; }
};
struct CAnimationSet {
    SAnimationSetEntry* first;                  // +0x00
    SAnimationSetEntry* last;                   // +0x04
    SAnimationSetEntry* LowerBound_005dc020(const CCharString& name);   // 0x005DC020
};

struct SScalarTag { SScalarTag() { } };
struct CRetailVector_dword {                    // STLport vector<const CAnimationEntry*> seen as dwords
    long* first;
    long* last;
    long* end;
    CRetailVector_dword() : first(0), last(0), end(0) { }
    ~CRetailVector_dword() { if (first) free(first); }
    void InsertGrow_005e1420(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x005E1420
    void push_back(const long& value)
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow_005e1420(last, value, SScalarTag(), 1, true);
    }
    const long& operator[](long n) const { return *(first + n); }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
__declspec(noinline) void CRetailVector_dword::InsertGrow_005e1420(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    long* p = (long*)GrowVectorStorage_s7(this, count);
    if (p)
        *p = value;
    
}
#pragma auto_inline(on)

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CAppearanceDef : CDefPointeeBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
struct CThingBaseDef : CDefPointeeBase {
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
    long GetAppropriateCombatAnimationFromList(const CRetailVector_dword& animations);   // 0x00666B50
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
    CRetailVector_dword candidates;
    SAnimationSetEntry* found = animations.LowerBound_005dc020(name);
    for (SAnimationSetEntry* it = found; it != animations.last && it->Key == found->Key; ++it)
        candidates.push_back(it->GetEntryAddress());
    long index = GetAppropriateCombatAnimationFromList(candidates);
    return (const CAnimationEntry*)candidates[index];
}