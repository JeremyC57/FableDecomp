#pragma once

#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CDATABANK
// Retail opt-in (chunk 735): the shared engine headers (the local CIVCountedPointer below would redefine
// engine/CIVCountedPointer.h, the retail model of the same Ego_r class template).
#include "fable_string.h"
#include "engine/CIVCountedPointer.h"
#else

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
#ifdef FABLE_RETAIL_CDATABANK
        // Retail opt-in (chunk 735): Ego_r.pdb / FableWin.pdb ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z,
        // out of line at 0x009C95E0 (the text of a symbol, returned as a const value; landed binding).
        const CWideString GetTextBySymbol(const CCharString& symbol) const;
#endif

    private:
        fable_u8 storage_[0x214];
    };
}
