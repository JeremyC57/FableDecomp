#pragma once

#include "rebuild_abi.h"

template<typename T>
class CIVCountedPointer
{
public:
    CIVCountedPointer()
        : data_(0)
    {
    }

    CIVCountedPointer(const CIVCountedPointer& other)
        : data_(other.data_)
    {
    }

    ~CIVCountedPointer()
    {
    }

    T* data_;
};

#ifdef FABLE_RETAIL_NGAMETEXT_CDATABANK
class CWideString;
class CCharString;
#endif
namespace NGameText
{
    class CGroupEntry;

    class CDataBank
    {
    public:
        CDataBank();

        CIVCountedPointer<const CGroupEntry> GetPGroupEntry(
            unsigned long index,
            bool allowPending) const;
#ifdef FABLE_RETAIL_NGAMETEXT_CDATABANK
        // Retail opt-in (2026-11-03, chunk 720): Ego_r ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z
        // 0x009C95E0 (const return by value; landed callers 0x005C770C bind this spelling).
        const CWideString GetTextBySymbol(const CCharString& symbol) const;
#endif

    private:
        fable_u8 storage_[0x214];
    };
}
