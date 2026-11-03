#ifndef FABLE_ENGINE_CGENERICEXCEPTION_H
#define FABLE_ENGINE_CGENERICEXCEPTION_H
class CExceptionBase { public: CExceptionBase() {} CExceptionBase(const char*); };
class CGenericException : public CExceptionBase { public: CGenericException() {} CGenericException(const char*); };
#endif
