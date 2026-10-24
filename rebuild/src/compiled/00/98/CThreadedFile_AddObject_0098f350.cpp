// CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc) -- queue a load request for this file with the global threaded file manager and return the new object UID
// Leaf: ?AddObject@CThreadedFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098F350, length 19 bytes (+ int3 padding), thiscall `ret 4`.
// Callee: CThreadedFileManager::AddObject(const CThreadedFileObjectDesc&, CThreadedFile*) @ 0x0098F0F0 (landed, `ret 8`)
//         on g_ThreadedFileManager @ 0x013BC870.
// Callers: CBankFileAsync::RestartReads 0x009D5A90 and CBankFileAsyncEntry::SetupRead 0x009D5970 (landed; they declare
// this body as CThreadedFile::AddObject / QueueRead) and 0x009D5650.
// Shape: arguments pushed right to left -- `this` (ecx) first, then the descriptor -- and the manager's result passes
// straight through eax.
// Label correction: the catalog's `_Cons_val<allocator<pair<unsigned long,CAnimationEntry>>>` is propagated.
struct CThreadedFileObjectDesc { unsigned char _body[0x18]; };   // 24 bytes (landed 0x0098F0F0 layout)
class CThreadedFile;
class CThreadedFileManager {
public:
    unsigned long AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile* file);   // 0x0098F0F0
};
extern CThreadedFileManager g_ThreadedFileManager;    // 0x013BC870

class CThreadedFile {
public:
    void* m_vtable;                                    // +0x00
    long  DeviceID;                                    // +0x04
    unsigned long AddObject(const CThreadedFileObjectDesc& desc);
};

unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc)
{
    return g_ThreadedFileManager.AddObject(desc, this);
}