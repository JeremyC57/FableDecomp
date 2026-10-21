// CShaderRenderManager::AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight) -- grow the light table (keeping the existing lights, defaulting new ones) and the per-light entry buffer from the frame pool, and size the light-constant vector
// Grade: DIFFER(448v446) -- frame only (0x58 v 0x48) plus the 64-bit overflow test's extra jb: retail homes the first __copy's random_access_iterator_tag in the still-live numEntriesPerLight argument slot and the resize value C4DVector(1,1,1,1) over the old-lights vector (whose start it keeps in edi for the final free); ours gives both their own frame words. Tried a direct tag temporary, an iterator-category inline, a STLport-4.5 __copy_aux(__false_type) layer
// Leaf: AllocateLights
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CLight {                                    // 0x38 bytes
    unsigned char Params[0x30];
    long          Index;                           // +0x30
    bool          Enabled;                         // +0x34
    bool          Dirty;                           // +0x35
    unsigned char _pad36[2];
    CLight() : Index(0), Enabled(false), Dirty(true) {}
};
CLight* __fastcall LightCopy(const CLight* first, const CLight* last, CLight* result,
                             const random_access_iterator_tag&, int*);   // 0x0098C2F0 (STLport __copy)
inline random_access_iterator_tag IteratorCategory(const CLight*) { return random_access_iterator_tag(); }
inline CLight* Copy(const CLight* first, const CLight* last, CLight* result)
{
    return LightCopy(first, last, result, IteratorCategory(first), (int*)0);
}
struct CLightVector {                              // STLport vector<CLight>
    CLight* _M_start;
    CLight* _M_finish;
    CLight* _M_end_of_storage;
    CLightVector(unsigned long count);             // 0x0098CCC0
    ~CLightVector() { if (_M_start) free(_M_start); }
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct C4DVectorVector {                           // STLport vector<C4DVector>
    C4DVector* _M_start;
    C4DVector* _M_finish;
    C4DVector* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    C4DVector* erase(C4DVector* first, C4DVector* last);                               // 0x0098CF70
    void _M_fill_insert(C4DVector* position, unsigned long count, const C4DVector& value);   // 0x0098D360
    void resize(unsigned long newSize, const C4DVector& value)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
};
struct CSimpleMemoryPool {
    void FreeAll();                                // 0x00A5C720
    void* Allocate(unsigned long bytes);           // 0x00A5C910
};
template <class T> inline T* PoolNewArray(CSimpleMemoryPool& pool, unsigned long count)
{
    return (count != 0 && !((unsigned __int64)count * sizeof(T) > 0xffffffff)) ? (T*)pool.Allocate(count * sizeof(T)) : 0;
}
struct C4DFloat { float V[4]; };
struct CShaderRenderManager {
    unsigned char     _pad000[0x164];
    CLight*           Lights;                      // +0x164
    C4DFloat*         LightEntries;                // +0x168
    unsigned char     _pad16c[4];
    unsigned long     NumLights;                   // +0x170
    unsigned long     NumEntriesPerLight;          // +0x174
    CSimpleMemoryPool Pool;                        // +0x178
    unsigned char     _pad179[0x27];
    C4DVectorVector   LightConstants;              // +0x1a0
    void AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight);
};

void CShaderRenderManager::AllocateLights(unsigned long numLights, unsigned long numEntriesPerLight)
{
    CLightVector oldLights(NumLights);
    if (NumLights > 0)
        Copy(Lights, Lights + NumLights, oldLights._M_start);
    unsigned long oldCount = NumLights;
    if (NumLights < numLights)
        NumLights = numLights;
    if (NumEntriesPerLight < numEntriesPerLight)
        NumEntriesPerLight = numEntriesPerLight;
    Pool.FreeAll();
    Lights = PoolNewArray<CLight>(Pool, NumLights);
    Copy(oldLights._M_start, oldLights._M_finish, Lights);
    if (numLights > oldCount) {
        CLight* last = Lights + numLights;
        CLight value;
        for (CLight* p = Lights + oldCount; p != last; ++p)
            *p = value;
    }
    LightEntries = PoolNewArray<C4DFloat>(Pool, NumEntriesPerLight * NumLights);
    LightConstants.resize((numLights + 3) / 4 * 2, C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
}
