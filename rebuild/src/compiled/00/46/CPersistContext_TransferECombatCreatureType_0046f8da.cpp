// CPersistContext::Transfer<ECombatCreatureType>(ECombatCreatureType& value) -- unnamed binary-only enum transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@W4ECombatCreatureType@@@CPersistContext@@QAEXAAW4ECombatCreatureType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046F8DA, 65 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = ECombatCreatureType): CCombatTypeDef::Transfer 0x0046F86E passes +0x90 (after the map +0x84, 0x0046FA22) = Ego_r CombatCreatureType +0xA4 (ECombatCreatureType; Ego_r vectors/map are 4 bytes larger).
//   Ego_r.pdb / FableWin.pdb name ??$Transfer@W4ECombatCreatureType@@@CPersistContext@@... (the named three-argument form) for this T; the retail binary-in instance
//   CPersistTraits<ECombatCreatureType>::TransferBinaryIn is 0x0046F91B. The one-argument parameter list is the established descriptive
//   spelling of the unnamed retail-only transfer family (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 /
//   0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<T> (DefaultVal zeroed, `and [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x0046F91B); Mode 3: the traits' inline TransferBinaryOut =
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

enum ECombatCreatureType { };

template <> void CPersistContext::Transfer<ECombatCreatureType>(ECombatCreatureType& value)
{
    CheckCRC("");
    CPersistTraits<ECombatCreatureType> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
