// behaviour test for 0x0043F750
#include <stdio.h>
static int g_dtors = 0; static void* g_last = 0;
class CWideString {
public:
    void* PData;
    ~CWideString() { ++g_dtors; g_last = this; }  //                                        // 0x0099B510
};
class CRecordWithWideStringAt10 {
public:
    unsigned char _pad00[0x10];
    CWideString Text;                                         // +0x10
    ~CRecordWithWideStringAt10() {}
};
void DeleteCRecordWithWideStringAt10(CRecordWithWideStringAt10* p)                 // emission helper, not a retail function
{
    delete p;
}

int main()
{
    CRecordWithWideStringAt10* p = new CRecordWithWideStringAt10; char* text = (char*)&p->Text;
    int ok = text - (char*)p == sizeof(p->_pad00);
    DeleteCRecordWithWideStringAt10(p);
    ok &= g_dtors == 1 && g_last == text;
    printf(ok ? "0043f750_TEST PASS\n" : "0043f750_TEST FAIL\n");
    return ok ? 0 : 1;
}