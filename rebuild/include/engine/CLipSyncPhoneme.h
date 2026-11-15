#ifdef FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#ifndef FABLE_ENGINE_CLIPSYNCPHONEME_H
#define FABLE_ENGINE_CLIPSYNCPHONEME_H
#include "fable_string.h"
// Original Ego_r class keyV;private fields0/4/8 and public default/copy/destructor;retail size12.
class CLipSyncSampleData;
class CLipSyncFrameData;
class CLipSyncPhoneme {
 friend class CLipSyncSampleData;
public:
 CLipSyncPhoneme():ID(0),Weight(0.0f){}
 CCharString GetSymbol()const;
 unsigned long GetID()const{return ID;}
 float GetWeight()const{return Weight;}
 unsigned long Sizeof()const;
private:
 CLipSyncPhoneme(const CCharString&,unsigned long,float);
 CCharString Symbol;
 unsigned long ID;
 float Weight;
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncPhoneme)==12);
#endif
#endif
