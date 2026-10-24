// Behaviour test for 0x0057F6E9 CPersistContext_TransferableVector<ECategory>::TransferIn: the long reader fills a
// local that is then stored into first[index]; the vector's data pointer is read before the reader runs.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CPersistContext { int id; };
struct CStringParser { long next; };
struct CCharString { int id; };

static int g_calls = 0; static CPersistContext* g_ctx = 0;
void __fastcall TransferEnumIn_009e2590(CPersistContext* context, CStringParser* parser, long* value)
{
    ++g_calls; g_ctx = context;
    *value = parser->next;
}

typedef long ECategory;
template <class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; };

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<ECategory>* TransVector;
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    ECategory* first = TransVector->m_pFirst;
    long value;
    TransferEnumIn_009e2590(context, parser, &value);
    first[index] = value;
}
long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long) { }

int main()
{
    ECategory cats[4] = { 1, 2, 3, 4 };
    CRetailVector<ECategory> vec; vec.m_pFirst = cats; vec.m_pLast = cats + 4; vec.m_pEnd = cats + 4;
    CPersistContext_TransferableVector wrapper; wrapper.TransVector = &vec;
    CPersistContext_TransferableVector* hook = &wrapper;
    CPersistContext context; CStringParser parser;
    parser.next = 42; hook->TransferIn(&context, &parser, 2);
    parser.next = -7; hook->TransferIn(&context, &parser, 0);
    if (g_calls != 2 || g_ctx != &context || cats[0] != -7 || cats[1] != 2 || cats[2] != 42 || cats[3] != 4) {
        std::printf("0057f6e9_TEST FAIL\n"); return 1;
    }
    std::printf("0057f6e9_TEST PASS\n");
    return 0;
}