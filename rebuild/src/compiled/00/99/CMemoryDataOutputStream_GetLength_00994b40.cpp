// CMemoryDataOutputStream::GetLength() const -- the number of bytes written: Data.Size - Data.Start of the CArray<char> buffer
// Leaf: ?GetLength@CMemoryDataOutputStream@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994B40, 7 bytes: mov eax, [ecx+0xc] ; sub eax, [ecx+8] ; ret.
// Identity (DECISIONS_W17_RESOLVED 0x00994B40, chunk c681): the data-stream block, Ego_r.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ;
//   the CMemoryDataOutputStream callers (0x004FBDE0, 0x00523B40, 0x006F92A0, 0x009B08C0, 0x009D4F90, 0x00A382E0, 0x00AAD150,
//   0x00AC3B00, 0x00ADFDC0 ...) bind it. Layout: Ego_r CMemoryDataOutputStream {CDataOutputStream base 8, CArray<char> Data +0x8
//   (Start +0x8, Size +0xc, ...), CurrentPosition +0x18}.
// Naming wave 18: landed as CCreatureActionBase::GetFramesRemaining (a shape label); respelled on the PDB member (name-only: the
//   bytes are unchanged; the body is an ICF-shared shape, this address is the stream's).
class CDataOutputStream { public:
    virtual ~CDataOutputStream();                    // slot 0 (vtable +0)
    unsigned char _pad04[4];                         // +0x04
};
template <class T> class CArray { public:
    T*            Start;                             // +0x00
    unsigned long Size;                              // +0x04
    unsigned char _pad08[4];                         // +0x08
};
class CMemoryDataOutputStream : public CDataOutputStream { public:
    CArray<char>  Data;                              // +0x08
    unsigned long CurrentPosition;                   // +0x18
    unsigned long GetLength() const;
};

unsigned long CMemoryDataOutputStream::GetLength() const
{
    return Data.Size - (unsigned long)Data.Start;
}
