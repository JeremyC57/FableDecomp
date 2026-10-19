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