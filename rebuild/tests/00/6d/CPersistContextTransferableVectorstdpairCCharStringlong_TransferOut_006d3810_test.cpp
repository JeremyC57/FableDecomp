// Behaviour test for 0x006D3810 CPersistContext_TransferableVector<std::pair<CCharString,long> >::TransferOut: traits default ("", 0) built from an implicit "" temporary (destroyed before the call), &first[index], the traits string destroyed after.
#include <cstdio>
#ifndef _MSC_VER
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

class CPersistContext { public: int id; };
class CStringParser { public: int id; };
static int g_strings = 0, g_emptyStrings = 0;
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { ++g_strings; if (text[0] == 0 && length == -1) ++g_emptyStrings; }
    CCharString(const CCharString& other) : Text(other.Text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair() : first(""), second(0) { }      // test-only: lets the test build an element array
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<pair<CCharString, long> > {
    pair<CCharString, long> DefaultVal;
    CPersistTraits() : DefaultVal("", 0) { }
    void TransferOut(CPersistContext* context, CCharString* out, pair<CCharString, long>* value);
    void TransferIn(CPersistContext* context, CStringParser* parser, pair<CCharString, long>* value);
};

static int g_calls = 0, g_defaultsOk = 0;
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static pair<CCharString, long>* g_value = 0;

void CPersistTraits<pair<CCharString, long> >::TransferOut(CPersistContext* context, CCharString* arg, pair<CCharString, long>* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.first.Text[0] == 0 && DefaultVal.second == 0 && g_strings == 5 && (g_emptyStrings == 4 || g_emptyStrings == 5);
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<pair<CCharString, long> >* TransVector;
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<pair<CCharString, long> > traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) { }


int main()
{
    pair<CCharString, long> elems[3];
    CRetailVector<pair<CCharString, long> > vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CCharString arg("x");
    wrapper.TransferOut(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferOut(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(g_strings == 4 && g_emptyStrings == 5) || hook->GetCount() != 3) {
        std::printf("006d3810_TEST FAIL\n"); return 1;
    }
    std::printf("006d3810_TEST PASS\n");
    return 0;
}