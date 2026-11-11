// CPersistContext::Transfer<EGameEventType>(EGameEventType& value) -- unnamed binary-only enum transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@W4EGameEventType@@@CPersistContext@@QAEXAAW4EGameEventType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456AD9, 65 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = EGameEventType): CExpressionDef::Transfer 0x00456A4F passes +0x74 = Ego_r GameEventType +0x78 (CDefString run +0x3C.. = Ego_r Animation +0x40..).
//   Ego_r.pdb / FableWin.pdb name ??$Transfer@W4EGameEventType@@@CPersistContext@@... (the named three-argument form) for this T; the retail binary-in instance
//   CPersistTraits<EGameEventType>::TransferBinaryIn is 0x00456B1A. The one-argument parameter list is the established descriptive
//   spelling of the unnamed retail-only transfer family (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 /
//   0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<T> (DefaultVal zeroed, `and [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x00456B1A); Mode 3: the traits' inline TransferBinaryOut =
//   PSaveStream->WriteSLONG(value) (0x00993EB0).
// Shape: size TU with frame pointers (`optimize("s", on)` + `("y", off)`).
#include "fable_compiler.h"
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

enum EGameEventType { };

template <> void CPersistContext::Transfer<EGameEventType>(EGameEventType& value)
{
    CheckCRC("");
    CPersistTraits<EGameEventType> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
