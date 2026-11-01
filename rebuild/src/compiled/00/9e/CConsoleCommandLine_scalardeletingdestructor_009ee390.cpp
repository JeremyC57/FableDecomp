#pragma optimize("t", on)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
void* __fastcall CConsoleCommandLine_CompositeScalarDeletingDestructor(void* line, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(line) + 4))->~CCharString();
    ((CBaseClass*)(line))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(line);
    return line;
}