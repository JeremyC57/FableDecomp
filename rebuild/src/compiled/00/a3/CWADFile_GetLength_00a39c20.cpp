// Leaf: ?GetLength@CWADFile@@UBEJXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CWADFile.h (class-model switch FABLE_RETAIL_CWADFILE; was ?GetLength@CWADFile_Methods@@QAEHXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CWADFile_MovePosition_00a39c00.cpp.
#define FABLE_RETAIL_CWADFILE
#include "engine/CWADFile.h"
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long a0) const;   // 0x009CC240 (Ego_r.pdb ?GetEntryDataSize@CBankFile@@QBEKK@Z)
};
class CBankFileAsync {  };
long CWADFile::GetLength() const{ return ((CBankFile*)(this->Bank))->GetEntryDataSize(this->Index); }