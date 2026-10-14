// C3DMeshLODInfo::OnResourceLoadAborted() -- discard the pending async file data, reset its counted pointer, remove this LOD from the owning mesh info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A254A0, length 80 bytes.
// Callees: 0x009D68A0 DiscardAsyncData(CCountedPointer<CBankFileAsyncData>&) (__fastcall), 0x00BFE9BC operator delete, 0x00A249A0 C3DMeshInfo::RemoveLOD(unsigned long, bool), CCPPointerInfo::DeleteFunc through the block
// The counted-pointer Reset() is inlined on the member object (esi = &FileData): it re-reads the block pointer
// after the decrement, calls the block's delete function on its Data, frees the block with an untested operator
// delete and nulls Info then Data.
//
#include "rebuild_abi.h"

struct CCPPointerInfo {                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
template<class T> struct CCountedPointer {
    T*              m_pData;                             // PDB: Data
    CCPPointerInfo* m_pInfo;                             // PDB: Info
    bool IsValid() const { return m_pData != 0; }
    unsigned long GetRefCount() const { unsigned long count = 0; if (m_pInfo) count = m_pInfo->RefCount; return count; }
    bool IsUnique() const { return GetRefCount() == 1; }
    void Reset()
    {
        if (m_pInfo) {
            --m_pInfo->RefCount;
            if (m_pInfo->RefCount == 0) {
                m_pInfo->DeleteFunc(m_pInfo->Data);
                delete m_pInfo;
            }
        }
        m_pInfo = 0;
        m_pData = 0;
    }
};
struct C3DMeshInfo {
    void RemoveLOD(unsigned long lod, bool keepStats);   // 0x00A249A0
};
struct C3DMesh2; struct CBankFileAsyncData;
// Local model of rebuild/include/engine/C3DMeshLODInfo.h (same offsets): the header flattens the two counted
// pointers into Data/Info fields, and this body is the pointer's own inline member function on that object.
struct C3DMeshLODInfo {
    unsigned char                       _resource[0x1c];   // CResource base
    CCountedPointer<C3DMesh2>           Mesh;              // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;          // +0x24
    C3DMeshInfo*                        MeshInfo;          // +0x2c
    unsigned char                       LOD;               // +0x30
    bool                                UsedMeshStats;     // +0x31   sizeof == 0x34
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshLODInfo) == 0x34);

void __fastcall DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& fileData);   // 0x009D68A0
struct C3DMeshLODInfo_Methods : C3DMeshLODInfo {
    void OnResourceLoadAborted();
};
void C3DMeshLODInfo_Methods::OnResourceLoadAborted()
{
    DiscardAsyncData(FileData);
    FileData.Reset();
    MeshInfo->RemoveLOD(LOD, false);
}