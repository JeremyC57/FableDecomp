// Behaviour test for 0x0070D2F0 CAnimSet::AddAnim (append index + {alpha, alpha}, stamp the frame).
#include <cstdio>
#include <cstdlib>
#include <new>

static long g_frame = 41;
static long GFGetFrameCount() { return ++g_frame; }
class CTransitionBlendAlpha { public: float Current; float Last; CTransitionBlendAlpha(float c, float l) : Current(c), Last(l) {} };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) T(x); ++_M_finish; return; }
        unsigned int size = (unsigned int)(_M_finish - _M_start), len = size + (size < 1 ? 1 : size);
        T* p = (T*)std::malloc(len * sizeof(T));
        for (unsigned int i = 0; i < size; ++i) new (p + i) T(_M_start[i]);
        new (p + size) T(x);
        std::free(_M_start);
        _M_start = p; _M_finish = p + size + 1; _M_end_of_storage = p + len;
    }
};
class CAnimSet { public:
    CVector<short> Anims; CVector<CTransitionBlendAlpha> InternalAlphas; unsigned long FrameInternalAlphasUpdated;
    void AddAnim(short anim, float alpha)
    {
        Anims.push_back(anim);
        InternalAlphas.push_back(CTransitionBlendAlpha(alpha, alpha));
        FrameInternalAlphasUpdated = GFGetFrameCount();
    }
};

int main()
{
    CAnimSet s;
    s.Anims._M_start = s.Anims._M_finish = s.Anims._M_end_of_storage = 0;
    s.InternalAlphas._M_start = s.InternalAlphas._M_finish = s.InternalAlphas._M_end_of_storage = 0;
    s.AddAnim(12, 0.5f);
    s.AddAnim(-3, 1.0f);
    s.AddAnim(7, 0.25f);
    bool ok = s.Anims._M_finish - s.Anims._M_start == 3 && s.Anims._M_start[1] == -3 && s.Anims._M_start[2] == 7
           && s.InternalAlphas._M_finish - s.InternalAlphas._M_start == 3 && s.InternalAlphas._M_start[0].Current == 0.5f
           && s.InternalAlphas._M_start[0].Last == 0.5f && s.InternalAlphas._M_start[2].Last == 0.25f && s.FrameInternalAlphasUpdated == 44;
    if (!ok) { std::printf("0070d2f0_TEST FAIL\n"); return 1; }
    std::printf("0070d2f0_TEST PASS\n");
    return 0;
}