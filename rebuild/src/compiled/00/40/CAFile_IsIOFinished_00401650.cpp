// CAFile::IsIOFinished() const -- synchronous files are always finished
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401650, 3 bytes. Callees: none.
//
// CAFile is the asynchronous-capable file interface. The base implementation has no outstanding I/O to
// wait for, so it reports "finished" unconditionally; the Win32 disk file and threaded reader classes
// override this with a real check on their pending request. Callers poll it before touching a buffer
// they asked to be filled.
//
// Matching: `this` is unused, body is `mov al,1; ret`.
class CAFile { public: unsigned char pad[4]; };

bool __fastcall CAFile_IsIOFinished(const CAFile* file)
{
    (void)file;
    return true;
}
