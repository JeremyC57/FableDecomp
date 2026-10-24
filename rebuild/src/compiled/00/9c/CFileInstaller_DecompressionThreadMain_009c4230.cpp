// CFileInstaller::DecompressionThreadMain() -- decompression thread loop: until told to exit, decompress the current write file while it has input and a buffer is free, otherwise wait on the decompression event
// Leaf: ?DecompressionThreadMain@CFileInstaller@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C4230, 155 bytes, plain `ret` (thiscall, aligned `and esp,-8` frame).
// Identity: FableWin.pdb ?DecompressionThreadMain@CFileInstaller@@IAEXXZ (entered from BeginDecompressionThread);
//   catalogued as `CFileInstaller_ProcessLoop`.
// Callees: CCriticalSectionLock ctor 0x00A62920 (the section, false) / Release 0x00A62960 / dtor 0x00A629A0,
//   CCompressorZlib::HasStreamInput() const @ 0x00A3A7E0 (`Stream.next_in != 0`; propagated label C3DMeshInfo::HasPhysicsMesh),
//   CFileInstaller::ProcessActiveFileForDecompression(CActiveFile&) @ 0x009C3C20, KERNEL32 WaitForSingleObjectEx (IAT 0x0143FE08).
// Layout: retail CFileInstaller CriticalSection +0x04, AvailableBuffers +0x30, PCurrentWriteFile +0x54,
//   DecompressionThreadFinished +0x79, ExitThread +0x7A, DecompressionEvent +0x88; retail CActiveFile Decompressor +0x38,
//   PendingDecompression (STLport list<CActiveRead*>, 4 bytes) +0x94.
// Shape: the decision is a bool cleared after the lock (`xor bl,bl`) and set by one `&&` chain; the lock is released
//   before acting and destroyed at the end of each pass.  The 8-byte lock whose address is taken inside the loop gives
//   VC7.1 3077's `and esp,-8` frame.
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void* handle, unsigned long milliseconds, int alertable);   // IAT 0x0143FE08

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };   // Win32 critical section, 24 bytes

class CCriticalSectionLock {                    // 8 bytes on the stack (Ego_r.pdb)
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    void Release();                                                       // 0x00A62960
    _RTL_CRITICAL_SECTION* CriticalSection;     // +0x00
    bool Locked;                                // +0x04
};

class CCompressorZlib {                         // 0x58 bytes
public:
    bool HasStreamInput() const;                // 0x00A3A7E0
private:
    unsigned char _body[0x58];
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> class list {               // STLport list: one sentinel pointer
public:
    bool empty() const { return _M_node->_M_next == _M_node; }
private:
    _List_node_base* _M_node;
};
}

class CFileInstaller {
public:
    class CActiveRead;
    class CActiveFile {
    public:
        unsigned char             _pad00[0x38];
        CCompressorZlib           Decompressor;           // +0x38
        _STL::list<void*>         PendingWrites;          // +0x90
        _STL::list<CActiveRead*>  PendingDecompression;   // +0x94
    };
protected:
    void ProcessActiveFileForDecompression(CActiveFile& file);   // 0x009C3C20
    void DecompressionThreadMain();
private:
    void*                 _padding_;            // +0x00
    _RTL_CRITICAL_SECTION CriticalSection;      // +0x04
    unsigned char         _pad1c[0x14];         // +0x1C BufferChunks, PReadBuffer, PWriteBuffer
    long                  AvailableBuffers;     // +0x30
    unsigned char         _pad34[0x20];         // +0x34 AllocatedInputBuffers .. PCurrentReadFile
    CActiveFile*          PCurrentWriteFile;    // +0x54
    unsigned char         _pad58[0x21];         // +0x58 NoActiveReads .. ThreadFinished
    bool                  DecompressionThreadFinished;   // +0x79
    bool                  ExitThread;           // +0x7A
    unsigned char         _pad7b[0x0D];         // +0x7B Running .. Event
    void*                 DecompressionEvent;   // +0x88
};

void CFileInstaller::DecompressionThreadMain()
{
    while (!ExitThread) {
        CCriticalSectionLock lock(CriticalSection, false);
        bool process = false;
        CActiveFile* file = PCurrentWriteFile;
        if (file && (file->Decompressor.HasStreamInput() || !file->PendingDecompression.empty()) && AvailableBuffers > 0)
            process = true;
        lock.Release();
        if (process)
            ProcessActiveFileForDecompression(*PCurrentWriteFile);
        else
            WaitForSingleObjectEx(DecompressionEvent, 100, 1);
    }
    DecompressionThreadFinished = true;
}