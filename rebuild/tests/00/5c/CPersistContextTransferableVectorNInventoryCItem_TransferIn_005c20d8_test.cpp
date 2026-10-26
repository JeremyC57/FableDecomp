// Behaviour test for 0x005C20D8 CPersistContext_TransferableVector<NInventory::CItem>::TransferIn: a zeroed traits item, &first[index], one counted-pointer destructor per call.
#include <cstdio>
#ifndef _MSC_VER
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

class CPersistContext { public: int id; };
class CStringParser { public: int id; };
class CCharString { public: int id; };
static int g_pointerDtors = 0;
struct CPerItemData; class CCPPointerInfo;
struct CCountedPointer_CPerItemData {
    CPerItemData*   Data;
    CCPPointerInfo* Info;
    CCountedPointer_CPerItemData() : Data(0), Info(0) { }
    ~CCountedPointer_CPerItemData() { ++g_pointerDtors; }
};
namespace NInventory {
class CItem { public:
    long                         ItemType;
    long                         NumberOfItems;
    CCountedPointer_CPerItemData PPerItemData;
    long                         AverageBoughtPrice;
    CItem() : ItemType(0), NumberOfItems(0), PPerItemData(), AverageBoughtPrice(0) { }
};
}
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
};

static int g_calls = 0, g_defaultsOk = 0;
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static NInventory::CItem* g_value = 0;

template <>
void CPersistTraits<NInventory::CItem>::TransferIn(CPersistContext* context, CStringParser* arg, NInventory::CItem* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.ItemType == 0 && DefaultVal.NumberOfItems == 0 && DefaultVal.PPerItemData.Data == 0 && DefaultVal.PPerItemData.Info == 0 && DefaultVal.AverageBoughtPrice == 0;
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<NInventory::CItem>* TransVector;
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<NInventory::CItem> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long) { }


int main()
{
    NInventory::CItem elems[3];
    CRetailVector<NInventory::CItem> vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CStringParser arg;
    wrapper.TransferIn(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferIn(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(g_pointerDtors == 2) || hook->GetCount() != 3) {
        std::printf("005c20d8_TEST FAIL\n"); return 1;
    }
    std::printf("005c20d8_TEST PASS\n");
    return 0;
}