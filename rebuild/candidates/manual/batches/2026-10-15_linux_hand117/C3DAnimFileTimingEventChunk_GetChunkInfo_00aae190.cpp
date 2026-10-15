// C3DAnimFileTimingEventChunk::GetChunkInfo() const -- "Name: " + Name + ", Time: " + the time with six decimals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAE190, 144 bytes. Callees: CCharString::FormatDouble 0x009A08B0
// (static __fastcall: hidden result ecx, digits edx, the double on the stack), CCharString(const char*, long)
// 0x0099EBF0, operator+(const CCharString&, const CCharString&) 0x0099F570, operator+(const CCharString&,
// const char*) 0x0099F600 (both __fastcall: hidden result ecx, left edx, right on the stack), ~CCharString 0x0099EAE0.
// Layout as the landed C3DAnimFileTimingEventChunk::WriteToFile (0x00AAE220): Name +0x10, Time +0x14 on the
// plain CChunkedFileChunk base. The chained `+` is evaluated right to left: the formatted time first, then
// the ", Time: " literal and &Name are pushed, then the explicit "Name: " temporary (its returned pointer
// goes straight to edx); the four temporaries die in construction order after the result is built.
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);                          // 0x0099EBF0
    ~CCharString();                                                      // 0x0099EAE0
    static CCharString __fastcall FormatDouble(long digits, double value);   // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

struct C3DAnimFileTimingEventChunk {
    virtual ~C3DAnimFileTimingEventChunk();
    unsigned char _base_body[0x0C];                 // +0x04 .. +0x0F (CChunkedFileChunk: type, parent)
    CCharString Name;                               // +0x10
    float Time;                                     // +0x14
    CCharString GetChunkInfo() const;
};

CCharString C3DAnimFileTimingEventChunk::GetChunkInfo() const
{
    return CCharString("Name: ", -1) + Name + ", Time: " + CCharString::FormatDouble(6, Time);
}
