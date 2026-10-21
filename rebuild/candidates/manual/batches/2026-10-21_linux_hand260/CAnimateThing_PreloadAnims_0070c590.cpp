// CAnimateThing::PreloadAnims(const CCompleteAnimation&) -- touch every animation the complete animation (and the delayed combination anims) will need
// Layout: no engine/CAnimSet.h / CCompleteAnimation.h; retail STLport vector is 12 bytes and list 4 bytes, so CAnimSet is 0x54,
// CCompleteAnimation keeps TransitionAnimSets at +0x54 / CombinationAnims at +0x58, CAnimateThing::DelayedAnim at +0x64 (ctor 0x0070E710).
struct C3DAnimationInfo { void Precache(bool wait); };                    // 0xA26730
struct CMeshDataBank { C3DAnimationInfo* PeekAnimation(unsigned long index) const; };   // 0xA242C0
struct CWorld { char _pad[0x3c]; CMeshDataBank* PMainMeshBank; };
struct CMainGameComponent { char _pad[0x24]; CWorld* PWorld; };
extern CMainGameComponent* g_PMainGameComponent;   // 0x13B86A0

struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);
    virtual void Delete();
    unsigned long IVRefCount;
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};
struct C3DAnimationBlendState : CBaseClass {       // vtable 0x01239838
    CIVCountedPointer Animation;
    virtual void v000();
    C3DAnimationBlendState() { }
};

struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const short& operator[](unsigned int i) const { return _M_start[i]; }
};
struct CTransitionBlendAlpha { float CurrentAlpha; float LastAlpha; };
struct CAlphaVector {
    CTransitionBlendAlpha* _M_start; CTransitionBlendAlpha* _M_finish; CTransitionBlendAlpha* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
};

inline void PreloadAnimation(long anim)
{
    CMeshDataBank* bank = g_PMainGameComponent->PWorld->PMainMeshBank;
    bank->PeekAnimation(anim)->Precache(false);
}

class CAnimSet
{
public:
    long UpdateMode;
    CShortVector Anims;
    CAlphaVector InternalAlphas;
    char _pad1c[0x54 - 0x1c];
    __forceinline void Preload() const
    {
        for (unsigned int i = 0; i < (unsigned int)(Anims._M_finish - Anims._M_start); i++)
            PreloadAnimation(Anims._M_start[i]);
    }
};

struct CAnimSetNode { CAnimSetNode* _M_next; CAnimSetNode* _M_prev; CAnimSet _M_data; };
struct CAnimSetList { CAnimSetNode* _M_node; };

struct CCompleteAnimation {
    CAnimSet MainAnimSet;                  // +0x00
    CAnimSetList TransitionAnimSets;       // +0x54
    CAnimSetList CombinationAnims;         // +0x58
};

class CAnimateThing
{
public:
    char _pad0[0x64];
    CAnimSetList DelayedCombinationAnims;  // +0x64 (DelayedAnim.CombinationAnims)
    void PreloadAnims(const CCompleteAnimation& animation);
};

void CAnimateThing::PreloadAnims(const CCompleteAnimation& animation)
{
    C3DAnimationBlendState state;
    const CAnimSet& main = animation.MainAnimSet;
    long count = main.Anims.size();
    if ((count == 1 || (count == 2 && main.Anims[1] > 0)) && main.Anims[0] > 0 && count == (long)main.InternalAlphas.size()) {
        for (CAnimSetNode* node = animation.TransitionAnimSets._M_node->_M_next; node != animation.TransitionAnimSets._M_node; node = node->_M_next)
            node->_M_data.Preload();
        main.Preload();
    }
    for (CAnimSetNode* node = animation.CombinationAnims._M_node->_M_next; node != animation.CombinationAnims._M_node; node = node->_M_next)
        node->_M_data.Preload();
    for (CAnimSetNode* node = DelayedCombinationAnims._M_node->_M_next; node != DelayedCombinationAnims._M_node; node = node->_M_next)
        node->_M_data.Preload();
}
