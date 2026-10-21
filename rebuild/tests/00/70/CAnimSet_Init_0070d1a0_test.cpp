// Behaviour test for 0070d1a0: src/0070d1a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAnimSet::Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAnimSet::Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha) -- reset the set from its init data: one anim and one blend alpha (alpha, alpha), the current world frame, the speed/flag/priority fields and the looping/forwards bits
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070D1A0, length 329 bytes (`ret 0xc`).
// Callees: memmove @ 0x00BFEAE6 (the inlined vector<short>::clear's trivial copy), vector<short>::_M_insert_overflow
//          @ 0x00710980 and vector<CTransitionBlendAlpha>::_M_insert_overflow @ 0x00710F70 (manifest "Vector_Reallocate_*"),
//          CWorld::GetWorldFrame @ 0x0049D870 (static).
// Layout: Ego_r.pdb CAnimSet with the two 12-byte STLport vectors (PDB offsets 4 high after Anims and 8 high after
//          InternalAlphas): UpdateMode +0, Anims +4, InternalAlphas +0x10, CurrentPercentageComplete +0x1c,
//          LastPercentageComplete +0x20, Speed +0x24, FrameInternalAlphasUpdated +0x28, CurrentAlpha +0x2c, LastAlpha +0x30,
//          LastUpdateChange +0x34, ChangeAlphaBy +0x38, PAnimator +0x3c, Flags +0x40, TypeFlags +0x44, Priority +0x4c,
//          Type +0x50, bits +0x51 (Looping 0, Finished 1, Forwards 2, HadUpdate 3). CTransitionBlendAlpha {CurrentAlpha,
//          LastAlpha}. The init data block (descriptive names): UpdateMode +0, Type +4, Looping +5, Anim +8 (short), Speed
//          +0xc, Alpha +0x10, PercentageComplete +0x18, LastPercentageComplete +0x1c, Flags +0x20, TypeFlags +0x24,
//          Priority +0x28.
// Shape: each vector is `clear()` (inline erase(begin(), end()): memmove-based trivial copy for shorts, an element loop for
//          the 8-byte alphas) then `push_back` (inline fast path, _M_insert_overflow -- same-TU body -- with the __true_type tag
//          otherwise); `CurrentAlpha = LastAlpha = alpha;` before the zeroed change fields gives retail's store order (CurrentAlpha
//          through eax before `xor eax,eax`, LastAlpha from edx after); the
//          short temporary is homed in the dead animator argument slot.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }
struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);
    static T* copy(T* first, T* last, T* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    T* erase(T* first, T* last)
    {
        T* i = copy(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else {
            __true_type tag;
            _M_insert_overflow(_M_finish, x, tag, 1UL, true);
        }
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)
template <> short* vector<short>::copy(short* first, short* last, short* result)   // STLport __copy_trivial
{
    return (last == first) ? result
        : (short*)((char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
}

struct CTransitionBlendAlpha {
    float CurrentAlpha;
    float LastAlpha;
    CTransitionBlendAlpha(float current, float last) : CurrentAlpha(current), LastAlpha(last) { }
};
struct CWorld {
    static unsigned long GetWorldFrame();          // 0x0049D870
};
struct CAnimateThing;
struct CAnimSetInitData {
    long           UpdateMode;                     // +0x00
    unsigned char  Type;                           // +0x04
    bool           Looping;                        // +0x05
    unsigned char  _pad06[2];
    unsigned short Anim;                           // +0x08 (converted to the short element: a temp)
    float          Speed;                          // +0x0c
    float          Alpha;                          // +0x10
    long           _pad14;
    float          PercentageComplete;             // +0x18
    float          LastPercentageComplete;         // +0x1c
    unsigned long  Flags;                          // +0x20
    unsigned long  TypeFlags;                      // +0x24
    long           Priority;                       // +0x28
};

struct CAnimSet {
    long          UpdateMode;                      // +0x00
    vector<short> Anims;                           // +0x04
    vector<CTransitionBlendAlpha> InternalAlphas;  // +0x10
    float         CurrentPercentageComplete;       // +0x1c
    float         LastPercentageComplete;          // +0x20
    float         Speed;                           // +0x24
    unsigned long FrameInternalAlphasUpdated;      // +0x28
    float         CurrentAlpha;                    // +0x2c
    float         LastAlpha;                       // +0x30
    float         LastUpdateChange;                // +0x34
    float         ChangeAlphaBy;                   // +0x38
    CAnimateThing* PAnimator;                      // +0x3c
    unsigned long Flags;                           // +0x40
    unsigned long TypeFlags;                       // +0x44
    long          AnimDelay;                       // +0x48
    long          Priority;                        // +0x4c
    unsigned char Type;                            // +0x50
    bool          Looping : 1;                     // +0x51
    bool          Finished : 1;
    bool          Forwards : 1;
    bool          HadUpdate : 1;
    bool          OwnedByMainAnim : 1;
    void Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha);
};

void CAnimSet::Init(CAnimateThing* animator, const CAnimSetInitData& data, float alpha)
{
    PAnimator = animator;
    Anims.clear();
    Anims.push_back(data.Anim);
    InternalAlphas.clear();
    InternalAlphas.push_back(CTransitionBlendAlpha(data.Alpha, data.Alpha));
    FrameInternalAlphasUpdated = CWorld::GetWorldFrame();
    CurrentPercentageComplete = data.PercentageComplete;
    LastPercentageComplete = data.LastPercentageComplete;
    CurrentAlpha = LastAlpha = alpha;
    LastUpdateChange = 0.0f;
    ChangeAlphaBy = 0.0f;
    Type = data.Type;
    UpdateMode = data.UpdateMode;
    Looping = data.Looping;
    Speed = data.Speed;
    Finished = false;
    Forwards = true;
    Flags = data.Flags;
    TypeFlags = data.TypeFlags;
    HadUpdate = false;
    Priority = data.Priority;
}

// ---- stubs and driver ----

#include <stdlib.h>
static int g_grow = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; char** v = (char**)vector; unsigned long used = (unsigned long)(v[1] - v[0]);
    char* block = (char*)malloc(used + 64); if (v[0]) memcpy(block, v[0], used);
    v[0] = block; v[1] = block + used; v[2] = block + used + 64;
    char* p = v[1]; v[1] += (count == 1) ? 0 : 0; return p;   // the caller's element size is unknown here: bump below
}
unsigned long CWorld::GetWorldFrame() { return 1234; }
int main()
{
    static CAnimSet set; static short animStore[4] = { 9, 8, 7, 6 }; static CTransitionBlendAlpha alphaStore[2] = { CTransitionBlendAlpha(0.1f, 0.2f), CTransitionBlendAlpha(0.3f, 0.4f) };
    set.Anims._M_start = animStore; set.Anims._M_finish = animStore + 3; set.Anims._M_end_of_storage = animStore + 4;
    set.InternalAlphas._M_start = alphaStore; set.InternalAlphas._M_finish = alphaStore + 2; set.InternalAlphas._M_end_of_storage = alphaStore + 2;
    set.Looping = false; set.Finished = true; set.Forwards = false; set.HadUpdate = true; set.OwnedByMainAnim = true;
    set.LastUpdateChange = 5.0f; set.ChangeAlphaBy = 6.0f;
    CAnimSetInitData data; memset(&data, 0, sizeof data);
    data.UpdateMode = 3; data.Type = 2; data.Looping = true; data.Anim = 0x1234; data.Speed = 1.5f; data.Alpha = 0.75f;
    data.PercentageComplete = 0.25f; data.LastPercentageComplete = 0.125f; data.Flags = 0x11; data.TypeFlags = 0x22; data.Priority = 4;
    set.Init((CAnimateThing*)&data, data, 0.5f);
    CHECK(set.PAnimator == (CAnimateThing*)&data); CHECK(set.Anims._M_start == animStore); CHECK(set.Anims._M_finish == animStore + 1); CHECK(animStore[0] == 0x1234);
    CHECK(set.InternalAlphas._M_finish == set.InternalAlphas._M_start + 1 || g_grow == 1);
    CHECK(g_grow == 0); CHECK(alphaStore[0].CurrentAlpha == 0.75f && alphaStore[0].LastAlpha == 0.75f);
    CHECK(set.FrameInternalAlphasUpdated == 1234); CHECK(set.CurrentPercentageComplete == 0.25f); CHECK(set.LastPercentageComplete == 0.125f);
    CHECK(set.CurrentAlpha == 0.5f); CHECK(set.LastAlpha == 0.5f); CHECK(set.LastUpdateChange == 0.0f); CHECK(set.ChangeAlphaBy == 0.0f);
    CHECK(set.Type == 2); CHECK(set.UpdateMode == 3); CHECK(set.Looping == true); CHECK(set.Speed == 1.5f); CHECK(set.Finished == false); CHECK(set.Forwards == true);
    CHECK(set.Flags == 0x11); CHECK(set.TypeFlags == 0x22); CHECK(set.HadUpdate == false); CHECK(set.Priority == 4); CHECK(set.OwnedByMainAnim == true);
    // full vectors: both pushes take the growth path
    set.Anims._M_end_of_storage = set.Anims._M_finish = set.Anims._M_start; set.InternalAlphas._M_end_of_storage = set.InternalAlphas._M_finish = set.InternalAlphas._M_start;
    set.Init(0, data, 0.25f); CHECK(g_grow == 2); CHECK(*(short*)set.Anims._M_start == 0x1234); CHECK(set.CurrentAlpha == 0.25f); CHECK(set.PAnimator == 0);
    if (fails == 0) printf("0070d1a0_TEST PASS\n"); else printf("0070d1a0_TEST FAIL\n");
    return fails;
}