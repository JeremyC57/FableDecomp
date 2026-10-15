// Behaviour test for CAnimateThing::GetMeshAnims (0x0070E6A0): refresh, then counted-pointer
// assignment of the two cached anims (release of the old pointee, add-ref of the new one).
#include <cstdio>

static int g_released = 0;

struct C3DAnimation {
    long m_refCount;
    virtual ~C3DAnimation() {}
    virtual void Release() { ++g_released; }
};

struct CPtr {
    C3DAnimation* m_pObject;
    CPtr& operator=(const CPtr& other)
    {
        if (m_pObject != other.m_pObject) {
            if (m_pObject) {
                C3DAnimation* old = m_pObject;
                if (--old->m_refCount == 0)
                    old->Release();
                m_pObject = 0;
            }
            m_pObject = other.m_pObject;
            if (m_pObject)
                ++m_pObject->m_refCount;
        }
        return *this;
    }
};

struct CAnimateThing {
    CPtr m_cachedFirst, m_cachedSecond;
    int updates;
    void UpdateCachedAnims() { ++updates; }
    void GetMeshAnims(CPtr& first, CPtr& second)
    {
        UpdateCachedAnims();
        first = m_cachedFirst;
        second = m_cachedSecond;
    }
};

int main()
{
    C3DAnimation a, b, old;
    a.m_refCount = 1; b.m_refCount = 1; old.m_refCount = 1;
    CAnimateThing t;
    t.m_cachedFirst.m_pObject = &a; t.m_cachedSecond.m_pObject = &b; t.updates = 0;
    CPtr x, y;
    x.m_pObject = &old; y.m_pObject = &b;
    t.GetMeshAnims(x, y);
    if (t.updates != 1 || x.m_pObject != &a || a.m_refCount != 2 || old.m_refCount != 0 || g_released != 1
        || y.m_pObject != &b || b.m_refCount != 1) {
        std::printf("0070e6a0_TEST FAIL\n"); return 1;
    }
    std::printf("0070e6a0_TEST PASS\n");
    return 0;
}