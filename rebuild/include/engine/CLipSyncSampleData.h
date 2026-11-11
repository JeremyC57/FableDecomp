#ifndef FABLE_ENGINE_CLIPSYNCSAMPLEDATA_H
#define FABLE_ENGINE_CLIPSYNCSAMPLEDATA_H
// New header (chunk 1024, row 0x006E2ED0 CTCTalk::GetCurrentLipSyncFrameData): Ego_r.pdb CLipSyncSampleData, the member the row calls:
// ?GetFrameInterpolate@CLipSyncSampleData@@QBEXAAVCLipSyncFrameData@@M@Z, out of line in retail at 0x00A48F10 (`fld [ebp+0Ch] ;
// fmul [ecx+0Ch]`: the time in seconds times the sample's frame rate, then the frame pair interpolated into the CLipSyncFrameData).
// Declaration only; the layout is not modelled.
class CLipSyncFrameData;
class CLipSyncSampleData {
public:
    void GetFrameInterpolate(CLipSyncFrameData& frame_data, float time) const;   // 0x00A48F10
};
#endif
