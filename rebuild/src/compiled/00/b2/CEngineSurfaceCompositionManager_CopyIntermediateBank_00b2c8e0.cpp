// CEngineSurfaceCompositionManager::CopyIntermediateBank() -- close the completed bank, copy the intermediate disk-cache file over the completed one through a pool (or heap) staging buffer, reopen the completed bank read-only on the new file with the engine graphic bank's texture manager, and clear every cached surface's completed-bank index
// Leaf: CopyIntermediateBank
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members Count->RefCount, Destroy->DeleteFunc, Item->Data are the header CCPPointerInfo's (same offsets).
// Layout: local model; retail offsets: CachedSurfaces list +0x88 (STLport list = one node pointer), CompletedBankFile +0x398, disk-cache
//         file names +0x9b0 (intermediate) / +0x9b4 (completed); engine 0x01436E98: memory pool CCountedPointer +0x8e0, graphic bank +0x954
//         (its CCountedPointer<CTextureManager> TextureManager at +0x280); CBankFile virtual slots 3 OpenFileReadOnly, 4 OpenFileReadWrite,
//         5 Close (Ego_r.pdb).
// Shape: the pool retry loop stays top-tested (`test ebp,ebp ; jle` then `sub ; jmp` back) only when its condition is a
//        __forceinline early-return helper; a plain `while (size > 0)` / for(;;) / goto is rotated by VC7.1. The engine pool is read
//        through an inlined by-value counted-pointer copy (CEngine::GetLoadingMemoryPool 0x00B25C80); the engine bank's texture
//        manager comes from the by-value GetPTextureManager() (temp copy + release) into SetTextureManager 0x009FD870 (labelled
//        NUISystem::CFrontEndScreen::AddActionOnLeftClicked; really CGraphicDataBank::SetTextureManager(const CCountedPointer&)). engine/CEngineSurfaceCompositionManager.h carries PDB offsets (list 0xc bytes, banks 0x388) that
//        do not match retail, so `this` is modelled locally.
// NF46 (round 7): the 0x01436E98 stand-in CEngineData_01436e98 / g_pEngine_01436e98 (and its local GetLoadingMemoryPool helper) is
//   NEngineInternal::GPResourceManager (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h:
//   +0x8e0 through FableWin.pdb's inline GetLoadingMemoryPool (overlay opt-in _GETLOADINGMEMORYPOOL), +0x954 through GetGraphicBank (_GETGRAPHICBANK,
//   as same-TU 0x00B2D9F0). The pool is the header CMovableResourceMemoryPool (a CHandleBasedMemoryPool; Allocate / Free from the
//   CHandleBasedMemoryPool.h overlay opt-in _ALLOCATE, Lock / Unlock from _LOCK); the local CWideString / CBaseClassNonCopyable moved to their headers.
#include "fable_compiler.h"
#include <stdlib.h>
extern "C" double __fastcall GFGetTime();               // 0x009E1BC0
void __cdecl operator delete(void* p);                 // 0x00BFE9BC

#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETLOADINGMEMORYPOOL
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_LOCK
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_ALLOCATE
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CEngineResourceManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                         // vtable 0x0122D06C
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);
    virtual void Close();                               // 0x0099A920
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    virtual void Read(void*a0, long a1, CASuspendableProcess*a2);          // 0x0099A960
    void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
    long GetLength() const { return Length; }
};


struct CByteVector {
    unsigned char* first;
    unsigned char* last;
    unsigned char* end;
    CByteVector() : first(0), last(0), end(0) {}
    void insert(unsigned char*, unsigned long, const unsigned char&);   // 0x00485F3A
    unsigned long size() const { return (unsigned long)(last - first); }
    FABLE_FORCEINLINE void resize(unsigned long count, const unsigned char& value)
    {
        if (count < size())
            last = first + count;
        else
            insert(last, count - size(), value);
    }
    void resize(unsigned long count) { resize(count, (unsigned char)0); }
    ~CByteVector() { if (first) free(first); }
};

class CTextureManager;
class CGraphicDataBank { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool OpenFileReadOnly(const CWideString& fileName, unsigned long flags);   // slot 3 (CBankFile)
    virtual void OpenFileReadWrite(const CWideString& fileName, unsigned long create, unsigned long flags);   // slot 4
    virtual void Close();                               // slot 5
    void SetTextureManager(const CCountedPointer<CTextureManager>& manager);   // 0x009FD870
    CCountedPointer<CTextureManager> GetPTextureManager() { return TextureManager; }
    unsigned char _pad[0x27c];
    CCountedPointer<CTextureManager> TextureManager;    // +0x280
};

FABLE_FORCEINLINE bool IsPositive(long n)
{
    if (n <= 0)
        return false;
    return true;
}

class CCompositeSurfaceCache { public:
    unsigned char _pad[0x58];
    unsigned long BankFileIndex;                        // +0x58
};
struct SCacheNode {
    SCacheNode* next;
    SCacheNode* prev;
    CCountedPointer<CCompositeSurfaceCache> value;
};

class CEngineSurfaceCompositionManager { public:
    unsigned char    _pad[0x88];
    SCacheNode*      CachedSurfaces;                    // +0x88
    unsigned char    _pad8c[0x398 - 0x8c];
    CGraphicDataBank CompletedBankFile;                 // +0x398
    unsigned char    _padbank[0x9b0 - 0x398 - sizeof(CGraphicDataBank)];
    CWideString      IntermediateDiskCacheFileName;     // +0x9b0
    CWideString      CompletedDiskCacheFileName;        // +0x9b4
    void CopyIntermediateBank();
};

void CEngineSurfaceCompositionManager::CopyIntermediateBank()
{
    CompletedBankFile.Close();
    GFGetTime();
    {
        CDiskFileWin32 source(IntermediateDiskCacheFileName, FILE_ACCESS_READ, 2);
        CDiskFileWin32 dest(CompletedDiskCacheFileName, FILE_ACCESS_WRITE, 2);
        CMovableResourceMemoryPool* pool = NEngineInternal::GPResourceManager->GetLoadingMemoryPool().Get();
        long handle = -1;
        unsigned char* buffer = 0;
        long bufferSize = 0x100000;
        while (IsPositive(bufferSize)) {
            if (pool->Allocate(&handle, bufferSize, 0)) {
                pool->Lock(handle);
                buffer = (unsigned char*)pool->GetPointerFromHandle(handle);
                break;
            }
            bufferSize -= 0x10000;
        }
        CByteVector heapBuffer;
        if (!buffer) {
            bufferSize = 0x10000;
            heapBuffer.resize(bufferSize);
            buffer = heapBuffer.size() ? heapBuffer.first : (unsigned char*)&heapBuffer;
        }
        for (long remaining = source.GetLength(); remaining > 0; ) {
            long chunk = remaining < bufferSize ? remaining : bufferSize;
            source.Read(buffer, chunk, 0);
            dest.Write(buffer, chunk, 0);
            remaining -= chunk;
        }
        if (handle != -1) {
            pool->Unlock(handle);
            pool->Free(handle);
        }
    }
    GFGetTime();
    CompletedBankFile.OpenFileReadOnly(CompletedDiskCacheFileName, 0x48);
    CompletedBankFile.SetTextureManager(NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->GetPTextureManager());
    for (SCacheNode* it = CachedSurfaces->next; it != CachedSurfaces; it = it->next)
        it->value.Data->BankFileIndex = 0;
}