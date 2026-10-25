// Behaviour test for 0x009A5020 CSystemManager::ExceptionHandler: only a valid manager in exclusive mode writes the
// message; each exception code gets its text (FLT_OVERFLOW reuses the divide-by-zero text); unknown codes write
// nothing; the result is always EXCEPTION_CONTINUE_SEARCH (0).
#include <cstdio>
#include <cstring>

static char g_SystemExceptionMessage[256];

struct CSystemManager {
    bool Valid;
    bool ExclusiveMode;
    bool IsValid() const { return Valid; }
    int ExceptionHandler(int exceptionCode)
    {
        if (IsValid() == true && ExclusiveMode == true) {
            switch (exceptionCode) {
            case 0xC0000005:   // EXCEPTION_ACCESS_VIOLATION
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - An access violation has occured.");
                break;
            case 0x80000004:   // EXCEPTION_SINGLE_STEP
                std::sprintf(g_SystemExceptionMessage, "STUPID ERROR - The program has encountered a single step signal.");
                break;
            case 0x80000003:   // EXCEPTION_BREAKPOINT
                std::sprintf(g_SystemExceptionMessage, "STUPID ERROR - The program has encountered a breakpoint.");
                break;
            case 0xC0000025:   // EXCEPTION_NONCONTINUABLE_EXCEPTION
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to continue from a non-continuable exception. Please quit the application.");
                break;
            case 0xC000008E:   // EXCEPTION_FLT_DIVIDE_BY_ZERO
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
                break;
            case 0xC000008D:   // EXCEPTION_FLT_DENORMAL_OPERAND
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point operation with a denormal operand.");
                break;
            case 0xC000008C:   // EXCEPTION_ARRAY_BOUNDS_EXCEEDED
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to access an out of bounds array element.");
                break;
            case 0xC000008F:   // EXCEPTION_FLT_INEXACT_RESULT
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The result of a floating point operation cannot be represented.");
                break;
            case 0xC0000090:   // EXCEPTION_FLT_INVALID_OPERATION
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted an illegal floating point operation.");
                break;
            case 0xC0000091:   // EXCEPTION_FLT_OVERFLOW (retail reuses the divide-by-zero text)
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
                break;
            case 0xC0000092:   // EXCEPTION_FLT_STACK_CHECK
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - A floating point operation overflowed or underflowed the stack.");
                break;
            case 0xC0000093:   // EXCEPTION_FLT_UNDERFLOW
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The exponent of a floating point operation is less than the magnitude allowed.");
                break;
            case 0xC0000094:   // EXCEPTION_INT_DIVIDE_BY_ZERO
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted an integer divide by zero.");
                break;
            case 0xC0000095:   // EXCEPTION_INT_OVERFLOW
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - An integer operation overflowed.");
                break;
            case 0xC0000096:   // EXCEPTION_PRIV_INSTRUCTION
                std::sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to use an illegal instruction.");
                break;
            }
        }
        return 0;
    }
};

static int Says(CSystemManager& m, unsigned int code, const char* text)
{
    g_SystemExceptionMessage[0] = 0;
    if (m.ExceptionHandler((int)code) != 0) return 0;
    return std::strcmp(g_SystemExceptionMessage, text) == 0;
}

int main()
{
    int ok = 1;
    CSystemManager m; m.Valid = true; m.ExclusiveMode = true;
    ok &= Says(m, 0xC0000005u, "CRITICAL ERROR - An access violation has occured.");
    ok &= Says(m, 0x80000003u, "STUPID ERROR - The program has encountered a breakpoint.");
    ok &= Says(m, 0xC0000091u, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
    ok &= Says(m, 0xC0000094u, "CRITICAL ERROR - The program attempted an integer divide by zero.");
    ok &= Says(m, 0xC0000096u, "CRITICAL ERROR - The program attempted to use an illegal instruction.");
    ok &= Says(m, 0xC000001Du, "");                         // not handled
    m.ExclusiveMode = false;
    ok &= Says(m, 0xC0000005u, "");                         // windowed: nothing written
    m.ExclusiveMode = true; m.Valid = false;
    ok &= Says(m, 0xC0000005u, "");                         // not initialised
    std::printf("009a5020_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}