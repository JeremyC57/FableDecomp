#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
// Model of std::const_mem_fun1_t<bool, CThing, CCharString>::operator() @ 0x0048B2F0: calls the bound const member
// (CThing::IsSameScriptName) on p with a copy of the script name.
static int gCopies = 0, gDtors = 0;
class CCharString {
public:
    void* PStringData;                                      // +0x0
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++gCopies; }                  // 0x0099EC30
    ~CCharString() { ++gDtors; }                                         // 0x0099EAE0
};

class CThing {
public:
    bool IsSameScriptName(CCharString name) const { return name.PStringData == ScriptName; }   // stand-in
    void* ScriptName;          // FableWin.pdb ?IsSameScriptName@CThing@@QBE_NVCCharString@@@Z
};

namespace std {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_t : public binary_function<const _Tp*, _Arg, _Ret> {
    typedef _Ret (_Tp::*__fun_type)(_Arg) const;
public:
    explicit const_mem_fun1_t(__fun_type __pf) : _M_f(__pf) { }
    _Ret operator()(const _Tp* __p, _Arg __x) const { return (__p->*_M_f)(__x); }
private:
    __fun_type _M_f;                                        // +0x0
};

}
int main()
{
    bool ok = true;
    static int names[2];
    CThing thing;
    thing.ScriptName = &names[0];
    union { double align; unsigned char bytes[sizeof(CCharString)]; } raw0, raw1;
    CCharString* same = (CCharString*)raw0.bytes;
    CCharString* other = (CCharString*)raw1.bytes;
    same->PStringData = &names[0];
    other->PStringData = &names[1];
    std::const_mem_fun1_t<bool, CThing, CCharString> f(&CThing::IsSameScriptName);
    if (!f(&thing, *same) || f(&thing, *other))
        ok = false;
    if (gCopies < 2 || gCopies != gDtors)
        ok = false;
    if (!ok) { printf("0048b2f0_TEST FAIL\n"); return 1; }
    printf("0048b2f0_TEST PASS\n");
    return 0;
}