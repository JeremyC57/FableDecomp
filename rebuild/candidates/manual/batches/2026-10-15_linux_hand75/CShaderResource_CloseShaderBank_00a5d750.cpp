// CShaderResource::CloseShaderBank() -- deletes every non-null shader of the array (virtual deleting destructor, flag 1), nulls its slot, then clears the shader bank pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D750, length 69 bytes.
// Callees: CShaderBase's virtual scalar deleting destructor (slot 0, `push 1 ; call [edx]`).
// Shape: an unsigned index in edi against `(last - first) >> 2` re-computed after every iteration (`jb`), the
// slot re-read through `first` after the call for the null store, `mov [esi+4],0` after the loop.
struct CShaderDataBank;

struct CShaderBase {
    virtual ~CShaderBase();
};

template <class T> struct CArray {              // retail CArray: first, last, end (12 bytes)
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};

struct CShaderResource {
    void*                 __vftable;            // +0x00
    CShaderDataBank*      ShaderBank;           // +0x04
    CArray<CShaderBase*>  Shaders;              // +0x08
    void CloseShaderBank();
};

void CShaderResource::CloseShaderBank()
{
    for (unsigned long i = 0; i < Shaders.Size(); ++i) {
        if (Shaders[i]) {
            delete Shaders[i];
            Shaders[i] = 0;
        }
    }
    ShaderBank = 0;
}
