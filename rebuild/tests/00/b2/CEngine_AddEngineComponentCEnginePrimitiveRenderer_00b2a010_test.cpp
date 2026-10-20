// Behaviour test for 00b2a010: body copied from src/00b2a010.cpp (CEngine::AddEngineComponent<CEnginePrimitiveRenderer>), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCtor = 0, g_strDtor = 0; static const char* g_strText = 0; static long g_strLen = 0;
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length) : m_pData((void*)text) { ++g_strCtor; g_strText = text; g_strLen = length; }
    ~CCharString() { ++g_strDtor; m_pData = 0; }
};
static int g_progress = 0; static float g_progressValue = 0; static const char* g_progressText = 0;
static void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{ ++g_progress; g_progressValue = progress; g_progressText = (const char*)text.m_pData; CHECK(!a && !b); }
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
static int g_setGlobal = 0, g_announce = 0;
struct CEngineComponent {
    virtual void Dummy() { }
    void** GlobalPointerAddress; bool RenderEnabled; unsigned char KeepAlive;
    void SetGlobalPointerAddress(void** address) { ++g_setGlobal; GlobalPointerAddress = address; }
    void Announce() { ++g_announce; }
};
struct CEnginePrimitiveRendererBody { virtual void Body() { } unsigned char _body[0x264 - 4]; };
struct CEnginePrimitiveRenderer : CEnginePrimitiveRendererBody, CEngineComponent { unsigned char _tail[0x350 - 0x274]; };
static int g_overflow = 0;
struct CEngineComponentVector {
    CEngineComponent** _M_start; CEngineComponent** _M_finish; CEngineComponent** _M_end_of_storage;
    void _M_insert_overflow(CEngineComponent** position, CEngineComponent* const& x, const __true_type&, unsigned int fill, bool atEnd)
    { ++g_overflow; CHECK(position == _M_finish && fill == 1 && atEnd); *_M_finish = x; ++_M_finish; ++_M_end_of_storage; }
    void push_back(CEngineComponent* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CEngineComponent*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CEngine {
    unsigned char _pad000[0x168];
    CEngineComponentVector EngineComponents;
    template <class T> void AddEngineComponent(T* component, T*& globalPointer, unsigned char flag);
};
template <class T> void CEngine::AddEngineComponent(T* component, T*& globalPointer, unsigned char flag)
{
    {
        CCharString text("Engine: Add Engine Component", -1);
        DisplayProgress(text, -1.0f, false, false);
    }
    globalPointer = component;
    component->SetGlobalPointerAddress(reinterpret_cast<void**>(&globalPointer));
    EngineComponents.push_back(component);
    component->Announce();
}
template void CEngine::AddEngineComponent<CEnginePrimitiveRenderer>(CEnginePrimitiveRenderer*, CEnginePrimitiveRenderer*&, unsigned char);
int main()
{
    CEngine engine; CEngineComponent* storage[4]; memset(storage, 0, sizeof storage);
    engine.EngineComponents._M_start = storage; engine.EngineComponents._M_finish = storage; engine.EngineComponents._M_end_of_storage = storage + 1;
    CEnginePrimitiveRenderer r1, r2; CEnginePrimitiveRenderer* global = 0;
    engine.AddEngineComponent<CEnginePrimitiveRenderer>(&r1, global, 0);
    CHECK(g_strCtor == 1 && g_strDtor == 1 && g_strLen == -1 && strcmp(g_strText, "Engine: Add Engine Component") == 0);
    CHECK(g_progress == 1 && g_progressValue == -1.0f && g_progressText == g_strText);
    CHECK(global == &r1 && g_setGlobal == 1 && r1.GlobalPointerAddress == (void**)&global);
    CHECK(engine.EngineComponents._M_finish - storage == 1 && storage[0] == static_cast<CEngineComponent*>(&r1) && g_overflow == 0 && g_announce == 1);
    engine.AddEngineComponent<CEnginePrimitiveRenderer>(&r2, global, 1);        // vector full: overflow path
    CHECK(global == &r2 && g_overflow == 1 && storage[1] == static_cast<CEngineComponent*>(&r2) && g_announce == 2 && g_strDtor == 2);
    CHECK((void*)storage[1] != (void*)&r2);                                     // the base-adjusted pointer is what is stored
    if (fails == 0) printf("00b2a010_TEST PASS\n"); else printf("00b2a010_TEST FAIL (%d)\n", fails);
    return fails;
}