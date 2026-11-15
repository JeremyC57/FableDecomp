#ifndef FABLE_ENGINE_CLIPSYNCSAMPLEDATA_H
#define FABLE_ENGINE_CLIPSYNCSAMPLEDATA_H
// New header (chunk 1024, row 0x006E2ED0 CTCTalk::GetCurrentLipSyncFrameData): Ego_r.pdb CLipSyncSampleData, the member the row calls:
// ?GetFrameInterpolate@CLipSyncSampleData@@QBEXAAVCLipSyncFrameData@@M@Z, out of line in retail at 0x00A48F10 (`fld [ebp+0Ch] ;
// fmul [ecx+0Ch]`: the time in seconds times the sample's frame rate, then the frame pair interpolated into the CLipSyncFrameData).
// Declaration only; the layout is not modelled.
#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#include "engine/CLipSyncFrameData.h"
class CDataInputStream;
class CDataOutputStream;
class CLipSyncSampleData {
public:
 void GetFrameInterpolate(CLipSyncFrameData&,float)const;
 void ReadFromFile(CDataInputStream&);
 void WriteToFile(CDataOutputStream&)const;
 unsigned long Sizeof()const;
private:
 void ClearFrames();
 void AddFrame(const CLipSyncFrameData&);
 void SetFrameRate(float fps){FramesPerSecond=fps;}
 CArray<CLipSyncFrameData> FrameData;
 float FramesPerSecond;
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncSampleData)==16);
#else
class CLipSyncFrameData;
class CLipSyncSampleData {
public:
    void GetFrameInterpolate(CLipSyncFrameData& frame_data, float time) const;   // 0x00A48F10
};
#endif
#endif
