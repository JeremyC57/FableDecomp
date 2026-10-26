// Behaviour test for 0x009A0190 (CBasicString<char> / CCharStringData destructor)
#include <stdio.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    T*            PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    ~CBasicString()
    {
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        StringLength = 0;
        DataLength = 0;
    }
};
class CCharStringData { public:
    CBasicString<char> Data;
    long               NoRefs;
    ~CCharStringData() { }
};
#pragma pack(pop)
int main()
{
    int ok = 1;
    CCharStringData* d = new CCharStringData;
    d->Data.PData = new char[8]; d->Data.StringLength = 3; d->Data.DataLength = 8; d->NoRefs = 2;
    d->~CCharStringData();
    if (d->Data.PData != 0 || d->Data.StringLength != 0 || d->Data.DataLength != 0 || d->NoRefs != 2) ok = 0;
    CBasicString<char>* s = new CBasicString<char>;
    s->PData = 0; s->StringLength = 4; s->DataLength = 9;
    s->~CBasicString<char>();
    if (s->PData != 0 || s->StringLength != 0 || s->DataLength != 0) ok = 0;
    operator delete(d); operator delete(s);
    printf(ok ? "009a0190_TEST PASS\n" : "009a0190_TEST FAIL\n");
    return ok ? 0 : 1;
}