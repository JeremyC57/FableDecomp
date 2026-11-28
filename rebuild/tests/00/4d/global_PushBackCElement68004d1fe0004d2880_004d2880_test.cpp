#define FABLE_PORTABLE
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_CARRAY_PUSHBACK
#include "engine/CArray.h"
#include <stdio.h>

class CSubtitledMessage {
public:
    explicit CSubtitledMessage(long value = 0) : Index(value) { ++Live; }
    CSubtitledMessage(const CSubtitledMessage& source) : Index(source.Index) { ++Live; }
    ~CSubtitledMessage() { --Live; }
    long Index;
    static int Live;
};
int CSubtitledMessage::Live = 0;

int main()
{
    {
        CArray<CSubtitledMessage> messages;
        messages.reserve(2);
        CSubtitledMessage value(17);
        messages.push_back(value);
        value.Index = 32;
        messages.push_back(value);
        if (messages.size() != 2 || messages[0].Index != 17 || messages[1].Index != 32)
            return 1;
        messages.push_back(messages[0]);
        if (messages.size() != 3 || messages[0].Index != 17 ||
            messages[1].Index != 32 || messages[2].Index != 17)
            return 2;
    }
    if (CSubtitledMessage::Live != 0)
        return 3;
    puts("004d2880_TEST PASS");
    return 0;
}
