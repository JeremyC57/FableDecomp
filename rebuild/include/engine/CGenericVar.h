#ifndef FABLE_ENGINE_CGENERICVAR_H
#define FABLE_ENGINE_CGENERICVAR_H
#ifdef FABLE_RETAIL_CGENERICVAR_PASS27
#include "fable_string.h"
enum EGenericVarType {
    GENERIC_VAR_NULL, GENERIC_VAR_SLONG, GENERIC_VAR_ULONG, GENERIC_VAR_FLOAT,
    GENERIC_VAR_BOOL, GENERIC_VAR_STRING, GENERIC_VAR_GENERIC_VAR
};
class CGenericVar {
public:
#ifdef FABLE_RETAIL_PROCESS_COMMAND_LINE_PASS29
    virtual ~CGenericVar() {}
    virtual void Init(const float&);
    virtual void Init(const long&);
    virtual void Init(const unsigned long&);
    virtual void Init(const bool&);
    virtual void Init(const CCharString&);
    bool ReadFromParser(class CStringParser&);
#else
    virtual ~CGenericVar();
#endif
    float GetFloat() const { return ValFloat; }
protected:
    EGenericVarType Type;
    CCharString ValString;
    long ValSLONG;
    unsigned long ValULONG;
    bool ValBool;
    float ValFloat;
};
#endif
#endif
