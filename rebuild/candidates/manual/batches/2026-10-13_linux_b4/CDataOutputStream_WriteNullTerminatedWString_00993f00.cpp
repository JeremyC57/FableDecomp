// CDataOutputStream::WriteNullTerminatedWString(CWideString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00993F00, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,[esp+14h] ; mov esi,ecx ; mov ecx,edi ; call <wide text> ; mov ebp,[esi] ; mov ecx,edi ; mov ebx,eax ; call <wide length> ; lea eax,[eax+eax+2] ; push eax ; push ebx ; mov ecx,esi ; call [ebp+10h] ; pop edi ; pop esi ; pop ebp ; pop ebx ; ret 4
// Writes the string's characters plus the terminating zero: (length + 1) * sizeof(wchar_t) bytes
// starting at the text pointer, through virtual slot 4 (WriteBytes). The two string calls are
// the wide-string text accessor (0x0099AF10) and length (0x0099B220); their manifest labels name
// the byte-identical CCharString versions.

struct CWideStringCalls {
    const wchar_t* GetText();       // 0x0099AF10
    long           GetLength();     // 0x0099B220
};

struct CDataOutputStreamVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void WriteBytes(const void* data, long byteCount);     // slot 4 (+0x10)
};

struct CDataOutputStream_Methods {
    void WriteNullTerminatedWString(CWideStringCalls* text);
};

void CDataOutputStream_Methods::WriteNullTerminatedWString(CWideStringCalls* text)
{
    const wchar_t* characters = text->GetText();
    long byteCount = (text->GetLength() + 1) * 2;
    reinterpret_cast<CDataOutputStreamVirtuals*>(this)->WriteBytes(characters, byteCount);
}
