// Grade: DIFFER(953v993) -- register/frame allocation: retail hoists zero into ebp (push ebp / cmp reg,ebp / mov [x],ebp) and keeps the set iterator in a frame word (+0x18, frame 0x28); ours keeps the iterator in ebp and uses immediates for zero (frame 0x20)
// CEngineSurfaceCompositionManager::PreloadCompletedBank() -- collect the completed-bank entries of every cached surface used within 100 frames of the most recently used one, then repeatedly request and stream their frames (top mip) for up to MaxPreloadTime seconds until none is still pending
// Leaf: PreloadCompletedBank
// Layout: local model; retail offsets: CachedSurfaces list +0x88, CompletedBankFile +0x398 (CBankFileAsync part: Size +4; CResourceBank part +0x164
//         -> +0x4fc; GraphicList records +0x1e0 -> +0x578), MaxPreloadTime +0x9cc, CurrentFrame +0x9d0; CCompositeSurfaceCache CurrentMorph +0x20,
//         LastUsedFrame +0x5c; STLport set<unsigned long> (header node 0x14 bytes, value +0x10).
#include <stdlib.h>
void __cdecl operator delete(void* block);         // 0x00BFE9BC
void* operator new(unsigned int size);             // 0x00BFEA1A
extern "C" double __fastcall GFGetTime();          // 0x009E1BC0

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
struct CTexture;
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    CGraphicFrame(const CGraphicFrame& other) : FrameData(other.FrameData) { if (FrameData) ++FrameData->ReferenceCount; }
    __forceinline ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data && --data->ReferenceCount == 0) {
            data->~CGraphicFrameData();
            operator delete(data);
        }
    }
};

struct CResource;
struct CResourceList {
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long           m_references;                   // +0x04
    CResourceList* List;                           // +0x08
    unsigned char  _pad0C[0x18 - 0x0c];
    unsigned long  Stamp;                          // +0x18
    void ListRemove();                             // 0x009FBFB0
    void ListInsert(CResource* before, CResource* after);   // 0x009FBF90
    __forceinline void Touch()
    {
        if (List && Stamp != List->Stamp) {
            ListRemove();
            ListInsert(List->Head, (CResource*)&List->Tail);
            Stamp = List->Stamp;
        }
    }
};
struct CVolumeHolder {
    void*      _word00;                            // +0x00
    CResource* Volume;                             // +0x04
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {
    CGraphicFrame Frame;                           // +0x1c
    CVolumeHolder VolumeHolder;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    signed char   SmallestResidentMip;             // +0x38
    unsigned char _pad39;
    signed char   LowestRequestedMip;              // +0x3a
    unsigned char _pad3B;
    unsigned long LastFrameStamp;                  // +0x3c
    unsigned char _pad40[4];
    CFrameData(CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long mip, unsigned long flags);   // 0x009FDE80
};
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
struct CGraphicInfo {
    unsigned char  _pad00[0x20];
    CFrameCounter* Counter;                        // +0x20
};
template <class T> struct CIVCountedPointer {
    T* Data;
    void Reset()
    {
        T* p = Data;
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Reset(); Data = p; return *this; }
    T* operator->() const { return Data; }
};
struct CGraphicInfoFrame {
    void*                         _word00;
    unsigned short                FrameNum;
    signed char                   SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;            // +0x08
    CGraphicInfo*                 GraphicInfo;     // +0x0c
};
struct SBankGraphic {
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrame;                   // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CBankFileAsync {
    void*         __vftable;
    unsigned long Size;                            // +0x04
    unsigned char _pad08[0x164 - 8];
};
struct CResourceBank {                             // +0x164 in CGraphicDataBank
    unsigned char _pad[0x7c];
    void ProcessLoading(bool force, double time);  // 0x009FC410
};
struct CGraphicDataBank : CBankFileAsync, CResourceBank {
    CRetailVector<SBankGraphic> GraphicList;       // +0x1e0
    unsigned char               _pad1ec[0x30c - 0x1ec];
    void FrameEnd();                               // 0x009FD660
    __forceinline SBankGraphic* Get(unsigned long index) { return index < GraphicList.size() ? &GraphicList[index] : 0; }
    __forceinline CGraphicInfoFrame* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};

struct CEngineTextureMorph {
    long          OriginalTextureIndex;
    unsigned char _rest[0x10];
};
struct CCompositeSurfaceCache : CResource {
    void*               Owner;                     // +0x1c
    CEngineTextureMorph CurrentMorph;              // +0x20
    CEngineTextureMorph CachedMorph;               // +0x34
    unsigned long       MipmapLevel;               // +0x48
    CGraphicFrame       GraphicFrame;              // +0x4c
    unsigned char       ManagedTexture[8];         // +0x50
    unsigned long       BankFileIndex;             // +0x58
    unsigned long       LastUsedFrame;             // +0x5c
};
struct SCacheNode {
    SCacheNode*             next;
    SCacheNode*             prev;
    CCompositeSurfaceCache* value;                 // CCountedPointer data
    void*                   info;
};

struct CSetNode {
    bool          color;                           // +0x00
    CSetNode*     parent;                          // +0x04
    CSetNode*     left;                            // +0x08
    CSetNode*     right;                           // +0x0c
    unsigned long value;                           // +0x10
};
struct CSetIterator {
    CSetNode* node;
    __forceinline void Increment()
    {
        if (node->right) {
            node = node->right;
            while (node->left)
                node = node->left;
        } else {
            CSetNode* y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
    }
};
struct CInsertResult {
    CSetIterator it;
    bool         inserted;
};
struct CULongSet {                                 // STLport set<unsigned long>
    CSetNode*     header;
    unsigned long count;
    long          compare;
    CULongSet() : count(0)
    {
        header = (CSetNode*)malloc(sizeof(CSetNode));
        count = 0;
        header->color = false;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
    CInsertResult insert(const unsigned long& value);   // 0x00514C10
    void Erase(CSetNode* node);                    // 0x004CF810
    __forceinline void EraseTree(CSetNode* node)
    {
        while (node) {
            Erase(node->right);
            CSetNode* left = node->left;
            free(node);
            node = left;
        }
    }
    __forceinline ~CULongSet()
    {
        if (count) {
            EraseTree(header->parent);
            header->left = header;
            header->parent = 0;
            header->right = header;
            count = 0;
        }
        if (header)
            free(header);
    }
};

struct CEngineSurfaceCompositionManager {
    unsigned char    _pad[0x88];
    SCacheNode*      CachedSurfaces;               // +0x88
    unsigned char    _pad8c[0x398 - 0x8c];
    CGraphicDataBank CompletedBankFile;            // +0x398
    unsigned char    _pad6a4[0x9cc - 0x6a4];
    float            MaxPreloadTime;               // +0x9cc
    unsigned long    CurrentFrame;                 // +0x9d0
    unsigned long FindIndexInCompletedBank(const CEngineTextureMorph* morph);   // 0x00B2AE60
    void PreloadCompletedBank();
};

void CEngineSurfaceCompositionManager::PreloadCompletedBank()
{
    long newest = 0x7fffffff;
    CULongSet indices;
    SCacheNode* it;
    for (it = CachedSurfaces->next; it != CachedSurfaces; it = it->next) {
        CCompositeSurfaceCache* cache = it->value;
        if (FindIndexInCompletedBank(&cache->CurrentMorph) && cache->LastUsedFrame) {
            long age = CurrentFrame - cache->LastUsedFrame;
            if (newest >= age)
                newest = age;
        }
    }
    for (it = CachedSurfaces->next; it != CachedSurfaces; it = it->next) {
        CCompositeSurfaceCache* cache = it->value;
        unsigned long index = FindIndexInCompletedBank(&cache->CurrentMorph);
        if (index && cache->LastUsedFrame) {
            long age = CurrentFrame - cache->LastUsedFrame;
            if (age <= newest + 100)
                indices.insert(index);
        }
    }
    double start = GFGetTime();
    bool pending;
    unsigned long loaded;
    for (;;) {
        pending = false;
        loaded = 0;
        CSetIterator entry;
        for (entry.node = indices.header->left; entry.node != indices.header; entry.Increment()) {
            CGraphicInfoFrame* frame = CompletedBankFile.GetFrame(entry.node->value);
            if (frame->Data.Data) {
                if (frame->Data->Frame.FrameData) {
                    frame->Data->LastFrameStamp = frame->GraphicInfo->Counter->FrameStamp;
                    CVolumeHolder& holder = frame->Data->VolumeHolder;
                    if (holder.Volume)
                        holder.Volume->Touch();
                    frame->Data->Touch();
                    if ((unsigned char)frame->Data->LowestRequestedMip > 0)
                        frame->Data->LowestRequestedMip = 0;
                    if ((unsigned char)frame->Data->SmallestResidentMip > 0)
                        frame->Data->BeginLoading(0, 0);
                    CGraphicFrame loadedFrame(frame->Data->Frame);
                    ++loaded;
                } else {
                    frame->Data->BeginLoading(0, 0);
                    pending = true;
                }
            } else {
                frame->Data = new CFrameData(frame);
                frame->Data->BeginLoading(0, 0);
                pending = true;
            }
        }
        if (GFGetTime() - start < MaxPreloadTime) {
            CompletedBankFile.FrameEnd();
            CompletedBankFile.ProcessLoading(true, MaxPreloadTime + start);
            if (pending)
                continue;
        }
        break;
    }
    if (loaded)
        GFGetTime();
}
