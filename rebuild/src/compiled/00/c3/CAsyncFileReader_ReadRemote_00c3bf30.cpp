// CAsyncFileReader::ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags, IUserCallback* callback) -- libpk PKCFile.cpp remote read: a block-aligned request goes straight to the reader's block read (slot 1); otherwise a master object is created for the callback and the request is split into block runs -- an unaligned buffer as one padded script-info load, else a loop of slave loads with a padded head / short tail as script-info loads -- and the master is finalised
// Leaf: ?ReadRemote@CAsyncFileReader@@QAE_NKKPAXKPAUIUserCallback@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3BF30, 1063 bytes, thiscall `ret 14h`, EH-framed (the CRACBuffer, state 0; the
//   three trace lines, states 1-3). Callers 0x00C32038, 0x00C320C8 (the landed CRemoteLoader::LoadDataFromRemote binds
//   this spelling), 0x00C4A794, 0x00C4A894, 0x00C4A96F.
// Identity: libpk (no PDB, no RTTI): the trace lines name `.\PKCFile.cpp` (lines 28 "EMEM", 87 "Cannot remote read if we
//   have padding!", 88 "The bytes size (n) was not aligned to m"); descriptive names; the catalog's
//   "CBankFile_ReadRemote" is a propagated guess.
// Callees: the reader's slot 1 (block read) and slot 2 (block size); the PKRACBuffer.cpp buffer CRACBuffer: ctor
//   0x00C4C080, dtor 0x00C4BF90, IsAligned 0x00C4C030 / IsRangeAligned 0x00C4C050 / IsAligned(bytes) 0x00C4BFA0,
//   CheckAlignmentOffset 0x00C4C1D0, GetSize 0x00C4C020, GetPointer 0x00C4C000, GetHeadPadding 0x00C4C010, GetBlockCount
//   0x00C4C2D0, GetFirstBlock 0x00C4C2C0, GetAlignedSize 0x00C4BFF0, GetAlignment 0x00C4BFC0; the master-object factory
//   0x00C4CC90 (`__fastcall`: callback ecx, flags edx) and its LoadScriptInfo 0x00C4C8B0, LoadSlave 0x00C4CAB0, Finalize
//   0x00C4CD80; the trace string 0x00C0DEC0 / 0x00C0DF00 / 0x00C0DF10 / ~CString 0x00C0BA40, CDebugBreak 0x00C16750.
extern char* g_pchNil;                                          // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;                           // 0x0143BB2C

class CString {                                                 // libpk's MFC-style string (one word)
public:
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);           // 0x00C0DEC0
    CString& __fastcall operator<<(long value);                 // 0x00C0DF00
    CString& __fastcall operator<<(unsigned long value);        // 0x00C0DF10
    char* m_pchData;
};
struct CDebugBreak {
    CDebugBreak();                                              // 0x00C16750
};

class CRACBuffer {                                              // PKRACBuffer.cpp: a byte range seen as device blocks (0x18 bytes)
public:
    CRACBuffer(unsigned char* base, unsigned long size, unsigned long offset, unsigned long alignment);   // 0x00C4C080
    ~CRACBuffer();                                              // 0x00C4BF90
    bool IsAligned() const;                                     // 0x00C4C030
    bool IsRangeAligned() const;                                // 0x00C4C050
    bool IsAligned(unsigned long bytes);                        // 0x00C4BFA0
    unsigned long CheckAlignmentOffset(unsigned long bytes);    // 0x00C4C1D0 (bytes to blocks)
    unsigned long GetSize();                                    // 0x00C4C020
    unsigned char* GetPointer(unsigned long offset) const;      // 0x00C4C000
    unsigned long GetHeadPadding();                             // 0x00C4C010
    unsigned long GetBlockCount();                              // 0x00C4C2D0
    unsigned long GetFirstBlock();                              // 0x00C4C2C0
    unsigned long GetAlignedSize(unsigned long count) const;    // 0x00C4BFF0 (blocks to bytes)
    unsigned long GetAlignment();                               // 0x00C4BFC0
    unsigned char* m_pBase;                                     // +0x00
    unsigned long  m_size;                                      // +0x04
    unsigned long  m_offset;                                    // +0x08
    unsigned long  m_alignment;                                 // +0x0c
    unsigned long  m_headPadding;                               // +0x10
    unsigned long  m_tailPadding;                               // +0x14
};

struct IUserCallback;
struct CAsyncFileReader;
class CMasterObject_00c4c8b0 {                                  // PKRACMasterObject.cpp
public:
    void LoadScriptInfo(CAsyncFileReader* reader, unsigned long block, unsigned long blocks, unsigned long padding, unsigned char* dest, unsigned long size);   // 0x00C4C8B0
    void LoadSlave(CAsyncFileReader* reader, unsigned long block, unsigned long blocks, unsigned char* dest);   // 0x00C4CAB0
    bool Finalize();                                            // 0x00C4CD80
};
CMasterObject_00c4c8b0* __fastcall CreateConsoleVariable_00c4cc90(IUserCallback* callback, unsigned long flags);   // 0x00C4CC90

struct CAsyncFileReader {
    virtual void v00();
    virtual bool ReadBlocks(unsigned long block, unsigned long blocks, unsigned long size, void* dest, unsigned long flags, IUserCallback* callback);   // slot 1
    virtual unsigned long GetBlockSize();                       // slot 2
    bool ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags, IUserCallback* callback);
};

bool CAsyncFileReader::ReadRemote(unsigned long offset, unsigned long size, void* dest, unsigned long flags, IUserCallback* callback)
{
    if (size == 0)
        return false;
    CRACBuffer buffer((unsigned char*)dest, size, offset, GetBlockSize());
    if (buffer.IsAligned() && buffer.IsRangeAligned())
        return ReadBlocks(buffer.CheckAlignmentOffset(offset), buffer.CheckAlignmentOffset(size), size, dest, flags, callback);
    CMasterObject_00c4c8b0* master = CreateConsoleVariable_00c4cc90(callback, flags);
    if (!master) {
        {
            CString line;
            line << ".\\PKCFile.cpp" << "(" << 28L << ") : " << "EMEM" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    if (!buffer.IsAligned()) {
        master->LoadScriptInfo(this, buffer.GetFirstBlock(), buffer.GetBlockCount(), buffer.GetHeadPadding(), buffer.GetPointer(0), buffer.GetSize());
    } else {
        unsigned long blocksLeft = buffer.GetBlockCount();
        unsigned long bytesLeft = buffer.GetSize();
        unsigned long padding = buffer.GetHeadPadding();
        unsigned long block = buffer.GetFirstBlock();
        unsigned char* out = buffer.GetPointer(0);
        while (blocksLeft != 0) {
            unsigned long blocks = blocksLeft;
            bool padded = false;
            unsigned long bytes = bytesLeft + padding;
            if (padding != 0) {
                blocks = 1;
                padded = true;
            }
            if (!buffer.IsAligned(bytes)) {
                if (!padded && blocks > 1) {
                    --blocks;
                } else {
                    blocks = 1;
                    padded = true;
                }
            }
            if (bytes > buffer.GetAlignedSize(blocks))
                bytes = buffer.GetAlignedSize(blocks);
            if (padded) {
                master->LoadScriptInfo(this, block, blocks, padding, out, bytes - padding);
            } else {
                if (padding != 0) {
                    {
                        CString line;
                        line << ".\\PKCFile.cpp" << "(" << 87L << ") : " << "Cannot remote read if we have padding!" << "\n";
                        if (g_pfnAssertHook)
                            g_pfnAssertHook(line.m_pchData);
                    }
                    CDebugBreak();
                }
                if (bytes % buffer.GetAlignment() != 0) {
                    {
                        CString line;
                        line << ".\\PKCFile.cpp" << "(" << 88L << ") : " << "The bytes size (" << bytes << ") was not aligned to " << buffer.GetAlignment() << "\n";
                        if (g_pfnAssertHook)
                            g_pfnAssertHook(line.m_pchData);
                    }
                    CDebugBreak();
                }
                master->LoadSlave(this, block, blocks, out);
            }
            blocksLeft -= blocks;
            bytesLeft -= bytes - padding;
            out += bytes - padding;
            block += blocks;
            padding = 0;
        }
    }
    return master->Finalize();
}