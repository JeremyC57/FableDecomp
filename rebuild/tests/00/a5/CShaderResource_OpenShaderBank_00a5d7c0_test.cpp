// Behaviour test for 0x00A5D7C0 CShaderResource::OpenShaderBank(const CShaderDataBank*)
#include <cstdio>
#include <cstdlib>

class CBankFile {
public:
    long MaxEntryIndex;
    long GetMaxEntryIndex() const { return MaxEntryIndex; }    // 0x009CC350 stand-in
};
class CShaderDataBank : public CBankFile {
};
class CShaderBase {
public:
    int Dummy;
};

template <class T> class CArray {
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CArray() { std::free(m_pFirst); }
    void resize(unsigned long n, const T& x)                    // 0x00A5E6A0 stand-in: keeps old elements, fills new
    {
        unsigned long old = (unsigned long)(m_pLast - m_pFirst);
        if (n > (unsigned long)(m_pEnd - m_pFirst)) {
            T* mem = (T*)std::malloc(n * sizeof(T));
            for (unsigned long i = 0; i < old; ++i) mem[i] = m_pFirst[i];
            std::free(m_pFirst);
            m_pFirst = mem;
            m_pEnd = mem + n;
        }
        for (unsigned long i = old; i < n; ++i) m_pFirst[i] = x;
        m_pLast = m_pFirst + n;
    }
    T& operator[](unsigned long i) { return m_pFirst[i]; }
};

class CShaderResource {
public:
    const CShaderDataBank* ShaderBank;
    CArray<CShaderBase*>   Shaders;
    void OpenShaderBank(const CShaderDataBank* bank)
    {
        ShaderBank = bank;
        unsigned long count = bank->GetMaxEntryIndex() + 1;
        Shaders.resize(count, 0);
        CShaderBase** shaders = &Shaders[0];
        for (unsigned long i = 0; i < count; ++i)
            shaders[i] = 0;
    }
};

int main()
{
    CShaderBase s;
    CShaderResource r;
    r.ShaderBank = 0;
    r.Shaders.resize(3, &s);                            // stale shaders from an earlier bank
    CShaderDataBank bank;
    bank.MaxEntryIndex = 5;
    r.OpenShaderBank(&bank);
    bool ok = r.ShaderBank == &bank && r.Shaders.m_pLast - r.Shaders.m_pFirst == 6;
    for (unsigned long i = 0; ok && i < 6; ++i)
        ok = r.Shaders[i] == 0;
    if (!ok) { std::printf("00a5d7c0_TEST FAIL\n"); return 1; }
    std::printf("00a5d7c0_TEST PASS\n");
    return 0;
}