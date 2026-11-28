#ifndef FABLE_ENGINE_CFRAMERATESMOOTHER_H
#define FABLE_ENGINE_CFRAMERATESMOOTHER_H
// CFrameRateSmoother (NF275, row 0x00418DCA CMainGameComponent::CMainGameComponent, member FrameRateSmoother +0x16178).
// Members in Ego_r.pdb order and spelling (ghidra_out/struct_layouts_egor.tsv: base CInitBaseClass +0, CArray<float> Times +0x08,
// MaxNoTimes, NoTimes, NoFramesToChangeFrameRateOver, FirstTime, LastTime, double SmoothedTime; Ego_r sizeof 56 with a 16-byte
// Dinkumware vector) at retail sizes: the STLport CArray is 12 bytes, so retail is 0x30. Two retail witnesses for 0x30: the
// CMainGameComponent ctor constructs the member at +0x16178 and stores the next member, LastRenderFrameStartTime, at +0x161A8
// (0x00418E70 / 0x00418E8F), and the out-of-line default ctor 0x00419747 zeroes exactly three words at +0x08..+0x10 (Times).
// The ctor ??0CFrameRateSmoother@@QAE@XZ (Ego_r.pdb / FableWin.pdb) and the virtual dtor ??1CFrameRateSmoother@@UAE@XZ
// (FableWin.pdb) are declarations only. Times stays opaque (no row reads it); member access is not proven.
#include "engine/CInitBaseClass.h"
#include "rebuild_abi.h"
class CFrameRateSmoother : public CInitBaseClass {
public:
    CFrameRateSmoother();                                    // 0x00419747
    virtual ~CFrameRateSmoother();
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_PASS30
    bool Init(long, long);  // FableWin and Ego_r, native 0049BA70.
#endif
    unsigned char Times[0xC];                                // +0x08 CArray<float>
    long          MaxNoTimes;                                // +0x14
    long          NoTimes;                                   // +0x18
    long          NoFramesToChangeFrameRateOver;             // +0x1C
    long          FirstTime;                                 // +0x20
    long          LastTime;                                  // +0x24
    double        SmoothedTime;                              // +0x28
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CFrameRateSmoother) == 0x30);
#endif // FABLE_ENGINE_CFRAMERATESMOOTHER_H
