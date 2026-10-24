// CQuestManager::ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources) -- activate one quest by wrapping its name in a one-element array and handing it to ActivateMultipleQuests
// Leaf: ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B4A10, 137 bytes (144 with padding), `ret 0Ch` (__fastcall).
// Identity: Ego_r.pdb `?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z`; the landed callers (for example
//   CCreatureAction_ActivateQuest::Initialise 0x00843FC0) declare it at 0x004B4A10.
// Callees: vector<CCharString>::_M_insert_overflow 0x00433530 (the push_back growth path; a same-TU stand-in body
//   precedes the function, see Shape), CQuestManager::ActivateMultipleQuests(const CArray<CCharString>&, bool, bool)
//   0x004B4260 (same TU in retail; a same-TU stand-in body precedes the function, see Shape), ~CCharString 0x0099EAE0 (the inlined vector destructor), free 0x00BFEA14.
// Shape: `CArray<CCharString> quests; quests.push_back(quest_name); return ActivateMultipleQuests(quests, flag,
//   load_resources);`. VC7.1 folds the empty vector's `_M_finish != _M_end_of_storage` test, so only the
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` call remains (position 0 pushed as an immediate) and the
//   tag temporary is homed on the dead quest-name argument slot -- which needs the growth member's body visible and
//   tag-free (GOTCHAS retry 10f). The bool result is parked in that slot across the inlined STLport ~vector
//   (`~vector() { _Destroy(_M_start, _M_finish); }` by value, then ~_Vector_base's `if (_M_start != 0) deallocate`):
//   retail keeps start/finish in edi/ebp across the element destructors only because the array's address provably
//   does not escape, which VC7.1 sees once ActivateMultipleQuests has a same-TU body (declared-only: start reloaded).
// Label correction: catalogued as `CQuestManager_ActivateQuest` (right).
#include <stdlib.h>                                   // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

struct __false_type { __false_type() { } };            // user constructor: no zero-initialised tag temporary

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

template <class _Tp> struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void _Destroy(CCharString* __first, CCharString* __last)
{
    for ( ; __first != __last; ++__first)
        __first->~CCharString();
}

struct _CCharStringVectorBase {                      // STLport _Vector_base<CCharString>
    CCharString* _M_start;                           // +0x00
    CCharString* _M_finish;                          // +0x04
    _STLP_alloc_proxy<CCharString*, CCharString, allocator<CCharString> > _M_end_of_storage;   // +0x08
    _CCharStringVectorBase() : _M_start(0), _M_finish(0), _M_end_of_storage(allocator<CCharString>(), (CCharString*)0) { }
    ~_CCharStringVectorBase()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

struct CCharStringVector : _CCharStringVectorBase {   // STLport vector<CCharString>
    ~CCharStringVector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_insert_overflow(CCharString* __position, const CCharString& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);   // 0x00433530
    void push_back(const CCharString& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) CCharString(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the STLport growth COMDAT 0x00433530: reads the vector and x first, never touches the tag.
void CCharStringVector::_M_insert_overflow(CCharString* __position, const CCharString& __x, const __false_type&,
                                           unsigned int __fill_len, bool __atend)
{
    CCharString* __old_start = _M_start;
    CCharString* __old_finish = _M_finish;
    unsigned int __old_size = unsigned int(__old_finish - __old_start);
    unsigned int __len = __old_size + (__old_size < __fill_len ? __fill_len : __old_size);
    CCharString* __new_start = _M_end_of_storage.allocate(__len);
    CCharString* __new_finish = __new_start;
    for (CCharString* __p = __old_start; __p != __position; ++__p, ++__new_finish)
        new (__new_finish) CCharString(*__p);
    for ( ; __fill_len > 0; --__fill_len, ++__new_finish)
        new (__new_finish) CCharString(__x);
    if (!__atend)
        for (CCharString* __q = __position; __q != __old_finish; ++__q, ++__new_finish)
            new (__new_finish) CCharString(*__q);
    for (CCharString* __d = __old_start; __d != __old_finish; ++__d)
        __d->~CCharString();
    _M_end_of_storage.deallocate(__old_start, _M_end_of_storage._M_data - __old_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)

template <class T> class CArray;
template <> class CArray<CCharString> : public CCharStringVector { };

class CQuestManager {
public:
    void* GetActiveQuestInfo(const CCharString* quest_name);   // 0x004B00C0 (the per-name step of ActivateMultipleQuests)
    bool ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool flag, bool load_resources);   // 0x004B4260
    bool ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources);
};

#pragma auto_inline(off)
// Same-TU stand-in for CQuestManager::ActivateMultipleQuests 0x004B4260 (compiled in this TU in retail, not inlined):
// it walks the names by index and never keeps the array's address, so VC knows the local array does not escape and
// keeps its start/finish words in registers across the element destructors, as retail does.
bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& quest_names, bool flag, bool load_resources)
{
    bool activated = false;
    for (unsigned int i = 0; i < unsigned int(quest_names._M_finish - quest_names._M_start); ++i)
        if (GetActiveQuestInfo(quest_names._M_start + i) && (flag || load_resources))
            activated = true;
    return activated;
}
#pragma auto_inline(on)

bool CQuestManager::ActivateQuest(const CCharString& quest_name, bool flag, bool load_resources)
{
    CArray<CCharString> quests;
    quests.push_back(quest_name);
    return ActivateMultipleQuests(quests, flag, load_resources);
}