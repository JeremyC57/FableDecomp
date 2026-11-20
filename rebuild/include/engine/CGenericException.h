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
#ifdef FABLE_RETAIL_CGENERICEXCEPTION_TEXT_CTOR
    // Opt-in (Account B re-port, 2026-11-20): Ego_r.pdb / FableWin.pdb ??0CGenericException@@QAE@PBD@Z, parameter `text` (Ego_d method
    // list 0xCA77), for the rows that throw a message: CBankFileManager::GetDevBankFilePath 0x009A7CA0 / GetRetailBankFilePath 0x009A7D40 and
    // CWorldMap::GetLevelFilesFromWorldFile 0x00501D30. Inline and empty in retail: each throw site builds the message (or not) and
    // copies the 1-byte object with no constructor call. With FABLE_RETAIL_CWAVELOADER_PASS22_EXCEPTIONS (CExceptionBase's
    // `(const char*)` form, both PDBs) the text is handed to the base, as the GetLevelFilesFromWorldFile row models it.
#ifdef FABLE_RETAIL_CWAVELOADER_PASS22_EXCEPTIONS
    CGenericException(const char* text) : CExceptionBase(text) { }
#else
    CGenericException(const char* text) { }
#endif
#endif
};
#endif
