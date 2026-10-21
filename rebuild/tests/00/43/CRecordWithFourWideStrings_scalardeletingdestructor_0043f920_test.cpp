#include <cstdio>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
class CRecordWithFourWideStrings {
public:
    CWideString Text0;                                   // +0x00
    CWideString Text1;                                   // +0x04
    CWideString Text2;                                   // +0x08
    CWideString Text3;                                   // +0x0c
    ~CRecordWithFourWideStrings() {}
};
void DeleteCRecordWithFourWideStrings(CRecordWithFourWideStrings* p)      // emission helper, not a retail function
{
    delete p;
}

static int g_dtors; static void* g_order[4];
CWideString::~CWideString() { if (g_dtors < 4) g_order[g_dtors] = this; ++g_dtors; }
int main()
{
    CRecordWithFourWideStrings* p = new CRecordWithFourWideStrings;
    void* a0 = &p->Text0; void* a1 = &p->Text1; void* a2 = &p->Text2; void* a3 = &p->Text3;
    bool ok = sizeof(CRecordWithFourWideStrings) == 16 && (char*)a3 - (char*)p == 12;
    DeleteCRecordWithFourWideStrings(p);
    ok = ok && g_dtors == 4 && g_order[0] == a3 && g_order[1] == a2 && g_order[2] == a1 && g_order[3] == a0;
    std::printf(ok ? "0043f920_TEST PASS\n" : "FAIL\n");
    return 0;
}