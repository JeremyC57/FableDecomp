// Behaviour test for 0x00411250 CPersistContext_TransferableVector<CActionInputControl>::TransferIn: a zeroed default binding, &first[index].
#include <cstdio>
#ifndef _MSC_VER
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { int id; };
class C2DVector { public: float X; float Y; };
struct CActionInputControl {
    long      GameAction;
    long      ControllerType;
    long      KeyboardKey;
    long      XboxButton;
    long      MouseButton;
    C2DVector ControlDirection;
    CActionInputControl() : GameAction(0), ControllerType(0), KeyboardKey(0), XboxButton(0), MouseButton(0)
    {
        ControlDirection.X = 0.0f;
        ControlDirection.Y = 0.0f;
    }
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
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static CActionInputControl* g_value = 0;

template <>
void CPersistTraits<CActionInputControl>::TransferIn(CPersistContext* context, CStringParser* arg, CActionInputControl* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.GameAction == 0 && DefaultVal.ControllerType == 0 && DefaultVal.KeyboardKey == 0 && DefaultVal.XboxButton == 0 && DefaultVal.MouseButton == 0 && DefaultVal.ControlDirection.X == 0.0f && DefaultVal.ControlDirection.Y == 0.0f;
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CActionInputControl>* TransVector;
};

void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CActionInputControl> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferOut(CPersistContext*, CCharString*, long) { }


int main()
{
    CActionInputControl elems[3];
    CRetailVector<CActionInputControl> vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CStringParser arg;
    wrapper.TransferIn(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferIn(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(1) || hook->GetCount() != 3) {
        std::printf("00411250_TEST FAIL\n"); return 1;
    }
    std::printf("00411250_TEST PASS\n");
    return 0;
}