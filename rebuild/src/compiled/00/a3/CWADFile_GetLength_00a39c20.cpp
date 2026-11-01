#include "engine/CWADFile.h"  // retyped onto the PDB layout; byte parity re-verified
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long a0) const;   // 0x009CC240 (Ego_r.pdb ?GetEntryDataSize@CBankFile@@QBEKK@Z)
};
class CBankFileAsync {  };
struct CWADFile_Methods : CWADFile {
    int GetLength();
};
int CWADFile_Methods::GetLength(){ return ((CBankFile*)(this->Bank))->GetEntryDataSize(this->Index); }