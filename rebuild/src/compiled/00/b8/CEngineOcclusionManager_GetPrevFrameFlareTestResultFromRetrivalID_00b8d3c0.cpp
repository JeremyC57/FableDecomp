// CEngineOcclusionManager::GetPrevFrameFlareTestResultFromRetrivalID(unsigned long retrievalID) -- map a flare retrieval ID to its occluder handle through last frame's sorted handle map and return that handle's previous-frame flare test result, or 0 when the ID is not mapped
// Leaf: GetPrevFrameFlareTestResultFromRetrivalID
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8D3C0, 96 bytes, thiscall, `ret 4`, float in st0.
// Identity: Ego_r.pdb ?GetPrevFrameFlareTestResultFromRetrivalID@CEngineOcclusionManager@@QAEMK@Z; the map searched is
//   PrevFrameHandleMapper (retail +0x24048, the second sorted vector<pair<unsigned long, unsigned long> > after
//   HandleMapper +0x2403C) and the tail-called callee 0x00B8D2F0 reads PrevFrameFlareTests (+0x24030), i.e.
//   ?GetPrevFrameFlareTestResult@CEngineOcclusionManager@@QAEMK@Z. One caller 0x00B645A2. Catalog
//   "SunFlare_BinarySearch" is descriptive.
// Callees: CEngineOcclusionManager::GetPrevFrameFlareTestResult(unsigned long handle) 0x00B8D2F0 (a tail jump with the
//   handle written over the argument slot).
// Layout: as the landed AllocateOccluderHandleWithRetrivalID 0x00B8D6C0 (retail = Ego_r.pdb less 4 per narrower CArray):
//   FlareTests +0x24024, PrevFrameFlareTests +0x24030, HandleMapper +0x2403C, PrevFrameHandleMapper +0x24048.
// Shape: the inlined STLport lower_bound over a signed distance (`sar 3`, half `sar 1`, `count -= half + 1` as
//   `or ebp,-1 ; sub ebp,edx ; add eax,ebp`), the same loop as 0x00B8D6C0; the miss returns the 0.0f constant.
struct SHandlePair {                            // std::pair<unsigned long, unsigned long>
    unsigned long first;
    unsigned long second;
};

struct CHandleMapper {                          // STLport vector<pair<unsigned long, unsigned long> >
    SHandlePair* _M_start;
    SHandlePair* _M_finish;
    SHandlePair* _M_end_of_storage;
};

static inline SHandlePair* LowerBound(SHandlePair* first, SHandlePair* last, unsigned long key)
{
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SHandlePair* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

class CEngineOcclusionManager {
public:
    unsigned char   _pad00000[0x2403c];
    CHandleMapper   HandleMapper;               // +0x2403c
    CHandleMapper   PrevFrameHandleMapper;      // +0x24048
    float GetPrevFrameFlareTestResult(unsigned long handle);             // 0x00B8D2F0
    float GetPrevFrameFlareTestResultFromRetrivalID(unsigned long retrievalID);
};

float CEngineOcclusionManager::GetPrevFrameFlareTestResultFromRetrivalID(unsigned long retrievalID)
{
    SHandlePair* it = LowerBound(PrevFrameHandleMapper._M_start, PrevFrameHandleMapper._M_finish, retrievalID);
    if (it != PrevFrameHandleMapper._M_finish && it->first == retrievalID)
        return GetPrevFrameFlareTestResult(it->second);
    return 0.0f;
}