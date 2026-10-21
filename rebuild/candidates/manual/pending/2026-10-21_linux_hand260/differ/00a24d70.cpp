// Grade: DIFFER(413v414) -- retail caches the bank (edi) and decoder (ebx) before PeekData and passes a different hidden-result slot to Decode 0x00A99510 (frame accounting unresolved)
// CAnimationData::BuildSharedAnimation() -- wait for the bank data, build the shared C3DAnimation2 from it (freeing resources and retrying on failure), then drop the file data
// Layout: retail CAnimationData is 4 below the PDB after Animations (STLport map 12 bytes): SharedAnimation +0x28, FileData +0x30, AnimInfo +0x38.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall WaitTrace(const CCharString& message, bool b, float time, long flags);   // 0x009E9F40

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
template<class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};

struct CAnimationFileData;
struct CRenderDevice { void FreeAnimationMemory(); };     // 0x00A02730
struct CResourceList { void* PHead; void MakeFreeSpace(bool all); void Remove(void* item); };   // 0x009FC210 / 0x009FC380
struct CAnimationDecoder { CCountedPointer<CAnimationFileData> Decode(const void* data); };   // 0x00A99510
struct CMeshDataBank {
    char _pad[0x168];
    CAnimationDecoder* PDecoder;          // +0x168
    char _pad16c[0x268 - 0x16c];
    CResourceList PendingBuilds;          // +0x268
    CResourceList Resources;              // +0x26C
    char _pad270[0x360 - 0x270];
    CRenderDevice* PDevice;               // +0x360
    unsigned long GetEntryFlags(unsigned long index);   // 0x009CC210
};
struct C3DAnimationInfo { void* __vftable; CMeshDataBank* Bank; unsigned long Index; };
struct CBankFileAsyncData {
    char _pad[0x10];
    long Handle;                          // +0x10
    bool IsLoaded();                      // 0x009D54E0
    void WaitForLoad();                   // 0x009D5B60
    const void* PeekData(long handle, CRenderDevice* device);   // 0x009D5530
};
struct C3DAnimation2 {
    char _d[0x7c];
    C3DAnimation2();                      // 0x00A4EE10
    void SetDevice(CRenderDevice* device);   // 0x00A4BE00
    bool Load(CAnimationFileData* data, long quality);   // 0x00A4FE60
};
struct CAnimationPointer : CCountedPointer<C3DAnimation2> {
    CAnimationPointer& operator=(C3DAnimation2* object);   // 0x00A29C50
};

class CAnimationData {
public:
    char _pad[0x28];
    CAnimationPointer SharedAnimation;                       // +0x28
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x30
    C3DAnimationInfo* AnimInfo;                              // +0x38
    void BuildSharedAnimation();
};

void CAnimationData::BuildSharedAnimation()
{
    if (!FileData.m_pObject->IsLoaded()) {
        {
            CCharString message("Asset Wait: Animation");
            WaitTrace(message, false, -1.0f, 0);
        }
        FileData.m_pObject->WaitForLoad();
    }
    CMeshDataBank* bank = AnimInfo->Bank;
    CCountedPointer<CAnimationFileData> data = bank->PDecoder->Decode(FileData.m_pObject->PeekData(FileData.m_pObject->Handle, bank->PDevice));
    SharedAnimation = new C3DAnimation2();
    SharedAnimation.m_pObject->SetDevice(bank->PDevice);
    long quality = 0;
    unsigned long format = AnimInfo->Bank->GetEntryFlags(AnimInfo->Index) & 0xf;
    if (format == 10)
        quality = 4;
    else if (format == 7 || format == 8)
        quality = 5;
    else if (format == 9)
        quality = 3;
    for (long attempt = 0; !SharedAnimation.m_pObject->Load(data.m_pObject, quality); attempt++) {
        if (attempt == 0)
            AnimInfo->Bank->Resources.MakeFreeSpace(false);
        else if (attempt == 1)
            AnimInfo->Bank->PDevice->FreeAnimationMemory();
        else
            break;
    }
    FileData.Reset();
    AnimInfo->Bank->PendingBuilds.Remove(this);
}
