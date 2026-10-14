// CHttpUrl_0083f420::GetHost_0083f5c0() const -- parse the URL if it has not been parsed yet, then return the [begin, end) character range at +0x2c/+0x30 as a new counted string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083F5C0, length 48 bytes.
// Callees: CHttpUrl_0083f420::Parse @ 0x0083F420 (auto-named "LHHttp2_ParseURL": splits the stored
// URL into scheme/host/port/path ranges, default port 80 at +0x44), the string's range constructor
// @ 0x00567D00 (`__thiscall (const char* first, const char* last, const STag&)`: allocates last-first+1,
// copies, NUL-terminates; labelled "CTCElectrocutedEffect::InitFromStringRange").
// The 12-byte result {begin, end, capacity} is a temporary built straight into the hidden return
// slot (VC7.1 has the unnamed return-value optimisation only: a named local is copied out through
// the copy constructor) by the string's inline constructor from a character RANGE taken by
// reference: zero the three pointers, then the out-of-line range construction reading
// range.begin/range.end -- which is why the two member loads come after the zero stores and `this`
// stays live in edi. The empty iterator-category tag is the 1-byte temporary homed in the dead
// hidden-pointer slot (`lea eax,[esp+0Ch]`). The class is the networking module's URL holder (members at +0x1c..
// +0x44 are character ranges, +0x44 the port); the manifest's "CColourConverter" label is
// propagated. Its sibling 0x0083F600 returns the +0x38/+0x3c range the same way.
struct SForwardIteratorTag { SForwardIteratorTag() { } };

struct SCharRange { const char* begin; const char* end; };  // a [begin, end) slice of the URL text

struct CNetString_Retail {                                  // 12 bytes: begin, end, end of storage
    char* begin;
    char* end;
    char* capacity;
    void Construct(const char* first, const char* last, const SForwardIteratorTag& tag);   // 0x00567D00
    CNetString_Retail(const SCharRange& range) : begin(0), end(0), capacity(0)
    {
        Construct(range.begin, range.end, SForwardIteratorTag());
    }
};

struct CHttpUrl_0083f420 {
    unsigned char _pad_0x00[0x2c];
    SCharRange m_host;                                      // +0x2c
    unsigned char _pad_0x34[0x4];
    SCharRange m_path;                                      // +0x38
    unsigned char _pad_0x40[0x4];
    unsigned short m_port;                                  // +0x44
    void Parse();                                           // 0x0083F420
    CNetString_Retail GetHost_0083f5c0();
};

CNetString_Retail CHttpUrl_0083f420::GetHost_0083f5c0()
{
    Parse();
    return CNetString_Retail(m_host);
}
