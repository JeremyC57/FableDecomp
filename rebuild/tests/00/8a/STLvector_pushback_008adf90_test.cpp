#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Test model of CScriptThing (polymorphic root + CCountedPointer PImp) with the counted pointer's copy/assignment.
class CCPPointerInfo { public:
    unsigned int RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
static int g_deleted = 0;
static void __fastcall CountDelete(void*) { ++g_deleted; }
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ShareData(0, 0); }
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
class CBaseClass {
public:
    virtual ~CBaseClass() { }
};
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing() { }
};
static CCPPointerInfo* NewInfo(unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs;
    info->DeleteFunc = CountDelete;
    info->Data = 0;
    return info;
}
// std::vector<CScriptThing>::push_back @ 0x008ADF90: placement copy at the end when there is room, otherwise
// _M_insert_overflow(finish, x, __false_type(), 1, true) (a growing stand-in here).
struct __false_type { __false_type() { } };
static int g_overflows = 0;
template <class _Tp>
class Vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    Vector(unsigned int capacity)
    {
        _M_start = (_Tp*)malloc(capacity * sizeof(_Tp));
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + capacity;
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
    {
        ++g_overflows;
        unsigned int old = (unsigned int)(__position - _M_start);
        unsigned int len = old + (old > __fill_len ? old : __fill_len);
        _Tp* fresh = (_Tp*)malloc(len * sizeof(_Tp));
        for (unsigned int i = 0; i < old; ++i) {
            new (fresh + i) _Tp(_M_start[i]);
            _M_start[i].~_Tp();
        }
        for (unsigned int j = 0; j < __fill_len; ++j)
            new (fresh + old + j) _Tp(__x);
        free(_M_start);
        _M_start = fresh;
        _M_finish = fresh + old + __fill_len;
        _M_end_of_storage = fresh + len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1U, true);
    }
};
int main()
{
    bool ok = true;
    CCPPointerInfo* info = NewInfo(1);
    CScriptThing thing;
    thing.PImp.Info = info;
    Vector<CScriptThing> v(2);
    v.push_back(thing);
    v.push_back(thing);
    if (v._M_finish - v._M_start != 2 || g_overflows != 0 || info->RefCount != 3) ok = false;
    v.push_back(thing);                                 // full: goes through _M_insert_overflow
    if (v._M_finish - v._M_start != 3 || g_overflows != 1 || info->RefCount != 4) ok = false;
    for (int i = 0; i < 3; ++i)
        if (v._M_start[i].PImp.Info != info) ok = false;
    if (!ok) { std::printf("008adf90_TEST FAIL\n"); return 1; }
    std::printf("008adf90_TEST PASS\n");
    return 0;
}