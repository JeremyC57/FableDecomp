// Behaviour test for CConsoleCommandBase::GetFormattedName (0x009EA3A0): the name followed by OPEN_BRACKET, through the
// CCharString + CCharString operator (0x0099F570). Rewritten on the shared headers (NF202, 2026-11-20, q12225): the row's OPEN_BRACKET
// is a static of its own TU's anonymous namespace, so the row object cannot be linked by a separate test TU; this TU carries the same
// one-line body over engine/CConsoleCommandBase.h (FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME) and checks the operands passed.
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME
#include "engine/CConsoleCommandBase.h"
#include <cstdio>

// Out-of-line CCharString members this test touches (stubs; the handle is only compared by address).
CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { }
CConsoleInputBase::~CConsoleInputBase() { }

static const CCharString* g_left = 0;
static const CCharString* g_right = 0;
static CCharStringData* const RESULT = (CCharStringData*)0x1234;
class CResultString : public CCharString { public: void Set(CCharStringData* d) { PStringData = d; } CCharStringData* Get() const { return PStringData; } };
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right)
{
    g_left = &left; g_right = &right;
    CResultString r; r.Set(RESULT); return r;
}

namespace { CCharString OPEN_BRACKET; }   // "(" in retail (0x013CAA48)

CCharString CConsoleCommandBase::GetFormattedName() const
{
    return Name + OPEN_BRACKET;
}

class CTestCommand : public CConsoleCommandBase { public:
    CCharString Call() const { return GetFormattedName(); }
    const CCharString* NameAddress() const { return &Name; }
};

int main()
{
    CTestCommand command;
    CCharString formatted = command.Call();
    bool ok = g_left == command.NameAddress() && g_right == &OPEN_BRACKET
              && static_cast<const CResultString&>(formatted).Get() == RESULT;
    std::printf(ok ? "GETFMTNAME_OK\n" : "GETFMTNAME_FAIL\n");
    return ok ? 0 : 1;
}
