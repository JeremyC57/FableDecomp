#ifndef FABLE_ENGINE_CCONSOLECOMMANDLINE_H
#define FABLE_ENGINE_CCONSOLECOMMANDLINE_H
#include "engine/CBaseClass.h"
#include "fable_string.h"

// Original FableWin TPI and the inline subobject construction in retail 009ECD80.
class CConsoleCommandLine : public CBaseClass {
public:
    CConsoleCommandLine();
    virtual ~CConsoleCommandLine();
private:
    CCharString CurrentEntry;
    bool UserEntry;
    long CursorPosition;
};
FABLE_STATIC_ASSERT(sizeof(CConsoleCommandLine)==0x10);
#endif
