// CShaderResource::OpenShaderBank(const CShaderDataBank* bank) -- remember the shader bank, size the shader array to the bank's highest entry index + 1 and null every slot
// Leaf: ?OpenShaderBank@CShaderResource@@QAEXPBVCShaderDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D7C0, 60 bytes (+4 int3), `ret 4`.
// Callees: CBankFile::GetMaxEntryIndex() const @ 0x009CC350, vector<CShaderBase*>::resize(size_type, const T&) @ 0x00A5E6A0
//   (the null fill value homed in the dead bank argument slot); the nulling loop is a `rep stosd`.
// Callers: 0x009D9BB5, 0x00B3B691, 0x00B3B791, 0x00B3D71E.
// Label: the manifest's `CResourceIndex_InitializeFromBank` is a guess; FableWin.pdb names
//   `?OpenShaderBank@CShaderResource@@QAEXPBVCShaderDataBank@@@Z` (its neighbours 0x00A5D750 CloseShaderBank etc. are landed).
// Layout (Ego_r.pdb, as the landed CloseShaderBank @ 0x00A5D750): vptr +0, ShaderBank +4, Shaders (CArray<CShaderBase*>,
//   12-byte STLport vector) +8.
// Shape: the nulling loop runs over a local copy of the array start (an indexed Shaders[i] re-reads the vector's first
//   pointer after every store and stays a loop; the hoisted pointer becomes rep stosd).
class CBankFile {
public:
    long GetMaxEntryIndex() const;              // 0x009CC350
};
class CShaderDataBank : public CBankFile {
};
class CShaderBase;

template <class T> class CArray {               // retail CArray: STLport vector first, last, end (12 bytes)
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    void resize(unsigned long n, const T& x);   // 0x00A5E6A0
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};

class CShaderResource {
public:
    virtual ~CShaderResource();
    const CShaderDataBank* ShaderBank;          // +0x04
    CArray<CShaderBase*>   Shaders;             // +0x08
    void OpenShaderBank(const CShaderDataBank* bank);
};

void CShaderResource::OpenShaderBank(const CShaderDataBank* bank)
{
    ShaderBank = bank;
    unsigned long count = bank->GetMaxEntryIndex() + 1;
    Shaders.resize(count, 0);
    CShaderBase** shaders = &Shaders[0];
    for (unsigned long i = 0; i < count; ++i)
        shaders[i] = 0;
}