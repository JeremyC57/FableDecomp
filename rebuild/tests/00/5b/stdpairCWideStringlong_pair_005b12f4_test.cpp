#include <cstdio>
#include <stddef.h>
#include <utility>
#if defined(_WIN32)
#include "fable_string.h"
static CWideStringData* observed_storage = 0;
#else
struct TestWideStringData {
    wchar_t* text;
    long unknown04;
    long unknown08;
    int owners;
};
class CWideString { public:
    TestWideStringData* storage_;
    explicit CWideString(const wchar_t* text);
    CWideString(const CWideString& other);
    ~CWideString();
};
static TestWideStringData* observed_storage = 0;
CWideString::CWideString(const wchar_t* text) : storage_(new TestWideStringData) {
    storage_->text = (wchar_t*)text;
    storage_->unknown04 = storage_->unknown08 = 0;
    storage_->owners = 1;
    observed_storage = storage_;
}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {
    if (storage_) ++storage_->owners;
}
CWideString::~CWideString() {
    if (storage_ && --storage_->owners == 0) {
        if (storage_ == observed_storage) observed_storage = 0;
        delete storage_;
    }
}
#endif

#if defined(_WIN32)
CWideString::CWideString(const wchar_t* text) : storage_(new CWideStringData) {
    storage_->text = (wchar_t*)text;
    storage_->unknown04 = storage_->unknown08 = 0;
    storage_->owners = 1;
    observed_storage = storage_;
}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {
    if (storage_) ++storage_->owners;
}
CWideString::~CWideString() {
    if (storage_ && --storage_->owners == 0) {
        if (storage_ == observed_storage) observed_storage = 0;
        delete storage_;
    }
}
#endif

namespace std {
template<class A, class B> struct pair;
template<> struct pair<CWideString, long> {
    CWideString first;
    long second;
    pair(const CWideString& text, long value) : first(text), second(value) {}
    pair(const pair& other);
};
pair<CWideString, long>::pair(const pair& other)
    : first(other.first), second(other.second) {}
}

typedef std::pair<CWideString, long> WideStringLongPair;
#if defined(_WIN32)
FABLE_STATIC_ASSERT(sizeof(WideStringLongPair) == 8);
FABLE_STATIC_ASSERT(offsetof(WideStringLongPair, second) == 4);
#endif

int main()
{
    bool okay = true;
    {
        CWideString empty(L"");
        {
            WideStringLongPair before(empty, -37);
            if (!observed_storage || observed_storage->text[0] != 0 || before.second != -37)
                okay = false;
            long refs = observed_storage ? observed_storage->owners : 0;
            {
                WideStringLongPair after(before);
                if (after.second != -37 || !observed_storage || observed_storage->owners != refs + 1)
                    okay = false;
            }
            if (!observed_storage || observed_storage->owners != refs)
                okay = false;
        }
        if (!observed_storage || observed_storage->owners != 1)
            okay = false;
    }
    if (observed_storage != 0) okay = false;
    if (!okay) { std::printf("005b12f4_TEST FAIL\n"); return 1; }
    std::printf("005b12f4_TEST PASS\n");
    return 0;
}