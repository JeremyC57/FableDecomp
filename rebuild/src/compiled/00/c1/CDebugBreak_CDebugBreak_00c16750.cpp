// CDebugBreak::CDebugBreak() -- the empty assertion temporary's constructor: break into the debugger
// Leaf: ??0CDebugBreak@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C16750, 13 bytes, thiscall, returns this: `call [DebugBreak]` (KERNEL32, stdcall,
//   import 0x0143FE64) with this kept in esi.
// Identity (probable): every libpk assertion -- the landed CString::GetData 0x00C0B8C0, Append 0x00C0DD40, and this
//   chunk's CString::Find rows -- constructs this empty temporary right after the g_pfnAssertHook call; the landed
//   sources name it CDebugBreak (also spelled CTrace / CAssertTrace in other batches). Label correction: the catalog's
//   list<CMessageEvent...> constructor label is wrong (the body touches no list).
extern "C" __declspec(dllimport) void __stdcall DebugBreak(void);

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();
};

CDebugBreak::CDebugBreak()
{
    DebugBreak();
}