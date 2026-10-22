// CTextEntry::ReadFromDataStream(CDataInputStream* in) -- read the text entry's identifier, wide string text and per-entry fields from the data stream
// Grade: DIFFER(459v452) -- reads and calls right; ours keeps temporaries in the dead param slot, caches the count and CSEs the index address where retail reloads
// Leaf: ReadFromDataStream
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
};
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CDataInputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CCharString ReadNullTerminatedString();         // slot 6 (+0x18)
    virtual CWideString ReadNullTerminatedWideString();     // slot 7 (+0x1c)
    unsigned long StreamPos;                                // +0x04
    long StreamSize;                                        // +0x08
    const unsigned char* CurrentSourcePtr;                  // +0x0c
    unsigned long SourceChunkStreamPos;                     // +0x10
    long SourceChunkBytesRemaining;                         // +0x14
    CCharString ReadPresizedString();                       // 0x00996390
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
extern long g_MemoryCategoryA;                              // 0x0138DE48
extern long g_MemoryCategoryB;                              // 0x0138E064
struct CMemoryCategoryScope {
    long OldA;
    long OldB;
    __forceinline CMemoryCategoryScope(long category) : OldA(g_MemoryCategoryA), OldB(g_MemoryCategoryB)
    { g_MemoryCategoryA = category; g_MemoryCategoryB = category; }
    __forceinline ~CMemoryCategoryScope() { g_MemoryCategoryA = OldA; g_MemoryCategoryB = OldB; }
};
namespace NGameText {
struct CTextTag {
    long        Position;
    CCharString Name;
};
}
template <class T> struct CArray {
    T*   Data;
    long Size;
    long Capacity;
    long Pad;
    void resize(long size);                                 // 0x009CB7D0
    T& operator[](long i) { return Data[i]; }
};
struct CResource {
    void SetResourceSize(long size);                        // 0x009FBF20
};
struct CTextEntry : CResource {
    void* __vftable;
    unsigned char _pad04[0x18];
    CCharString SpeechBank;                                 // +0x1c
    CCharString Narrator;                                   // +0x20
    CCharString SoundName;                                  // +0x24
    CWideString Text;                                       // +0x28
    CArray<NGameText::CTextTag> Tags;                       // +0x2c
    long Sizeof() const;                                    // 0x009C7D90
    void ReadFromDataStream(CDataInputStream* in);
};

void CTextEntry::ReadFromDataStream(CDataInputStream* in)
{
    CMemoryCategoryScope scope(11);
    Text = in->ReadNullTerminatedWideString();
    SpeechBank = in->ReadPresizedString();
    Narrator = in->ReadPresizedString();
    SoundName = in->ReadPresizedString();
    long count;
    in->Read(&count, sizeof(count));
    Tags.resize(count);
    for (long i = 0; i < count; ++i) {
        long position;
        in->Read(&position, sizeof(position));
        Tags[i].Position = position;
        Tags[i].Name = in->ReadNullTerminatedString();
    }
    SetResourceSize(Sizeof());
}
