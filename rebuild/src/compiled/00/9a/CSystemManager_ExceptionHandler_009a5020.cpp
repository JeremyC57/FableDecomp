// CSystemManager::ExceptionHandler(int exceptionCode) -- when the system manager is valid and running in exclusive mode, write a description of the structured exception code into the critical-message buffer; always continue the search
// Leaf: ?ExceptionHandler@CSystemManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A5020, 466 bytes of code + its 7-entry jump table (0x009A51F4, cases
//   0xC0000090..0xC0000096; row length 496), thiscall, `ret 4`. FableWin.pdb `?ExceptionHandler@CSystemManager@@QAEHH@Z`;
//   the catalog's "CTCInventoryBase_ExceptionHandler" took its class from the propagated label of the callee 0x0099A330.
// Callees: CInitBaseClass::IsValid() const 0x0099A330 (out of line, `mov al,[ecx+4]`; the manifest's
//   "CTCInventoryBase::GetCanItemBeQuickAssigned" is a byte-identical propagation), sprintf thunk 0x00BFEB10.
// Data: the message buffer 0x013CB0C8; one literal per case (0x0129AB98..0x0129AFB4, laid out in reverse source order).
// Layout: CSystemManager derives from CInitBaseClass (vptr +0, Valid +4); the flag at retail +0x8E is the Ego_r.pdb
//   ExclusiveMode (+0x86 there; retail sits 8 bytes higher, as WindowInitialised/DDrawInitialised/ExclusiveMode run
//   +0x8C..+0x8E) -- name by position, not verified against a retail reader.
// Shape: `IsValid() == true && ExclusiveMode == true` (both `cmp al,1`), then one switch over the Win32 exception codes
//   whose bodies sit in source order; EXCEPTION_FLT_OVERFLOW carries the same text as EXCEPTION_FLT_DIVIDE_BY_ZERO, so
//   its jump-table slot shares that body; every case breaks to `return EXCEPTION_CONTINUE_SEARCH` (VC duplicates the
//   three-instruction epilogue into each case).
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

extern char g_SystemExceptionMessage[];                 // 0x013CB0C8

class CInitBaseClass
{
public:
    virtual void Validate() const;
    bool Valid;                                         // +0x04
    bool IsValid() const;                               // 0x0099A330
};

class CSystemManager : public CInitBaseClass
{
public:
    unsigned char _pad08[0x8E - 0x08];
    bool          ExclusiveMode;                        // +0x8E (Ego_r.pdb +0x86)
    int ExceptionHandler(int exceptionCode);
};

int CSystemManager::ExceptionHandler(int exceptionCode)
{
    if (IsValid() == true && ExclusiveMode == true) {
        switch (exceptionCode) {
        case 0xC0000005:   // EXCEPTION_ACCESS_VIOLATION
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - An access violation has occured.");
            break;
        case 0x80000004:   // EXCEPTION_SINGLE_STEP
            sprintf(g_SystemExceptionMessage, "STUPID ERROR - The program has encountered a single step signal.");
            break;
        case 0x80000003:   // EXCEPTION_BREAKPOINT
            sprintf(g_SystemExceptionMessage, "STUPID ERROR - The program has encountered a breakpoint.");
            break;
        case 0xC0000025:   // EXCEPTION_NONCONTINUABLE_EXCEPTION
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to continue from a non-continuable exception. Please quit the application.");
            break;
        case 0xC000008E:   // EXCEPTION_FLT_DIVIDE_BY_ZERO
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
            break;
        case 0xC000008D:   // EXCEPTION_FLT_DENORMAL_OPERAND
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point operation with a denormal operand.");
            break;
        case 0xC000008C:   // EXCEPTION_ARRAY_BOUNDS_EXCEEDED
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to access an out of bounds array element.");
            break;
        case 0xC000008F:   // EXCEPTION_FLT_INEXACT_RESULT
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The result of a floating point operation cannot be represented.");
            break;
        case 0xC0000090:   // EXCEPTION_FLT_INVALID_OPERATION
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted an illegal floating point operation.");
            break;
        case 0xC0000091:   // EXCEPTION_FLT_OVERFLOW (retail reuses the divide-by-zero text)
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted a floating point divide by zero.");
            break;
        case 0xC0000092:   // EXCEPTION_FLT_STACK_CHECK
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - A floating point operation overflowed or underflowed the stack.");
            break;
        case 0xC0000093:   // EXCEPTION_FLT_UNDERFLOW
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The exponent of a floating point operation is less than the magnitude allowed.");
            break;
        case 0xC0000094:   // EXCEPTION_INT_DIVIDE_BY_ZERO
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted an integer divide by zero.");
            break;
        case 0xC0000095:   // EXCEPTION_INT_OVERFLOW
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - An integer operation overflowed.");
            break;
        case 0xC0000096:   // EXCEPTION_PRIV_INSTRUCTION
            sprintf(g_SystemExceptionMessage, "CRITICAL ERROR - The program attempted to use an illegal instruction.");
            break;
        }
    }
    return 0;              // EXCEPTION_CONTINUE_SEARCH
}