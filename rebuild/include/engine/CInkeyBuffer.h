#ifndef FABLE_ENGINE_CINKEYBUFFER_H
#define FABLE_ENGINE_CINKEYBUFFER_H
#include "engine/CInitBaseClass.h"
class CCharEvent { public: wchar_t Char; bool Pressed; };
class CInkeyBuffer : public CInitBaseClass {
public:
    bool GetChar(wchar_t*,bool*);
private:
    CCharEvent Buffer[100];
    unsigned long FirstChar, LastChar;
};
extern CInkeyBuffer GInkeyBuffer;
#endif
