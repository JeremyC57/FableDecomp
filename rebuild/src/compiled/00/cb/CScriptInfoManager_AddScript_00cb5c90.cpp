// CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section) -- register a script: when it names a code section, register that section (short name, the script's name) with the code-section manager and remember it; then add the script to the sorted Scripts map
// Leaf: ?AddScript@CScriptInfoManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB5C90, 240 bytes. __fastcall, `ret 8`. FableWin.pdb
// ?AddScript@CScriptInfoManager@@IAEXABVCScriptInfo@@VCCharString@@@Z (protected). Manifest label "AddScript" is right.
// Callees: CCharString copy ctor 0x0099EC30, dtor 0x0099EAE0; CTBaseSingleton<CCodeSectionManager>::Get 0x00414CE0
// (static __fastcall, out of line in this TU); CCodeSectionManager::RegisterSection(CCharString, CCharString, bool,
// bool) 0x009F05A0 (FableWin `QAEKVCCharString@@0_N1@Z`); STLport vector<CCharString>::_M_insert_overflow 0x00433530;
// vector<pair<CCharString,CScriptInfo> >::push_back 0x00CB7210 (landed, the Scripts map's own vector at +4).
// Layout: CScriptInfoManager (Ego_r.pdb, retail 12-byte STLport vectors): CurrentScriptID +0, Scripts +4 (vector
// +4..+0xf, Compare +0x10, Dirty +0x11), CodeSectionNames +0x14.
// Shape: `section.GetLength() > 0` is the inline null-tested length read; the two by-value CCharString arguments are
// built right to left in their slots before the out-of-line Get(); push_back(section) is STLport's inline push_back
// (null-tested placement copy, else _M_insert_overflow with the __false_type temporary); the map's inline Add builds a
// local pair<CCharString,CScriptInfo>(key, value), pushes it, destroys it and sets Dirty. Add must be __forceinline
// (a plain inline member stays an out-of-line call: 148v228). With _M_insert_overflow declaration-only VC7.1 homes the
// __false_type temporary in the dead `info` slot (`lea edx,[esp+30h]`); retail homes it on the live by-value `section`
// slot (`lea edx,[esp+34h]`), which the same-TU noinline stand-in body below (never reading the tag) reproduces.
#include <stddef.h>

inline void* operator new(unsigned int, void* where) { return where; }

class CCharStringData {
public:
    const char*   Data;                                 // +0x00
    unsigned long Length;                               // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;                       // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    long GetLength() const { return PStringData ? (long)PStringData->Length : 0; }
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                     // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

class CCodeSectionManager {
public:
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload);   // 0x009F05A0
};

template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                         // 0x00414CE0
};

namespace _STL {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator { };

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for the out-of-row 0x00433530 (grow, copy the head, fill __fill_len copies of __x, copy the tail
// unless __atend): with a visible body that never reads the tag, VC7.1 homes the __false_type temporary on the
// by-value `section` slot as retail.
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                                 unsigned int __fill_len, bool __atend)
{
    unsigned int __old_size = (unsigned int)(_M_finish - _M_start);
    unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
    _Tp* __new_start = (_Tp*)operator new(__len * sizeof(_Tp));
    _Tp* __new_finish = __new_start;
    for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
        new (__new_finish) _Tp(*__p);
    for (; __fill_len > 0; --__fill_len, ++__new_finish)
        new (__new_finish) _Tp(__x);
    if (!__atend)
        for (_Tp* __q = __position; __q != _M_finish; ++__q, ++__new_finish)
            new (__new_finish) _Tp(*__q);
    for (_Tp* __d = _M_start; __d != _M_finish; ++__d)
        __d->~_Tp();
    operator delete(_M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
}
#pragma auto_inline(on)
}

template <class K, class V>
struct CKeyPairCompareLess { };

template <class K, class V, class C>
class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    __forceinline void Add(const K& key, const V& value)
    {
        this->push_back(_STL::pair<K, V>(key, value));
        Dirty = true;
    }
};

template <>
void _STL::vector<_STL::pair<CCharString, CScriptInfo> >::push_back(const _STL::pair<CCharString, CScriptInfo>& __x);   // 0x00CB7210

class CScriptInfoManager {
public:
    long CurrentScriptID;                               // +0x00
    CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> > Scripts;   // +0x04
    _STL::vector<CCharString> CodeSectionNames;         // +0x14
protected:
    void AddScript(const CScriptInfo& info, CCharString section);
};

void CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section)
{
    if (section.GetLength() > 0) {
        CTBaseSingleton<CCodeSectionManager>::Get().RegisterSection(section, info.Name, false, false);
        CodeSectionNames.push_back(section);
    }
    Scripts.Add(info.Name, info);
}