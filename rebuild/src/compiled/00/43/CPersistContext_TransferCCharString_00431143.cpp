// CPersistContext::Transfer<CCharString>(CCharString& value) -- unnamed binary-only string transfer: CRC check, then read the string from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Leaf: ??$Transfer@VCCharString@@@CPersistContext@@QAEXAAVCCharString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00431143, 91 bytes, `ret 4`. The CCharString twin of Transfer<CWideString> 0x004310A7
//   and sibling of the landed one-argument Transfer<float> 0x00431061 / <long> 0x00431102 / <unsigned long> 0x00431020.
//   labels_namepass_2026-10-26.tsv names it CPersistContext::Transfer<CCharString> (callers pass CCharString members,
//   e.g. CHeroMarriageDef 0x452398, CWeaponDef 0x4E8CE9); Ego_r.pdb declares the member template `Transfer(T&)`.
// Callees: CPersistContext::CheckCRC 0x00404500 with "" (0x0122D70E); CCharString::CCharString() 0x0099E4B0 (the traits'
//   DefaultVal); CDataInputStream slot 6 ReadNullTerminatedString (+0x18, by value through the hidden pointer);
//   CCharString::operator= 0x0099EFB0; CDataOutputStream::WriteNullTerminatedString 0x00995AE0; ~CCharString 0x0099EAE0.
// Shape: as 0x004310A7: size TU (`optimize("s", on)` + `("y", off)`), CPersistTraits<T> with the retail user ctor
//   (FABLE_RETAIL_CPERSISTTRAITS) built after CheckCRC, its TransferBinaryIn inline (`value = stream.ReadNullTerminatedString()`,
//   the landed three-argument Transfer<CCharString> 0x004109A0 form), and each case arm `return`s (with `break` VC7.1 swaps the
//   traits' frame slot and the read temporary's).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template <> __forceinline void CPersistTraits<CCharString>::TransferBinaryIn(CMemoryDataInputStream& stream, CCharString& value)
{
    value = stream.ReadNullTerminatedString();
}

template <> void CPersistContext::Transfer<CCharString>(CCharString& value)
{
    CheckCRC("");
    CPersistTraits<CCharString> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        return;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteNullTerminatedString(value);
        return;
    }
}