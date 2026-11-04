// CPersistContext::Transfer<CWideString>(CWideString& value) -- unnamed binary-only wide-string transfer: CRC check, then read the string from the load stream (Mode 2) or write it to the save stream (Mode 3)
// Leaf: ??$Transfer@VCWideString@@@CPersistContext@@QAEXAAVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004310A7, 91 bytes, `ret 4`. Sibling of the landed one-argument Transfer<float>
//   0x00431061 (whose notes name this address as the CWideString instantiation), Transfer<long> 0x00431102 and
//   Transfer<unsigned long> 0x00431020; the CCharString twin is 0x00431143. The manifest label TransferWideString is
//   descriptive (forge autoname); Ego_r.pdb declares the member template `Transfer(T&)` (engine/CPersistContext.h).
// Callees: CPersistContext::CheckCRC 0x00404500 with "" (0x0122D70E); CWideString::CWideString() 0x0099AED0 (the traits'
//   DefaultVal); CDataInputStream slot 7 ReadNullTerminatedWString (+0x1c, by value through the hidden pointer);
//   CWideString::operator= 0x0099B7D0; CDataOutputStream::WriteNullTerminatedWString 0x00993F00; ~CWideString 0x0099B510.
// Shape: the size TU of the landed siblings (`#pragma optimize("s", on)` + `("y", off)`: ebp frame, `leave`, unaligned
//   entry). CPersistTraits<T> (retail user ctor value-initialising DefaultVal, FABLE_RETAIL_CPERSISTTRAITS) is built after
//   CheckCRC; its TransferBinaryIn for CWideString is inline (`value = stream.ReadNullTerminatedWString()`, as in the landed
//   three-argument Transfer<CWideString> 0x00410AC0). Each case arm `return`s: with `break` VC7.1 swaps the two frame slots
//   (traits at [ebp-8], the read temporary at [ebp-4]); with `return` the traits keep [ebp-4] like retail.
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

template <> __forceinline void CPersistTraits<CWideString>::TransferBinaryIn(CMemoryDataInputStream& stream, CWideString& value)
{
    value = stream.ReadNullTerminatedWString();
}

template <> void CPersistContext::Transfer<CWideString>(CWideString& value)
{
    CheckCRC("");
    CPersistTraits<CWideString> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream, value);
        return;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteNullTerminatedWString(value);
        return;
    }
}