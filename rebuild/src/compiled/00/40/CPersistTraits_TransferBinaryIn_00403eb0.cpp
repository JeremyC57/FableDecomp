// CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& stream, bool& value) -- read one byte and store it as a bool (non-zero = true)
// Leaf: ?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403EB0, 114 bytes, `ret 8` (the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path).
// Shape: inline 1-byte read (CMemoryDataInputStream::ReadEBOOL) into a temporary homed in the dead stream-argument slot, then
//   `value = byte != 0` (test/setne); VC7.1 duplicates the store tail into the fast path. Speed-optimised (default) TU.
// NF64 (round 7, q10658): the Ego_r.pdb 0x00403910 / FableWin.pdb 0x01874A40 spelling (references, CMemoryDataInputStream) on the
//   explicit class specialisation engine/CPersistTraits.h declares (FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS), the body NF59's 0x004675F6
//   keeps visible (`value = is.ReadEBOOL()`). Was a local `CPersistTraits<bool>` taking (CDataInputStream*, bool*) through a local reader.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
#include "engine/CPersistTraits.h"

void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& is, bool& value)
{
    value = is.ReadEBOOL();
}
