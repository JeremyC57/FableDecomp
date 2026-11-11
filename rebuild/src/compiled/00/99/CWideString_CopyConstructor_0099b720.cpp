#define FABLE_RETAIL_CWIDESTRING_NOSTRINGS
#include "fable_string.h"

CWideString::CWideString(const CWideString& other)
    : storage_(0)
{
    ++NoStrings;                                   // 0x013BCA20 ?NoStrings@CWideString@@1JA

    if (&other != this && other.storage_ != storage_)
    {
        if (storage_ != 0)
            UnassignString();

        if (other.storage_ != 0)
        {
            storage_ = other.storage_;
            ++storage_->owners;
        }
    }
}
