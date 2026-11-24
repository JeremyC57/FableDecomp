// Leaf: ?SetPosition@CBufferedFileDataOutputStream@@UAEXK@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CBufferedFileDataOutputStream.h under FABLE_RETAIL_CBUFFEREDFILEDATAOUTPUTSTREAM_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CBufferedFileDataOutputStream_Methods::SetPosition); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CBUFFEREDFILEDATAOUTPUTSTREAM_VIRTUAL_MODEL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CBufferedFileDataOutputStream.h"  // retyped onto the PDB layout; byte parity re-verified
void CBufferedFileDataOutputStream::SetPosition(unsigned long v) { this->FilePos = v; }
