// std::vector::push_back(const CScriptThing& x) -- append a script thing (placement copy with a counted-pointer reference) or grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ADF90, length 72 bytes (+ int3 padding), `ret 4`.
// Identity: 38 callers across the script interface (0x008A8208 ... 0x00E622A4, e.g. the landed
//   CGameScriptInterface::GetAllCreaturesExcludingHero 0x008A89D0 inlines the same push_back). The catalog label
//   "Vector_PushBack_ScriptThing" is descriptive.
// Callees: vector<CScriptThing>::_M_insert_overflow @ 0x008AD5F0 (thiscall; position, x, the empty __false_type tag by
//   reference, fill length 1, at-end true).
// Shape: STLport 4.x `push_back`: `new (_M_finish) CScriptThing(x)` (placement null test, vtable 0x01238C8C, the
//   CCountedPointer copy constructor adding a reference) then `++_M_finish` (`add dword ptr [ecx+4],0Ch`); the
//   overflow call homes the empty tag in x's live argument slot, which VC7.1 does only with _M_insert_overflow's body
//   visible in the TU (GOTCHAS: a same-TU auto_inline(off) stand-in that reads x through its reference and never
//   touches the tag).
inline void* operator new(unsigned int, void* where) { return where; }
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCPPointerInfo { public:                                 // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void* Data;                                         // +0x08
};

template <class T>
class CCountedPointer {                                 // Ego_r.pdb, 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing : public CBaseClass {                // Ego_r.pdb, 12 bytes; vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    virtual ~CScriptThing();
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    void push_back(const _Tp& __x);
};

// Same-TU stand-in for the growth member 0x008AD5F0 (reads x through its reference only, never the tag).
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    if (*(void* const*)&__x != (void*)__position)
        return;
    _M_finish = __position + __fill_len + __atend;
}
#pragma auto_inline(on)

template <class _Tp>
void vector<_Tp>::push_back(const _Tp& __x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) _Tp(__x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
}
template void vector<CScriptThing>::push_back(const CScriptThing&);
}