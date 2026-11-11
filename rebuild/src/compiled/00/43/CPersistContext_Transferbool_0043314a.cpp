// CPersistContext::Transfer<bool>(bool& value) -- unnamed binary-only bool transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@_N@CPersistContext@@QAEXAA_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043314A, 67 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = bool): 249 callers pass bool members (CControlsDef::Transfer +0x48..+0x4D, CEngineVideoOptionsDef, CInventoryCategoryDef ...); the binary-in is the landed CPersistTraits<bool>::TransferBinaryIn 0x00403EB0 (also called by the three-argument Transfer<bool> 0x004045C0) and the writer CDataOutputStream::WriteEBOOL 0x00993E30 (FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@_N@@ inline, engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT). Row already named; respelled onto the shared headers.
//   The one-argument parameter list is the established descriptive spelling of the unnamed retail-only transfer family
//   (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<bool> (DefaultVal zeroed, `mov byte [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x00403EB0); Mode 3: the traits' inline TransferBinaryOut =
//   PSaveStream->WriteEBOOL(value).
// Replaces a pad-model stand-in (fake `Prep` callee for CheckCRC, local helper structs) with the shared headers.
// Shape: size TU with frame pointers (`optimize("s", on)` + `("y", off)`).
#include "fable_compiler.h"
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_BINARYOUT
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template <> void CPersistContext::Transfer<bool>(bool& value)
{
    CheckCRC("");
    CPersistTraits<bool> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
