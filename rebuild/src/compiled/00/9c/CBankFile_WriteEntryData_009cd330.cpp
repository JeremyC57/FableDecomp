// CBankFile::WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding) -- make the bank file writable, seek to the entry's position, copy `size` bytes from the source stream into it through a file output stream, then write the zero padding
// Leaf: WriteEntryData
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CD330, 144 bytes, __fastcall, ret 10h. Called from 0x009D15BB.
// The catalog label (CBankFile::MakeWritable) names the inlined first part (the landed MakeWritable() 0x009CBF90).
// Callees: CDiskFile virtual slots 7 (GetPosition), 2 (Close), 1 (Open), 5 (Seek); CFileDataOutputStream::
// CFileDataOutputStream(CAFile*) 0x00994400 and its destructor 0x00994430 (12 bytes: vptr, name string, file);
// the source stream's copy-out 0x009964E0 (labelled CDataInputStream::ReadWithChunking; __fastcall on the
// source with (output stream, size)); CBankFile::WriteZeros 0x009CCB20 (the zero padding).
// Layout: the retail CBankFile layout of the landed rows: BankFile_Data +0x7C, Filename +0x84, WrittenToFile +0x8E.
// Shape: MakeWritable() inlined, then the seek through the counted pointer (re-read), the output stream as a
// 12-byte local built on the disk file, the copy, the padding (out of line) and the stream's destructor.
// No exception frame: the TU is built without /EHsc.
struct CWideString;

struct CDiskFileCalls {
    virtual void v00();
    virtual void Open(const CWideString& filename, unsigned long mode, unsigned long share);   // slot 1 (+0x04)
    virtual void Close();                                                                       // slot 2 (+0x08)
    virtual void v03();
    virtual void Write(const void* data, unsigned long size, unsigned long flags);              // slot 4 (+0x10)
    virtual void Seek(unsigned long position);                                                  // slot 5 (+0x14)
    virtual void v06();
    virtual unsigned long GetPosition();                                                        // slot 7 (+0x1c)
};

class CFileDataOutputStream {
public:
    void* m_pVtable;                                // +0x00
    void* m_name;                                   // +0x04 CCharString
    CDiskFileCalls* m_pFile;                        // +0x08
    CFileDataOutputStream(CDiskFileCalls* file);    // 0x00994400
    ~CFileDataOutputStream();                       // 0x00994430
};

class CDataInputStream {
public:
    void CopyTo(CFileDataOutputStream& out, unsigned long size);   // 0x009964E0
};

struct CBankFile_Retail {                        // the retail layout; engine/CBankFile.h (PDB) disagrees with the bytes
    void*                     __vftable;            // +0x00
    unsigned long             Size;                 // +0x04
    unsigned char             _arrays[0x70];        // +0x08 Symbols .. PackedValid
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c CCountedPointer<CDiskFileWin32>::Data
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84 CWideString
    void*                     BankHandle_PStringData; // +0x88 CCharString
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    void MakeWritable()                             // 0x009CBF90 (landed)
    {
        if (!WrittenToFile) {
            unsigned long position = BankFile_Data->GetPosition();
            BankFile_Data->Close();
            BankFile_Data->Open(*(const CWideString*)&Filename_PStringData, 2, 4);
            BankFile_Data->Seek(position);
            WrittenToFile = true;
        }
    }
    void WriteZeros(unsigned long count);           // 0x009CCB20
    void WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding);
};

void CBankFile_Retail::WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding)
{
    MakeWritable();
    BankFile_Data->Seek(position);
    CFileDataOutputStream out(BankFile_Data);
    source.CopyTo(out, size);
    WriteZeros(padding);
}