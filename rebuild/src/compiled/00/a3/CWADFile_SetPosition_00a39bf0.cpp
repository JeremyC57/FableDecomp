// Leaf: ?SetPosition@CWADFile@@UAEXJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWADFile.h under FABLE_RETAIL_CWADFILE with the PDB parameter / return types (was ?SetPosition@CWADFile_Methods@@QAEXK@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWADFILE
#include "engine/CWADFile.h"
void CWADFile::SetPosition(long v) { this->FilePos = v; }
