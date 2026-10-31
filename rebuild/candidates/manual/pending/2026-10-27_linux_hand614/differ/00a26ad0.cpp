// C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2& mesh) const -- make sure the shared animation is built, then create and initialise a per-mesh instance (recovering animation memory and retrying when it fails), store it in the instance map and return it
// Grade: DIFFER(383v383) -- systematic eax/ecx/edx rotation through the IsLoaded/IsLoading guard and the map store, `sete cl` v `sete al` for the compressed flag, and the two InitialiseAnimationInstance call copies in swapped order; names now right (own symbol private const ABE, by-reference mesh, inline private BuildSharedAnimation, RecoverAnimationMemory switch, C3DAnimation2::SetMemoryPool); tried: for / while / do / for(;;) retry loops (inert), named CAnimationData* for the map store (89 lines from 92), animation before the flag (prior sweep)
// Leaf: ?BuildAnimationInstance@C3DAnimationInfo@@ABE?AV?$CCountedPointer@VC3DAnimation2@@@@ABVC3DMesh2@@@Z
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

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
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer() { ReleaseData(); }
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            ReleaseData();
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    void Reset(T* data);                    // 0x00A29C50 (same-TU stand-in body below)
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
CCPPointerInfo* NewPointerInfo(void* data);   // stand-in
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CCountedPointer<T>::Reset(T* data)
{
    ReleaseData();
    Data = data;
    Info = data ? NewPointerInfo(data) : 0;
}
#pragma auto_inline(on)

class C3DMesh2 {
public:
    unsigned char _00[0xe8];
    unsigned long MeshID;                   // +0xe8
    unsigned long GetMeshID() const { return MeshID; }
};
class CHandleBasedMemoryPool { public: void Defragment(); };                  // 0x00A02730
class CMovableResourceMemoryPool : public CHandleBasedMemoryPool { };
class C3DAnimation2 {
public:
    C3DAnimation2();                        // 0x00A4EE10
    unsigned char _body[0x7c];
    void SetMemoryPool(CMovableResourceMemoryPool* pool);                      // 0x00A4BE00
    bool InitialiseAnimationInstance(C3DAnimation2& shared, const C3DMesh2& mesh, bool compressed, float* duration);   // 0x00A50C20
};
namespace std {
template <class T> struct less { };
template <class T> class allocator { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    void* _M_header;
    unsigned long _M_node_count;
    C _M_key_compare;
    T& operator[](const K& k);              // 0x00A2D810
};
}
class CResourceList { public: void ShrinkResources(unsigned long size); };   // 0x009FC210
class CResourceBank { public: void* __vftable; CResourceList ResourceList; };
class CResource {
public:
    void RemoveFromResourceList();          // 0x009FC440
};
class CMeshDataBank {
public:
    unsigned long GetEntryDataType(unsigned long index) const;   // 0x009CC210 (CBankFile)
    unsigned char _00[0x268];
    CResourceBank AnimationResourceBank;    // +0x268 (ResourceList +0x26c)
    unsigned char _pad[0x360 - 0x268 - 0x8];
    CMovableResourceMemoryPool* AnimationMemoryPool;   // +0x360 CCountedPointer<CMovableResourceMemoryPool>::Data
};

class C3DAnimationInfo {
public:
    class CAnimationData : public CResource {
    public:
        unsigned char _00[0x1c];
        std::map<unsigned long, CCountedPointer<C3DAnimation2> > Animations;   // +0x1c
        CCountedPointer<C3DAnimation2> SharedAnimation;                       // +0x28
        void* FileData;                                                        // +0x30
        void BuildSharedAnimation();        // 0x00A24D70
    };
    void* __vftable;                        // +0x00
    CMeshDataBank* Bank;                    // +0x04
    unsigned long EntryIndex;               // +0x08
    CAnimationData* Data;                   // +0x0c CIVCountedPointer<CAnimationData>
    CMeshDataBank* GetBank() const { return Bank; }
    unsigned long GetEntryIndex() const { return EntryIndex; }
    bool IsLoaded() const { return Data && Data->SharedAnimation.Data; }
    bool IsLoading() const { return Data && Data->FileData; }
    void BeginLoading(bool wait) const;     // 0x00A26730
private:
    void BuildSharedAnimation() const
    {
        if (!IsLoaded()) {
            if (!IsLoading())
                BeginLoading(true);
            Data->RemoveFromResourceList();
            Data->BuildSharedAnimation();
        }
    }
    bool RecoverAnimationMemory(unsigned long attempt) const
    {
        switch (attempt) {
        case 0:
            GetBank()->AnimationResourceBank.ResourceList.ShrinkResources(0);
            return true;
        case 1:
            GetBank()->AnimationMemoryPool->Defragment();
            return true;
        }
        return false;
    }
    CCountedPointer<C3DAnimation2> BuildAnimationInstance(const C3DMesh2& mesh) const;
};

CCountedPointer<C3DAnimation2> C3DAnimationInfo::BuildAnimationInstance(const C3DMesh2& mesh) const
{
    BuildSharedAnimation();
    bool compressed = (GetBank()->GetEntryDataType(GetEntryIndex()) & 0x80) == 0x80;
    float duration;
    CCountedPointer<C3DAnimation2> animation;
    animation.Reset(new C3DAnimation2);
    animation->SetMemoryPool(GetBank()->AnimationMemoryPool);
    unsigned long attempt = 0;
    while (!animation->InitialiseAnimationInstance(*Data->SharedAnimation, mesh, compressed, &duration)) {
        if (!RecoverAnimationMemory(attempt))
            break;
        ++attempt;
    }
    CAnimationData* data = Data;
    data->Animations[mesh.GetMeshID()] = animation;
    return animation;
}
