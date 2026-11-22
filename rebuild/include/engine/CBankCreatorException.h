#ifndef FABLE_ENGINE_CBANKCREATOREXCEPTION_H
#define FABLE_ENGINE_CBANKCREATOREXCEPTION_H
#ifdef FABLE_RETAIL_CBANKFILE_UPDATE_PASS27

#include "fable_string.h"

class CBankCreatorException {
public:
    CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text);
    CBankCreatorException(const CBankCreatorException& other);
    ~CBankCreatorException();
    CCharString GetText() const;
private:
    CCharString EntrySymbol;
    CCharString ErrorText;
};

class CBankCreatorAbortException {
public:
    CBankCreatorAbortException() {}
};
#endif
#endif
