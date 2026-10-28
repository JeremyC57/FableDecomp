// std::__find<CScriptThing*, CScriptThing>(CScriptThing* first, CScriptThing* last, const CScriptThing& val, const random_access_iterator_tag&) -- STLport's four-way unrolled random-access find: the first script thing equal to val (CScriptThing::operator==, the implementations' IsEqualTo), or `last`
// Leaf: ??$__find@PAVCScriptThing@@V1@@std@@YIPAVCScriptThing@@PAV1@0ABV1@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB5D0, 265 bytes, `__fastcall` (first in ecx, last in edx), `ret 8` (the value and
//   the tag references).
// Identity: the only callers are CGameScriptInterface::GetFollowingEntityList 0x008A80F0 (landed; it already names this
//   "STLport __find<CScriptThing*, CScriptThing>(first, last, val, random_access_iterator_tag) @ 0x008AB5D0") and 0x008ABDDA;
//   the body is STLport 4.x _algobase.c `__find` for random-access iterators: trip count (last - first) >> 2 (the 12-byte
//   element divide by 0x2AAAAAAB), four compares per trip, then `switch (last - first)` cases 3 / 2 / 1 falling through.
//   The catalog label "std::lower_bound_with_vtable" is descriptive and wrong (it is a linear find, not a lower bound).
// Callees: CScriptThing::IsEqualTo(const CScriptThing&) const, virtual slot 78 (+0x138; CScriptThing 0x004AA8A0), reached
//   through the inline CScriptThing::operator==(const CScriptThing&) const (Ego_r.pdb ??8CScriptThing@@QBE_NABV0@@Z):
//   `PImp->IsEqualTo(*other.PImp)`.
// Layout: Ego_r.pdb CScriptThing (12 bytes): vptr, PImp CCountedPointer<CScriptThing> (+0x04 Data, +0x08 Info).
// Shape: /Gr TU (STLport templates are __fastcall here, as the landed __find_if 0x008C2F70); same template text as that row
//   with `*__first == __val` for the predicate.
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                        // +0x00
    CCPPointerInfo* Info;                                        // +0x04
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
class CBaseClass {
public:
    virtual ~CBaseClass();                                       // slot 0 (0x0099A2E0)
};
class CScriptThing : public CBaseClass {
public:
    // slots 1-77: placeholders (not called here, names not modelled)
    virtual void ScriptThingVSlot01(); virtual void ScriptThingVSlot02(); virtual void ScriptThingVSlot03(); virtual void ScriptThingVSlot04(); virtual void ScriptThingVSlot05();
    virtual void ScriptThingVSlot06(); virtual void ScriptThingVSlot07(); virtual void ScriptThingVSlot08(); virtual void ScriptThingVSlot09(); virtual void ScriptThingVSlot10();
    virtual void ScriptThingVSlot11(); virtual void ScriptThingVSlot12(); virtual void ScriptThingVSlot13(); virtual void ScriptThingVSlot14(); virtual void ScriptThingVSlot15();
    virtual void ScriptThingVSlot16(); virtual void ScriptThingVSlot17(); virtual void ScriptThingVSlot18(); virtual void ScriptThingVSlot19(); virtual void ScriptThingVSlot20();
    virtual void ScriptThingVSlot21(); virtual void ScriptThingVSlot22(); virtual void ScriptThingVSlot23(); virtual void ScriptThingVSlot24(); virtual void ScriptThingVSlot25();
    virtual void ScriptThingVSlot26(); virtual void ScriptThingVSlot27(); virtual void ScriptThingVSlot28(); virtual void ScriptThingVSlot29(); virtual void ScriptThingVSlot30();
    virtual void ScriptThingVSlot31(); virtual void ScriptThingVSlot32(); virtual void ScriptThingVSlot33(); virtual void ScriptThingVSlot34(); virtual void ScriptThingVSlot35();
    virtual void ScriptThingVSlot36(); virtual void ScriptThingVSlot37(); virtual void ScriptThingVSlot38(); virtual void ScriptThingVSlot39(); virtual void ScriptThingVSlot40();
    virtual void ScriptThingVSlot41(); virtual void ScriptThingVSlot42(); virtual void ScriptThingVSlot43(); virtual void ScriptThingVSlot44(); virtual void ScriptThingVSlot45();
    virtual void ScriptThingVSlot46(); virtual void ScriptThingVSlot47(); virtual void ScriptThingVSlot48(); virtual void ScriptThingVSlot49(); virtual void ScriptThingVSlot50();
    virtual void ScriptThingVSlot51(); virtual void ScriptThingVSlot52(); virtual void ScriptThingVSlot53(); virtual void ScriptThingVSlot54(); virtual void ScriptThingVSlot55();
    virtual void ScriptThingVSlot56(); virtual void ScriptThingVSlot57(); virtual void ScriptThingVSlot58(); virtual void ScriptThingVSlot59(); virtual void ScriptThingVSlot60();
    virtual void ScriptThingVSlot61(); virtual void ScriptThingVSlot62(); virtual void ScriptThingVSlot63(); virtual void ScriptThingVSlot64(); virtual void ScriptThingVSlot65();
    virtual void ScriptThingVSlot66(); virtual void ScriptThingVSlot67(); virtual void ScriptThingVSlot68(); virtual void ScriptThingVSlot69(); virtual void ScriptThingVSlot70();
    virtual void ScriptThingVSlot71(); virtual void ScriptThingVSlot72(); virtual void ScriptThingVSlot73(); virtual void ScriptThingVSlot74(); virtual void ScriptThingVSlot75();
    virtual void ScriptThingVSlot76(); virtual void ScriptThingVSlot77();
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
protected:
    virtual bool IsEqualTo(const CScriptThing& other) const;     // slot 78 (+0x138)
public:
    CCountedPointer<CScriptThing> PImp;                          // +0x04
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
template CScriptThing* __fastcall __find<CScriptThing*, CScriptThing>(CScriptThing*, CScriptThing*, const CScriptThing&,
                                                                      const random_access_iterator_tag&);
}