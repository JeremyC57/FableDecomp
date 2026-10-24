// std::remove_copy_if(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>* result, CIsThingDead pred) -- copy every live thing pointer down to result and return the new end
// Leaf: ??$remove_copy_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4210, 85 bytes (+ int3 padding), __fastcall `ret 8` (first in ecx, last in edx;
//   result and the empty predicate on the stack).
// Callers: CTCFadeOutAndIn::UpdateFadingThings 0x006C3EB7 (this chunk's row 36, the STLport remove_if tail) and 0x006C43DE.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (three times per element), SetPItem(CBaseObject*) @ 0x00A01B90 (landed).
// Layout: CIntelligentPointer<CThing> {vptr, PData} = 8 bytes; retail CThing keeps its state bits at +0x91 (Dead = 0x01,
//   as the landed CThingSearchTools rows).
// Shape: STLport 4.x `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return result;`
//   -- CIsThingDead is `GetPItem() == 0 || GetPItem()->IsDead()` (two fetches), the CIntelligentPointer assignment is
//   `SetPItem(other.GetPItem())` (a third fetch).
// Label correction: the catalog `_Remove_if<CIntelligentPointer<CThing>*,CIsThingDead>` is the Dinkumware donor name;
//   retail STLport calls it remove_copy_if (remove_if itself is inline in the caller).
class CBaseObject;
class CBaseObjectPointer;

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char StateBits;                            // +0x91 Dead 0x01
    bool IsDead() const { return (StateBits & 0x01) != 0; }
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CIsThingDead {
public:
    CIsThingDead() { }
    bool operator()(const CIntelligentPointer<CThing>& thing) const
    {
        return thing.GetPItem() == 0 || thing.GetPItem()->IsDead();
    }
};

namespace std {

template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for (; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}

template CIntelligentPointer<CThing>* __fastcall remove_copy_if(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
                                                                CIntelligentPointer<CThing>*, CIsThingDead);

}