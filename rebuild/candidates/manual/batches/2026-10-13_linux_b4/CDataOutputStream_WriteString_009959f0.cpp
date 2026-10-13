// CDataOutputStream::WriteString(CCharString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009959F0, oracle length 46 bytes.
// Grade: candidate -- graded under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
//
// Retail bytes:  mov eax,[esp+4] ; mov eax,[eax] ; test eax,eax ; mov edx,offset "" ; je E ; mov edx,[eax] ; mov eax,[eax+4] ; push esi ; mov esi,[ecx] ; push eax ; push edx ; call [esi+10h] ; pop esi ; ret 4 ; E: push esi ; mov esi,[ecx] ; xor eax,eax ; push eax ; push edx ; call [esi+10h] ; pop esi ; ret 4
// A CCharString is one pointer to shared string data {char* Text, long Length}. A string with no
// data writes the empty literal at 0x0129AAF4 with length 0; otherwise its text and length are
// written through virtual slot 4 (WriteBytes). VC7.1 keeps the two call sites separate.
//
// Lane feedback: STILL DIFFER(46v46): same instructions, but retail hoists `mov edx,offset ""` above the
// null test while our build keeps it in the else arm. Open: see the batch README.
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
    // Unshared (null) string data writes the empty literal with length 0.
    CCharStringData* data = text->PStringData;
    const char* characters = data ? data->Text : "";
    long length = data ? data->Length : 0;
    reinterpret_cast<CDataOutputStreamVirtuals*>(this)->WriteBytes(characters, length);
}
