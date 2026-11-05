#ifndef FABLE_ENGINE_CEXCEPTIONBASE_H
#define FABLE_ENGINE_CEXCEPTIONBASE_H
// Retail layout (chunk 779, hand-written): the engine's exception root. Ego_r.pdb @STRUCT CExceptionBase, 1 byte, no members;
// RTTI .?AVCExceptionBase@@ (class key V) is the second catchable type of every engine ThrowInfo (0x01369380 CBBBFileException,
// 0x01369580 CWaveFileException). Ego_r.pdb names ??0CExceptionBase@@QAE@XZ and ??0CExceptionBase@@QAE@PBD@Z; retail's throw
// sites (0x00AAD683, 0x00994B30) build the derived object with no constructor call, so the default constructor is empty inline
// here. The const char* form is not modelled (no landed caller binds it).
class CExceptionBase {
public:
    CExceptionBase() { }
};
#endif
