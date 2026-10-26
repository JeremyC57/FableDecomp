#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
// CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox(CEngineInternalPrimitiveOcclusionBase* primitive) -- run the primitive's occlusion test for the current layer: render it directly when occlusion is off; otherwise check last frame's result and, with no test pending, draw its bounding box as a new occlusion test (or mark it visible when the box crosses the near plane), plus the debug box colours
// Leaf: RenderBoundingBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91140, length 508 bytes (+ int3 padding), `ret 4`.
// Callees: CEnginePrimitiveOcclusionManager::CheckVisibilityResult 0x00B2F8E0 and the record reset 0x00B2FA40 (manifest
//   label "CV_BanditTollScript::Init" is wrong: an engine-module member called on the manager global 0x01436E58);
//   IsClippedByNearPlane 0x00B900B0, DrawVisibilityTestBox 0x00B906C0, DrawDebugVisibilityTestBox 0x00B90700 (on `this`; PDB `(const CMatrix3x4&, unsigned long)`); the reallocating
//   vector insert 0x00464ACE; memmove 0x00BFEAE6 (the dead arm of the inlined STLport erase); the primitive's virtual
//   slots 16 (+0x40, world bounding box) and 28 (+0x70, render without occlusion).
// Layout: Ego_r.pdb CEnginePrimitiveRendererSharedData (global 0x01436E78): CurrentOcclusionLayer +0x78, OcclusionEnable
//   +0xbd, ShowVisibilityInfo +0x24c; CEngineOcclusionManager (global 0x01436E5C, header engine/CEngineOcclusionManager.h
//   offsets): CurrentFrame +0x4010, Entries +0x4014, EntriesSize +0x401c; CEnginePrimitiveOcclusionInfo (landed
//   0x00B2F8E0): OcclusionTestHandles {first,last,end} +0xc, TestFrame +0x1c; CEngineInternalPrimitiveOcclusionBase:
//   Type +8 (CEnginePersistentPrimitiveBase), GridPos.Level (schar) +0x24 (CEngineInternalPrimitiveBase), OcclusionInfo
//   CIVCountedPointer[2] +0x48. The engine headers keep these as opaque bytes, so they are modelled locally.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
#ifdef _MSC_VER
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
#else
inline void* operator new(__SIZE_TYPE__, void* where) { return where; }
#endif

class CMatrix3x4 { public:
    float M[12];
};

struct SScalarTag { SScalarTag() {} };

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    unsigned int bytes = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, bytes) + bytes;
}

struct CHandleVector {                            // STLport vector<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    __forceinline void clear() { _M_finish = (unsigned long*)__copy_trivial(_M_finish, _M_finish, _M_start); }
    template <class Tag> void _M_insert_overflow(unsigned long* where, const unsigned long& value, const Tag&, unsigned long count,
                            bool atEnd);   // 0x00464ACE
    __forceinline void push_back(const unsigned long& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) unsigned long(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, SScalarTag(), 1, true);
    }
    __forceinline void Add(unsigned long value) { push_back(value); }
};

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
// Same-TU body of vector<unsigned long>::_M_insert_overflow 0x00464ACE (tag unused).
template <class Tag> __declspec(noinline) void CHandleVector::_M_insert_overflow(unsigned long* where, const unsigned long& value, const Tag&,
                                                                               unsigned long count, bool atEnd)
{
    unsigned long oldSize = _M_finish - _M_start;
    unsigned long len = oldSize + (oldSize < count ? count : oldSize);
    unsigned long* newStart = len ? (unsigned long*)malloc(len * sizeof(unsigned long)) : 0;
    unsigned long* newFinish = (unsigned long*)__copy_trivial(_M_start, where, newStart);
    for (; count != 0; --count)
        *newFinish++ = value;
    if (!atEnd)
        newFinish = (unsigned long*)__copy_trivial(where, _M_finish, newFinish);
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}

class CEnginePrimitiveOcclusionInfo { public:
    void*          _vptr;                          // +0x00
    unsigned long  RefCount;                       // +0x04
    long           InvisibleFrames;                // +0x08
    CHandleVector  OcclusionTestHandles;           // +0x0c
    bool           Seen;                           // +0x18
    unsigned long  TestFrame;                      // +0x1c
};

class CEnginePrimitiveOcclusionManager { public:
    void CheckVisibilityResult(CEnginePrimitiveOcclusionInfo& info);   // 0x00B2F8E0
    void SetVisible(CEnginePrimitiveOcclusionInfo& info);              // 0x00B2FA40
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58

struct COccluderEntry {
    unsigned long PixelCount;
    unsigned long Bits;
};

class CEngineOcclusionManager { public:
    unsigned char   _pad0000[0x4010];
    unsigned long   CurrentFrame;                  // +0x4010
    COccluderEntry* Entries;                       // +0x4014
    COccluderEntry* PrevFrameEntries;              // +0x4018
    unsigned long   EntriesSize;                   // +0x401c
    __forceinline unsigned long AllocateOccluderHandle()
    {
        if (EntriesSize < 0x2000) {
            unsigned long index = EntriesSize;
            COccluderEntry empty;
            empty.PixelCount = 0;
            empty.Bits = 0;
            Entries[index] = empty;
            ++EntriesSize;
            return index;
        }
        return 0xffffffff;
    }
};
extern CEngineOcclusionManager* g_pOcclusionManager;   // 0x01436E5C

struct CEnginePrimitiveRendererSharedData {
    unsigned char _pad000[0x78];
    long          CurrentOcclusionLayer;           // +0x78
    unsigned char _pad07c[0xbd - 0x7c];
    bool          OcclusionEnable;                 // +0xbd
    unsigned char _pad0be[0x24c - 0xbe];
    long          ShowVisibilityInfo;              // +0x24c
};
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData;   // 0x01436E78

class CEngineInternalPrimitiveOcclusionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void GetWorldBoundingBox(CMatrix3x4& box);                 // slot 16 (+0x40)
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void RenderWithoutOcclusion(long layer);                   // slot 28 (+0x70)
    long           RefCount;                       // +0x04
    long           Type;                           // +0x08
    unsigned char  _pad00c[0x24 - 0xc];
    signed char    GridLevel;                      // +0x24 GridPos.Level
    unsigned char  _pad025[0x48 - 0x25];
    CEnginePrimitiveOcclusionInfo* OcclusionInfo[2];   // +0x48
};

class CEnginePrimitiveManagerOcclusionBase {
public:
    bool IsClippedByNearPlane(const CMatrix3x4& box);                          // 0x00B900B0
    void DrawVisibilityTestBox(const CMatrix3x4& box, unsigned long handle);   // 0x00B906C0
    void DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour);   // 0x00B90700
    void RenderBoundingBox(CEngineInternalPrimitiveOcclusionBase* primitive);
};

void CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox(CEngineInternalPrimitiveOcclusionBase* primitive)
{
    long layer = g_pPrimitiveRendererSharedData->CurrentOcclusionLayer;
    if (layer == -1)
        return;
    CEnginePrimitiveOcclusionInfo* info = primitive->OcclusionInfo[layer];
    if (!info)
        return;
    if (!g_pPrimitiveRendererSharedData->OcclusionEnable) {
        primitive->RenderWithoutOcclusion(layer);
        return;
    }
    g_pPrimitiveOcclusionManager->CheckVisibilityResult(*info);
    CMatrix3x4 box;
    if (info->OcclusionTestHandles.empty()) {
        primitive->GetWorldBoundingBox(box);
        if (IsClippedByNearPlane(box)) {
            g_pPrimitiveOcclusionManager->SetVisible(*info);
            return;
        }
        unsigned long handle = g_pOcclusionManager->AllocateOccluderHandle();
        if (g_pOcclusionManager->CurrentFrame != info->TestFrame)
            info->OcclusionTestHandles.clear();
        info->OcclusionTestHandles.push_back(handle);
        info->TestFrame = g_pOcclusionManager->CurrentFrame;
        DrawVisibilityTestBox(box, handle);
        long show = g_pPrimitiveRendererSharedData->ShowVisibilityInfo;
        if (show == 1 || show == 2) {
            unsigned long colour;
            if (primitive->Type == 0x29)
                colour = 0x40ff0000;
            else
                colour = primitive->GridLevel >= 0 ? 0x400000ff : 0x40ffff00;
            DrawDebugVisibilityTestBox(box, colour);
        }
    }
    else if (g_pPrimitiveRendererSharedData->ShowVisibilityInfo == 2) {
        primitive->GetWorldBoundingBox(box);
        DrawDebugVisibilityTestBox(box, 0x8000ff00);
    }
}

// ---- behaviour-test stubs ----
static int g_checks, g_setVisible, g_renderDirect, g_boxes, g_clipped, g_tests, g_debug;
static long g_renderLayer;
static unsigned long g_testHandle, g_debugColour;
void CEngineInternalPrimitiveOcclusionBase::v00() {}
void CEngineInternalPrimitiveOcclusionBase::v01() {}
void CEngineInternalPrimitiveOcclusionBase::v02() {}
void CEngineInternalPrimitiveOcclusionBase::v03() {}
void CEngineInternalPrimitiveOcclusionBase::v04() {}
void CEngineInternalPrimitiveOcclusionBase::v05() {}
void CEngineInternalPrimitiveOcclusionBase::v06() {}
void CEngineInternalPrimitiveOcclusionBase::v07() {}
void CEngineInternalPrimitiveOcclusionBase::v08() {}
void CEngineInternalPrimitiveOcclusionBase::v09() {}
void CEngineInternalPrimitiveOcclusionBase::v10() {}
void CEngineInternalPrimitiveOcclusionBase::v11() {}
void CEngineInternalPrimitiveOcclusionBase::v12() {}
void CEngineInternalPrimitiveOcclusionBase::v13() {}
void CEngineInternalPrimitiveOcclusionBase::v14() {}
void CEngineInternalPrimitiveOcclusionBase::v15() {}
void CEngineInternalPrimitiveOcclusionBase::v17() {}
void CEngineInternalPrimitiveOcclusionBase::v18() {}
void CEngineInternalPrimitiveOcclusionBase::v19() {}
void CEngineInternalPrimitiveOcclusionBase::v20() {}
void CEngineInternalPrimitiveOcclusionBase::v21() {}
void CEngineInternalPrimitiveOcclusionBase::v22() {}
void CEngineInternalPrimitiveOcclusionBase::v23() {}
void CEngineInternalPrimitiveOcclusionBase::v24() {}
void CEngineInternalPrimitiveOcclusionBase::v25() {}
void CEngineInternalPrimitiveOcclusionBase::v26() {}
void CEngineInternalPrimitiveOcclusionBase::v27() {}

void CEngineInternalPrimitiveOcclusionBase::GetWorldBoundingBox(CMatrix3x4& box) { ++g_boxes; box.M[0] = 7.0f; }
void CEngineInternalPrimitiveOcclusionBase::RenderWithoutOcclusion(long layer) { ++g_renderDirect; g_renderLayer = layer; }
void CEnginePrimitiveOcclusionManager::CheckVisibilityResult(CEnginePrimitiveOcclusionInfo&) { ++g_checks; }
void CEnginePrimitiveOcclusionManager::SetVisible(CEnginePrimitiveOcclusionInfo&) { ++g_setVisible; }
bool CEnginePrimitiveManagerOcclusionBase::IsClippedByNearPlane(const CMatrix3x4& box) { return box.M[0] == 7.0f && g_clipped; }
void CEnginePrimitiveManagerOcclusionBase::DrawVisibilityTestBox(const CMatrix3x4&, unsigned long handle) { ++g_tests; g_testHandle = handle; }
void CEnginePrimitiveManagerOcclusionBase::DrawDebugVisibilityTestBox(const CMatrix3x4&, unsigned long colour) { ++g_debug; g_debugColour = colour; }

static CEnginePrimitiveOcclusionManager s_primManager;
static CEngineOcclusionManager s_occlusion;
static CEnginePrimitiveRendererSharedData s_shared;
static COccluderEntry s_entries[0x2000];
CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager = &s_primManager;
CEngineOcclusionManager* g_pOcclusionManager = &s_occlusion;
CEnginePrimitiveRendererSharedData* g_pPrimitiveRendererSharedData = &s_shared;

static void Reset() { g_checks = g_setVisible = g_renderDirect = g_boxes = g_tests = g_debug = 0; g_debugColour = 0; g_testHandle = 0; }

int main()
{
    int ok = 1;
    CEngineInternalPrimitiveOcclusionBase prim;
    CEnginePrimitiveOcclusionInfo info;
    CEnginePrimitiveManagerOcclusionBase mgr;
    info.OcclusionTestHandles._M_start = info.OcclusionTestHandles._M_finish = info.OcclusionTestHandles._M_end_of_storage = 0;
    info.TestFrame = 0;
    prim.OcclusionInfo[0] = 0; prim.OcclusionInfo[1] = &info;
    prim.Type = 0x29; prim.GridLevel = 0;
    s_occlusion.Entries = s_entries; s_occlusion.EntriesSize = 0; s_occlusion.CurrentFrame = 5;
    s_entries[0].PixelCount = 99; s_entries[0].Bits = 99;

    // 1: no current layer
    Reset(); s_shared.CurrentOcclusionLayer = -1; mgr.RenderBoundingBox(&prim);
    if (g_checks || g_renderDirect) { ok = 0; printf("case1\n"); }
    // 2: no occlusion info for layer 0
    Reset(); s_shared.CurrentOcclusionLayer = 0; mgr.RenderBoundingBox(&prim);
    if (g_checks || g_renderDirect) { ok = 0; printf("case2\n"); }
    // 3: occlusion disabled -> direct render
    Reset(); s_shared.CurrentOcclusionLayer = 1; s_shared.OcclusionEnable = false; mgr.RenderBoundingBox(&prim);
    if (g_renderDirect != 1 || g_renderLayer != 1 || g_checks) { ok = 0; printf("case3\n"); }
    // 4: clipped by the near plane -> visible, no test
    s_shared.OcclusionEnable = true; s_shared.ShowVisibilityInfo = 0;
    Reset(); g_clipped = 1; mgr.RenderBoundingBox(&prim);
    if (g_checks != 1 || g_setVisible != 1 || g_tests || s_occlusion.EntriesSize) { ok = 0; printf("case4\n"); }
    // 5: new test, frame differs (clear), push grows through _M_insert_overflow; show 1 + type 0x29
    Reset(); g_clipped = 0; s_shared.ShowVisibilityInfo = 1; mgr.RenderBoundingBox(&prim);
    if (g_tests != 1 || g_testHandle != 0 || s_occlusion.EntriesSize != 1 || s_entries[0].PixelCount || s_entries[0].Bits ||
        info.TestFrame != 5 || info.OcclusionTestHandles._M_finish - info.OcclusionTestHandles._M_start != 1 ||
        info.OcclusionTestHandles._M_start[0] != 0 || g_debug != 1 || g_debugColour != 0x40ff0000) { ok = 0; printf("case5\n"); }
    // 6: handles pending, show 2 -> debug box green; show 1 -> nothing
    Reset(); s_shared.ShowVisibilityInfo = 2; mgr.RenderBoundingBox(&prim);
    if (g_boxes != 1 || g_debug != 1 || g_debugColour != 0x8000ff00 || g_tests) { ok = 0; printf("case6\n"); }
    Reset(); s_shared.ShowVisibilityInfo = 1; mgr.RenderBoundingBox(&prim);
    if (g_boxes || g_debug || g_checks != 1) { ok = 0; printf("case6b\n"); }
    // 7: same frame (no clear) -> appends; grid level colours; show 2
    info.OcclusionTestHandles._M_finish = info.OcclusionTestHandles._M_start;   // drained by a result
    info.TestFrame = 6; s_occlusion.CurrentFrame = 6; prim.Type = 3; prim.GridLevel = -2; s_shared.ShowVisibilityInfo = 2;
    unsigned long* keep = info.OcclusionTestHandles._M_start;
    info.OcclusionTestHandles._M_start = 0; info.OcclusionTestHandles._M_finish = 0; info.OcclusionTestHandles._M_end_of_storage = 0;
    Reset(); mgr.RenderBoundingBox(&prim);
    if (g_debugColour != 0x40ffff00 || g_testHandle != 1 || s_occlusion.EntriesSize != 2) { ok = 0; printf("case7\n"); }
    info.OcclusionTestHandles._M_finish = info.OcclusionTestHandles._M_start;
    prim.GridLevel = 3;
    Reset(); mgr.RenderBoundingBox(&prim);
    if (g_debugColour != 0x400000ff || g_testHandle != 2) { ok = 0; printf("case7b\n"); }
    // 8: allocator full -> handle 0xffffffff, frame changes -> clear
    info.OcclusionTestHandles._M_finish = info.OcclusionTestHandles._M_start;
    s_occlusion.EntriesSize = 0x2000; s_occlusion.CurrentFrame = 9;
    Reset(); mgr.RenderBoundingBox(&prim);
    if (g_testHandle != 0xffffffff || info.TestFrame != 9 || info.OcclusionTestHandles._M_start[0] != 0xffffffff) { ok = 0; printf("case8\n"); }
    (void)keep;
    if (ok) printf("00b91140_TEST PASS\n");
    return ok ? 0 : 1;
}