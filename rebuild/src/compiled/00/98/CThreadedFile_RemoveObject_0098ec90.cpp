// CThreadedFile::RemoveObject(unsigned long id) -- forward through the global object
// Leaf: ?RemoveObject@CThreadedFile@@QAE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098EC90, 10 bytes: `mov ecx, offset 0x013BC870 ; jmp 0x0098EB20`.
// Shape (family K004, representative 0x0048573D): a forwarder whose only work is a member call on one global object, which
//   VC7.1 emits as the `this` load and a tail jump.
// Identity (proven): FableWin.pdb ?RemoveObject@CThreadedFile@@QAE_NK@Z and ?GetManager@CThreadedFile@@SIPAVCThreadedFileManager@@XZ; the jump target 0x0098EB20 is CThreadedFileManager::RemoveObject (locks, finds the id, frees an idle object and returns true, false while it is in flight); its callers, ~CBankFileAsyncData 0x009D61F0, spin on it with Sleep(0); the global 0x013BC870 is bound ?g_ThreadedFileManager@@3VCThreadedFileManager@@A (the label for 0x0098EC90 ?HasReadFinished@CThreadedFile@@ is a stand-in).
class CThreadedFileManager {
public:
    bool RemoveObject(unsigned long id);                 // 0x0098EB20
};
extern CThreadedFileManager g_ThreadedFileManager;       // 0x013BC870
class CThreadedFile {
public:
    static __forceinline CThreadedFileManager* __fastcall GetManager() { return &g_ThreadedFileManager; }
    bool RemoveObject(unsigned long id);
};
bool CThreadedFile::RemoveObject(unsigned long id)
{
    return GetManager()->RemoveObject(id);
}