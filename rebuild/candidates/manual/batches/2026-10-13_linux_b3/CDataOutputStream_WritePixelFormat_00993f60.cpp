// CDataOutputStream::WritePixelFormat(CPixelFormat*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F60, oracle length 41 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  sub esp,8 ; push esi ; lea eax,[esp+4] ; mov esi,ecx ; mov ecx,[esp+10h] ; push eax ; call CPixelFormat::GetInit ; mov edx,[esi] ; push 6 ; lea eax,[esp+8] ; push eax ; mov ecx,esi ; call [edx+10h] ; pop esi ; add esp,8 ; ret 4
// Serialises a pixel format: CPixelFormat::GetInit @ 0x009E3870 fills an 8-byte init record
// (returned through a hidden pointer, hence the local), of which the first 6 bytes are written
// through the stream's virtual slot 4 (WriteBytes(const void*, long)).
#include "engine/CPixelFormat.h"

struct CPixelFormatInit {
    unsigned char Raw[8];           // 6 significant bytes + padding
    CPixelFormatInit();
};

struct CPixelFormat_Calls : CPixelFormat {
    CPixelFormatInit GetInit();     // 0x009E3870
};

struct CDataOutputStreamVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void WriteBytes(const void* data, long byteCount);     // slot 4 (+0x10)
};

struct CDataOutputStream_Methods {
    void WritePixelFormat(CPixelFormat_Calls* format);
};

void CDataOutputStream_Methods::WritePixelFormat(CPixelFormat_Calls* format)
{
    CPixelFormatInit init = format->GetInit();
    reinterpret_cast<CDataOutputStreamVirtuals*>(this)->WriteBytes(&init, 6);
}
