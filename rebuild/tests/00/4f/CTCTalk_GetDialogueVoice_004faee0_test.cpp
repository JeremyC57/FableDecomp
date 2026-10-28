#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>
#ifdef _MSC_VER
#define private public
#include "fable_string.h"
#undef private
#else
#ifndef FABLE_STRING_TEST_STUB_H
#define FABLE_STRING_TEST_STUB_H
#include <cstring>
class CCharStringData { public: char* text; unsigned long unknown04, unknown08; unsigned char flags0C; long owners; };
class CCharString {
public:
    CCharString();
    CCharString(const char*, long);
    CCharString(const CCharString&);
    ~CCharString();
    operator const char*() const;
    const CCharString& operator=(const CCharString&);
private:
    void UnassignString();
    CCharStringData* AllocStringData(const char*, long);
    CCharStringData* storage_;
    friend struct CRetailCharString;
};
#endif
#endif

// CTCTalk::GetDialogueVoice() const -- return the talker's reference-counted dialogue voice.
struct CRetailCharString : CCharString {
    CRetailCharString(const CCharString& other);
};
class CTCTalk { public:
    unsigned char _beforeDialogueVoice[0x38];
    CCharString DialogueVoice;
    CRetailCharString GetDialogueVoice() const;
};
CRetailCharString CTCTalk::GetDialogueVoice() const { return DialogueVoice; }
typedef char CTCTalk_DialogueVoice_offset[(offsetof(CTCTalk, DialogueVoice) == 0x38) ? 1 : -1];

CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const char* text, long length) : storage_(0)
{
    storage_ = new CCharStringData;
    storage_->text = new char[length + 1];
    std::memcpy(storage_->text, text, length);
    storage_->text[length] = 0;
    storage_->unknown04 = 0; storage_->unknown08 = 0; storage_->flags0C = 0;
    storage_->owners = 1;
}
#ifndef _MSC_VER
CCharString::CCharString(const CCharString& other) : storage_(other.storage_)
{ if (storage_) ++storage_->owners; }
CRetailCharString::CRetailCharString(const CCharString& other) : CCharString(other) {}
#else
CRetailCharString::CRetailCharString(const CCharString& other) : CCharString()
{ storage_ = other.storage_; if (storage_) ++storage_->owners; }
#endif
CCharString::~CCharString()
{
    if (storage_ && --storage_->owners == 0) { delete[] storage_->text; delete storage_; }
}
const CCharString& CCharString::operator=(const CCharString& other)
{
    if (other.storage_) ++other.storage_->owners;
    if (storage_ && --storage_->owners == 0) { delete[] storage_->text; delete storage_; }
    storage_ = other.storage_; return *this;
}
CCharString::operator const char*() const { return storage_ ? storage_->text : ""; }
int main()
{
    CTCTalk owner;
    owner.DialogueVoice = CCharString("native", 6);
    CRetailCharString result = owner.GetDialogueVoice();
    if (std::strcmp((const char*)result, "native") != 0)
        { std::printf("004faee0_TEST FAIL\n"); return 1; }
    owner.DialogueVoice = CCharString();
    if (std::strcmp((const char*)result, "native") != 0) { std::printf("004faee0_TEST FAIL\n"); return 1; }
    std::printf("004faee0_TEST PASS\n"); return 0;
}