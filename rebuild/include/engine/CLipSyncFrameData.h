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
#ifdef FABLE_RETAIL_CLIPSYNCFRAMEDATA_INLINE_BODIES
 // Opt-in (hand3c2, row 0x00A48F10 CLipSyncSampleData::GetFrameInterpolate): ?HasPhonemeID@CLipSyncFrameData@@QBE_NK@Z inline (no
 // retail out-of-line copy; the row expands the search three times, 0x00A49016 / 0x00A49186). Text = Ego_d.pdb (debug length 84, source
 // lines 138..147: the `for` 139, the GetID() test 141 (24 bytes), `return true` 143, `return false` 146; parameter id, local c1).
 bool HasPhonemeID(unsigned long id)const
 {
  for(unsigned long c1=0;c1<Phonemes.size();c1++)
  {
   if(Phonemes[c1].GetID()==id)
   {
    return true;
   }
  }
  return false;
 }
#else
 bool HasPhonemeID(unsigned long)const;
#endif
 const CLipSyncPhoneme FindPhonemeID(unsigned long)const;
 unsigned long Sizeof()const;
private:
#ifdef FABLE_RETAIL_CLIPSYNCFRAMEDATA_INLINE_BODIES
 // (same opt-in) ?ClearPhonemes@CLipSyncFrameData@@AAEXXZ inline: Ego_d.pdb one source line, 19 bytes (the vector's clear(); the row
 // calls the vector's erase(begin, end) 0x00A4A320 in place, 0x00A48FCB).
 void ClearPhonemes(){Phonemes.clear();}
#else
 void ClearPhonemes();
#endif
 void AddPhoneme(const CLipSyncPhoneme& value){Phonemes.push_back(value);}
 CArray<CLipSyncPhoneme> Phonemes;
};
FABLE_STATIC_ASSERT_AT(20, sizeof(CLipSyncFrameData)==12);
#endif
#endif
