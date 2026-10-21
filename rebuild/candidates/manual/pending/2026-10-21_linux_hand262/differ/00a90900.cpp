// Grade: DIFFER(487v471) -- register allocation + frame: the body shape is right (new list, triangle resize + 6-byte copy loop, "XTST" strip sub-chunk converted to a derived counted pointer, inlined STLport resize/erase/__copy_trivial of the strip indices with its unfolded `cmp ecx,ecx`, push_back) but ours swaps ebx/ebp for the strip chunk's object/count block, keeps `this` at a different slot and has a 0x18 frame vs retail 0x20 (retail keeps a second copy of the list pointer at [esp+0x1c] for the push_back argument).
// CDeviceIndependent::AddTriangleList(C3DMeshFileTrianglesChunk* chunk) -- build a new ref-counted triangle list from the mesh chunk: copy
// its triangles, copy the indices of its "XTST" strip sub-chunk when present, and append the list to the device's triangle lists.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A90900, length 471 bytes (480 with pad), `ret 4`.
// Callees: operator new 0x00BFEA1A; chunk 0x00ABC440 triangles (manifest "GetBridgeVertexBufferAsCArray" is propagated) / 0x00ABA370 count
// (manifest "CWorldMap::DrawGetNoMaps" is propagated); triangle vector resize 0x00A97A50; CChunkedFileSuperChunk::FindFirstSubChunkOfType
// 0x00AA7F40; operator delete 0x00BFE9BC; memmove 0x00BFEAE6; vector<unsigned short>::_M_fill_insert 0x005149F0; counted-pointer vector
// _M_insert_overflow 0x00A97BE0; list slot 1 release. Triangle-list vtable 0x0129DF7C; "XTST" 0x0129DF60.
// Layout (descriptive): triangle list {vptr, refcount +4 (starts at 1), triangle vector +8 (6-byte triangles), strip index vector +0x14};
// device triangle-list vector +0x10; strip chunk index vector +0x10; counted pointer {object, count block {refs, destroy, object}}.
// Tried: one smart pointer for the list (490), a raw list pointer + smart pointer (487).
// Leaf: AddTriangleList
inline void* operator new(unsigned int, void* p) throw() { return p; }
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
};
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(0), m_pCount(0)
    {
        if (other.m_pCount) {
            m_pObject = (T*)other.m_pObject;
            m_pCount = other.m_pCount;
            ++m_pCount->m_references;
        }
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
    T* operator->() const { return m_pObject; }
    operator bool() const { return m_pObject != 0; }
};
inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)memmove(result, first, ((const char*)last - (const char*)first))) + ((const char*)last - (const char*)first);
}
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
    void _M_fill_insert(T* pos, unsigned int n, const T& x);   // 0x005149F0 (unsigned short)
    T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void resize(unsigned int newSize, T x = T())
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), x);
    }
};
struct CTriangle {
    unsigned short Index[3];
    CTriangle() {}
};
struct CTriangleVector {
    CTriangle* _M_start;
    CTriangle* _M_finish;
    CTriangle* _M_end_of_storage;
    void resize(unsigned int newSize, const CTriangle& x = CTriangle());   // 0x00A97A50
    CTriangle& operator[](unsigned int i) { return _M_start[i]; }
};
struct CRefCounted {
    virtual void v00();
    virtual void Release();                 // slot 1
    long RefCount;                          // +4
    CRefCounted() : RefCount(1) {}
};
struct CTriangleList : CRefCounted {        // 0x20 bytes, vtable 0x0129DF7C
    CTriangleVector Triangles;              // +0x08
    CRetailVector<unsigned short> StripIndices;   // +0x14
    CTriangleList() { Triangles._M_start = 0; Triangles._M_finish = 0; Triangles._M_end_of_storage = 0;
                      StripIndices._M_start = 0; StripIndices._M_finish = 0; StripIndices._M_end_of_storage = 0; }
    virtual void v00();
};
template <class T> struct CRefPtr {
    T* m_p;
    CRefPtr(T* p) : m_p(p) {}
    CRefPtr(const CRefPtr& o) : m_p(o.m_p) { if (m_p) ++m_p->RefCount; }
    ~CRefPtr() { if (m_p && --m_p->RefCount == 0) m_p->Release(); }
    T* operator->() const { return m_p; }
};
struct __true_type {};
struct CTriangleListArray {
    CRefPtr<CTriangleList>* _M_start;
    CRefPtr<CTriangleList>* _M_finish;
    CRefPtr<CTriangleList>* _M_end_of_storage;
    void _M_insert_overflow(CRefPtr<CTriangleList>* pos, const CRefPtr<CTriangleList>& x, const __true_type&, unsigned int n, bool atEnd);   // 0x00A97BE0
    void push_back(const CRefPtr<CTriangleList>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CRefPtr<CTriangleList>(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CChunkedFileChunk { virtual void v00(); };
struct C3DMeshFileStripChunk : CChunkedFileChunk {
    unsigned char _pad04[0x10 - 4];
    CRetailVector<unsigned short> Indices;  // +0x10
};
struct C3DMeshFileTrianglesChunk {
    const CTriangle* GetTriangles();        // 0x00ABC440
    unsigned int GetNoTriangles();          // 0x00ABA370
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;   // 0x00AA7F40
};
struct CDeviceIndependent {
    unsigned char _pad00[0x10];
    CTriangleListArray TriangleLists;       // +0x10
    void AddTriangleList(C3DMeshFileTrianglesChunk* chunk);
};

void CDeviceIndependent::AddTriangleList(C3DMeshFileTrianglesChunk* chunk)
{
    CTriangleList* list = new CTriangleList;
    CRefPtr<CTriangleList> pList(list);
    const CTriangle* triangles = chunk->GetTriangles();
    unsigned int count = chunk->GetNoTriangles();
    list->Triangles.resize(count);
    for (unsigned int i = 0; i < count; ++i)
        list->Triangles[i] = triangles[i];
    CCountedPointer<C3DMeshFileStripChunk> strips = chunk->FindFirstSubChunkOfType("XTST");
    if (strips) {
        unsigned int n = strips->Indices.size();
        list->StripIndices.resize(n);
        for (unsigned int j = 0; j < n; ++j)
            list->StripIndices[j] = strips->Indices[j];
    }
    TriangleLists.push_back(pList);
}
