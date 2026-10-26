// Behaviour test for CEngine::OnPreDeviceReset @ 0x00B26010
#include <cstdio>
#include <cstring>
static int g_deleted = 0;
class CSurface { public: virtual ~CSurface() { ++g_deleted; } };
class CEngineLayerRenderer { public: int m_resets; virtual void OnPreDeviceReset() { ++m_resets; } };
static void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last) return result;
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    return (char*)std::memmove(result, first, n) + n;
}
template <class T> struct CRetailVector {
    T* m_start; T* m_finish; T* m_endOfStorage;
    T* begin() const { return m_start; }
    T* end() const { return m_finish; }
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
    T* erase(T* first, T* last) { T* i = (T*)CopyTrivial(last, m_finish, first); m_finish = i; return first; }
    void clear() { erase(begin(), end()); }
};
class CEngine { public:
    CRetailVector<CSurface*> ScratchZBuffersD24S8;
    CRetailVector<CEngineLayerRenderer*> RenderLayers;
    void OnPreDeviceReset()
    {
        for (unsigned int i = 0; i < ScratchZBuffersD24S8.size(); ++i)
            delete ScratchZBuffersD24S8[i];
        ScratchZBuffersD24S8.clear();
        for (CEngineLayerRenderer** it = RenderLayers.begin(); it != RenderLayers.end(); ++it)
            (*it)->OnPreDeviceReset();
    }
};
int main()
{
    CSurface* surfaces[3] = { new CSurface, 0, new CSurface };
    CEngineLayerRenderer layers[2];
    CEngineLayerRenderer* lp[2] = { &layers[0], &layers[1] };
    layers[0].m_resets = 0; layers[1].m_resets = 0;
    CEngine e;
    e.ScratchZBuffersD24S8.m_start = surfaces; e.ScratchZBuffersD24S8.m_finish = surfaces + 3; e.ScratchZBuffersD24S8.m_endOfStorage = surfaces + 3;
    e.RenderLayers.m_start = lp; e.RenderLayers.m_finish = lp + 2; e.RenderLayers.m_endOfStorage = lp + 2;
    e.OnPreDeviceReset();
    if (g_deleted != 2 || e.ScratchZBuffersD24S8.size() != 0 || e.ScratchZBuffersD24S8.m_finish != surfaces ||
        layers[0].m_resets != 1 || layers[1].m_resets != 1) { std::printf("00b26010_TEST FAIL\n"); return 1; }
    e.OnPreDeviceReset();   // empty vector: nothing deleted, layers told again
    if (g_deleted != 2 || layers[1].m_resets != 2) { std::printf("00b26010_TEST FAIL\n"); return 1; }
    std::printf("00b26010_TEST PASS\n");
    return 0;
}