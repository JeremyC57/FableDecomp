// C3DAnimationInfo::CAnimationData::BuildSharedAnimation() -- wait for the bank file data, decode it into an anim file, build the shared C3DAnimation2 in the animation memory pool (recovering animation memory and retrying on failure), drop the file data and add the data to the animation resource bank
// Grade: DIFFER(414v414) -- 7 masked diff lines (was 39): retail loads AnimInfo->Bank into ecx before AnimInfo->Index (mov eax,[eax+8]; push eax), ours loads the index first through ecx (same as the parked residue)
// Leaf: ?BuildSharedAnimation@CAnimationData@C3DAnimationInfo@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A24D70, 414 bytes, plain `ret`.
// Lever (chunk 615): full PDB respell -- the owner is the nested C3DAnimationInfo::CAnimationData (CResource), the decode is
//   C3DAnimFileLoader::LoadAnimFileCompressed(data, length, CHandleBasedMemoryPool&) with CBankFileAsyncData::GetData() /
//   GetLength() (the parked PeekData(handle, device) call was a wrong shape), the retry is C3DAnimationInfo::RecoverAnimationMemory
//   (ShrinkResources(0) on the animation resource list, then CHandleBasedMemoryPool::Defragment), the release is ReleaseData and
//   the tail is CResourceBank::AddResource(*this). Tried for the residue: GetDataType() on the entry (same), GetBank/GetIndex
//   accessors (worse), the CMeshDataBank::GetAnimationType inline (worse: index before bank plus a jump table), a const info local,
//   the switch in the body, the type initialised after the call (all same or worse).
// Callees: CBankFileAsyncData::IsFinished() const 0x009D54E0 / WaitUntilFinished() 0x009D5B60 / GetData() 0x009D5530,
//   NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) 0x009E9F40 (fastcall), CCharString 0x0099EBF0 /
//   0x0099EAE0, C3DAnimFileLoader::LoadAnimFileCompressed 0x00A99510, operator new 0x00BFEA1A, C3DAnimation2::C3DAnimation2
//   0x00A4EE10 / SetMemoryPool 0x00A4BE00 / InitialiseSharedAnimation(const C3DAnimFile&, EAnimationType) 0x00A4FE60,
//   CCountedPointer<C3DAnimation2>::Reset(C3DAnimation2*) 0x00A29C50 (ICF-shared body), CBankFile::GetEntryDataType(unsigned long)
//   const 0x009CC210, CResourceList::ShrinkResources(unsigned long) 0x009FC210 (donor order + void/ulong shape),
//   CHandleBasedMemoryPool::Defragment 0x00A02730, CResourceBank::AddResource(CResource&) 0x009FC380 (body links the resource
//   into ResourceList at +4).
// Layout: retail CAnimationData (CResource 0x1c, Animations map 0xc, SharedAnimation +0x28, FileData +0x30, AnimInfo +0x38);
//   CMeshDataBank AnimLoader +0x168, AnimationResourceBank +0x268, AnimationMemoryPool +0x360 (engine/CMeshDataBank.h retail
//   offsets; that header is padding-only so the members are modelled locally); CMeshDataBankEntry Bank +4, Index +8 (Ego_r).
//   CCountedPointer is modelled locally so Reset(T*) stays an out-of-line call (engine/CCountedPointer.h defines it inline).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& message, float progress, bool a, bool b);   // 0x009E9F40
}
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    void Reset(T* data);
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
template <class T> class CIVCountedPointer { public: T* Object; };
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    long IVRefCount;
};
class CResourceList;
class CResource : public CIVCountedPointeeBase { public:
    CResourceList* ResourceList;                      // +0x08
    CResource* PrevResource;                          // +0x0c
    CResource* NextResource;                          // +0x10
    unsigned long ResourceSize;                       // +0x14
    unsigned long LastUsedFrame;                      // +0x18
};
class CResourceList { public:
    void ShrinkResources(unsigned long size);         // 0x009FC210
    unsigned char _body[0x3c];
};
class CResourceBank { public:
    void* __vftable;
    CResourceList ResourceList;                       // +0x04
    CResourceList LoadingQueue;                       // +0x40
    void AddResource(CResource& resource);            // 0x009FC380
    CResourceList& GetResourceList() { return ResourceList; }
};
class CHandleBasedMemoryPool { public:
    void Defragment();                                // 0x00A02730
};
class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { };
class C3DAnimFile;
enum EAnimationType { ANIMATION_TYPE_0 = 0, ANIMATION_TYPE_3 = 3, ANIMATION_TYPE_4 = 4, ANIMATION_TYPE_5 = 5 };   // descriptive enumerator names
class C3DAnimFileLoader { public:
    CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void* data, unsigned long length, CHandleBasedMemoryPool& pool);   // 0x00A99510
};
class C3DAnimFileXLoader : public C3DAnimFileLoader { };
class CBankFile { public:
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210
};
class CMeshDataBank : public CBankFile {
public:
    unsigned char _pad[0x168];
    C3DAnimFileXLoader* AnimLoader;                   // +0x168 scoped_ptr<C3DAnimFileXLoader>
    unsigned char _pad16c[0x268 - 0x16c];
    CResourceBank AnimationResourceBank;              // +0x268
    unsigned char _pad2e4[0x360 - 0x2e4];
    CCountedPointer<CMovableResourceMemoryPool> AnimationMemoryPool;   // +0x360
    CMovableResourceMemoryPool* GetAnimationMemoryPool() { return AnimationMemoryPool.Get(); }
    CResourceBank& GetAnimationResourceBank() { return AnimationResourceBank; }
};
class CMeshDataBankEntry { public:
    virtual void v00();
    CMeshDataBank* Bank;                              // +0x04
    unsigned long Index;                              // +0x08
};
class CBankFileAsyncData { public:
    unsigned char _pad[0x10];
    unsigned long Length;                             // +0x10
    bool IsFinished() const;                          // 0x009D54E0
    bool WaitUntilFinished();                         // 0x009D5B60
    void* GetData();                                  // 0x009D5530
    unsigned long GetLength() const { return Length; }
};
class C3DAnimation2 { public:
    unsigned char _d[0x7c];
    C3DAnimation2();                                  // 0x00A4EE10
    void SetMemoryPool(CMovableResourceMemoryPool* pool);   // 0x00A4BE00
    bool InitialiseSharedAnimation(const C3DAnimFile& file, EAnimationType type);   // 0x00A4FE60
};
class C3DAnimationInfo : public CMeshDataBankEntry {
public:
    class CAnimationData : public CResource {
    public:
        unsigned char Animations[0xc];                // +0x1c map<unsigned long, CCountedPointer<C3DAnimation2> >
        CCountedPointer<C3DAnimation2> SharedAnimation;   // +0x28
        CCountedPointer<CBankFileAsyncData> FileData;     // +0x30
        C3DAnimationInfo* AnimInfo;                       // +0x38
        void BuildSharedAnimation();
    };
private:
    C3DAnimFileXLoader& GetAnimLoader() const { return *Bank->AnimLoader; }
    bool RecoverAnimationMemory(unsigned long attempt) const
    {
        switch (attempt) {
        case 0:
            Bank->GetAnimationResourceBank().GetResourceList().ShrinkResources(0);
            return true;
        case 1:
            Bank->GetAnimationMemoryPool()->Defragment();
            return true;
        }
        return false;
    }
};

void C3DAnimationInfo::CAnimationData::BuildSharedAnimation()
{
    if (!FileData->IsFinished()) {
        {
            CCharString message("Asset Wait: Animation");
            NProgressDisplay::DisplayProgress(message, -1.0f, false, false);
        }
        FileData->WaitUntilFinished();
    }
    CMeshDataBank* bank = AnimInfo->Bank;
    C3DAnimFileXLoader* loader = bank->AnimLoader;
    CCountedPointer<C3DAnimFile> animFile = loader->LoadAnimFileCompressed(FileData->GetData(), FileData->GetLength(), *bank->GetAnimationMemoryPool());
    SharedAnimation.Reset(new C3DAnimation2());
    SharedAnimation->SetMemoryPool(bank->GetAnimationMemoryPool());
    EAnimationType type = ANIMATION_TYPE_0;
    unsigned long dataType = AnimInfo->Bank->GetEntryDataType(AnimInfo->Index) & 0xf;
    if (dataType == 10)
        type = ANIMATION_TYPE_4;
    else if (dataType == 7 || dataType == 8)
        type = ANIMATION_TYPE_5;
    else if (dataType == 9)
        type = ANIMATION_TYPE_3;
    for (unsigned long attempt = 0; !SharedAnimation->InitialiseSharedAnimation(*animFile, type); ++attempt)
        if (!AnimInfo->RecoverAnimationMemory(attempt))
            break;
    FileData.ReleaseData();
    AnimInfo->Bank->GetAnimationResourceBank().AddResource(*this);
}
