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
#if defined(FABLE_RETAIL_PHOTOJOURNAL) || defined(FABLE_RETAIL_ITEM_ENTRY) || defined(FABLE_RETAIL_TICK_SLIDER)
 CWideString(const unsigned short*);
 const CWideString& operator=(const unsigned short*);
#endif
#if defined(FABLE_RETAIL_CWIDESTRING_CTOR)
    CWideString();
#endif
#ifdef FABLE_RETAIL_CWIDESTRING_COPY
    CWideString(const CWideString&);
#endif
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    CCharString ToCharString() const;
#endif
#if defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_PHOTO_TIME_BODY) || defined(FABLE_RETAIL_PHOTO_SPLIT_BODY)
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
#if defined(FABLE_RETAIL_ITEMS_LIST) || defined(FABLE_RETAIL_LOAD_GAME_STATE)
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
#ifdef FABLE_RETAIL_PHOTOJOURNAL
CWideString __fastcall operator+(const CWideString&,const CWideString&);
CWideString __fastcall operator+(const CWideString&,const unsigned short*);
#endif
#endif
