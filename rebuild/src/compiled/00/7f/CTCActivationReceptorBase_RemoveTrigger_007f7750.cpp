// CTCActivationReceptorBase::RemoveTrigger(CTCActivationTrigger& trigger) -- erase every occurrence of the trigger's parent thing from the receptor's trigger list (STLport erase(remove(...), end()))
// Leaf: ?RemoveTrigger@CTCActivationReceptorBase@@QAEXAAVCTCActivationTrigger@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7750, 112 bytes (`ret 4`). FableWin.pdb
//   `?RemoveTrigger@CTCActivationReceptorBase@@QAEXAAVCTCActivationTrigger@@@Z`; callers 0x0077B09F / 0x0077B0BB.
//   Catalog label `vector_Erase_RemoveElement` was descriptive.
// Callees: STLport __find<CThing**, CThing*>(first, last, const T&, const random_access_iterator_tag&) @ 0x007F7970 (/Gr
//   `__fastcall`, four-way unrolled; manifest "vector_find_Value_16byte"), memmove @ 0x00BFEAE6.
// Layout: CTCActivationReceptorBase (Ego_r.pdb with a retail 12-byte STLport vector): CTCBase (0xc), Activated +0x0c,
//   Triggers +0x10 (vector<CThing*>: start +0x10, finish +0x14); CTCActivationTrigger's ParentThing +0x04.
// Shape: STLport remove (find, then the inline remove_copy compacting loop against the value held in ebx) and vector
//   erase(first, last) with STLport's __copy_trivial `(last == first) ? result : memmove(...) + (last - first)`; the new end
//   is a named local so erase's end() is re-read after the compaction (inside one call expression it is CSE'd first). The parent pointer comes through a by-value
//   accessor, so the const& value is a temporary homed in the dead `trigger` slot; the tag temporary (unnamed) shares
//   that slot only because the __find instantiation has its real body in the TU (noinline), as GOTCHAS 504. __find must be
//   a TEMPLATE: instantiated bodies are compiled after the caller, so VC does not learn that it preserves edx (a plain
//   function defined first lets the caller keep `last` in edx across the call; retail keeps it in esi). remove is the SGI
//   ternary form (`__first == __last ? __first : remove_copy(++__i, ...)`, one result register for both arms).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

class CThing;

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter first, _RandomAccessIter last, const _Tp& val,
                                                         const random_access_iterator_tag&)   // 0x007F7970
{
    long trip_count = (last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
        if (*first == val) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == val) return first;
        ++first;
    case 2:
        if (*first == val) return first;
        ++first;
    case 1:
        if (*first == val) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)

inline CThing** find(CThing** first, CThing** last, CThing* const& val)
{
    return __find(first, last, val, random_access_iterator_tag());
}

inline CThing** remove_copy(CThing** first, CThing** last, CThing** result, CThing* const& val)
{
    for (; first != last; ++first)
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
    return result;
}

inline CThing** remove(CThing** first, CThing** last, CThing* const& val)
{
    first = find(first, last, val);
    CThing** i = first;
    return first == last ? first : remove_copy(++i, last, first, val);
}

inline CThing** __copy_trivial(CThing** first, CThing** last, CThing** result)
{
    return (last == first) ? result
        : (CThing**)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
}

struct CThingVector {                                           // STLport vector<CThing*>
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
    CThing** erase(CThing** first, CThing** last)
    {
        CThing** i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
};

class CTCActivationTrigger {
public:
    virtual void v00();
    CThing* ParentThing;                                        // +0x04 (CTCBase)
    CThing* GetParentThing() const { return ParentThing; }      // by value: the find value is a temporary
};

class CTCActivationReceptorBase {
public:
    virtual void v00();
    CThing*       ParentThing;                                  // +0x04
    unsigned char _pad08[4];                                    // CTCBase flags
    bool          Activated;                                    // +0x0c
    CThingVector  Triggers;                                     // +0x10
    void RemoveTrigger(CTCActivationTrigger& trigger);
};

void CTCActivationReceptorBase::RemoveTrigger(CTCActivationTrigger& trigger)
{
    CThing** newEnd = remove(Triggers.begin(), Triggers.end(), trigger.GetParentThing());
    Triggers.erase(newEnd, Triggers.end());
}