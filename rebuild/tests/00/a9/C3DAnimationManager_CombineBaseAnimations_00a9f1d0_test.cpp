// C3DAnimationManager::CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh) -- evaluate a base animation state (or a transition between two, recursively) into the keyframe array
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9F1D0, 277 bytes (`ret 14h`).
// Ego_r.pdb: ?CombineBaseAnimations@C3DAnimationManager@@AAEXABVC3DAnimationStateBase@@PAVC3DKeyframe@@KKABVC3DMesh2@@@Z.
// Callees: the state's GetType (vtable slot 2), CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0 (manifest
// "CTCInventoryAbilities::GetAbilityLevel" is propagated), C3DAnimationInfo::GetAnimation(const C3DMesh2&) const 0x00A26C60
// (CCountedPointer<const C3DAnimation2> by hidden pointer), C3DAnimation2::CalcLocalTransformationsRelative 0x00A4D770 /
// CalcLocalTransformations 0x00A4CEC0, itself (recursion), C3DAnimationManager::BlendAnimations 0x00A9EB90,
// operator delete 0x00BFE9BC (count block).
// Shape: type 0 (C3DAnimationState) fetches the animation for the mesh and calls the relative or absolute evaluator by
// the animation's type bit 2 (whole-dword `shr edx,2 ; test dl,1`), then the counted pointer's long-form release; type 2
// (C3DAnimationTransitionState) evaluates the start state, and below depth 5 the end state into the next boneCount
// keyframes, then blends the two by Progress. `depth + 1` is a named local (it lands in the dead `state` slot).
// The animation index (unsigned long) and PeekAnimation's result are named locals: retail calls PeekAnimation before
// pushing GetAnimation's arguments, with the index in eax; the chained call pushes the arguments first (and an index
// read straight into the call rotates eax/ecx/edx by one through the rest of the block).
// Layout: Ego_r.pdb C3DAnimationState (AnimationIndex +8, AnimationTime +0xc), C3DAnimationTransitionState (StartState +8,
// EndState +0xc, Progress +0x10), C3DAnimationStats AnimType +8; C3DKeyframe 0x30 bytes; the bank at manager +4.
// Leaf: CombineBaseAnimations@C3DAnimationManager
struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> struct CCountedPointer {
    T* Object;                                     // +0
    SCountBlock* Info;                             // +4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
    }
    T* operator->() const { return Object; }
};

struct C3DKeyframe {
    float Rotation[4];
    float Position[4];
    float Scaling[4];                              // 0x30 bytes
};
struct C3DMesh2;

struct C3DAnimation2 {
    void* __vftable;                               // +0x00 (C3DAnimationStats base)
    long _04;
    unsigned long AnimType;                        // +0x08 EAnimationType
    bool IsRelative() const { return (AnimType >> 2) & 1; }
    void CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long count, float time, const C3DMesh2& mesh) const;   // 0x00A4D770
    void CalcLocalTransformations(C3DKeyframe* keyframes, unsigned long count, float time) const;   // 0x00A4CEC0
};

struct C3DAnimationInfo {
    CCountedPointer<const C3DAnimation2> GetAnimation(const C3DMesh2& mesh) const;   // 0x00A26C60
};
struct CMeshDataBank {
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};

struct C3DAnimationStateBase {
    virtual void v00();
    virtual void v01();
    virtual long GetType() const;                  // slot 2 (+8)
    unsigned long IVRefCount;                      // +0x04
};
struct C3DAnimationState : C3DAnimationStateBase {
    long AnimationIndex;                           // +0x08
    float AnimationTime;                           // +0x0c
};
struct C3DAnimationTransitionState : C3DAnimationStateBase {
    C3DAnimationStateBase* StartState;             // +0x08 CIVCountedPointer<C3DAnimationStateBase>
    C3DAnimationStateBase* EndState;               // +0x0c CIVCountedPointer<C3DAnimationStateBase>
    float Progress;                                // +0x10
};

struct C3DAnimationManager {
    virtual ~C3DAnimationManager();
    CMeshDataBank* MeshDataBank;                   // +0x04
    void BlendAnimations(C3DKeyframe* dest, const C3DKeyframe* src, unsigned long count, float alpha);   // 0x00A9EB90
    void CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh);
};

void C3DAnimationManager::CombineBaseAnimations(const C3DAnimationStateBase& state, C3DKeyframe* keyframes, unsigned long boneCount, unsigned long depth, const C3DMesh2& mesh)
{
    long type = state.GetType();
    if (type == 0) {
        const C3DAnimationState& base = (const C3DAnimationState&)state;
        unsigned long index = base.AnimationIndex;
        const C3DAnimationInfo* info = MeshDataBank->PeekAnimation(index);
        CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);
        if (animation->IsRelative())
            animation->CalcLocalTransformationsRelative(keyframes, boneCount, base.AnimationTime, mesh);
        else
            animation->CalcLocalTransformations(keyframes, boneCount, base.AnimationTime);
    } else if (type == 2) {
        const C3DAnimationTransitionState& transition = (const C3DAnimationTransitionState&)state;
        unsigned long nextDepth = depth + 1;
        CombineBaseAnimations(*transition.StartState, keyframes, boneCount, nextDepth, mesh);
        if (nextDepth < 5) {
            C3DKeyframe* second = keyframes + boneCount;
            CombineBaseAnimations(*transition.EndState, second, boneCount, nextDepth, mesh);
            BlendAnimations(keyframes, second, boneCount, transition.Progress);
        }
    }
}
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C3DMesh2 { int Dummy; };
static C3DAnimationInfo g_infos[4];
static C3DAnimation2 g_anims[4];
static unsigned long g_peekIndex = 99; static const CMeshDataBank* g_peekThis = 0;
static const C3DMesh2* g_getMesh = 0; static int g_destroyed = 0;
struct Eval { const C3DAnimation2* anim; C3DKeyframe* kf; unsigned long count; float time; const C3DMesh2* mesh; int relative; };
static Eval g_evals[8]; static int g_nEval = 0;
static C3DKeyframe* g_blendDest = 0; static const C3DKeyframe* g_blendSrc = 0; static unsigned long g_blendCount = 0; static float g_blendAlpha = -1.0f; static int g_nBlend = 0;
static void __fastcall DestroyAnim(void* p) { (void)p; ++g_destroyed; }
const C3DAnimationInfo* CMeshDataBank::PeekAnimation(unsigned long index) const { g_peekThis = this; g_peekIndex = index; return &g_infos[index]; }
CCountedPointer<const C3DAnimation2> C3DAnimationInfo::GetAnimation(const C3DMesh2& mesh) const
{
    g_getMesh = &mesh;
    CCountedPointer<const C3DAnimation2> r;
    r.Object = &g_anims[this - g_infos];
    r.Info = new SCountBlock;
    r.Info->m_references = 1; r.Info->m_pfnDestroy = DestroyAnim; r.Info->m_pObject = (void*)r.Object;
    return r;
}
void C3DAnimation2::CalcLocalTransformationsRelative(C3DKeyframe* keyframes, unsigned long count, float time, const C3DMesh2& mesh) const
{ Eval e = { this, keyframes, count, time, &mesh, 1 }; if (g_nEval < 8) g_evals[g_nEval++] = e; }
void C3DAnimation2::CalcLocalTransformations(C3DKeyframe* keyframes, unsigned long count, float time) const
{ Eval e = { this, keyframes, count, time, 0, 0 }; if (g_nEval < 8) g_evals[g_nEval++] = e; }
void C3DAnimationStateBase::v00() { }
void C3DAnimationStateBase::v01() { }
long C3DAnimationStateBase::GetType() const { return -1; }
C3DAnimationManager::~C3DAnimationManager() { }
void C3DAnimationManager::BlendAnimations(C3DKeyframe* dest, const C3DKeyframe* src, unsigned long count, float alpha)
{ g_blendDest = dest; g_blendSrc = src; g_blendCount = count; g_blendAlpha = alpha; ++g_nBlend; }
struct TState : C3DAnimationState { virtual long GetType() const { return 0; } };
struct TTransition : C3DAnimationTransitionState { virtual long GetType() const { return 2; } };
struct TOther : C3DAnimationStateBase { virtual long GetType() const { return 1; } };
static void Reset() { g_nEval = 0; g_nBlend = 0; g_destroyed = 0; g_peekIndex = 99; g_getMesh = 0; }
int main()
{
    memset(g_anims, 0, sizeof g_anims);
    g_anims[1].AnimType = 4;                    // relative
    g_anims[2].AnimType = 3;                    // absolute (bit 2 clear)
    CMeshDataBank bank;
    C3DAnimationManager mgr; mgr.MeshDataBank = &bank;
    C3DMesh2 mesh; mesh.Dummy = 0;
    static C3DKeyframe kf[16];
    TState s1; s1.IVRefCount = 0; s1.AnimationIndex = 1; s1.AnimationTime = 0.25f;
    TState s2; s2.IVRefCount = 0; s2.AnimationIndex = 2; s2.AnimationTime = 0.75f;

    Reset(); mgr.CombineBaseAnimations(s1, kf, 5, 0, mesh);
    CHECK(g_peekThis == &bank && g_peekIndex == 1 && g_getMesh == &mesh);
    CHECK(g_nEval == 1 && g_evals[0].relative == 1 && g_evals[0].anim == &g_anims[1]);
    CHECK(g_evals[0].kf == kf && g_evals[0].count == 5 && g_evals[0].time == 0.25f && g_evals[0].mesh == &mesh);
    CHECK(g_destroyed == 1);                    // the counted pointer released its only reference

    Reset(); mgr.CombineBaseAnimations(s2, kf, 5, 0, mesh);
    CHECK(g_peekIndex == 2 && g_nEval == 1 && g_evals[0].relative == 0 && g_evals[0].anim == &g_anims[2]);
    CHECK(g_evals[0].kf == kf && g_evals[0].count == 5 && g_evals[0].time == 0.75f && g_destroyed == 1);

    TTransition t; t.IVRefCount = 0; t.StartState = &s1; t.EndState = &s2; t.Progress = 0.5f;
    Reset(); mgr.CombineBaseAnimations(t, kf, 3, 0, mesh);
    CHECK(g_nEval == 2 && g_evals[0].anim == &g_anims[1] && g_evals[0].kf == kf);
    CHECK(g_evals[1].anim == &g_anims[2] && g_evals[1].kf == kf + 3 && g_evals[1].count == 3);
    CHECK(g_nBlend == 1 && g_blendDest == kf && g_blendSrc == kf + 3 && g_blendCount == 3 && g_blendAlpha == 0.5f);
    CHECK(g_destroyed == 2);

    Reset(); mgr.CombineBaseAnimations(t, kf, 3, 4, mesh);   // next depth 5: start state only, no blend
    CHECK(g_nEval == 1 && g_evals[0].anim == &g_anims[1] && g_nBlend == 0);

    TTransition outer; outer.IVRefCount = 0; outer.StartState = &t; outer.EndState = &s1; outer.Progress = 0.125f;
    Reset(); mgr.CombineBaseAnimations(outer, kf, 2, 0, mesh);   // nested transition: 3 evaluations, 2 blends
    CHECK(g_nEval == 3 && g_evals[0].kf == kf && g_evals[1].kf == kf + 2 && g_evals[2].kf == kf + 2);
    CHECK(g_nBlend == 2 && g_blendDest == kf && g_blendSrc == kf + 2 && g_blendAlpha == 0.125f);

    TOther o; o.IVRefCount = 0;
    Reset(); mgr.CombineBaseAnimations(o, kf, 3, 0, mesh);
    CHECK(g_nEval == 0 && g_nBlend == 0 && g_peekIndex == 99);
    if (fails == 0) printf("00a9f1d0_TEST PASS\n");
    return fails;
}