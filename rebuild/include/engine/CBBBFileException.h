#ifndef FABLE_ENGINE_CBBBFILEEXCEPTION_H
#define FABLE_ENGINE_CBBBFILEEXCEPTION_H
// Both original complete classes: empty CExceptionBase and protected nestedenum Type+0, size4.
#include "engine/CExceptionBase.h"
#include "rebuild_abi.h"
class CCharString;
class CBBBFileException:public CExceptionBase {
public:
 enum EFileExceptionType {
  GENERIC=0,
  BAD_PARAMETERS=1,
  FILE_NOT_FOUND=2,
  END_OF_FILE=3,
  CANNOT_READ=4,
  CANNOT_WRITE=5,
  CANNOT_CLOSE=6,
  CANNOT_OPEN=7,
  NOT_OPEN_FOR_WRITING=8,
  NOT_OPEN_FOR_READING=9,
  BAD_PATH=10,
  BAD_SEEK=11,
  BUFFER_TOO_SMALL=12
 };
 CBBBFileException(EFileExceptionType);
 CBBBFileException(CCharString,EFileExceptionType);
protected:
 EFileExceptionType Type;
};
FABLE_STATIC_ASSERT(sizeof(CBBBFileException)==4);
#endif
