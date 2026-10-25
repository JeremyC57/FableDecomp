// NGameText::CDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data) -- while the text bank is being built (read-only bank file), grows the entry table to cover `index` and records the new entry: type 0 a text entry (narrator index from the data), type 1 a group entry, type 2 the narrator list (a count then that many null-terminated names read from the data); then the base notification
// Grade: DIFFER(469v469) -- register choice only: retail builds each pushed address in ecx right before `lea ecx,<this>` (text/group push_back, the ReadLong slow path) and keeps the narrator value in eax until the entry stores after the push; ours uses eax for those addresses, stores NarratorIndex before the push, and the following scratch registers (edx/eax v ecx/edx) rotate with it
// Leaf: ?NotifyEntryAdded@CDataBank@NGameText@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C8C00, 469 bytes (padded to 480), `ret 0xc`; vtable 0x0129B4D4 slot 12.
//   Ego_r.pdb ?NotifyEntryAdded@CDataBank@NGameText@@MAEXKKABV?$CArray@D@@@Z.
// Callees: vector<CDataBankEntry*>::_M_insert_overflow @ 0x009CA950 (the inline push_back's slow path),
//   CArray<CDataBankTextEntry>::push_back @ 0x009CB740, CArray<CDataBankGroupEntry>::push_back @ 0x009CB790,
//   CMemoryDataInputStream(const void*, long) @ 0x00994930, CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the
//   inline ReadLong's slow path), CArray<CCharString>::resize(size_type) @ 0x0049B760, CMemoryDataInputStream::
//   ReadNullTerminatedString @ 0x00995D30 (landed; direct call on the local), CCharString::operator= @ 0x0099EFB0,
//   ~CCharString @ 0x0099EAE0, ~CMemoryDataInputStream @ 0x00993B10 (manifest "~CChunkedFileChunk" is propagated),
//   CBankFile::NotifyEntryAdded @ 0x009CBC20 (an empty `ret 0Ch`).
// Layout: CBankFile ReadOnly +0x8d (landed CBankFile rows); Ego_r.pdb CDataBank with 12-byte retail CArrays (landed ctor
//   0x009C85A0): Entries +0x1e0, TextEntryBuffer +0x1ec, GroupEntryBuffer +0x1f8, Narrators +0x204; CDataBankTextEntry
//   {PText, NarratorIndex} (8 bytes), CDataBankGroupEntry {PGroup} (4 bytes).
// Shape: STLport push_back inline for the pointer table (placement copy with its null test); the stream's ReadLong is the
//   landed inline CMemoryDataInputStream::Read; CArray::GetData is `size() ? first : this`.
#include "engine/CMemoryDataInputStream.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class CCharString {
public:
    void* PData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                      // 0x0099EAE0
};
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __true_type { __true_type() { } };   // a user constructor keeps the tag store-free
}
template <class T> class CArray {                        // retail 12-byte STLport vector
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T* GetData() const { return size() ? _M_start : (const T*)this; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    T& back() { return *(_M_finish - 1); }
    void _M_insert_overflow(T* pos, const T& x, const std::__true_type&, unsigned int fillLen, bool atEnd);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, std::__true_type(), 1, true);
    }
};

struct CMemoryDataInputStreamReader : CMemoryDataInputStream {
    CMemoryDataInputStreamReader(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStreamReader();                              // 0x00993B10
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    CCharString ReadNullTerminatedString();                       // 0x00995D30
    __forceinline void Read(void* out, long size)
    {
        if (size > 0) {
            __int64 next = (__int64)(unsigned)(position + size);
            if (next < 0 || next > 0x7fffffff) return;
            if (size <= chunkLength) { memcpy(out, (const void*)chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
            else ReadWithSrcChunkOverflow((unsigned char*)out, size);
        }
    }
    __forceinline unsigned long ReadLong() { unsigned long v; Read(&v, 4); return v; }
};

class CBankFile {
public:
    virtual void v00();
    unsigned char _pad04[0x8d - 0x04];
    bool ReadOnly;                                        // +0x8d
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);   // 0x009CBC20
};

namespace NGameText {
struct CDataBankEntry { };
}
// Same-TU stand-in for the out-of-line overflow (the real body is 0x009CA950): it never reads the tag.
#pragma auto_inline(off)
template <> __declspec(noinline) void CArray<NGameText::CDataBankEntry*>::_M_insert_overflow(NGameText::CDataBankEntry** pos,
    NGameText::CDataBankEntry* const& x, const std::__true_type&, unsigned int fillLen, bool atEnd)
{
    for (; fillLen != 0; --fillLen)
        *pos++ = x;
    if (atEnd)
        _M_finish = pos;
}
#pragma auto_inline(on)
namespace NGameText {
struct CTextEntry;
struct CGroupEntry;
template <class T> struct CIVCountedPointer {
    T* Data;
    CIVCountedPointer() : Data(0) { }
};
struct CDataBankTextEntry : CDataBankEntry {
    CIVCountedPointer<CTextEntry> PText;                  // +0x00
    long NarratorIndex;                                   // +0x04
    CDataBankTextEntry(long narratorIndex) : PText(), NarratorIndex(narratorIndex) { }
};
struct CDataBankGroupEntry : CDataBankEntry {
    CIVCountedPointer<CGroupEntry> PGroup;                // +0x00
};
template <class T> class CEntryBuffer : public CArray<T> {
public:
    void push_back(const T& x);                           // 0x009CB740 / 0x009CB790
};
class CNarratorArray : public CArray<CCharString> {
public:
    void resize(unsigned int newSize);                    // 0x0049B760
};

class CDataBank : public CBankFile {
public:
    unsigned char _pad90[0x1e0 - 0x90];
    CArray<CDataBankEntry*>            Entries;           // +0x1e0
    CEntryBuffer<CDataBankTextEntry>   TextEntryBuffer;   // +0x1ec
    CEntryBuffer<CDataBankGroupEntry>  GroupEntryBuffer;  // +0x1f8
    CNarratorArray                     Narrators;         // +0x204
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data);
};

void CDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    if (ReadOnly) {
        while (index >= Entries.size())
            Entries.push_back(0);
        if (type == 0) {
            TextEntryBuffer.push_back(CDataBankTextEntry(*(const long*)data.GetData()));
            Entries[index] = &TextEntryBuffer.back();
        }
        else if (type == 1) {
            GroupEntryBuffer.push_back(CDataBankGroupEntry());
            Entries[index] = &GroupEntryBuffer.back();
        }
        else if (type == 2) {
            CMemoryDataInputStreamReader stream(data.GetData(), data.size());
            unsigned long count = stream.ReadLong();
            Narrators.resize(count);
            for (unsigned long i = 0; i < count; ++i) {
                CCharString* name = Narrators._M_start;
                name[i] = stream.ReadNullTerminatedString();
            }
        }
        CBankFile::NotifyEntryAdded(type, index, data);
    }
}
}
