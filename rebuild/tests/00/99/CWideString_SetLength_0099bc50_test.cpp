#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __cdecl
#endif
static int bad, allocs, deletes, frees, failNext, ownCalls, resizeCalls, initCalls;
static void* lastFreedText;
void* operator new(size_t count)
{
    ++allocs;
    if (failNext) { failNext = 0; return 0; }
    return malloc(count);
}
#ifndef _MSC_VER
#define FABLE_WIDE_STRING_HOST_TYPES
// Native-width dependency adapter only; VC7.1 uses the real shared header.
typedef unsigned int fable_u32;
struct CWideStringData {wchar_t* text;unsigned int unknown04,unknown08;int owners;};
#endif
// CWideString::SetLength(long) -- clear, resize a private copy, or allocate space-filled string storage
// Retail 0x0099BC50, 162 bytes. String/data layouts use fable_string.h; that header omits
// SetLength and keeps storage_ private, so this method view preserves its one-pointer ABI.
// MakeOwnCopyOfData0099B560, resize0099CD60, initialize0099CA70; free00BFEA14/delete00BFE9BC.
#ifndef FABLE_WIDE_STRING_HOST_TYPES
#include "fable_string.h"
#endif
extern "C" void __cdecl FreeWideCharacters(void*); // 0x00BFEA14
struct CWideAllocator { CWideAllocator() {} };
struct CWideStringDataActions : CWideStringData {
    long RemoveReference() { return --owners; }
    void Initialize(long length, wchar_t fill, const CWideAllocator&); // 0x0099CA70
    void Resize(long length, wchar_t fill); // 0x0099CD60
    CWideStringDataActions(long length, wchar_t fill, const CWideAllocator& allocator) {
        Initialize(length, fill, allocator);
        owners = 1;
    }
    ~CWideStringDataActions() { if (text) FreeWideCharacters(text); }
};
struct CWideStringLengthView {
    CWideStringDataActions* storage_;
    void MakeOwnCopyOfData(); // 0x0099B560
    void SetLength(long);
};
void CWideStringLengthView::SetLength(long length)
{
    if (length == 0) {
        if (storage_) {
            long owners = storage_->RemoveReference();
            if (owners <= 0) delete storage_;
            storage_ = 0;
        }
    } else if (storage_) {
        MakeOwnCopyOfData();
        storage_->Resize(length, 0);
    } else {
        storage_ = new CWideStringDataActions(length, L' ', CWideAllocator());
    }
}

void operator delete(void* pointer) throw()
{
    if (!pointer) return;
    CWideStringDataActions* data = (CWideStringDataActions*)pointer;
    if (data->text && data->text != lastFreedText) bad = 1;
    ++deletes;
    free(pointer);
}
extern "C" void __cdecl FreeWideCharacters(void* pointer)
{
    ++frees;
    lastFreedText = pointer;
    free(pointer);
}
void CWideStringDataActions::Initialize(long length, wchar_t fill, const CWideAllocator&)
{
    ++initCalls;
    text = (wchar_t*)malloc((length+1)*sizeof(wchar_t));
    for (long i=0;i<length;++i) text[i] = fill;
    text[length] = 0;
    unknown04 = unknown08 = (fable_u32)length;
}
void CWideStringDataActions::Resize(long length, wchar_t fill)
{
    if (ownCalls != 1 || fill != 0 || owners != 1) bad = 1;
    ++resizeCalls;
    long old = (long)unknown04;
    text = (wchar_t*)realloc(text,(length+1)*sizeof(wchar_t));
    for (long i=old;i<length;++i) text[i] = fill;
    text[length] = 0;
    unknown04 = unknown08 = (fable_u32)length;
}
void CWideStringLengthView::MakeOwnCopyOfData()
{
    ++ownCalls;
    if (storage_->owners > 1) {
        CWideStringDataActions* old = storage_;
        CWideStringDataActions* fresh = new CWideStringDataActions(old->unknown04,L' ',CWideAllocator());
        for (unsigned long i=0;i<=old->unknown04;++i) fresh->text[i]=old->text[i];
        --old->owners;
        storage_ = fresh;
    }
}
static void reset()
{
    bad=allocs=deletes=frees=failNext=ownCalls=resizeCalls=initCalls=0;
    lastFreedText = 0;
}
static int clear(int owners, int nullText)
{
    CWideStringLengthView value;
    value.storage_ = new CWideStringDataActions(3,L'x',CWideAllocator());
    CWideStringDataActions* old = value.storage_;
    old->owners = owners;
    if (nullText) { free(old->text); old->text=0; }
    reset();
    value.SetLength(0);
    if (bad || value.storage_ || allocs || ownCalls || resizeCalls || initCalls) return 0;
    if (owners > 1) {
        if (old->owners != owners-1 || frees || deletes) return 0;
        delete old;
    } else if (deletes != 1 || frees != (nullText ? 0 : 1)) return 0;
    return !bad;
}
static int resize(int shared)
{
    CWideStringLengthView value;
    value.storage_ = new CWideStringDataActions(2,L'x',CWideAllocator());
    CWideStringDataActions* old = value.storage_;
    old->owners = shared ? 2 : 1;
    reset();
    value.SetLength(5);
    CWideStringDataActions* result = value.storage_;
    if (bad || !result || result->owners != 1 || result->unknown04 != 5 ||
        result->text[0]!=L'x' || result->text[1]!=L'x' || result->text[2]!=0 ||
        result->text[4]!=0 || result->text[5]!=0 || ownCalls!=1 || resizeCalls!=1) return 0;
    if (shared ? (result==old || old->owners!=1 || old->unknown04!=2) : result!=old) return 0;
    delete result;
    if (shared) delete old;
    return !bad;
}
static int make(int fail)
{
    CWideStringLengthView value;
    value.storage_ = 0;
    reset();
    failNext = fail;
    value.SetLength(4);
    if (bad || allocs!=1 || ownCalls || resizeCalls) return 0;
    if (fail) return !value.storage_ && !initCalls && !deletes && !frees;
    CWideStringDataActions* result=value.storage_;
    if (!result || result->owners!=1 || result->unknown04!=4 || initCalls!=1 ||
        result->text[0]!=L' ' || result->text[3]!=L' ' || result->text[4]!=0) return 0;
    delete result;
    return !bad && frees==1 && deletes==1;
}
int main()
{
    CWideStringLengthView empty;
    empty.storage_=0;
    reset();empty.SetLength(0);
    if (bad || allocs || frees || deletes || ownCalls || resizeCalls || empty.storage_ ||
        !clear(2,0) || !clear(1,0) || !clear(0,0) || !clear(1,1) ||
        !resize(0) || !resize(1) || !make(0) || !make(1)) {
        printf("0099bc50_TEST FAIL\n");return 1;
    }
    printf("0099bc50_TEST PASS\n");return 0;
}