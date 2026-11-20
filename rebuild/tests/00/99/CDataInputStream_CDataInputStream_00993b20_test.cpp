// Behaviour test for 0x00993B20 CDataInputStream::CDataInputStream() on the shared headers (engine/CDataInputStream.h with
// FABLE_RETAIL_CDATAINPUTSTREAM): the row's include and body verbatim; the out-of-line bases (CBaseClass / CBaseClassNonCopyable /
// CDataStream ctors and dtors) and the class's other virtuals are test stubs. The ctor must run the base ctor once, zero
// +0x04..+0x14 and leave a usable CDataInputStream (GetPosition 0); a mutant that skips a member or the base ctor fails.
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"

CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}

static int g_base = 0;
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_base; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CDataStream::~CDataStream() { }
void CDataInputStream::Close() { }
void CDataInputStream::SetPosition(unsigned long position) { StreamPos = position; }
void CDataInputStream::ReadCompressed(void*, long) { }
void CDataInputStream::ReadZlibCompressed(void*, long) { }
CCharString::CCharString() { }   // never called (vtable stubs only)
CWideString::CWideString() { }
CCharString CDataInputStream::ReadNullTerminatedString() { return CCharString(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { return CWideString(); }
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&, long&) { }
bool CDataInputStream::NeedsBufferedRead(long) { return false; }
void CDataInputStream::InternalReadUnbuffered(unsigned char*, long) { }

int main()
{
    static unsigned char buf[sizeof(CDataInputStream) + 16];
    memset(buf, 0x77, sizeof(buf));
    CDataInputStream* s = new (buf) CDataInputStream;
    const unsigned long* words = (const unsigned long*)buf;   // +0x04..+0x14: StreamPos, StreamSize, CurrentSourcePtr, SourceChunkStreamPos, SourceChunkBytesRemaining (private)
    int ok = sizeof(CDataInputStream) == 0x18 && g_base == 1 && s->GetPosition() == 0 && s->GetLength() == 0
          && words[1] == 0 && words[2] == 0 && words[3] == 0 && words[4] == 0 && words[5] == 0 && words[6] == 0x77777777;
    s->SetPosition(5);
    ok = ok && s->GetPosition() == 5;
    printf(ok ? "00993b20_TEST PASS\n" : "00993b20_TEST FAIL\n");
    return ok ? 0 : 1;
}
