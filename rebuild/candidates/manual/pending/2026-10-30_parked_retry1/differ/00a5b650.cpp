// Grade: DIFFER(186v184) (retry1) -- ebx scoped push now matches: the STLport ternary __copy_trivial (n computed in the arm) gives retail's push ebx/pop ebx inside the memmove arm, and the 0x009E9900 call is the ordinary __fastcall std::__copy<const uchar*,back_insert_iterator<CArray<uchar> >,int> (ecx = hidden result, edx = first; last, iterator, &tag, (int*)0) -- not a custom convention. Residue: retail homes the returned back_insert_iterator in the dead `other` argument slot together with the tag (lea ecx,[esp+10h] / [esp+1Ch]); ours opens a push ecx slot for it (3 displacements + push/pop ecx) | tried: named tag local, std::copy/back_inserter inline chain (worse, ebp) | next: find the source form that lets the result temp share the dead arg slot
// CCachedFile::Copy(const CCachedFile& other) -- assignment body shared by the copy ctor and Clone: position, flags, pathname, a fresh shared reference to the disk file (or release it), then the data buffer replaced by the other's bytes
// Leaf: Copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5B650, 192 bytes incl. padding (ret 4).
// Callees: CWideString::operator= @ 0x0099B7D0, CAFile slot 14 (+0x38, GetSharedFile), CCountedPointer<CAFile>::Reset
//          @ 0x0050F550, operator delete @ 0x00BFE9BC, memmove @ 0x00BFEAE6 (inline vector erase(begin, end)),
//          the range append @ 0x009E9900 (LTCG custom convention: ecx = result slot, edx = first; stack last, &vector,
//          &tag, 0 -- both temporaries homed in the dead `other` argument slot).
// Layout: the landed CCachedFile models (0x00A5B590/0x00A5B760/0x00A5B210): +4 Position, +8 BufferStartPosition,
//          +0xC DataBuffer {first, last, end}, +0x18 Pathname, +0x1C PDiskFile {object, count}, +0x24 Opened, +0x25 BufferValid.
#include <string.h>

struct CWideString {
    void* m_pStringData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct CAFile {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual CAFile* GetSharedFile();            // slot 14 (+0x38)
};

struct SCountBlock {
    long m_references;                          // +0
    void (__fastcall* m_pfnDestroy)(void*);     // +4
    void* m_pObject;                            // +8
};

struct CCountedPointer_CAFile {
    CAFile*      m_pObject;                     // +0
    SCountBlock* m_pCount;                      // +4
    void Reset(CAFile* file);                   // 0x0050F550
};

struct CByteArray;
namespace std {
struct random_access_iterator_tag { };
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
};
template <class InIt, class OutIt, class Dist>
OutIt __fastcall __copy(InIt first, InIt last, OutIt result, const random_access_iterator_tag&, Dist*);   // 0x009E9900
}
typedef std::back_insert_iterator<CByteArray> CBackInserter;

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result :
        ((char*)memmove(result, first, ((const char*)last - (const char*)first))) +
        ((const char*)last - (const char*)first);
}

struct CByteArray {
    unsigned char* m_pFirst;                    // +0
    unsigned char* m_pLast;                     // +4
    unsigned char* m_pEnd;                      // +8
    unsigned char* erase(unsigned char* first, unsigned char* last)
    {
        unsigned char* i = (unsigned char*)__copy_trivial(last, m_pLast, first);
        m_pLast = i;
        return first;
    }
    unsigned char* begin() { return m_pFirst; }
    unsigned char* end() { return m_pLast; }
    void clear() { erase(begin(), end()); }
};

struct CCachedFile {
    void*                  _vptr;               // +0x00
    long                   Position;            // +0x04
    long                   BufferStartPosition; // +0x08
    CByteArray             DataBuffer;          // +0x0C
    CWideString            Pathname;            // +0x18
    CCountedPointer_CAFile PDiskFile;           // +0x1C
    bool                   Opened;              // +0x24
    bool                   BufferValid;         // +0x25
    void Copy(const CCachedFile& other);
};

void CCachedFile::Copy(const CCachedFile& other)
{
    Position = other.Position;
    Opened = other.Opened;
    Pathname = other.Pathname;
    BufferStartPosition = other.BufferStartPosition;
    BufferValid = other.BufferValid;
    CAFile* file = other.PDiskFile.m_pObject;
    if (file) {
        PDiskFile.Reset(file->GetSharedFile());
    } else {
        if (PDiskFile.m_pCount) {
            --PDiskFile.m_pCount->m_references;
            if (PDiskFile.m_pCount->m_references == 0) {
                PDiskFile.m_pCount->m_pfnDestroy(PDiskFile.m_pCount->m_pObject);
                delete PDiskFile.m_pCount;
            }
        }
        PDiskFile.m_pCount = 0;
        PDiskFile.m_pObject = 0;
    }
    DataBuffer.clear();
    std::random_access_iterator_tag tag;
    std::__copy(other.DataBuffer.m_pFirst, other.DataBuffer.m_pLast, CBackInserter(DataBuffer), tag, (int*)0);
}
