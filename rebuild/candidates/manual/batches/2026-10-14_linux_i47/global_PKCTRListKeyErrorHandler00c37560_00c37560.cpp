// PKCTRList_KeyErrorHandler_00c37560(void*, void*) -- EH-framed debug message: a CDebugStream over the shared buffer @ 0x0139C900, 6 `<<` pieces, the report callback @ 0x0143BB2C when set, then a debugger break (constructor @ 0x00C16750); returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37560, oracle length 175 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; mov eax,[g_pDebugBuffer] ; mov [esp+4],eax ; mov edx,offset lit ; lea ecx,[esp+4] ; mov dword ptr [esp+10h],0 ; call operator<< ; (mov edx,piece ; mov ecx,eax ; call operator<<) x5 ; mov eax,[g_pfnErrorReport] ; test eax,eax ; je S ; mov ecx,[esp+4] ; call eax ; S: lea ecx,[esp+4] ; mov dword ptr [esp+10h],-1 ; call ~CDebugStream ; lea ecx,[esp+3] ; call CDebugBreak::CDebugBreak ; mov ecx,[esp+8] ; xor eax,eax ; mov fs:[0],ecx ; add esp,14h ; ret 8
// Error-handler callback (two unused arguments, `__stdcall`): a CDebugStream local over the shared
// debug buffer (EH state 0 while it lives) receives the message pieces through the `__fastcall`
// `operator<<` overloads @ 0x00C0DEC0 (text) / 0x00C0DF00 (number) chained on the returned reference,
// the report callback @ 0x0143BB2C is called with the buffer when installed, the stream is
// destroyed (@ 0x00C0BA40, state -1) and an empty CDebugBreak temporary -- its constructor @
// 0x00C16750 breaks into the debugger -- is built in the high byte of the dead frame dword;
// the handler returns 0. Built under /EHsc (the `__ehhandler` is not compared; the unwind
// funclet lands separately through the funclet pass). Literals are the retail strings; the
// manifest label is propagated. One of 14 bodies of this shape.
struct CDebugStream {                      // 4 bytes: the message buffer
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);         // 0x00C0DF00
    ~CDebugStream();                       // 0x00C0BA40
};

struct CDebugBreak {                       // empty class whose constructor breaks into the debugger
    CDebugBreak();                         // 0x00C16750
};

extern char* g_pDebugBuffer;               // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char* text);   // 0x0143BB2C

long __stdcall PKCTRList_KeyErrorHandler_00c37560(void* unused1, void* unused2)
{
    {
        CDebugStream s(g_pDebugBuffer);
        s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCTRList.h" << "(" << 39 << ") : " << "Error: Cannot resolve access to a key object in a list!" << "\n";
        if (g_pfnErrorReport)
            g_pfnErrorReport(s.m_pBuffer);
    }
    CDebugBreak();
    return 0;
}
