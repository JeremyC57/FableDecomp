// CPersistContext::Transfer<NInventory::EInventoryIdentifier>(NInventory::EInventoryIdentifier& value) -- unnamed binary-only enum transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@W4EInventoryIdentifier@NInventory@@@CPersistContext@@QAEXAAW4EInventoryIdentifier@NInventory@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00468E10, 65 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = NInventory::EInventoryIdentifier): CInventoryDef::Transfer 0x00467C24 passes +0x1F0 after the ulong +0x1EC (= Ego_r InventoryName +0x208; the long run +0x1E0..+0x1E8 = Ego_r MaxNumberOf* +0x1FC..+0x204, C2DVector +0x1F4 = MapScreenMapDisplayBoxTLPos +0x210) = Ego_r InventoryIdentifier +0x20C (NInventory::EInventoryIdentifier).
//   Ego_r.pdb / FableWin.pdb name ??$Transfer@W4EInventoryIdentifier@NInventory@@@CPersistContext@@... (the named three-argument form) for this T; the retail binary-in instance
//   CPersistTraits<NInventory::EInventoryIdentifier>::TransferBinaryIn is 0x00468E51. The one-argument parameter list is the established descriptive
//   spelling of the unnamed retail-only transfer family (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 /
//   0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<T> (DefaultVal zeroed, `and [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x00468E51); Mode 3: the traits' inline TransferBinaryOut =
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

namespace NInventory { enum EInventoryIdentifier { }; }
// VC7.1 resolves the traits' `DefaultVal()` value-initialisation by the unqualified enum name (C2065 without this).
using NInventory::EInventoryIdentifier;

template <> void CPersistContext::Transfer<NInventory::EInventoryIdentifier>(NInventory::EInventoryIdentifier& value)
{
    CheckCRC("");
    CPersistTraits<NInventory::EInventoryIdentifier> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
