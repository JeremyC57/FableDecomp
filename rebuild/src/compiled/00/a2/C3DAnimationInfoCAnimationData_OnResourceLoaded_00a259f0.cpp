// C3DAnimationInfo::CAnimationData::OnResourceLoaded() -- when the owning bank's animation memory pool is shutting down, discard the pending file data and drop the animation info's reference to this data; otherwise build the shared animation
// Leaf: ?OnResourceLoaded@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A259F0, length 126 bytes (+ int3 padding), vtable slot 5 of
// C3DAnimationInfo::CAnimationData (0x0129CE50; slots 0/2/4 are the landed ??_G 0x00A2DB10, UnloadResource 0x00A25AE0 and
// IsResourceLoaded 0x00A23FC0). FableWin.pdb lists ?OnResourceLoaded@CAnimationData@C3DAnimationInfo@@UAEXXZ as the
// remaining virtual.
// Callees: CBankFileAsync::DiscardAsyncData(CCountedPointer<CBankFileAsyncData>&) @ 0x009D67E0 (on the info's bank, ecx kept
// from the flag test), the block's DeleteFunc (CCPPointerInfo +4), operator delete @ 0x00BFE9BC, the data's virtual slot 1
// (+4, its self-delete), CAnimationData::BuildSharedAnimation @ 0x00A24D70 (tail jump).
// Layout: as the landed CanUnload 0x00A25A70 -- FileData {object, info} at +0x30, AnimInfo at +0x38; the info's bank at +4
// (CMeshDataBank, AnimationMemoryPool.Data at +0x360) and its CIVCountedPointer<CAnimationData> Data at +0xc (Ego_r.pdb);
// CHandleBasedMemoryPool ShutdownFlag +0x25.
// Shape: FileData = 0 is the counted pointer's release (`--Info->RefCount` re-reading the member, GOTCHAS 24h) then Info and
// Data zeroed; AnimInfo->Data = 0 is the CIVCountedPointer operator=(T*) of landed 0x00A25AE0 (`add esi,0Ch` once, local
// pointer release, double null store).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(int)
    {
        Release();
        Info = 0;
        Data = 0;
        return *this;
    }
};

template<class T> struct CIVCountedPointer {
    T* m_pObject;
    void Release()
    {
        T* p = m_pObject;
        if (p) {
            if (--p->m_refCount == 0)
                p->DeleteThis();
            m_pObject = 0;
        }
    }
    CIVCountedPointer& operator=(T* object)
    {
        Release();
        m_pObject = object;
        return *this;
    }
};

class CBankFileAsyncData;
struct CHandleBasedMemoryPool {
    void*         __vftable;                                 // +0x00
    unsigned char _pad04[0x25 - 4];
    bool          ShutdownFlag;                              // +0x25
};
struct CBankFileAsync {
    void DiscardAsyncData(CCountedPointer<CBankFileAsyncData>& data);   // 0x009D67E0
};
struct CMeshDataBank : CBankFileAsync {
    unsigned char           _pad00[0x360];
    CHandleBasedMemoryPool* AnimationMemoryPool;             // +0x360 CCountedPointer<CMovableResourceMemoryPool>::Data
};

struct C3DAnimationInfo {
    class CAnimationData;
    void*                             __vftable;             // +0x00
    CMeshDataBank*                    PBank;                 // +0x04 the owning mesh data bank
    unsigned char                     _pad08[4];
    CIVCountedPointer<CAnimationData> Data;                  // +0x0c
};

class C3DAnimationInfo::CAnimationData {
public:
    virtual void v00();
    virtual void DeleteThis();                               // slot 1 (+4)
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void OnResourceLoaded();                         // slot 5
    long                                m_refCount;          // +0x04
    unsigned char                       _pad08[0x30 - 8];
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x30
    C3DAnimationInfo*                   AnimInfo;            // +0x38
    void BuildSharedAnimation();                             // 0x00A24D70
};

void C3DAnimationInfo::CAnimationData::OnResourceLoaded()
{
    CMeshDataBank* bank = AnimInfo->PBank;
    if (bank->AnimationMemoryPool->ShutdownFlag) {
        bank->DiscardAsyncData(FileData);
        FileData = 0;
        AnimInfo->Data = 0;
    } else {
        BuildSharedAnimation();
    }
}