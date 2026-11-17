#ifndef FABLE_LIBPK_CPLAYBACKSLOT_H
#define FABLE_LIBPK_CPLAYBACKSLOT_H
class CSoundEvent;
// NativeGetC13700 uses0x2c stride; source updateC1C460 and independent traceC127C0.
struct CPlaybackSlot { long Unrecovered00; CSoundEvent*PEvent;long Unrecovered08;void* SourceHandle;unsigned char Unrecovered10[0x15];bool Positional;unsigned char Unrecovered26[6];
#ifdef FABLE_RETAIL_CPLAYBACKSLOT_UPDATE
 // NF63 (round 7, row 0x00C11F50): the per-slot update 0x00C276B0 (catalog CCodaPlayback::Update, a descriptive libpk name kept).
 // Declaration only.
 void Update();                                                      // 0x00C276B0
#endif
};
#endif
