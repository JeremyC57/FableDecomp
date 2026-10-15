// CEngine::OnPreDeviceReset() -- before the render device is reset: delete every scratch D24S8 z-buffer surface and empty their vector, then tell every engine layer renderer that the device is about to reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B26010, length 151 bytes (+ int3 padding).
// Callees: CSurface virtual slot 0 (the deleting destructor, `push 1`), memmove @ 0x00BFEAE6 (STLport's trivial copy inside erase),
// CEngineLayerRenderer virtual slot 11 (+0x2c, the per-layer pre-reset hook). Mangled ?OnPreDeviceReset@CEngine@@EAEXXZ (private virtual).
// Layout: the engine header CEngine.h has the PDB offsets, which do not hold in retail (vectors are 12 bytes): ScratchZBuffersD24S8
// +0x140 (PDB +0x154), RenderLayers +0x15c (PDB +0x168); modelled locally.
// Shape: an index loop bounded by the re-computed unsigned `size()` (`jbe` / `jb`), each element through STLport `*(begin() + i)` and
// `delete` (null test + virtual deleting destructor); then STLport `clear()` = `erase(begin(), end())`, whose trivial pointer copy is
// `if (first == last) return result; return memmove(result, first, n) + n;` (`cmp eax,eax ; jne` with first == last == finish; the
// `n ? ... : result` form tests the difference with `je` instead); the layer loop re-reads `end()` every pass.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct CSurface {
    virtual ~CSurface();
};

struct CEngineLayerRenderer {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void OnPreDeviceReset();         // slot 11 (+0x2c)
};

inline void* CopyTrivial(const void* first, const void* last, void* result)   // STLport __copy_trivial
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}

template <class T>
struct CRetailVector {                       // STLport vector of pointers: 12 bytes
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    T* begin() const { return m_start; }
    T* end() const { return m_finish; }
    unsigned long size() const { return m_finish - m_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    T* erase(T* first, T* last)
    {
        T* i = (T*)CopyTrivial(last, m_finish, first);
        m_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CEngine {
    virtual void v00();
    unsigned char _pad04[0x140 - 4];
    CRetailVector<CSurface*>             ScratchZBuffersD24S8;   // +0x140
    unsigned char _pad14c[0x15c - 0x14c];
    CRetailVector<CEngineLayerRenderer*> RenderLayers;           // +0x15c
private:
    virtual void OnPreDeviceReset();
};

void CEngine::OnPreDeviceReset()
{
    for (unsigned long i = 0; i < ScratchZBuffersD24S8.size(); ++i)
        delete ScratchZBuffersD24S8[i];
    ScratchZBuffersD24S8.clear();
    for (CEngineLayerRenderer** it = RenderLayers.begin(); it != RenderLayers.end(); ++it)
        (*it)->OnPreDeviceReset();
}