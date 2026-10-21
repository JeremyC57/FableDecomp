// behaviour test for 0x0043F980
#include <stdio.h>
static int g_dtors = 0; static void* g_last = 0;
class CCharString {
public:
    void* PData;
    ~CCharString() { ++g_dtors; g_last = this; }  //                                        // 0x0099EAE0
};
class CRecordWithCharStringAtC {
public:
    unsigned char _pad00[0xc];
    CCharString Text;                                         // +0xc
    ~CRecordWithCharStringAtC() {}
};
void DeleteCRecordWithCharStringAtC(CRecordWithCharStringAtC* p)                 // emission helper, not a retail function
{
    delete p;
}

int main()
{
    CRecordWithCharStringAtC* p = new CRecordWithCharStringAtC; char* text = (char*)&p->Text;
    int ok = text - (char*)p == sizeof(p->_pad00);
    DeleteCRecordWithCharStringAtC(p);
    ok &= g_dtors == 1 && g_last == text;
    printf(ok ? "0043f980_TEST PASS\n" : "0043f980_TEST FAIL\n");
    return ok ? 0 : 1;
}