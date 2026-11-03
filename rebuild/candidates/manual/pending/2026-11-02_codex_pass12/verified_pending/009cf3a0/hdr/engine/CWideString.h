#ifndef FABLE_RETAIL_HEADER_CWIDESTRING
#define FABLE_RETAIL_HEADER_CWIDESTRING
#ifdef FABLE_RETAIL_QUICK_ACCESS
#include <stddef.h>
#endif
// Original PDB CWideString, class key V, one protected pointer at +0.
#ifdef FABLE_RETAIL_HERO_LOGBOOK
#include "engine/CWideStringData.h"
#endif
class CCharString;
class CWideStringData;
class CWideString {
public:
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    CCharString ToCharString() const;
#endif
#ifdef FABLE_RETAIL_CENTERED_TEXT
    operator const unsigned short*() const;
    const unsigned short* GetAsCharArray() const;
    bool operator!=(const unsigned short*) const;
    const CWideString& operator=(const CWideString&);
#endif
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    bool operator==(const CWideString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData || !other.PStringData)return false;
        return *PStringData==*other.PStringData;
    }
    bool Contains(const CWideString&)const;
#endif
#ifdef FABLE_RETAIL_QUICK_ACCESS
    const CWideString& operator+=(const CWideString&);
    void __cdecl Sprintf(const wchar_t*,...);
#endif
#ifdef FABLE_RETAIL_BANK_SYMBOLS
    long GetLength()const;
    const CWideString& operator=(const CWideString&);
#endif
    ~CWideString();
protected:
#ifdef FABLE_RETAIL_HERO_LOGBOOK
    static long NoStrings;
    void UnassignString() { if(PStringData) { if(--PStringData->NoRefs<=0)delete PStringData;PStringData=0; } }
#endif
    CWideStringData* PStringData;
};
#endif
