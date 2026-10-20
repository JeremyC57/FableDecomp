// Behaviour test for CTCAnimatingObject::FrameUpdate (0x007F8E20).
#include <cstdio>
#include <cstdlib>

static long g_framesPerSecond = 20;
struct CAnimInit { long Anim; };
template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {
    SListNode<T>* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    T& front() { return _M_node->_M_next->_M_data; }
    void erase(SListNode<T>* p) { SListNode<T>* n = p->_M_next; SListNode<T>* q = p->_M_prev; q->_M_next = n; n->_M_prev = q; std::free(p); }
    void pop_front() { erase(_M_node->_M_next); }
    void Init() { _M_node = (SListNode<T>*)std::malloc(sizeof(SListNode<T>)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void PushBack(const T& v)
    {
        SListNode<T>* n = (SListNode<T>*)std::malloc(sizeof(SListNode<T>));
        n->_M_data = v; n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev; _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
};
struct CAnimFrameTime { float Seconds; long Reserved1, Reserved2; CAnimFrameTime(float s) : Seconds(s), Reserved1(0), Reserved2(0) {} };
static float g_lastSeconds = 0.0f;
struct CAnimateThing {
    int m_pCurrent, m_pEnd; unsigned char m_stateFlags;
    void UpdateFrameLogic(const CAnimFrameTime& t) { g_lastSeconds = t.Seconds; }
    bool IsFinished() const { return m_pCurrent == m_pEnd || (m_stateFlags & 2); }
};
struct CTCAnimationComplex { CAnimateThing anim; CAnimateThing& GetAnimation() { return anim; } };
struct CBulletTime {
    void* m_pController; bool m_active; float scale;
    float GetTimeScale() { return scale; }
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CWorld { CBulletTime* m_pBulletTime; };
struct CThing { bool affected; bool IsAffectedByBulletTime() { return affected; } };
static long g_played = -1; static int g_playCount = 0;
class CTCAnimatingObject {
public:
    CThing* ParentThing; CTCAnimationComplex* complex; CWorld* world;
    bool Animating;
    CRetailList<CAnimInit> QueuedAnims;
    CWorld* GetWorld() const { return world; }
    void PlayAnimation(const CAnimInit& init, long) { g_played = init.Anim; ++g_playCount; Animating = true; }
    CTCAnimationComplex* GetAnimationComplex() const { return complex; }
    void FrameUpdate()
    {
        CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
        if (Animating) {
            if (animate.IsFinished())
                Animating = false;
        }
        if (!Animating) {
            if (!QueuedAnims.empty()) {
                PlayAnimation(QueuedAnims.front(), -1);
                QueuedAnims.pop_front();
            }
        }
        float seconds = 1.0f / g_framesPerSecond;
        CAnimFrameTime time((GetWorld()->m_pBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime())
                                ? seconds / GetWorld()->m_pBulletTime->GetTimeScale() : seconds);
        animate.UpdateFrameLogic(time);
    }
};

int main()
{
    int ok = 1;
    CTCAnimationComplex c; c.anim.m_pCurrent = 1; c.anim.m_pEnd = 2; c.anim.m_stateFlags = 0;
    CBulletTime bt; bt.m_pController = 0; bt.m_active = false; bt.scale = 0.5f;
    CWorld w; w.m_pBulletTime = &bt;
    CThing t; t.affected = true;
    CTCAnimatingObject o; o.ParentThing = &t; o.complex = &c; o.world = &w; o.Animating = true;
    o.QueuedAnims.Init(); CAnimInit a; a.Anim = 5; o.QueuedAnims.PushBack(a); a.Anim = 6; o.QueuedAnims.PushBack(a);
    o.FrameUpdate();
    if (g_playCount != 0 || g_lastSeconds != 0.05f) ok = 0;            // still animating, unscaled time
    c.anim.m_stateFlags = 2;
    o.FrameUpdate();
    if (g_playCount != 1 || g_played != 5 || !o.Animating) ok = 0;     // finished: next queued anim starts
    bt.m_active = true;
    o.FrameUpdate();
    if (g_playCount != 2 || g_played != 6 || !o.QueuedAnims.empty() || g_lastSeconds != 0.1f) ok = 0;   // bullet time halves the speed
    t.affected = false;
    o.FrameUpdate();
    if (g_lastSeconds != 0.05f) ok = 0;
    if (!ok) { std::printf("007f8e20_TEST FAIL\n"); return 1; }
    std::printf("007f8e20_TEST PASS\n");
    return 0;
}
