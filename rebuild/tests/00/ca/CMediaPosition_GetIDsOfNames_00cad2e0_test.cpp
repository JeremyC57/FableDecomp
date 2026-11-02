// Behaviour test for 0x00CAD2E0 CMediaPosition::GetIDsOfNames: the caller's riid is replaced by IID_IMediaPosition and every other
// argument is forwarded unchanged to the embedded CBaseDispatch.
#include <cstdio>
struct GUIDModel { unsigned int Data1; };
static const GUIDModel IID_IMediaPositionModel = { 0x56a868b2u };
static const GUIDModel IID_OtherModel = { 0x12345678u };
struct CBaseDispatchModel {
    const GUIDModel* LastIID; void* LastNames; unsigned int LastCount; unsigned int LastLcid; void* LastIds;
    long GetIDsOfNames(const GUIDModel& riid, void* names, unsigned int count, unsigned int lcid, void* ids)
    { LastIID = &riid; LastNames = names; LastCount = count; LastLcid = lcid; LastIds = ids; return 7; }
};
struct CMediaPositionModel {
    CBaseDispatchModel m_basedisp;
    long GetIDsOfNames(const GUIDModel& riid, void* names, unsigned int count, unsigned int lcid, void* ids)
    { return m_basedisp.GetIDsOfNames(IID_IMediaPositionModel, names, count, lcid, ids); }
};
int main()
{
    CMediaPositionModel pos;
    int names = 0, ids = 0;
    long hr = pos.GetIDsOfNames(IID_OtherModel, &names, 3, 0x409, &ids);
    if (hr != 7 || pos.m_basedisp.LastIID != &IID_IMediaPositionModel || pos.m_basedisp.LastNames != &names ||
        pos.m_basedisp.LastCount != 3 || pos.m_basedisp.LastLcid != 0x409 || pos.m_basedisp.LastIds != &ids) {
        std::printf("00cad2e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cad2e0_TEST PASS\n");
    return 0;
}