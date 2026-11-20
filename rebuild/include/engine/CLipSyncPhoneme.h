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
#ifdef FABLE_RETAIL_CLIPSYNCPHONEME_GETSYMBOL_INLINE
 // Opt-in (hand3c2, row 0x00A48BA0 CLipSyncSampleData::WriteToFile): Ego_d.pdb ?GetSymbol@CLipSyncPhoneme@@QBE?AVCCharString@@XZ, inline
 // in retail: the row builds the returned copy in its frame (0x00A48C55 `call 0x0099EC30`, the CCharString copy constructor on
 // phoneme.Symbol) and destroys it after the map assignment (0x00A48C7F).
 CCharString GetSymbol()const{return Symbol;}
#else
 CCharString GetSymbol()const;
#endif
 unsigned long GetID()const{return ID;}
 float GetWeight()const{return Weight;}
 unsigned long Sizeof()const;
private:
 CLipSyncPhoneme(const CCharString&,unsigned long,float);
 CCharString Symbol;
 unsigned long ID;
 float Weight;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CLipSyncPhoneme)==12);
#endif
#endif
