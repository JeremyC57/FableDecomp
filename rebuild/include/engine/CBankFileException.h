#ifndef FABLE_ENGINE_CBANKFILEEXCEPTION_H
#define FABLE_ENGINE_CBANKFILEEXCEPTION_H

#ifdef FABLE_RETAIL_CBANKFILE_READFILEINFORMATION_PASS29
#include "engine/CExceptionBase.h"
#include "fable_string.h"

class CBankFileException : public CExceptionBase {
public:
    enum EBankFileExceptionType {
        BANK_IS_CORRUPT,
        BANK_IS_INCORRECT_VERSION,
        ENTRY_NOT_FOUND,
        ENTRY_IS_CORRUPT
    };

    CBankFileException(EBankFileExceptionType type) : Type(type) { }
    CBankFileException(CCharString message, EBankFileExceptionType type)
        : CExceptionBase(message), Type(type) { }

protected:
    EBankFileExceptionType Type;
};

FABLE_STATIC_ASSERT(sizeof(CBankFileException) == 4);
#endif
#endif
