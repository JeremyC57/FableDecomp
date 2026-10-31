// CBankFile::WriteFileInformation(bool writeEntryList) -- make the bank writable, rewrite the "BBBB" file header at offset 0: format
// Grade: DIFFER(624v620) -- class key CBankFile (was CBankFile_Retail: the leaf matched no symbol) with private
//   WriteFileInformation/GetRandomValidationValue (AAE); residue: retail gives the strength-reduced RuntimeData[i]
//   offset temp its own zeroed slot [esp+0x28] in a 0x2c frame, ours seeds it from i in the lowest word of a 0x28 frame
//   version, data-format version, validation word (1 when the entry list follows), alignment, highest live index, live count and
//   the aligned end of entry data; record the header size, pad to the alignment, then either write the entry list after the data
//   or (non-retail banks) the recovery file
// Leaf: ?WriteFileInformation@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CE370, 620 bytes, ret 4; PDB ?WriteFileInformation@CBankFile@@AAEX_N@Z. Flags: /GS
//   (cookie 0x0139C8A8, check 0x00BFE9F9; char header[5] from "BBBB" 0x0129B5A4).
// Callees: CFileDataOutputStream ctor 0x00994400 / dtor 0x00994430 / SetPosition 0x00994450 / Write 0x00994480 / GetPosition
//   0x00994460, CDataOutputStream WriteULong 0x00993EA0, CPackedUIntArray::operator[] 0x00A629F0, CBankFile::GetRandomValidationValue
//   0x009CC0B0 (thiscall: PDB AAEKXZ), WritePadding 0x009CCB20, WriteRecoveryFile 0x009CDF50, InternalWriteEntryList 0x009CDA70,
//   vtable slot 8 GetDataFormatVersion (pure in CBankFile: vtable 0x0129B54C slot 8 = _purecall 0x00BFEA08), the data file's slots
//   7/2/1/5 (inline MakeWritable 0x009CBF90).
// Layout: the landed CBankFile_Retail view (CBankFile::Align 0x009CBB90): retail CArrays are 12 bytes, so from +0x38 every member
//   sits 0x10 below Ego_r.pdb (engine/CBankFile.h): Alignment +0x98, BankFormatVersion +0x9c, HeaderSize +0xa0.
// Tried: local declaration permutations (6), locals before the stream, a const-ref / by-value record local, signed index, nested
//   ifs, a named update pointer, an inline HasWrittenData helper, header in its own block: all 622v620 or worse.
struct CDiskFileCalls;
struct CWideString;
struct CAFile;
struct CDataOutputStream;
struct CBankFileEntryUpdateData {
    unsigned long StateBlockCRC;                   // +0x00
    unsigned long InfoSize;                        // +0x04
    char*         InfoBuffer;                      // +0x08
    unsigned char Filenames[8];                    // +0x0c CSmallVector<CCharString,8>
    bool          RequiresUpdate;                  // +0x14
    bool          Exists;                          // +0x15
};
struct CRuntimeData {
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool          Valid;
    unsigned char _pad[2];
};
struct CPackedUIntArray {
    unsigned long  m_bitsPerValue;
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned long  m_capacity;
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CDiskFileCalls {
    virtual void v00();
    virtual void Open(const CWideString& filename, unsigned long mode, unsigned long share);   // slot 1 (+0x04)
    virtual void Close();                                                                       // slot 2 (+0x08)
    virtual void v03();
    virtual void v04();
    virtual void Seek(unsigned long position);                                                  // slot 5 (+0x14)
    virtual void v06();
    virtual unsigned long GetPosition();                                                        // slot 7 (+0x1c)
};
struct CFileDataOutputStream {                     // 12 bytes
    void*   __vftable;
    CAFile* File;
    unsigned long _pos;
    explicit CFileDataOutputStream(CAFile* file);  // 0x00994400
    ~CFileDataOutputStream();                      // 0x00994430
    void SetPosition(unsigned long position);      // 0x00994450
    void Write(const void* data, long size);       // 0x00994480
    void WriteULong(unsigned long value);          // 0x00993EA0 (CDataOutputStream)
    unsigned long GetPosition() const;             // 0x00994460
};
template <class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
class CBankFile {
public:
                                                    // +0x00 vptr
    unsigned long             Size;                 // +0x04
    unsigned char             Symbols[0x0c];        // +0x08
    unsigned char             Checksums[0x0c];      // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    CPackedUIntArray          PackedDataOffset;     // +0x38
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned long             OpenFlags;            // +0x78
    CDiskFileCalls*           BankFile_Data;        // +0x7c
    void*                     BankFile_Info;        // +0x80
    void*                     Filename_PStringData; // +0x84
    void*                     BankHandle_PStringData; // +0x88
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long             ChangeCount;          // +0x90
    bool                      FileValid;            // +0x94
    bool                      UpdatingFlag;         // +0x95
    unsigned long             Alignment;            // +0x98
    unsigned long             BankFormatVersion;    // +0x9c
    unsigned long             HeaderSize;           // +0xa0
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual unsigned long GetDataFormatVersion();   // slot 8 (+0x20), pure in CBankFile
    __forceinline void MakeWritable()               // 0x009CBF90 (landed)
    {
        if (!WrittenToFile) {
            unsigned long position = BankFile_Data->GetPosition();
            BankFile_Data->Close();
            BankFile_Data->Open(*(const CWideString*)&Filename_PStringData, 2, 4);
            BankFile_Data->Seek(position);
            WrittenToFile = true;
        }
    }
    unsigned long GetEntryDataOffset(unsigned long index) const   // 0x009CC270 (landed)
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    unsigned long GetEntryDataSize(unsigned long index) const     // 0x009CC240 (landed)
    {
        if (PackedDataSize.IsPacked())
            return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
private:
    unsigned long GetRandomValidationValue();       // 0x009CC0B0
    void WritePadding(unsigned long count);         // 0x009CCB20
    void WriteRecoveryFile(unsigned long validation, unsigned long maxIndex, unsigned long count);   // 0x009CDF50
    unsigned long InternalWriteEntryList(CFileDataOutputStream& stream);   // 0x009CDA70
    void WriteFileInformation(bool writeEntryList);
};

void CBankFile::WriteFileInformation(bool writeEntryList)
{
    MakeWritable();
    CFileDataOutputStream stream((CAFile*)BankFile_Data);
    stream.SetPosition(0);
    char header[] = "BBBB";
    stream.Write(header, 4);
    unsigned long maxIndex = 0;
    unsigned long count = 0;
    unsigned long maxEnd = HeaderSize;
    for (unsigned long i = 0; i < UpdateData.GetCount(); ++i) {
        if (RuntimeData[i].Valid && RuntimeData[i].DataOffset && !UpdateData[i]->RequiresUpdate) {
            ++count;
            unsigned long end = GetEntryDataOffset(i) + GetEntryDataSize(i);
            maxEnd = Max(maxEnd, end);
            if (maxIndex < i)
                maxIndex = i;
        }
    }
    unsigned long fileSize = (maxEnd + Alignment - 1) / Alignment * Alignment;
    unsigned long validation = GetRandomValidationValue();
    unsigned long version = GetDataFormatVersion();
    stream.WriteULong(BankFormatVersion);
    stream.WriteULong(version);
    stream.WriteULong(writeEntryList ? 1 : validation);
    stream.WriteULong(Alignment);
    stream.WriteULong(maxIndex);
    stream.WriteULong(count);
    stream.WriteULong(fileSize);
    HeaderSize = stream.GetPosition();
    WritePadding((HeaderSize + Alignment - 1) / Alignment * Alignment - HeaderSize);
    if (writeEntryList) {
        stream.SetPosition(fileSize);
        InternalWriteEntryList(stream);
    } else if (!RetailMode)
        WriteRecoveryFile(validation, maxIndex, count);
}
