#include <cstdio>
#include <cstring>
// CPersistContext::TransferObjectSaveText: "<name> " + the component's own text + ";\r\n" appended to PStringToSaveTo
struct CCharString {
    char buf[128];
    CCharString() { buf[0] = 0; }
    const CCharString& operator+=(const char* text) { std::strcat(buf, text); return *this; }
};
struct CPersistContext;
struct TransferableComponentGenericType {
    virtual void TransferSaveText(CPersistContext* context, CCharString* text) = 0;
};
struct CPersistContext {
    void* p[8];
    CCharString* PStringToSaveTo;
    void TransferObjectSaveText(const char* name, TransferableComponentGenericType* component)
    {
        *PStringToSaveTo += name;
        *PStringToSaveTo += " ";
        component->TransferSaveText(this, PStringToSaveTo);
        *PStringToSaveTo += ";\r\n";
    }
};
static CPersistContext* g_seen = 0;
struct CTestComponent : TransferableComponentGenericType {
    virtual void TransferSaveText(CPersistContext* context, CCharString* text) { g_seen = context; *text += "{ 1 }"; }
};
int main()
{
    CCharString out; CPersistContext c; c.PStringToSaveTo = &out; CTestComponent comp;
    c.TransferObjectSaveText("Health", &comp);
    if (std::strcmp(out.buf, "Health { 1 };\r\n") != 0 || g_seen != &c) { std::printf("009bad60_TEST FAIL\n"); return 1; }
    std::printf("009bad60_TEST PASS\n");
    return 0;
}
