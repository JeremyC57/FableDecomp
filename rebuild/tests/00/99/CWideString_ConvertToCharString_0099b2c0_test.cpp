#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define FABLE_WIDE_STRING_HOST_TYPES
struct CCharStringData { char* text; };
class CCharString {
    CCharStringData* storage_;
public:
    CCharString(const char*, long);
    ~CCharString();
    operator const char*() const;
};
extern wchar_t g_FableEmptyWideString_0129A8E0[1];
#endif
static void* testAllocate(size_t);
static void testFree(void*);
static void* testZero(void*, int, size_t);
#define malloc testAllocate
#define free testFree
#define memset testZero
// CWideString::ConvertToCharString() -- truncate UTF-16 code units into an owned narrow string
// Retail 0x0099B2C0,177 bytes, ending ret4 at0099B36E. malloc00BFEA0E/free00BFEA14,
// CCharString text/length constructor0099EBF0. Shared header lacks this method and keeps
// storage private; its unknown04/08 words are the vector Finish/End pointers in this body.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
#include <stdlib.h>
#include <string.h>
struct CWideVectorStorageView {
    wchar_t* Start;
    wchar_t* Finish;
    wchar_t* End;
    long Owners;
};
struct CTemporaryNarrowBuffer {
    char* Text;
    CTemporaryNarrowBuffer(long size) : Text(size ? (char*)malloc(size) : 0) {
        memset(Text, 0, size);
    }
    ~CTemporaryNarrowBuffer() { if (Text) free(Text); }
};
struct CWideStringConversionView {
    CWideVectorStorageView* Storage;
    long GetLength() const { return !Storage ? 0 : Storage->Finish - Storage->Start; }
    const wchar_t* GetText() const { return !Storage ? g_FableEmptyWideString_0129A8E0 : Storage->Start; }
    CCharString ConvertToCharString() const;
};
CCharString CWideStringConversionView::ConvertToCharString() const
{
    CTemporaryNarrowBuffer buffer(GetLength() + 1);
    for (long i = 0; i < GetLength(); ++i)
        buffer.Text[i] = (char)GetText()[i];
    buffer.Text[GetLength()] = 0;
    return CCharString(buffer.Text, -1);
}

#undef malloc
#undef free
#undef memset
wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};
static int bad,allocCalls,freeCalls,zeroCalls,constructCalls,destroyCalls;
static size_t expectedSize;
static void* allocated;
static const unsigned char* expected;
static size_t expectedLength;
static CCharStringData returnedData;
static char returnedText[128];
static void* testAllocate(size_t n) {
    ++allocCalls;
    if (n!=expectedSize || zeroCalls || constructCalls || allocated) bad=1;
    allocated=malloc(n);
    if (!allocated) exit(2);
    memset(allocated,0xa5,n);
    return allocated;
}
static void* testZero(void* p,int value,size_t n) {
    ++zeroCalls;
    if (allocCalls!=1 || p!=allocated || value || n!=expectedSize || constructCalls) bad=1;
    return memset(p,value,n);
}
static void testFree(void* p) {
    ++freeCalls;
    if (p!=allocated || constructCalls!=1 || destroyCalls || zeroCalls!=1) bad=1;
    free(p);allocated=0;
}
CCharString::CCharString(const char* text,long length) {
    ++constructCalls;
    if (text!=allocated || length!=-1 || freeCalls || zeroCalls!=1) bad=1;
    for (size_t i=0;i<expectedLength;++i)
        if ((unsigned char)text[i]!=expected[i]) bad=1;
    if (text[expectedLength]) bad=1;
    memcpy(returnedText,text,strlen(text)+1);
    returnedData.text=returnedText;
    storage_=&returnedData;
}
CCharString::~CCharString() { ++destroyCalls; }
CCharString::operator const char*() const { return storage_->text; }
static int run(wchar_t* text,long count,int nullStorage,const unsigned char* bytes) {
    bad=allocCalls=freeCalls=zeroCalls=constructCalls=destroyCalls=0;
    allocated=0;expected=bytes;expectedLength=count;expectedSize=count+1;
    CWideVectorStorageView data;
    data.Start=text;data.Finish=text+count;data.End=text+count+1;data.Owners=17;
    CWideStringConversionView wide;wide.Storage=nullStorage ? 0 : &data;
    {
        CCharString result=wide.ConvertToCharString();
        const char* converted=result;
        if (allocCalls!=1 || freeCalls!=1 || zeroCalls!=1 || constructCalls!=1 || allocated || bad)
            return 0;
        size_t actualLength=0;
        while (actualLength<expectedLength && bytes[actualLength]) ++actualLength;
        for (size_t i=0;i<actualLength;++i)
            if ((unsigned char)converted[i]!=bytes[i]) return 0;
        if (converted[actualLength] || data.Owners!=17 || data.Start!=text || data.Finish!=text+count)
            return 0;
    }
    return !bad && destroyCalls>=1;
}
int main() {
    wchar_t empty[1]={0}; unsigned char noBytes[1]={0};
    wchar_t ascii[4]={L'A',L'b',L'9',0};unsigned char asciiBytes[3]={'A','b','9'};
    wchar_t unicode[5]={0x1234,0x00ff,0x20ac,0xd852,0};unsigned char lowBytes[4]={0x34,0xff,0xac,0x52};
    wchar_t embedded[4]={L'A',0,L'Z',0};unsigned char embeddedBytes[3]={'A',0,'Z'};
    if (!run(empty,0,1,noBytes) || !run(empty,0,0,noBytes) || !run(ascii,3,0,asciiBytes) ||
        !run(unicode,4,0,lowBytes) || !run(embedded,3,0,embeddedBytes)) {
        printf("0099b2c0_TEST FAIL\n");return 1;
    }
    printf("0099b2c0_TEST PASS\n");return 0;
}