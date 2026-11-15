#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#ifndef FABLE_ENGINE_CLIPSYNCFRAMEDATA_H
#define FABLE_ENGINE_CLIPSYNCFRAMEDATA_H
#include "engine/CLipSyncPhoneme.h"
#include "engine/CArray.h"
// Actual private member and accessors from Ego_r;native vector width12 replaces donor16.
class CLipSyncFrameData {
 friend class CLipSyncSampleData;
public:
 unsigned long GetPhonemeCount()const{return Phonemes.size();}
 const CLipSyncPhoneme&GetPhoneme(unsigned long n)const{return Phonemes[n];}
 bool HasPhonemeID(unsigned long)const;
 const CLipSyncPhoneme FindPhonemeID(unsigned long)const;
 unsigned long Sizeof()const;
private:
 void ClearPhonemes();
 void AddPhoneme(const CLipSyncPhoneme& value){Phonemes.push_back(value);}
 CArray<CLipSyncPhoneme> Phonemes;
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncFrameData)==12);
#endif
#endif
