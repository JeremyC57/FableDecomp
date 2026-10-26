#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
class CBankFileAsync { public: int Method(int); };
struct CWADFile_Methods : CWADFile {
    int GetLength();
};
int CWADFile_Methods::GetLength(){ return this->Bank->Method(this->Index); }