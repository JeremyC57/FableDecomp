#include <cstdio>
#include <cstring>
// CPersistContext::TransferSubComponentSaveText: "<name>." then the sub-component writes its own text
struct CCharString {
    char buf[128];
    CCharString() { buf[0] = 0; }
    const CCharString& operator+=(const char* text) { std::strcat(buf, text); return *this; }
};
struct CPersistContext;
struct TransferableSubComponentGenericType {
    virtual void TransferSaveText(CPersistContext* context) = 0;
};
struct CPersistContext {
    void* p[8];
    CCharString* PStringToSaveTo;
    void TransferSubComponentSaveText(const char* name, TransferableSubComponentGenericType* sub)
    {
        *PStringToSaveTo += name;
        *PStringToSaveTo += ".";
        sub->TransferSaveText(this);
    }
};
struct CTestSub : TransferableSubComponentGenericType {
    virtual void TransferSaveText(CPersistContext* context) { *context->PStringToSaveTo += "Speed 3;\r\n"; }
};
int main()
{
    CCharString out; CPersistContext c; c.PStringToSaveTo = &out; CTestSub sub;
    c.TransferSubComponentSaveText("Movement", &sub);
    if (std::strcmp(out.buf, "Movement.Speed 3;\r\n") != 0) { std::printf("009bada0_TEST FAIL\n"); return 1; }
    std::printf("009bada0_TEST PASS\n");
    return 0;
}
