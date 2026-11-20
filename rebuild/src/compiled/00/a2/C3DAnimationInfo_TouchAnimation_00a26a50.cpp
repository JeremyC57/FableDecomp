// C3DAnimationInfo::TouchAnimation() -- marks the animation data as used this frame: moves it to the tail of its resource list and stamps LastUsedFrame; starts loading when the data or its shared animation is missing
// Name (NF143, q4942 / q4943): ListRemove 0x009FBFB0 / ListInsert 0x009FBF90 are private CResource members in Ego_r.pdb / FableWin.pdb
//   (?ListRemove@CResource@@AAEXXZ / ?ListInsert@CResource@@AAEXPAV1@0@Z); the row's local CResource declares them private and the
//   touch (FableWin.pdb's inline CResource::TouchResource body) is a public inline CResource member, the 0x00A262B0 form (was public
//   QAE declarations and a Touch on the derived class).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A26A50, length 75 bytes.
// Callees: CResource::ListRemove @ 0x009FBFB0; CResource::ListInsert @ 0x009FBF90 (prev, next);
// C3DAnimationInfo::BeginLoading @ 0x00A26730 (bool).
// Layout from engine/C3DAnimationInfo.h (Data_Object +0xc) and engine/CResource.h (ResourceList
// +8, PrevResource +0xc, NextResource +0x10, LastUsedFrame +0x18); CAnimationData derives from
// CResource and holds its shared C3DAnimation2 counted pointer at +0x28 (PDB +0x2c with the
// 16-byte Ego map). CResourceList is {_, CResource Head (+4), ..., CurrentFrame +0x2c} (PDB).
// The list pointer is re-read from the data after the two calls (no local caches it).
class CResourceList;

class CResource { public:
    virtual void v00();
    unsigned long IVRefCount;                  // +4
    CResourceList* ResourceList;           // +8
    CResource* PrevResource;               // +0xc
    CResource* NextResource;               // +0x10
    unsigned long ResourceSize;            // +0x14
    unsigned long LastUsedFrame;           // +0x18
private:
    void ListRemove();                     // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next);   // 0x009FBF90
public:
    void Touch();
};

class CResourceList { public:
    long m_pad0;                           // +0
    CResource Head;                        // +4
    unsigned long ResourceCount;           // +0x20
    long AllocatedMemory;                  // +0x24
    long MaximumMemory;                    // +0x28
    unsigned long CurrentFrame;            // +0x2c
};
inline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CAnimationData : CResource {
    unsigned char Animations[0xc];         // +0x1c map<unsigned long, CCountedPointer<C3DAnimation2>> (12 bytes in retail)
    void* SharedAnimation;                 // +0x28 CCountedPointer<C3DAnimation2>::m_pObject
};

class C3DAnimationInfo { public:                  // engine/C3DAnimationInfo.h offsets
    virtual void v00();
    void* Bank;                            // +4
    unsigned long Index;                   // +8
    CAnimationData* Data;                  // +0xc
    void TouchAnimation();
public:
    void BeginLoading(bool) const;   // 0x00A26730
};

void C3DAnimationInfo::TouchAnimation()
{
    CAnimationData* data = Data;
    if (data == 0 || data->SharedAnimation == 0) {
        BeginLoading(false);
        return;
    }
    data->Touch();
}