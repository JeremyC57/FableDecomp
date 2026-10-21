// CEngineWaterRenderer::FrameStart(const CEngineComponentParams& params) -- empty the ten per-frame water render lists (STLport clear() as erase(begin, end)), then advance the sea loading
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B71FB0, length 464 bytes (body 0x1c5 + int3 padding), `ret 4` (virtual; params unused).
// Callees: memmove @ 0x00BFEAE6 (the trivial-copy erase of the pointer lists), the `__fastcall` copy loop for the 28-byte
//   pair<C3DBoundingBox, CWaterPatchMesh*> elements @ 0x00B7AC20 (catalogued _Copy_impl<CActionInputControl*>, propagated; ecx = first,
//   edx = last, then result, the random-access tag and the distance pointer on the stack), CEngineWaterRenderer::ProcessSeaLoading
//   @ 0x00B6DC40 (bool result dropped).
// Layout: Ego_r.pdb CEngineWaterRenderer, but retail CArray (STLport vector) is 12 bytes where the PDB/engine header has 16 and the
//   lists start 0xc earlier: ForegroundRenderList +0x1fc, BackgroundRenderList +0x208, AlphaedBackgroundRenderList +0x214,
//   IceForegroundRenderList +0x220 (the pair list), IceBackgroundRenderList +0x22c, IceAlphaedBackgroundRenderList +0x238,
//   SeaReflectiveForegroundRenderList +0x244, SeaNonReflectiveForegroundRenderList +0x250, SeaMidgroundRenderList +0x25c,
//   SeaAlphaedMidgroundRenderList +0x268 -- so the shared header cannot model `this` here.
// Shape: clear() -> erase(first, last) -> `last = copy(last, finish, first)`: the trivial copy tests `first == last` (here the same
//   load, `cmp eax,eax`) and returns `memmove(result, first, n) + n`; the pair list copies through the out-of-line fastcall loop.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

template <class T>
inline T* CopyTrivial(const T* first, const T* last, T* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (T*)((char*)memmove(result, first, n) + n);
}

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };
struct CWaterPatchMesh;
struct CEngineWaterBackgroundSubPatch;
struct CEngineComponentParams;

struct SIcePatchEntry {                            // std::pair<C3DBoundingBox, CWaterPatchMesh*>, 28 bytes
    C3DBoundingBox   first;
    CWaterPatchMesh* second;
    SIcePatchEntry& operator=(const SIcePatchEntry& other);
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SIcePatchEntry* __fastcall CopyIcePatchEntries(SIcePatchEntry* first, SIcePatchEntry* last, SIcePatchEntry* result,
                                               const SRandomAccessIteratorTag& category, long* distance);   // 0x00B7AC20

template <class T> struct CPtrArray {              // retail STLport vector<T*>, 12 bytes
    T** m_pFirst;
    T** m_pLast;
    T** m_pEnd;
    void erase(T** first, T** last) { m_pLast = CopyTrivial(last, m_pLast, first); }
    void clear() { erase(m_pFirst, m_pLast); }
};

struct CIcePatchArray {                            // retail STLport vector<pair<C3DBoundingBox, CWaterPatchMesh*> >
    SIcePatchEntry* m_pFirst;
    SIcePatchEntry* m_pLast;
    SIcePatchEntry* m_pEnd;
    void erase(SIcePatchEntry* first, SIcePatchEntry* last)
    {
        m_pLast = CopyIcePatchEntries(last, m_pLast, first, SRandomAccessIteratorTag(), (long*)0);
    }
    void clear() { erase(m_pFirst, m_pLast); }
};

struct CEngineWaterRenderer {
    virtual void FrameStart(const CEngineComponentParams& params);
    unsigned char _pad004[0x1fc - 0x4];
    CPtrArray<CWaterPatchMesh>                ForegroundRenderList;            // +0x1fc
    CPtrArray<CEngineWaterBackgroundSubPatch> BackgroundRenderList;            // +0x208
    CPtrArray<CEngineWaterBackgroundSubPatch> AlphaedBackgroundRenderList;     // +0x214
    CIcePatchArray                            IceForegroundRenderList;         // +0x220
    CPtrArray<CEngineWaterBackgroundSubPatch> IceBackgroundRenderList;         // +0x22c
    CPtrArray<CEngineWaterBackgroundSubPatch> IceAlphaedBackgroundRenderList;  // +0x238
    CPtrArray<CWaterPatchMesh>                SeaReflectiveForegroundRenderList;     // +0x244
    CPtrArray<CWaterPatchMesh>                SeaNonReflectiveForegroundRenderList;  // +0x250
    CPtrArray<CEngineWaterBackgroundSubPatch> SeaMidgroundRenderList;          // +0x25c
    CPtrArray<CEngineWaterBackgroundSubPatch> SeaAlphaedMidgroundRenderList;   // +0x268
    bool ProcessSeaLoading();                      // 0x00B6DC40
};

void CEngineWaterRenderer::FrameStart(const CEngineComponentParams& params)
{
    ForegroundRenderList.clear();
    BackgroundRenderList.clear();
    AlphaedBackgroundRenderList.clear();
    IceForegroundRenderList.clear();
    IceBackgroundRenderList.clear();
    IceAlphaedBackgroundRenderList.clear();
    SeaReflectiveForegroundRenderList.clear();
    SeaNonReflectiveForegroundRenderList.clear();
    SeaMidgroundRenderList.clear();
    SeaAlphaedMidgroundRenderList.clear();
    ProcessSeaLoading();
}