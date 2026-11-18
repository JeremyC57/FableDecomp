// CGraphicInfoFrame::GetData() const -- the frame's data, created on first use (a 0x44-byte CFrameData for this frame adopted by the mutable Data pointer)
// Leaf: ?GetData@CGraphicInfoFrame@@ABEPAVCFrameData@1@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A00D90, length 72 bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A (0x44), CGraphicInfoFrame::CFrameData::CFrameData(const CGraphicInfoFrame*) 0x009FE380 (Ego_r.pdb + FableWin.pdb
//   ??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z; the null arm yields 0), the old pointee's virtual slot 1 when its count reaches zero (the inlined
//   CIVCountedPointer::Reset: release with the zero store, then the plain store), then Data's object returned.
// Identity: FableWin.pdb ?GetData@CGraphicInfoFrame@@ABEPAVCFrameData@1@XZ (private const, no arguments, returns the frame data) is the only
//   CGraphicInfoFrame member of this shape in either PDB; the body is exactly that accessor over the Ego_r.pdb member `mutable CIVCountedPointer
//   <CFrameData> Data` (+0x08, engine/CGraphicInfoFrame.h), with the create-on-first-use the header's GetFrame spells (`Data.Reset(new
//   CFrameData(this))`). No retail call or address reference reaches 0x00A00D90 (callers.py / absrefs.py). The catalogue's
//   "_global::CIVCountedPointer_Assign_00a00d90" and the landed leaf "InitialiseFrame" (Ego_r.pdb's InitialiseFrame takes a CGraphicFrame&,
//   a CManagedTexture& and a char) did not fit. DESCRIPTIVE-RISK: FableWin-only name (see NOTES).
// NF79 (round 7, q10965): re-authored on the shared header (SURFACECOMPOSITION retail branch + my overlay opt-in CGRAPHICINFOFRAME_GETDATA);
//   was a local CIVCountedPointeeBase / CFrameData_00a00d90 / CGraphicInfoFrame model binding 0x009FE380 as ??0CFrameData_00a00d90@@.
#define FABLE_RETAIL_CGRAPHICDATABANK_SURFACECOMPOSITION
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETDATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#include "engine/CGraphicInfoFrame.h"

CGraphicInfoFrame::CFrameData* CGraphicInfoFrame::GetData() const
{
    if (Data.IsNull())
        Data.Reset(new CFrameData(this));
    return Data.Get();
}
