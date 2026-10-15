// CMemoryFile::CMemoryFile(const CMemoryFile&) -- copy constructor: copy the access settings, position, length, open flag and pathname, then append the other buffer's bytes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E99C0, 112 bytes (padding included).
// Grade: DIFFER(114v112) -- frame: retail puts the __copy result temporary in the same dead `other` slot as the empty tag (no frame word), ours gives the result its own word (push ecx / pop ecx, +4 on every frame offset, `mov ecx,ebp` scheduled earlier). Tried: inline copy(), STLport __iterator_category/__distance_type helpers, a direct __copy call. Known lane gap (chunk 132: separate frame slots for two call temporaries)
// Shape: CBaseClassNonCopyable ctor (0x0099A2F0), vtable 0x0122E0FC, the STLport vector<unsigned char>
// DataBuffer (+0x10) default-constructed inline (three zero stores from one register) and the
// CWideString Pathname (+0x20) out of line (0x0099AED0); the body copies AccessType/Position/Length/
// Opened, assigns the pathname (0x0099B7D0) and appends the other buffer with
// std::copy(begin, end, back_inserter(DataBuffer)) whose inlined body calls the SGI
// __copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0) (0x009E9900, __fastcall with
// the hidden result in ecx; the result and the empty tag share the dead `other` argument slot).
struct CWideStringData;

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                        // 0x0099A2F0
};

struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                  // 0x0099AED0
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct CArrayUChar {                                // CArray<unsigned char> (STLport vector)
    unsigned char* m_start;
    unsigned char* m_finish;
    unsigned char* m_endOfStorage;
    CArrayUChar() : m_start(0), m_finish(0), m_endOfStorage(0) { }
    const unsigned char* begin() const { return m_start; }
    const unsigned char* end() const { return m_finish; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };   // user empty ctor: no zero byte store for the temporary

struct back_insert_iterator {
    CArrayUChar* container;
    explicit back_insert_iterator(CArrayUChar& c) : container(&c) { }
};

inline back_insert_iterator back_inserter(CArrayUChar& c) { return back_insert_iterator(c); }

back_insert_iterator __fastcall __copy(const unsigned char* first, const unsigned char* last, back_insert_iterator result,
                                       const random_access_iterator_tag&, long*);   // 0x009E9900

inline back_insert_iterator copy(const unsigned char* first, const unsigned char* last, back_insert_iterator result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (long*)0);
}

struct CMemoryFile : CBaseClassNonCopyable {        // vtable 0x0122E0FC
    long          AccessType;                       // +0x04
    long          Position;                         // +0x08
    long          Length;                           // +0x0c
    CArrayUChar   DataBuffer;                       // +0x10
    bool          Opened;                           // +0x1c
    CWideString   Pathname;                         // +0x20
    virtual void v00();
    CMemoryFile(const CMemoryFile& other);
};

CMemoryFile::CMemoryFile(const CMemoryFile& other)
{
    AccessType = other.AccessType;
    Position = other.Position;
    Length = other.Length;
    Opened = other.Opened;
    Pathname = other.Pathname;
    copy(other.DataBuffer.begin(), other.DataBuffer.end(), back_inserter(DataBuffer));
}
