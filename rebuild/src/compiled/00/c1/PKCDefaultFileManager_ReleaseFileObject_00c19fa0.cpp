// PKCDefaultFileManager::ReleaseFileObject(CAudioFile*) -- EH-framed thiscall (`this` unused, `ret 4`): when the file object is null a log line is built on the stack from the shared buffer @ 0x0139C900 (state 0, in its own block: `.\PKCDefaultFileManager.cpp(91) : NULL File Object`, chained `__fastcall` builders, the optional sink @ 0x0143BB2C, release @ 0x00C0BA40) and an empty trace temporary built in the dead argument slot (@ 0x00C16750); else the object's virtual slot 4 runs with 1
// Leaf: ?ReleaseFileObject@PKCDefaultFileManager@@QAEXPAVCAudioFile@@@Z
// Flags: /EHsc
// Name-fix NF38: libpk/PKCDefaultFileManager.h declares ReleaseFileObject(CAudioFile*) for 0x00C19FA0 (descriptive, no PDB) and the landed
// caller binds ?ReleaseFileObject@PKCDefaultFileManager@@QAEXPAVCAudioFile@@@Z; slot 4 of libpk/CAudioFile.h is the virtual destructor, so
// `call [eax+10h]` with 1 is `delete file`. Was the free __stdcall stand-in ReleaseFileObject_00c19fa0(CFileObject_00c19fa0*) (same bytes:
// the thiscall `this` is never read).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19FA0, oracle length 199 bytes. Built under /EHsc.
// Retail bytes:  ... mov ecx,[esp+14h] ; test ecx,ecx ; jne rel ; mov eax,[g_logBuffer] ; mov [esp],eax ; mov [esp+0Ch],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp] ; call Append ; ... AppendNumber(5Bh) ... "NULL File Object" ... "\n" ; mov eax,[g_pfnLogSink] ; test eax,eax ; je done ; mov ecx,[esp] ; call eax ; done: lea ecx,[esp] ; mov dword ptr [esp+0Ch],-1 ; call ~CLogLine ; lea ecx,[esp+14h] ; call CTrace::CTrace ; ... ret 4 ; rel: mov edx,[ecx] ; push 1 ; call [edx+10h] ; ... ret 4
// The trace temporary lives only in the null branch here. The manifest label is a description;
// named by the shape.
#define FABLE_RETAIL_CSTRING_BUILDERS
#define FABLE_RETAIL_CSTRING_ALLOCCOPY
#define FABLE_RETAIL_CSTRING_DEFAULT_CTOR
#include "libpk/PKCDefaultFileManager.h"
#include "libpk/CAudioFile.h"
#include "engine/CString.h"

void PKCDefaultFileManager::ReleaseFileObject(CAudioFile* file)
{
    if (!file) {
        {
            CString line;
            line << ".\\PKCDefaultFileManager.cpp" << "(" << 91L << ") : " << "NULL File Object" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
        return;
    }
    delete file;
}
