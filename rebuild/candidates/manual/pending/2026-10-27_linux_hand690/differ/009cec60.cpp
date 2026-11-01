// CBankFile::Update() -- rescan every live entry's source files against the checksum cache, flag the changed ones, then rebuild each flagged entry (status text, progress, periodic file-information writes), turning a CBankCreatorException into one naming the entry; aborts throw CBankCreatorAbortException
// Grade: DIFFER(1840v1684) (true extent 0x73A: DIFFER(1840v1850)) -- residue: the oracle row stops at the catch funclet 0x009CF2F4 but the function runs to ret 0x009CF399 (funclet + continuation inline, as LoadAllEventPackages); against the true bytes the residue is scratch-register rotation in the first scan loop (eax/ecx/edx shifted one), GetEntryDataType storing per branch instead of after the join, and the second loop dropping retail's redundant `cmp edi,eax` in the inlined EntryExists (15 probes). next: re-cut the row to 1850 bytes, then the scan-loop register rotation.
// Leaf: ?Update@CBankFile@@UAEXXZ
extern const unsigned long g_crc32Table[256];           // 0x0129A168
extern bool g_BankCreationAborted;                      // 0x013CA821 (set by CBankFile::AbortBankCreation)

class CWideStringData;
class CCharStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);          // 0x0099BE70
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);       // 0x0099BF30
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CWideString ToWideString() const;                   // 0x0099E480
    CCharString ToLower() const;                        // 0x0099ED10
};
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);          // 0x0099F570
CCharString __fastcall operator+(const CCharString& a, const char* b);                 // 0x0099F600

class CAFile { public:
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext);   // 0x00997620
    static void __fastcall SplitPath(const CCharString& path, CCharString* drive, CCharString* dir, CCharString* name, CCharString* ext);   // 0x009976D0
};
class CBankCreatorException { public:
    CCharString EntrySymbol;                            // +0x00
    CCharString ErrorText;                              // +0x04
    CBankCreatorException(const CCharString& entry_symbol, const CCharString& error_text);   // 0x009CB8F0
    CBankCreatorException(const CBankCreatorException& other);                             // 0x009D47E0
    ~CBankCreatorException();                                                              // 0x009D47C0
    CCharString GetText() const;                                                           // 0x009CB930
};
class CBankCreatorAbortException { public: };

class CProgressCallback { public:
    virtual void SetProgress(unsigned long done, unsigned long total);   // slot 0
    virtual void SetStatus(const CCharString& text);                     // slot 1
    virtual void WriteLog(const CCharString& text);                      // slot 2
};
class CChecksumCacheFile { public:
    unsigned long _pad00[6];
    void Open(const CWideString& path);                 // 0x00A60D40
    void Close();                                       // 0x00A60C30
    unsigned long GetChecksum(const CWideString& path); // 0x00A60FB0
};
class CBankFileEntryUpdateData { public:
    unsigned long StateBlockCRC;                       // +0x00
    unsigned long InfoSize;                            // +0x04
    char*         InfoBuffer;                          // +0x08
    CCharString*  Filenames_Start;                     // +0x0c CSmallVector<CCharString,8>
    signed char   Filenames_Size;                      // +0x10
    unsigned char _pad11[3];
    bool          RequiresUpdate;                      // +0x14
    bool          Exists;                              // +0x15
};
struct CRuntimeData {                                   // CBankFile::CRuntimeData, 12 bytes
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
template <class T> class CArray { public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    struct iterator {
        T* m_p;
        iterator(T* p) : m_p(p) {}
        iterator operator+(unsigned long n) const { return iterator(m_p + n); }
        T& operator*() const { return *m_p; }
    };
    iterator begin() const { return iterator(m_pFirst); }
    bool IsEmpty() const { return m_pFirst == m_pLast; }
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};

class CBankFile { public:
    unsigned long             Size;                 // +0x04
    CArray<CCharString>       Symbols;              // +0x08
    CArray<unsigned long>     Checksums;            // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    CPackedUIntArray          PackedDataOffset;     // +0x38
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned long             OpenFlags;            // +0x78
    void*                     BankFile_Data;        // +0x7c
    void*                     BankFile_Info;        // +0x80
    CWideString               Filename;             // +0x84
    CCharString               BankHandle;           // +0x88
    bool                      RetailMode;           // +0x8c
    bool                      ReadOnly;             // +0x8d
    bool                      WrittenToFile;        // +0x8e
    bool                      Solid;                // +0x8f
    unsigned long             ChangeCount;          // +0x90
    bool                      FileValid;            // +0x94
    bool                      UpdatingFlag;         // +0x95
    unsigned long             Alignment;            // +0x98
    unsigned char             _pad9c[0xec - 0x9c];
    CProgressCallback*        ProgressCallback;     // +0xec
    CCharString               HeaderEnumerationType; // +0xf0
    bool                      WarningFlag;          // +0xf4
    CChecksumCacheFile        ChecksumCache;        // +0xf8

    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04();
    virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08();
    virtual unsigned long AdjustUpdateEntries();                            // slot 9 (+0x24)
    virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14();
    virtual void s15(); virtual void s16();
    virtual bool IsDiskFile(unsigned long type, unsigned long fileIndex);   // slot 17 (+0x44)
    virtual void Update();

    long GetSize() const { return Size; }
    bool EntryExists(unsigned long index) const
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked())
                return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData[index].DataType;
    }
    CCharString GetEntrySymbol(unsigned long index) const               // 0x009CC1D0
    {
        if (Symbols.IsEmpty())
            return CCharString("(Unknown)", -1);
        return CCharString(*(Symbols.begin() + index));
    }
    unsigned long CountEntries() const;                                 // 0x009CC3B0
    void DeleteEntry(unsigned long index);                              // 0x009CD3C0
    void BeginUpdateEntries();                                          // 0x009CE8D0
    void EndUpdateEntries();                                            // 0x009CEA70
    void WriteFileInformation(bool final);                              // 0x009CE370
    void SetBankUpdateProgress(unsigned long done, unsigned long total)
    {
        if (ProgressCallback)
            ProgressCallback->SetProgress(done, total);
    }
    void SetBankUpdateStatus(const CCharString& text)                   // 0x009CBD30
    {
        if (ProgressCallback)
            ProgressCallback->SetStatus(text);
    }
    void WriteBankLog(const CCharString& text)
    {
        if (ProgressCallback)
            ProgressCallback->WriteLog(text);
    }
};

void CBankFile::Update()
{
    CWideString drive, dir, name, ext;
    CAFile::SplitPath(Filename, &drive, &dir, &name, &ext);
    ChecksumCache.Open(drive + dir + name + L".cks");
    {
        CCharString scanning("Scanning files...", -1);
        SetBankUpdateStatus(scanning);
    }
    unsigned long count = CountEntries();
    SetBankUpdateProgress(0, count);
    unsigned long updated = 0;
    unsigned long scanned = 0;
    for (unsigned long i = 0; i < Size; ++i) {
        if (RuntimeData[i].Valid) {
        CCharString symbol = GetEntrySymbol(i);
        CBankFileEntryUpdateData* update = UpdateData[i];
        if (!update->Exists) {
            DeleteEntry(i);
        } else {
            unsigned long type = GetEntryDataType(i);
            if (!update->RequiresUpdate) {
                unsigned long checksum = 0;
                for (unsigned long j = 0; j < (unsigned long)update->Filenames_Size; ++j) {
                    if (IsDiskFile(type, j)) {
                        CCharString filename = update->Filenames_Start[j];
                        unsigned long crc = ChecksumCache.GetChecksum(filename.ToWideString());
                        const unsigned char* bytes = (const unsigned char*)&crc;
                        for (unsigned long b = 0; b < sizeof(crc); ++b)
                            checksum = g_crc32Table[bytes[b] ^ (checksum & 0xff)] ^ (checksum >> 8);
                    }
                }
                if (checksum != Checksums[i]) {
                    WriteBankLog(CCharString("Source file has been modified for ", -1) + GetEntrySymbol(i));
                    update->RequiresUpdate = true;
                }
            }
            if (update->RequiresUpdate)
                ++updated;
            ++scanned;
            SetBankUpdateProgress(scanned, count);
            if (g_BankCreationAborted)
                { CBankCreatorAbortException abort; throw abort; }
        }
        }
    }
    if (FileValid && ChangeCount == 0 && updated == 0 && GetSize() > 0) {
        ChecksumCache.Close();
        return;
    }
    updated += AdjustUpdateEntries();
    BeginUpdateEntries();
    SetBankUpdateProgress(0, updated);
    unsigned long done = 0;
    unsigned long batch = 0;
    for (unsigned long k = 0; k < Size; ++k) {
        if (!EntryExists(k))
            continue;
        CBankFileEntryUpdateData* update = UpdateData[k];
        if (update->RequiresUpdate) {
            unsigned long type = GetEntryDataType(k);
            try {
                CCharString status;
                if (IsDiskFile(type, 0)) {
                    CCharString fdrive, fdir, fname, fext;
                    CAFile::SplitPath(update->Filenames_Start[0], &fdrive, &fdir, &fname, &fext);
                    status = CCharString("Reading ", -1) + fname + fext + "\n in " + (fdrive + fdir).ToLower();
                } else {
                    status = CCharString("Processing ", -1) + update->Filenames_Start[0] + "...";
                }
                SetBankUpdateStatus(status);
            }
            catch (CBankCreatorException& e) {
                CCharString symbol = GetEntrySymbol(k);
                DeleteEntry(k);
                EndUpdateEntries();
                throw CBankCreatorException(symbol, e.GetText());
            }
            ++done;
            if (++batch == 10 && !RetailMode) {
                WriteFileInformation(false);
                batch = 0;
            }
            SetBankUpdateProgress(done, updated);
        }
        if (g_BankCreationAborted) {
            EndUpdateEntries();
            { CBankCreatorAbortException abort; throw abort; }
        }
    }
    EndUpdateEntries();
    ChecksumCache.Close();
}
