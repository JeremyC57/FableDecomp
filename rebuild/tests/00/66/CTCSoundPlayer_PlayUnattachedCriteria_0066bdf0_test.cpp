// Behaviour test for 0x0066BDF0 (source body copied from src/0066bdf0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
namespace _STL {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { for (; __first != __last; ++__first) _Destroy(__first); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~_Vector_base() { if (_M_start != 0) _Alloc().deallocate(_M_start, _M_end_of_storage - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    vector() { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
class CASoundBank;
class CThing {
public:
    CASoundBank& GetSampleBank() const;                // 0x004C7A80
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
extern bool          g_bSoundEnabled;                  // 0x0137E974
extern void*         g_pGameState;                     // 0x013B8394
extern unsigned long g_InvalidSoundHandle;             // 0x0129D25C
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04
};
class CTCSoundPlayer : public CTCBase {
public:
    void GetPotentialCriteriaStrings(const CCharString& criteria, _STL::vector<CCharString>& names);   // 0x0066BC10
    unsigned long InternalPlaySoundEventAtPos(CASoundBank& bank, const C3DVector& pos, const CCharString& name, bool loop, bool attached);   // 0x0066A920
    unsigned long PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached);
};
#pragma auto_inline(off)
// 0x0066BC10 (this TU): same-TU noinline stand-in -- it fills `names` but never keeps its address (chunk-422 recipe)
__declspec(noinline) void CTCSoundPlayer::GetPotentialCriteriaStrings(const CCharString& criteria, _STL::vector<CCharString>& names)
{
    CCharString* block = (CCharString*)malloc(sizeof(CCharString) * 2);
    block[0].PStringData = criteria.PStringData;
    names._M_start = block;
    names._M_finish = block + 1;
    names._M_end_of_storage = block + 2;
}
#pragma auto_inline(on)

unsigned long CTCSoundPlayer::PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& pos, bool loop, bool attached)
{
    if (g_bSoundEnabled && g_pGameState) {
        CASoundBank& bank = ParentThing->GetSampleBank();
        _STL::vector<CCharString> names;
        GetPotentialCriteriaStrings(criteria, names);
        for (_STL::vector<CCharString>::iterator it = names.begin(); it != names.end(); ++it) {
            unsigned long handle = InternalPlaySoundEventAtPos(bank, ParentThing->GetPosition(), *it, loop, attached);
            if (handle != g_InvalidSoundHandle) {
#pragma inline_depth(0)
                return handle;
#pragma inline_depth()
            }
        }
    }
    return g_InvalidSoundHandle;
}


// ---- test harness ----
bool          g_bSoundEnabled = false;
void*         g_pGameState = 0;
unsigned long g_InvalidSoundHandle = 0xFFFFFFFFu;
static int g_dtors = 0, g_plays = 0, g_bank = 0;
static unsigned long g_result = 0;
static const void* g_lastName = 0;
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
static const C3DVector* g_lastPos = 0;
CCharString::~CCharString() { ++g_dtors; }
CASoundBank& CThing::GetSampleBank() const { return *(CASoundBank*)&g_bank; }
const C3DVector& CThing::GetPosition() const { return g_pos; }
unsigned long CTCSoundPlayer::InternalPlaySoundEventAtPos(CASoundBank&, const C3DVector& pos, const CCharString& name, bool loop, bool attached)
{
    ++g_plays; g_lastName = name.PStringData; g_lastPos = &pos;
    return (loop && !attached) ? g_result : g_InvalidSoundHandle;
}
int main()
{
    static double traw[sizeof(CTCSoundPlayer) / sizeof(double) + 1];
    memset(traw, 0, sizeof traw);
    CTCSoundPlayer* p = (CTCSoundPlayer*)traw;
    static double thingRaw[4];
    p->ParentThing = (CThing*)thingRaw;
    CCharString criteria; criteria.PStringData = (void*)0x1234;
    C3DVector unused = { 9.0f, 9.0f, 9.0f };
    int ok = 1;
    if (p->PlayUnattachedCriteria(criteria, unused, true, false) != g_InvalidSoundHandle || g_plays != 0) ok = 0;   // sound off
    g_bSoundEnabled = true; g_pGameState = (void*)&g_bank; g_result = 42;
    if (p->PlayUnattachedCriteria(criteria, unused, true, false) != 42 || g_plays != 1) ok = 0;                  // first plays
    if (g_lastName != (void*)0x1234 || g_lastPos != &g_pos || g_dtors != 1) ok = 0;
    if (p->PlayUnattachedCriteria(criteria, unused, false, false) != g_InvalidSoundHandle || g_plays != 2 || g_dtors != 2) ok = 0;
    printf("0066bdf0_TEST %s\n", ok ? "PASS" : "FAIL");
    criteria.PStringData = 0;
    return ok ? 0 : 1;
}