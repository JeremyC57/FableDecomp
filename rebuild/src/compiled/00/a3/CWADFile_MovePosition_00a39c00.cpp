// Leaf: ?MovePosition@CWADFile@@UAEXJ@Z
#define FABLE_RETAIL_CWADFILE   // alias route: engine/CWADFile.h declares MovePosition under this class-model switch
#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
void CWADFile::MovePosition(long a){ this->FilePos += a; }