// CBankFile::DeleteEntry(unsigned long index) -- forget an entry: reset its runtime record, delete its pending update data, clear the block order and bump the change count
// Leaf: DeleteEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CD3C0, length 184 bytes (+8 int3), ret 4.
// Callees: operator delete[] @ 0x00BFEB1C (the update's InfoBuffer), CSmallVector<CCharString,8>::Clear @ 0x009D2110,
// operator delete @ 0x00BFE9BC, memmove @ 0x00BFEAE6 (the inlined BlockOrder.clear()).
// Shape: `RuntimeData[index] = CRuntimeData()` -- CRuntimeData has a user ctor, so the temporary is built in the dead
// index-argument slot (two zero bytes, the third word copied from there); the update pointer's delete runs the inline
// destructor (InfoBuffer delete[] + zero, InfoSize = 0, Filenames cleared) and the slot is re-read for the zero store;
// BlockOrder.clear() is the erase(begin(), end()) copy-down helper of the landed vector resize 0x005697C0 (`cmp eax,eax`).
// Layout: the landed retail CBankFile model (0x009CC240): RuntimeData +0x20, UpdateData +0x2c, ChangeCount +0x90,
// BlockOrder +0xe0 (CArray<unsigned long>, PDB +0x100). Ego_r.pdb CBankFileEntryUpdateData: StateBlockCRC +0, InfoSize +4,
// InfoBuffer +8, Filenames +0xc, RequiresUpdate +0x14, Exists +0x15.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
void __cdecl operator delete(void* p);             // 0x00BFE9BC
void __cdecl operator delete[](void* p);           // 0x00BFEB1C

struct CRuntimeData {                              // CBankFile::CRuntimeData, 12 bytes
    unsigned long DataOffset;                      // +0x00
    unsigned long DataSize;                        // +0x04
    unsigned char DataType;                        // +0x08
    bool          Valid;                           // +0x09
    CRuntimeData() : DataOffset(0), DataSize(0), DataType(0), Valid(false) {}
};
struct CFilenameVector {                           // CSmallVector<CCharString,8>
    void* m_first;
    void* m_last;
    void Clear();                                  // 0x009D2110
    ~CFilenameVector() { Clear(); }
};
class CBankFileEntryUpdateData { public:
    unsigned long StateBlockCRC;                   // +0x00
    unsigned long InfoSize;                        // +0x04
    char* InfoBuffer;                              // +0x08
    CFilenameVector Filenames;                     // +0x0c
    bool RequiresUpdate;                           // +0x14
    bool Exists;                                   // +0x15
    ~CBankFileEntryUpdateData()
    {
        if (InfoBuffer) {
            delete[] InfoBuffer;
            InfoBuffer = 0;
        }
        InfoSize = 0;
    }
};
template <class T> class CArray { public:                 // retail CArray: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (unsigned long)((char*)srcEnd - (char*)src);
        return (T*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void clear() { m_pLast = CopyDown(m_pLast, m_pLast, m_pFirst); }
};

class CBankFile {
public:
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04
    unsigned char             Symbols[0x0c];        // +0x08 CArray<CCharString>
    unsigned char             Checksums[0x0c];      // +0x14 CArray<unsigned long>
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    unsigned char             _pad38[0x58];         // +0x38 .. +0x8f (packed arrays, flags; see 0x009CC240)
    unsigned long             ChangeCount;          // +0x90
    unsigned char             _pad94[0x4c];         // +0x94 .. +0xdf
    CArray<unsigned long>     BlockOrder;           // +0xe0
    void DeleteEntry(unsigned long index);
};

void CBankFile::DeleteEntry(unsigned long index)
{
    RuntimeData[index] = CRuntimeData();
    if (index < UpdateData.size()) {
        CBankFileEntryUpdateData* update = UpdateData[index];
        if (update)
            delete update;
        UpdateData[index] = 0;
    }
    BlockOrder.clear();
    ++ChangeCount;
}