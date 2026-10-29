// CDataOutputStream::WriteNullTerminatedWString(CWideString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F00, oracle length 46 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,[esp+14h] ; mov esi,ecx ; mov ecx,edi ; call <wide text> ; mov ebp,[esi] ; mov ecx,edi ; mov ebx,eax ; call <wide length> ; lea eax,[eax+eax+2] ; push eax ; push ebx ; mov ecx,esi ; call [ebp+10h] ; pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// Writes the string's characters plus the terminating zero: (length + 1) * sizeof(wchar_t) bytes
// starting at the text pointer, through virtual slot 4 (WriteBytes). The two string calls are
// CWideString::GetAsCharArray (0x0099AF10) and GetLength (0x0099B220); their manifest labels name
// the byte-identical CCharString versions.
//
// Lane feedback: SRC_FAIL was the wchar_t typedef; byte parity then needs the stream pointer in a local
// (vtable loaded into ebp before the length call). RELOCATION_MATCH.
#include <stddef.h>     // wchar_t is a typedef under VC7.1 without /Zc:wchar_t (the retail build)

class CWideString { public:
    const wchar_t* GetAsCharArray() const;   // 0x0099AF10
    long           GetLength() const;        // 0x0099B220
};

struct CDataOutputStreamVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void WriteBytes(const void* data, long byteCount);     // slot 4 (+0x10)
};

struct CDataOutputStream_Methods {
    void WriteNullTerminatedWString(CWideString* text);
};

void CDataOutputStream_Methods::WriteNullTerminatedWString(CWideString* text)
{
    // Characters first, then (length + 1) * sizeof(wchar_t) bytes including the terminator.
    // The stream local makes the vtable load precede the GetLength call (retail keeps the
    // vtable in ebp across it).
    CDataOutputStreamVirtuals* stream = reinterpret_cast<CDataOutputStreamVirtuals*>(this);
    const wchar_t* characters = text->GetAsCharArray();
    stream->WriteBytes(characters, (text->GetLength() + 1) * sizeof(wchar_t));
}