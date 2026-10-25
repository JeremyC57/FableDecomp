// CGamePlayerInterface::IsControlInputSharedBetweenGameActions(const vector<EGameAction>& actions) const -- does any control input bound to one of the actions also appear (operator==) among the inputs already collected from the actions before it
// Grade: DIFFER(496v473) -- frame and addressing only: ours keeps the action key and the lower-bound entry in separate frame words (0x2C frame vs retail 0x24, where the key slot [esp+10h] is reused for the entry) and walks `lea ebp,[entry+4]` (the counted pointer) instead of retail's ebp = entry with +4/+8 offsets; the inlined push_back/_M_insert_overflow (needs __forceinline on both), the out-of-line __destroy_aux 0x00448860 with its tag homed on the live `actions` slot, and the call targets all agree.
// Leaf: ?IsControlInputSharedBetweenGameActions@CGamePlayerInterface_Methods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00446D10, 473 bytes (row 480), thiscall `ret 4`; Ego_r.pdb
// ?IsControlInputSharedBetweenGameActions@CGamePlayerInterface@@QBE_NABV?$vector@W4EGameAction@@...@std@@@Z. Caller 0x0068F664
// (CInputProcessCombat's ProcessCombatInput). Label correction: the catalog's "ProcessCombatInput_MapInputsToActions" is descriptive.
// Callees: Controls (+0x7a8) UpperBound 0x00447CE0 then LowerBound 0x00447C80, CInputTypeBase::operator== (slot 2), malloc
// 0x00BFEA0E, __destroy_aux<CCountedPointer<const CInputTypeBase>*> 0x00448860 (fastcall, tag), free 0x00BFEA14.
// Tried: plain template _M_insert_overflow (called out of line), `inline` (still out of line), __forceinline overflow
// (push_back then called out of line), __forceinline overflow + push_back (496v473, this file).
#include "engine/CGamePlayerInterface.h"
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
void __cdecl operator delete(void* p);                    // 0x00BFE9BC

enum EGameAction { GAME_ACTION_NONE };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();                                       // slot 0
    virtual long GetInputType() const;                               // slot 1
    virtual bool operator==(const CInputTypeBase& other) const;      // slot 2 (+0x8)
};
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T& operator*() const { return *Data; }
    T* operator->() const { return Data; }
};
struct CControlsEntry {                                  // std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >
    EGameAction                             first;
    CCountedPointer<const CInputTypeBase>   second;
};
class CControlsMap {                                     // CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, ...>
public:
    CControlsEntry* First;
    CControlsEntry* Last;
    CControlsEntry* EndOfStorage;
    const CControlsEntry* LowerBound(const EGameAction& key) const;   // 0x00447C80
    const CControlsEntry* UpperBound(const EGameAction& key) const;   // 0x00447CE0
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)   // 0x00448860
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void _M_clear() { _Destroy(_M_start, _M_finish); if (_M_start != 0) free(_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage = __e; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);
    __forceinline void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Tp>
__forceinline void vector<_Tp>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                                     bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = allocate(__len);
    pointer __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp> class const_vector_view {            // the caller's const vector<EGameAction>
public:
    const _Tp* _M_start;
    const _Tp* _M_finish;
    const _Tp* _M_end_of_storage;
    const _Tp* begin() const { return _M_start; }
    const _Tp* end() const { return _M_finish; }
};

}   // namespace std

struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsMap& GetControls() const { return *reinterpret_cast<const CControlsMap*>(Controls); }
    bool IsControlInputSharedBetweenGameActions(const std::const_vector_view<EGameAction>& actions) const;
};

bool CGamePlayerInterface_Methods::IsControlInputSharedBetweenGameActions(const std::const_vector_view<EGameAction>& actions) const
{
    std::vector<CCountedPointer<const CInputTypeBase> > inputs;
    for (const EGameAction* it = actions.begin(); it != actions.end(); ++it) {
        EGameAction action = *it;
        const CControlsEntry* end = GetControls().UpperBound(action);
        for (const CControlsEntry* entry = GetControls().LowerBound(action); entry != end; ++entry) {
            for (std::vector<CCountedPointer<const CInputTypeBase> >::iterator input = inputs.begin(); input != inputs.end(); ++input) {
                if (**input == *entry->second)
                    return true;
            }
            inputs.push_back(entry->second);
        }
    }
    return false;
}
