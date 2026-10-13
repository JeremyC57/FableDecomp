// CDataOutputStream::WriteString(CCharString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009959F0, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov eax,[eax] ; test eax,eax ; mov edx,offset "" ; je E ; mov edx,[eax] ; mov eax,[eax+4] ; push esi ; mov esi,[ecx] ; push eax ; push edx ; call [esi+10h] ; pop esi ; ret 4 ; E: push esi ; mov esi,[ecx] ; xor eax,eax ; push eax ; push edx ; call [esi+10h] ; pop esi ; ret 4
// A CCharString is one pointer to shared string data {char* Text, long Length}. A string with no
// data writes the empty literal at 0x0129AAF4 with length 0; otherwise its text and length are
// written through virtual slot 4 (WriteBytes). VC7.1 keeps the two call sites separate.

struct CCharStringData {
    const char* Text;        // +0x00
    long        Length;      // +0x04
};

struct CCharString {
    CCharStringData* PStringData;   // +0x00
};

struct CDataOutputStreamVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void WriteBytes(const void* data, long byteCount);     // slot 4 (+0x10)
};

struct CDataOutputStream_Methods {
    void WriteString(CCharString* text);
};

void CDataOutputStream_Methods::WriteString(CCharString* text)
{
    CCharStringData* data = text->PStringData;
    if (data)
        reinterpret_cast<CDataOutputStreamVirtuals*>(this)->WriteBytes(data->Text, data->Length);
    else
        reinterpret_cast<CDataOutputStreamVirtuals*>(this)->WriteBytes("", 0);
}
