// Grade: DIFFER(512v510) -- primitive/vector registers swapped (ebx vs edi, ebx push hoisted), handle copy store sunk below the frame test, __true_type tag in its own byte slot (retail: the live primitive arg slot)
// CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox(CEngineInternalPrimitiveOcclusionBase* primitive) -- run the primitive's occlusion test for the current layer: render it directly when occlusion is off; otherwise check last frame's result and, with no test pending, draw its bounding box as a new occlusion test (or mark it visible when the box crosses the near plane), plus the debug box colours
// Leaf: RenderBoundingBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91140, length 508 bytes (+ int3 padding), `ret 4`.
// Callees: CEnginePrimitiveOcclusionManager::CheckVisibilityResult 0x00B2F8E0 and the record reset 0x00B2FA40 (manifest
//   label "CV_BanditTollScript::Init" is wrong: an engine-module member called on the manager global 0x01436E58);
//   IsClippedByNearPlane 0x00B900B0, DrawVisibilityTestBox 0x00B906C0, DrawDebugBox 0x00B90700 (on `this`); the reallocating
//   vector insert 0x00464ACE; memmove 0x00BFEAE6 (the dead arm of the inlined STLport erase); the primitive's virtual
//   slots 16 (+0x40, world bounding box) and 28 (+0x70, render without occlusion).
// Layout: Ego_r.pdb CEnginePrimitiveRendererSharedData (global 0x01436E78): CurrentOcclusionLayer +0x78, OcclusionEnable
//   +0xbd, ShowVisibilityInfo +0x24c; CEngineOcclusionManager (global 0x01436E5C, header engine/CEngineOcclusionManager.h
//   offsets): CurrentFrame +0x4010, Entries +0x4014, EntriesSize +0x401c; CEnginePrimitiveOcclusionInfo (landed
//   0x00B2F8E0): OcclusionTestHandles {first,last,end} +0xc, TestFrame +0x1c; CEngineInternalPrimitiveOcclusionBase:
//   Type +8 (CEnginePersistentPrimitiveBase), GridPos.Level (schar) +0x24 (CEngineInternalPrimitiveBase), OcclusionInfo
//   CIVCountedPointer[2] +0x48. The engine headers keep these as opaque bytes, so they are modelled locally.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

struct CMatrix3x4 {
    float M[12];
};

struct CRGBColour {
    unsigned long ARGB;
    CRGBColour(unsigned long argb) : ARGB(argb) {}
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
    void _M_insert_overflow(unsigned long* where, const unsigned long& value, const SScalarTag& tag, unsigned long count,
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

struct CEnginePrimitiveOcclusionInfo {
    void*          _vptr;                          // +0x00
    unsigned long  RefCount;                       // +0x04
    long           InvisibleFrames;                // +0x08
    CHandleVector  OcclusionTestHandles;           // +0x0c
    bool           Seen;                           // +0x18
    unsigned long  TestFrame;                      // +0x1c
};

struct CEnginePrimitiveOcclusionManager {
    void CheckVisibilityResult(CEnginePrimitiveOcclusionInfo& info);   // 0x00B2F8E0
    void SetVisible(CEnginePrimitiveOcclusionInfo& info);              // 0x00B2FA40
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58

struct COccluderEntry {
    unsigned long PixelCount;
    unsigned long Bits;
};

struct CEngineOcclusionManager {
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
    void DrawDebugBox(const CMatrix3x4& box, CRGBColour colour);               // 0x00B90700
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
        info->OcclusionTestHandles.Add(handle);
        info->TestFrame = g_pOcclusionManager->CurrentFrame;
        DrawVisibilityTestBox(box, handle);
        long show = g_pPrimitiveRendererSharedData->ShowVisibilityInfo;
        if (show == 1 || show == 2) {
            if (primitive->Type == 0x29)
                DrawDebugBox(box, CRGBColour(0x40ff0000));
            else
                DrawDebugBox(box, CRGBColour(primitive->GridLevel < 0 ? 0x40ffff00 : 0x400000ff));
        }
    }
    else if (g_pPrimitiveRendererSharedData->ShowVisibilityInfo == 2) {
        primitive->GetWorldBoundingBox(box);
        DrawDebugBox(box, CRGBColour(0x8000ff00));
    }
}
