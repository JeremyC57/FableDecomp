// Behaviour test for 0x0057F754 CPersistContext_TransferableVector<CHeroMarriageInfo>::TransferOut: a zeroed default marriage record, &first[index].
#include <cstdio>
#ifndef _MSC_VER
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

class CPersistContext { public: int id; };
class CStringParser { public: int id; };
class CCharString { public: int id; };
class CHeroMarriageInfo { public:
    u64           WifeUID;
    u64           MaritalHomeUID;
    unsigned char State[0x4];
    bool          GayMarriage;
    unsigned char _pad_0x15[0x3];
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<CHeroMarriageInfo> {
    CHeroMarriageInfo DefaultVal;
    CPersistTraits()
    {
        DefaultVal.WifeUID = 0;
        DefaultVal.MaritalHomeUID = 0;
        *(long*)DefaultVal.State = 0;
        DefaultVal.GayMarriage = false;
    }
    void TransferOut(CPersistContext* context, CCharString* out, CHeroMarriageInfo* value);
    void TransferIn(CPersistContext* context, CStringParser* parser, CHeroMarriageInfo* value);
};

static int g_calls = 0, g_defaultsOk = 0;
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static CHeroMarriageInfo* g_value = 0;

void CPersistTraits<CHeroMarriageInfo>::TransferOut(CPersistContext* context, CCharString* arg, CHeroMarriageInfo* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.WifeUID == 0 && DefaultVal.MaritalHomeUID == 0 && *(long*)DefaultVal.State == 0 && !DefaultVal.GayMarriage;
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CHeroMarriageInfo>* TransVector;
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CHeroMarriageInfo> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) { }


int main()
{
    CHeroMarriageInfo elems[3];
    CRetailVector<CHeroMarriageInfo> vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CCharString arg;
    wrapper.TransferOut(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferOut(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(1) || hook->GetCount() != 3) {
        std::printf("0057f754_TEST FAIL\n"); return 1;
    }
    std::printf("0057f754_TEST PASS\n");
    return 0;
}