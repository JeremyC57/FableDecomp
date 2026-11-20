// CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser, long index) -- parses mini-map marker `index` through a default CPersistTraits<CUIMiniMapMarker>
// Leaf: ?TransferIn@?$CPersistContext_TransferableVector@VCUIMiniMapMarker@CTCInventoryMap@@@@EAEXAAVCPersistContext@@AAVCStringParser@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00615920, 120 bytes (+8 int3 padding), `ret 0Ch`.
// Callees: 0x0099EBF0 CCharString::CCharString(const char*, long) with "" @ 0x0122D70E; 0x006159A0
// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext*, CStringParser*, CUIMiniMapMarker*)
// (thiscall, `ret 0Ch`; manifest label `TransferOut_HeroTraits` is a guess); 0x0099EAE0 CCharString::~CCharString.
// Persist READ hook (vtable 0x01257164 slot 3; slot 2 is TransferOut 0x00614000, slot 1 Resize 0x006193B0, slot 0 the
// count 0x00613FE0) of CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>. The manifest's
// `std::pair<CCharString,...>` is propagated: the 28-byte stride (`imul ecx,ecx,1Ch`) and the traits callee name the
// type (Ego_r.pdb CUIMiniMapMarker 0x1c bytes: Region, CCharString UIDefName, C2DVector Position, DefNameLength,
// GraphicIndex, float Angle). The record's inline default constructor stores Region = 0, builds UIDefName("") in
// place (`lea ecx,[esp+10h]` = traits+4) and then zeroes Position..Angle with immediates; `this` is parked in esi
// across the string constructor. The implicit traits destructor destroys only the string (traits+4).
// The module is built without C++ exceptions (no EH frame around the string).
// agentNF187 (q5203): ported onto the shared headers (engine/CPersistContext_TransferableVector.h, engine/CPersistTraits.h,
//   engine/CTCInventoryMap.h) from the all-stand-in model (non-template CPersistContext_TransferableVector, global
//   CUIMiniMapMarker, local CCharString / C2DVector / CRetailVector); it now binds the traits callee 0x006159A0 under its landed
//   Ego_r spelling ?TransferIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAV...
//   Self rename (was the stand-in ?TransferIn@CPersistContext_TransferableVector@@UAEXPAVCPersistContext@@PAVCStringParser@@J@Z):
//   Ego_r.pdb 0x00666E50 + FableWin.pdb 0x020C4EC0 ?TransferIn@?$CPersistContext_TransferableVector@VCUIMiniMapMarker@CTCInventoryMap@@@@EAE...J@Z
//   (private virtual, slot 3 of the vtable 0x01257164; no landed row references 0x00615920).
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableVector.h"
#include "engine/CTCInventoryMap.h"

// The traits reader is out of line in retail (0x006159A0, its own landed row, an explicit specialisation there): declared only.
template <> void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& context, CStringParser& parser,
                                                                                CTCInventoryMap::CUIMiniMapMarker& value);   // 0x006159A0

template <class T>
void CPersistContext_TransferableVector<T>::TransferIn(CPersistContext& context, CStringParser& parser, long index)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, (*TransVector)[index]);
}
template void CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext&, CStringParser&, long);
