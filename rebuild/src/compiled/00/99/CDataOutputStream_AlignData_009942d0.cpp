// CDataOutputStream::AlignData(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009942D0, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+8] ; mov edx,[esp+8] ; lea ecx,[edx-1] ; test eax,ecx ; je R ; lea edx,[eax+edx-1] ; not ecx ; and edx,ecx ; sub edx,eax ; push edx ; push 0 ; mov ecx,esi ; call <write fill> ; R: pop esi ; ret 4
// Pads the stream up to a power-of-two boundary: the current position comes from virtual slot 2
// (GetPosition); if it is not already aligned, ((pos + align - 1) & ~(align - 1)) - pos zero
// bytes are emitted through the fill writer at 0x00994230 (__fastcall: value, count).

struct CDataOutputStreamVirtuals {
    virtual void v00(); virtual void v04();
    virtual long GetPosition();              // slot 2 (+0x08)
};

struct CDataOutputStream_Methods {
    void WriteFill(long value, long byteCount);     // 0x00994230
    void AlignData(long alignment);
};

void CDataOutputStream_Methods::AlignData(long alignment)
{
    long position = reinterpret_cast<CDataOutputStreamVirtuals*>(this)->GetPosition();
    if (position & (alignment - 1))
        WriteFill(0, ((position + alignment - 1) & ~(alignment - 1)) - position);
}