// CPersistContext::Transfer<EWallMountEffects>(EWallMountEffects& value) -- unnamed binary-only enum transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@W4EWallMountEffects@@@CPersistContext@@QAEXAAW4EWallMountEffects@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DEDDA, 65 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = EWallMountEffects): CWallMountEffectsDef::Transfer +0x28 = Ego_r Effect (EWallMountEffects, first member).
//   Ego_r.pdb / FableWin.pdb name ??$Transfer@W4EWallMountEffects@@@CPersistContext@@... (the named three-argument form) for this T; the retail binary-in instance
//   CPersistTraits<EWallMountEffects>::TransferBinaryIn is 0x004DEE1B. The one-argument parameter list is the established descriptive
//   spelling of the unnamed retail-only transfer family (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 /
//   0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<T> (DefaultVal zeroed, `and [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x004DEE1B); Mode 3: the traits' inline TransferBinaryOut =
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

enum EWallMountEffects { };

template <> void CPersistContext::Transfer<EWallMountEffects>(EWallMountEffects& value)
{
    CheckCRC("");
    CPersistTraits<EWallMountEffects> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
