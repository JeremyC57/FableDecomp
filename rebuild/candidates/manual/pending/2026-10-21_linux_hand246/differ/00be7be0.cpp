// Grade: DIFFER(361v391) -- the main-path sub-patch pointer: retail keeps it in edx (stored twice into the dead first-argument slot), hoists the constant 4 into esi (type compare and inline push_back step) and lays the 3/4/5 arm out first; ours keeps it in esi. Tried: branch inversion (fixed the water-only block), switch, by-value inline push_back, same-TU push_back/_M_insert_overflow stand-ins (fixed the home slot), negated type test
// CLandscapeBackgroundPatch::Render(bool withOcclusion, long a, long b) -- render the patch (with or without occlusion), then queue its water sub-patch (when it has polygons) on the water renderer's list for its water type -- types 3..5, 8 and the rest each have a "rendered" and an "occluded" list; a water-only patch just queues its sub-patch on the plain list
// Leaf: Render
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE7BE0, length 389 bytes, `ret 0Ch`.
// Callees: CLandscapeBackgroundPatch::RenderWithOcclusion @ 0x00BE79D0 (two arguments, result unused) and Render(long)
//   @ 0x00BE6F70 (bool), vector<CEngineWaterBackgroundSubPatch*>::push_back @ 0x00BE8FF0 (the out-of-line copy) and its
//   _M_insert_overflow @ 0x00BE8F10 (the two inlined copies), on the water renderer global @ 0x01436E54 (lists at +0x208,
//   +0x214, +0x22C, +0x238, +0x25C, +0x268).
// Layout: engine/CLandscapeBackgroundPatch.h (WaterSubPatch +0xF4, WaterOnlyPatch +0x11A) through a derived shell as in the
//   landed RelocateData 0x00BE65A0; the water sub-patch's polygon count (unsigned short +0x30) and water type (+0x40).
// Shape: the push_back is expanded inline at the two "plain list" sites and called out of line at the other four (the
//   explicit split of the landed CEngineLandscapePatch::RenderForegroundWater 0x00BF44A0); the sub-patch pointer pushed by
//   reference lives in the dead first-argument slot; the list choice is a ternary on the render result.
#include "engine/CLandscapeBackgroundPatch.h"
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
struct CEngineWaterBackgroundSubPatch {
    unsigned char  _pad00[0x30];
    unsigned short PolyCount;                      // +0x30
    unsigned char  _pad32[0x40 - 0x32];
    long           WaterType;                      // +0x40
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill, bool atEnd);   // 0x00BE8F10
    void push_back(const T& x);                    // 0x00BE8FF0 (the out-of-line copy)
    __forceinline void push_back_inline(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
typedef CRetailVector<CEngineWaterBackgroundSubPatch*> CWaterSubPatchList;
void* GrowWaterList(void* list, unsigned int count);
#pragma auto_inline(off)
template <class T> void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill, bool atEnd)
{
    T* p = (T*)GrowWaterList(this, fill);
    if (p)
        *p = x;
}
template <class T> void CRetailVector<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) T(x);
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
}
#pragma auto_inline(on)
struct CEngineWaterRenderer {
    unsigned char      _pad000[0x208];
    CWaterSubPatchList OtherOccluded;              // +0x208
    CWaterSubPatchList OtherRendered;              // +0x214
    unsigned char      _pad220[0x22c - 0x220];
    CWaterSubPatchList Type8Occluded;              // +0x22c
    CWaterSubPatchList Type8Rendered;              // +0x238
    unsigned char      _pad244[0x25c - 0x244];
    CWaterSubPatchList PlainOccluded;              // +0x25c
    CWaterSubPatchList PlainRendered;              // +0x268
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54

struct CLandscapeBackgroundPatch_Render : CLandscapeBackgroundPatch {
    void RenderWithOcclusion(long a, long b);      // 0x00BE79D0
    bool RenderPatch(long flags);                  // 0x00BE6F70
    void Render(bool withOcclusion, long a, long b);
};

void CLandscapeBackgroundPatch_Render::Render(bool withOcclusion, long a, long b)
{
    if (!WaterOnlyPatch) {
        bool rendered = false;
        if (withOcclusion)
            RenderWithOcclusion(a, b);
        else
            rendered = RenderPatch(0);
        CEngineWaterBackgroundSubPatch* water = WaterSubPatch;
        if (water && water->PolyCount > 0) {
            long type = water->WaterType;
            if (type == 3 || type == 4 || type == 5)
                (rendered ? g_pEngineWaterRenderer->PlainRendered : g_pEngineWaterRenderer->PlainOccluded).push_back_inline(water);
            else if (type == 8)
                (rendered ? g_pEngineWaterRenderer->Type8Rendered : g_pEngineWaterRenderer->Type8Occluded).push_back(water);
            else
                (rendered ? g_pEngineWaterRenderer->OtherRendered : g_pEngineWaterRenderer->OtherOccluded).push_back(water);
        }
    } else {
        CEngineWaterBackgroundSubPatch* water = WaterSubPatch;
        if (water && water->PolyCount > 0)
            g_pEngineWaterRenderer->PlainOccluded.push_back_inline(water);
    }
}
