// Behaviour test for 0x00614000 CPersistContext_TransferableVector<CTCInventoryMap::CUIMiniMapMarker>::TransferOut: a default marker with an empty UIDefName, &first[index], the traits string destroyed after the call.
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
class C2DVector { public: float X; float Y; C2DVector() : X(0.0f), Y(0.0f) { } };
struct CUIMiniMapMarker {
    long        Region;
    CCharString UIDefName;
    C2DVector   Position;
    long        DefNameLength;
    long        GraphicIndex;
    float       Angle;
    CUIMiniMapMarker() : Region(0), UIDefName(""), Position(), DefNameLength(0), GraphicIndex(0), Angle(0.0f) { }
};
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
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static CUIMiniMapMarker* g_value = 0;

template <>
void CPersistTraits<CUIMiniMapMarker>::TransferOut(CPersistContext* context, CCharString* arg, CUIMiniMapMarker* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.Region == 0 && DefaultVal.UIDefName.Text[0] == 0 && DefaultVal.Position.X == 0.0f && DefaultVal.Position.Y == 0.0f && DefaultVal.DefNameLength == 0 && DefaultVal.GraphicIndex == 0 && DefaultVal.Angle == 0.0f && g_strings == 5;
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CUIMiniMapMarker>* TransVector;
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CUIMiniMapMarker> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) { }


int main()
{
    CUIMiniMapMarker elems[3];
    CRetailVector<CUIMiniMapMarker> vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CCharString arg("x");
    wrapper.TransferOut(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferOut(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(g_strings == 4) || hook->GetCount() != 3) {
        std::printf("00614000_TEST FAIL\n"); return 1;
    }
    std::printf("00614000_TEST PASS\n");
    return 0;
}