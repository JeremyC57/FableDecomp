#ifndef FABLE_ENGINE_CGENERICEXCEPTION_H
#define FABLE_ENGINE_CGENERICEXCEPTION_H
// New header (hand3c2, row 0x009BF7E0 CDisplayManager::CreateDevice), as engine/CWaveFileException.h: the exception the display
// manager throws when the adapter's display mode cannot be read. Fable.exe ThrowInfo 0x013692F8: catchable types
// .?AVCGenericException@@ (size 1) then .?AVCExceptionBase@@ (RTTI names read from the image); Ego_d.pdb LF_CLASS 0xCA79
// CGenericException (1 byte, base CExceptionBase) with two constructors (method list 0xCA77: `(const char*)` = Ego_r.pdb / FableWin.pdb
// ??0CGenericException@@QAE@PBD@Z, parameter `text`, and `()`). Retail's throw sites (0x009BF9EA in the row, 0x009BE340
// ReadDeviceRefreshRate, the landed Init 0x009C0E50) copy a 1-byte temporary with no constructor call (`mov dl,[esp+2Ch] ; push
// offset ThrowInfo ; ... ; mov [esp+34h],dl ; call _CxxThrowException`): modelled as the empty default constructor, the message form
// is not bound by any row (the CWaveFileException precedent).
#include "engine/CExceptionBase.h"
class CGenericException : public CExceptionBase {
public:
    CGenericException() { }
};
#endif
