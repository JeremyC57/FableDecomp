// CDataOutputStream::WriteNullTerminatedWString(const CWideString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F00, oracle length 46 bytes.
// Name: Ego_r.pdb / FableWin.pdb ?WriteNullTerminatedWString@CDataOutputStream@@QAEXABVCWideString@@@Z (the const reference
//        is the pointer at [esp+14h]: the same bytes as the CWideString* the earlier model took).
//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,[esp+14h] ; mov esi,ecx ; mov ecx,edi ; call <wide text> ; mov ebp,[esi] ; mov ecx,edi ; mov ebx,eax ; call <wide length> ; lea eax,[eax+eax+2] ; push eax ; push ebx ; mov ecx,esi ; call [ebp+10h] ; pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// Writes the string's characters plus the terminating zero: (length + 1) * sizeof(wchar_t) bytes
// starting at the text pointer, through virtual slot 4 (WriteBytes). The two string calls are
// CWideString::GetAsCharArray (0x0099AF10) and GetLength (0x0099B220) from the shared fable_string.h.
// Model: engine/CDataOutputStream.h is the generated data-only layout (vptr + NewLine) and cannot declare the member, so
//        the class is modelled locally with its first five virtual slots (slot names other than WriteBytes not needed).
// Byte parity needs the stream pointer in a local (the vtable load then precedes the GetLength call).
#include "fable_string.h"

class CDataOutputStream { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void WriteBytes(const void* data, long byteCount);     // slot 4 (+0x10)
    void WriteNullTerminatedWString(const CWideString& text);      // 0x00993F00
};

void CDataOutputStream::WriteNullTerminatedWString(const CWideString& text)
{
    // Characters first, then (length + 1) * sizeof(wchar_t) bytes including the terminator.
    CDataOutputStream* stream = this;
    const wchar_t* characters = text.GetAsCharArray();
    stream->WriteBytes(characters, (text.GetLength() + 1) * sizeof(wchar_t));
}
