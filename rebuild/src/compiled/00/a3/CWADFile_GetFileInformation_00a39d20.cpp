// CWADFile::GetFileInformation() -- the WAD entry's file information (creation/access/write times, read-only flag) read from the bank entry's info buffer
// Leaf: ?GetFileInformation@CWADFile@@UAE?AVCFileInformation@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39D20, 85 bytes (+11 int3 padding), thiscall, `ret 4` (hidden result pointer).
//   CWADFile vtable 0x0129CF8C slot 15; the PDB name set lists ?GetFileInformation@CWADFile@@UAE?AVCFileInformation@@XZ.
// Callees: CDateAndTime::CDateAndTime @ 0x009E9AB0 (zeroes the seven words), CBankFileEntryUpdateData::ReadInfo
//   (unsigned long offset, void* dest, unsigned long size) const @ 0x009CBE90 (landed; its header names this caller).
// Layout: Ego_r.pdb CFileInformation (0x58) = three CDateAndTime (0x1c each) + ReadOnly; the bank's UpdateData CArray
//   sits at +0x2c in retail (12-byte CArrays, as the landed CBankFile::FindEmptyBlockOffset 0x009D0EF0 model). The
//   CWADFile model is the landed local polymorphic one (engine/CWADFile.h flattens the vptr).
// Shape: VC7.1 has no NRVO: the local is built by the implicit constructor (three out-of-line CDateAndTime calls,
//   ReadOnly left uninitialised), filled by ReadInfo(0, &info, 0x58), then copied out by the trivial copy constructor
//   (`mov ecx,16h ; rep movsd`).
class CDateAndTime {
public:
    CDateAndTime();                         // 0x009E9AB0
    long          Year;                     // +0x00
    unsigned long Month;                    // +0x04
    unsigned long Day;                      // +0x08
    unsigned long Hour;                     // +0x0c
    unsigned long Minute;                   // +0x10
    unsigned long Second;                   // +0x14
    unsigned long Millisecond;              // +0x18
};

class CFileInformation {
public:
    CDateAndTime CreationTime;              // +0x00
    CDateAndTime LastAccessTime;            // +0x1c
    CDateAndTime LastWriteTime;             // +0x38
    bool         ReadOnly;                  // +0x54
};

class CBankFileEntryUpdateData {
public:
    void ReadInfo(unsigned long offset, void* dest, unsigned long size) const;   // 0x009CBE90
};

template <class T> class CArray {           // retail CArray: 12 bytes
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CBankFileAsync {                      // CBankFile base part as far as the update data (retail offsets)
public:
    void*         __vftable;                // +0x00
    unsigned long Size;                     // +0x04
    unsigned char Symbols[0x0c];            // +0x08 CArray<CCharString>
    unsigned char Checksums[0x0c];          // +0x14 CArray<unsigned long>
    unsigned char RuntimeData[0x0c];        // +0x20 CArray<CBankFile::CRuntimeData>
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
};

class CBase {                               // the engine's root polymorphic base
public:
    virtual ~CBase();
    CBase();                                // 0x0099A2F0
};

class CWADFile : public CBase {             // vtable 0x0129CF8C (rtti-vtables.tsv); sizeof 0x10
public:
    CBankFileAsync* Bank;                   // +0x04
    unsigned long   Index;                  // +0x08
    long            FilePos;                // +0x0c
    virtual CFileInformation GetFileInformation();   // slot 15
};

CFileInformation CWADFile::GetFileInformation()
{
    CFileInformation info;
    Bank->UpdateData[Index]->ReadInfo(0, &info, sizeof(info));
    return info;
}