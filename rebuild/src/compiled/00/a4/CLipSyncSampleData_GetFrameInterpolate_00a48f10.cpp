// CLipSyncSampleData::GetFrameInterpolate(CLipSyncFrameData& dest, float t) const -- build the phoneme set at a time between two sample frames: phonemes in both frames interpolate their weight, a phoneme only in the first fades out, one only in the second fades in
// Leaf: ?GetFrameInterpolate@CLipSyncSampleData@@QBEXAAVCLipSyncFrameData@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48F10, 811 bytes, `ret 8`, ebp frame 0x2C (the floor helper's asm block forces the frame).
// Retail-asm: 0x00A48F36 `fsub dword ptr [ebp-8]` + 0x00A48F39 `fistp dword ptr [ebp+0Ch]` -- the engine's inline-assembly floor helper
//   GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h, FABLE_PORTABLE twin there; its half / one constants in the frame at
//   [ebp-8] / [ebp-0Ch], 0x00A48F1F / 0x00A48F29). This row carries no asm of its own.
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?GetFrameInterpolate@CLipSyncSampleData@@QBEXAAVCLipSyncFrameData@@M@Z (the shared
//   header's declaration; caller = landed CTCTalk::GetCurrentLipSyncFrameData 0x006E2ED0). Parameters and locals exactly as Ego_d.pdb
//   (debug length 1691): dest, t; pos, frame2, weight2, index2, frame1, index1; { c1 { phoneme1 { phoneme2 } } }; { c2 { phoneme2 } }.
// Shape: Ego_d.pdb's C13 line table (ref/00a48f10/egod_lines.tsv, source lines 14..51; Ego_d is a VC10 /Od build with `this` at
//   [ebp-0B8h], so a line is 3 bytes longer per `this` use and per float argument, 7 / 4 per EH state store; lines 15, 32 and 38 are
//   debug-only asserts of 326 / 366 / 388 bytes):
//   16 pos (15 = our 12 + this); 17 index1 = GFFloatToLongFloor(pos) (15 = 12 + float argument); 18 index2 (9 = ours); 19 weight2
//   (9 = ours); 20 / 21 the two limits, ONE line each of 51 bytes (= our 45 + two `this` uses): the if / else-if text below, the upper
//   bound evaluated twice inside the else branch -- retail computes the frame count only there (0x00A48F64, 0x00A48F8E) and compares
//   it unsigned (`jbe`), which the GFLimit template (bound evaluated once, up front) does not give (794v811); 22 / 23 the two frame
//   references (18 = 15 + this); 24 ClearPhonemes (8 = ours); 25 the c1 loop; 27 phoneme1 (28 = 21 + EH); 28 the HasPhonemeID test
//   (28 = our 24 + long jump); 30 phoneme2 (25 = 21 + EH); 31 the interpolation, 29 bytes = three float arguments and one __fastcall
//   float call (GFGetInterpolatedValue, our 20 + 3 x 3; a member expression is 17, one through GetWeight() 42); 33 / 39 / 48
//   AddPhoneme (12 = ours); 37 the fade-out, 13 bytes = `Weight * (1.0f - weight2)` with the member loaded first (`fld ; fld1 ;
//   fsub ; fmulp ; fstp`; the compound `*=` is 11); 42 the c2 loop; 44 phoneme2; 45 the negated test (24 = ours); 47 the fade-in
//   (9 = ours).
// Inline helpers expanded by retail: CLipSyncFrameData::GetPhonemeCount / GetPhoneme / AddPhoneme (shared header), HasPhonemeID and
//   ClearPhonemes (NEW overlay opt-in FABLE_RETAIL_CLIPSYNCFRAMEDATA_INLINE_BODIES, Ego_d texts), CLipSyncPhoneme::GetID, the implicit
//   CLipSyncPhoneme copy constructor / destructor, STLport vector size / operator[] / push_back / clear.
// Callees: 0x00A4A320 vector<CLipSyncPhoneme>::erase(first, last) (through clear()), 0x0099EC30 CCharString copy constructor,
//   0x0099EAE0 CCharString destructor, 0x00A48A40 CLipSyncFrameData::FindPhonemeID (landed), 0x00A4A710
//   vector<CLipSyncPhoneme>::_M_insert_overflow (three push_back sites).
// Globals: 0x0122DED8 the float 1.0.
#define FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ
#define FABLE_RETAIL_CLIPSYNCFRAMEDATA_INLINE_BODIES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "fable_string.h"
#include "engine/CLipSyncSampleData.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFGetInterpolatedValue.h"

void CLipSyncSampleData::GetFrameInterpolate(CLipSyncFrameData& dest, float t) const
{
    float pos = t * FramesPerSecond;
    long index1 = GFFloatToLongFloor(pos);
    long index2 = index1 + 1;
    float weight2 = pos - index1;
    if (index1 < 0) index1 = 0; else if (index1 > FrameData.size() - 1) index1 = FrameData.size() - 1;
    if (index2 < 0) index2 = 0; else if (index2 > FrameData.size() - 1) index2 = FrameData.size() - 1;
    const CLipSyncFrameData& frame1 = FrameData[index1];
    const CLipSyncFrameData& frame2 = FrameData[index2];
    dest.ClearPhonemes();
    for (unsigned long c1 = 0; c1 < frame1.GetPhonemeCount(); c1++)
    {
        CLipSyncPhoneme phoneme1 = frame1.GetPhoneme(c1);
        if (frame2.HasPhonemeID(phoneme1.GetID()))
        {
            CLipSyncPhoneme phoneme2 = frame2.FindPhonemeID(phoneme1.GetID());
            phoneme1.Weight = GFGetInterpolatedValue(phoneme1.Weight, phoneme2.Weight, weight2);
            dest.AddPhoneme(phoneme1);
        }
        else
        {
            phoneme1.Weight = phoneme1.Weight * (1.0f - weight2);
            dest.AddPhoneme(phoneme1);
        }
    }
    for (unsigned long c2 = 0; c2 < frame2.GetPhonemeCount(); c2++)
    {
        CLipSyncPhoneme phoneme2 = frame2.GetPhoneme(c2);
        if (!frame1.HasPhonemeID(phoneme2.GetID()))
        {
            phoneme2.Weight *= weight2;
            dest.AddPhoneme(phoneme2);
        }
    }
}