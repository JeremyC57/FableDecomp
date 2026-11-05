#ifndef FABLE_ENGINE_CWAVEFILEEXCEPTION_H
#define FABLE_ENGINE_CWAVEFILEEXCEPTION_H
// Retail layout (chunk 779, hand-written): the exception CWaveLoader::LoadWaveFile throws (ThrowInfo 0x01369580: catchable types
// .?AVCWaveFileException@@ size 1 then .?AVCExceptionBase@@; RTTI base CExceptionBase). FableWin.pdb names only
// ??0CWaveFileException@@QAE@PBD@Z; retail's four throw sites in 0x00AAD650 copy a 1-byte temporary with no constructor call
// (`mov cl,[esp+68h] ; mov [esp+70h],cl ; push offset ThrowInfo ; call _CxxThrowException`), the shape of a default-constructed
// temporary -- modelled as the empty default constructor; the message form is not bound by any landed row.
#include "engine/CExceptionBase.h"
class CWaveFileException : public CExceptionBase {
public:
    CWaveFileException() { }
};
#endif
