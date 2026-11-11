// CPersistContext::Transfer<unsigned char>(unsigned char& value) -- unnamed binary-only unsigned char transfer: CRC check, then load (Mode 2) or save (Mode 3)
// Leaf: ??$Transfer@E@CPersistContext@@QAEXAAE@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00473593, 67 bytes, `ret 4`; ecx = the context, one pushed field address.
// Identity (T = unsigned char): callers CAppearanceDef::Transfer +0x54/+0x55 (Ego_r OutlineEffectAlphaBias / AlphaRef, uchar) and NUISystem::CUIDef +0x14A; the binary-in is CPersistTraits<unsigned char>::TransferBinaryIn 0x00405940 (bound under that spelling by the landed three-argument Transfer<unsigned char> 0x00405A60), the writer the generic traits TransferBinaryOut = WriteSLONG 0x00993EB0 (movzx). Row already named; respelled onto the shared headers.
//   The one-argument parameter list is the established descriptive spelling of the unnamed retail-only transfer family
//   (engine/CPersistContext.h `Transfer(T&)`; landed 0x00431020 / 0x00431061 / 0x00431143): no PDB carries it (descriptive, NOTES).
// Body: CheckCRC("") (0x00404500, the pooled empty literal 0x0122D70E); a CPersistTraits<unsigned char> (DefaultVal zeroed, `mov byte [ebp-4],0`);
//   Mode 2: traits.TransferBinaryIn(*PLoadStream, value) (0x00405940); Mode 3: the traits' inline TransferBinaryOut =
//   PSaveStream->WriteSLONG(value).
// Replaces a pad-model stand-in (fake `Prep` callee for CheckCRC, local helper structs) with the shared headers.
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

template <> void CPersistContext::Transfer<unsigned char>(unsigned char& value)
{
    CheckCRC("");
    CPersistTraits<unsigned char> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
}
