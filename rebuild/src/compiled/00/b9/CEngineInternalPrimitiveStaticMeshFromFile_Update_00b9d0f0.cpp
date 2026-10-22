#include "engine/CEngineInternalPrimitiveStaticMeshFromFile.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineInternalPrimitiveStaticMeshFromFile_Methods : CEngineInternalPrimitiveStaticMeshFromFile {
    void M(int primitive, int b);
};
void CEngineInternalPrimitiveStaticMeshFromFile_Methods::M(int primitive, int b) { this->fc = b; }