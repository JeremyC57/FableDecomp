// CEngineWorldMap::FrameStart(const CEngineComponentParams& params) -- per-frame start: auto-build the world map grid, walk the map list from index 1 (a loop whose body compiled away), discard the pending changed-area list and let the static map bank file process its disposal list, then clear FirstWorldUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B424D0, length 86 bytes (the oracle row carries 10 bytes of int3 padding).
// Callees: CEngineWorldMap::AutoBuildWorldMapGrid @ 0x00B40070, the 16-byte-element vector erase(first, last) @ 0x00515BC0 (manifest
// "Vector_RemoveAt", also used by CEngineManager::OnRender) on ChangedAreas, CBankFileAsync::ProcessDisposalList @ 0x009D6730 on StaticMapFile.
// Shape: the empty loop survives because its bound re-reads MapList's size each iteration (`(end - begin) >> 2`, unsigned compare); the
// erase arguments are read last-first (`mov edx,[+1B8h] ; mov eax,[+1B4h]`). Retail offsets differ from engine/CEngineWorldMap.h (Ego PDB):
// the retail CArray is 12 bytes and the list 4, so MapList +0x20 is followed by StaticMapFile at +0x34, ChangedAreas at +0x1b4 and
// FirstWorldUpdate at +0x1a5 (PDB +0x234 / +0x225); the retail layout is modelled locally.
struct CEngineComponentParams;
struct CEngineMap;

struct C2DBoxI_00b424d0 { long x0, y0, x1, y1; };

struct CRetailBoxVector_00b424d0 {                // vector<C2DBoxI>: first, last, end
    C2DBoxI_00b424d0* m_pFirst;
    C2DBoxI_00b424d0* m_pLast;
    C2DBoxI_00b424d0* m_pEnd;
    void erase(C2DBoxI_00b424d0* first, C2DBoxI_00b424d0* last);   // 0x00515BC0
};

struct CRetailMapArray_00b424d0 {                 // CArray<CEngineMap*>: begin, end, capacity end
    CEngineMap** m_pBegin;
    CEngineMap** m_pEnd;
    CEngineMap** m_pCapacity;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
};

struct CBankFileAsync_00b424d0 {                  // the CStaticMapBankFile member: 0x171 bytes in retail (+0x34..+0x1a5)
    unsigned char _body[0x171];
    void ProcessDisposalList();                    // 0x009D6730
};

struct CEngineWorldMap {
    unsigned char             _pad0[0x1c];        // +0x04 (after the vptr)
    CRetailMapArray_00b424d0  MapList;             // +0x20
    unsigned char             _pad1[0x34 - 0x2c];
    CBankFileAsync_00b424d0   StaticMapFile;       // +0x34 (CStaticMapBankFile, a CBankFileAsync)
    bool                      FirstWorldUpdate;    // +0x1a5
    unsigned char             _pad3[0x1b4 - 0x1a6];
    CRetailBoxVector_00b424d0 ChangedAreas;        // +0x1b4
    void AutoBuildWorldMapGrid();                  // 0x00B40070
    virtual void FrameStart(const CEngineComponentParams& params);
    CEngineWorldMap();
};

void CEngineWorldMap::FrameStart(const CEngineComponentParams& params)
{
    AutoBuildWorldMapGrid();
    for (unsigned long i = 1; i < MapList.Size(); ++i) {
    }
    ChangedAreas.erase(ChangedAreas.m_pFirst, ChangedAreas.m_pLast);
    StaticMapFile.ProcessDisposalList();
    FirstWorldUpdate = false;
}