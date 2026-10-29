// CCachedFile::CCachedFile(const CCachedFile& other) -- copy constructor: CBase, own vtable, empty data buffer, empty pathname, null disk file, then Copy(other)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B760, length 59 bytes.
// Leaf: ??0CCachedFile
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CBaseClassNonCopyable::CBaseClassNonCopyable ; mov dword ptr [esi],offset CCachedFile::`vftable' ; xor edi,edi ;
//                mov [esi+0Ch],edi ; mov [esi+10h],edi ; lea ecx,[esi+18h] ; mov [esi+14h],edi ; call CWideString::CWideString ;
//                mov eax,[esp+0Ch] ; push eax ; mov ecx,esi ; mov [esi+1Ch],edi ; mov [esi+20h],edi ; call CCachedFile::Copy ;
//                pop edi ; mov eax,esi ; pop esi ; ret 4
// Manifest label "CRenderTarget::CRenderTarget" is propagated and wrong: vtable 0x012724D4 is CCachedFile
// (rtti-vtables.tsv), and this is the out-of-line copy constructor that the landed CCachedFile::Clone (0x00A5B710)
// inlines -- same model. Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::CWideString @ 0x0099AED0,
// CCachedFile::Copy @ 0x00A5B650.
// Layout (landed CCachedFile::CCachedFile @ 0x00A5B590): +4 Position, +8 BufferStartPosition (left to Copy), +0xC
// DataBuffer ({first,last} zeroed by its inline constructor) and +0x14 its capacity word, +0x18 Pathname, +0x1C
// PDiskFile (CCountedPointer<CAFile>, zeroed inline), +0x24 Opened / +0x25 BufferValid (left to Copy). The vtable
// store precedes the zero stores because the zeroed members are embedded objects with inline constructors.
// Shape: the CWideString `lea ecx,[esi+18h]` sits between the {first,last} stores and the end-of-storage store: the
// array's end word is constructed as its own inline-constructed part (a plain three-word array hoists the `lea`
// above all three stores; a plain capacity initialiser puts `xor edi,edi` before the vtable store).
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

class CAFile : public CBaseClassNonCopyable { public:
};

struct CByteArrayRange {                        // retail CArray<unsigned char>: {first, last} ...
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    CByteArrayRange() : m_pFirst(0), m_pLast(0) { }
};

struct CByteArrayEnd {                          // ... + the end-of-storage word, its own inline-constructed part
    unsigned char* m_pEnd;
    CByteArrayEnd() : m_pEnd(0) { }
};

struct CByteArray : CByteArrayRange {
    CByteArrayEnd m_end;
};

typedef unsigned short wchar_t;                 // VC7.1 without /Zc:wchar_t

class CWideString { public:
    wchar_t* m_pString;
    CWideString();                              // 0x0099AED0
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CCachedFile : public CAFile { public:
    virtual CAFile* Clone() const;              // own vtable (0x012724D4)
    long            Position;                   // +0x04
    long            BufferStartPosition;        // +0x08
    CByteArray      DataBuffer;                 // +0x0C
    CWideString     Pathname;                   // +0x18
    CCountedPointer PDiskFile;                  // +0x1C
    bool            Opened;                     // +0x24
    bool            BufferValid;                // +0x25
    CCachedFile(const CCachedFile& other);
    void Copy(const CCachedFile& other);        // 0x00A5B650
};

CCachedFile::CCachedFile(const CCachedFile& other)
    : CAFile(), DataBuffer(), Pathname(), PDiskFile()
{
    Copy(other);
}