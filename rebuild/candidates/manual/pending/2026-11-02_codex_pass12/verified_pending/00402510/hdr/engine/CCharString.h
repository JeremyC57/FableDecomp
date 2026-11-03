#ifdef FABLE_RETAIL_UI_ORDINARY_INLINE
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
#if defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#define FABLE_RETAIL_CCHARSTRING_DATA
#include "fable_string.h"
#endif
class CCharStringData;
#ifdef FABLE_RETAIL_LOAD_GAME_STATE
class CWideString;
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWideString;
#endif
#ifdef FABLE_RETAIL_FACTIONS
class CCharString;
#include "engine/CCRC.h"
#endif
class CCharString {
public:
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_FACTIONS
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():&NullChar;}
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
    bool operator<(const CCharString& other)const{
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWideString ToWideString() const;
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    CCharString GetSubString(unsigned long,unsigned long)const;
    bool operator==(const char* text)const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
    bool operator==(const CCharString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&)const;
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_TEXT
    bool operator<(const CCharString& other)const {
        if(other.PStringData==PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
#endif
#ifdef FABLE_RETAIL_LOAD_GAME_STATE
    CWideString ToWideString()const;
    const char* GetAsCharArray()const {return PStringData==0?&NullChar:PStringData->Data.PeekData();}
    // PDB original Replace exists in debug only; native loader expands all four sites.
    __forceinline void Replace(const CCharString& find,const CCharString& replacement) {
        CCharString result;
        char* start=(char*)GetAsCharArray();
        const char* key=find.GetAsCharArray();
        char* hit=strstr(start,key);
        while(hit) {
            *hit=0;
            hit+=strlen(key);
            result+=CCharString(start);
            result+=replacement;
            start=hit;
            hit=strstr(hit,key);
        }
        result+=CCharString(start);
        *this=result;
    }
#endif
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#if defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned long GetCRC() const; // Ego_r00403c00; native00404310 calls the original CRC kernel.
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    bool operator<(const CCharString& other)const {
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOAD
    operator const char*() const;
    const CCharString& operator=(const CCharString&);
#endif
#if defined(FABLE_RETAIL_LOAD_GAME_STATE) || defined(FABLE_RETAIL_LOAD_SINGLE_THING)
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#else
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_LOAD
    bool operator!=(const char* text) const;
#else
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
#endif
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
#if defined(FABLE_RETAIL_STATUS_MENU) && !defined(FABLE_RETAIL_UI_ORDINARY_INLINE)
    __forceinline
#endif
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    void UnassignString();
    static long NoStrings;
#endif
#if defined(FABLE_RETAIL_LOAD_GAME_STATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#if defined(FABLE_RETAIL_ITEMS_LIST) || defined(FABLE_RETAIL_TICK_SLIDER)
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall operator+(const char*,const CCharString&);
#endif
#endif

#else
#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
#if defined(FABLE_RETAIL_BODYGUARD) || defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE) || defined(FABLE_RETAIL_GFMAIN)
#define FABLE_RETAIL_CCHARSTRING_DATA
#include "fable_string.h"
#endif
class CCharStringData;
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWideString;
#endif
#ifdef FABLE_RETAIL_FACTIONS
class CCharString;
#include "engine/CCRC.h"
#endif
#ifdef FABLE_RETAIL_GFMAIN
#include "engine/CCRC.h"
#endif
class CCharString {
public:
#ifdef FABLE_RETAIL_SOUND_INIT
 CWideString ToWideString()const;
#endif
#ifdef FABLE_RETAIL_GFMAIN
 CCharString(class CAFile&,long=-1);
 operator const char*()const;
 class CWideString ToWideString()const;
 void LoadFromFile(class CAFile&,class CASuspendableProcess*,long);
 unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
 long GetLength()const{if(!PStringData)return 0;return PStringData->Data.GetLength();}
 const char*GetAsCharArray()const{if(!PStringData)return &NullChar;return PStringData->Data.PeekData();}
 const CCharString&operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_ESCORT_TRADER
    const CCharString& operator=(const CCharString&);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_BODYGUARD
    long GetLength() const { if(!PStringData)return 0; return PStringData->Data.GetLength(); }
    void Truncate(unsigned long);
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_FACTIONS
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():&NullChar;}
    long GetLength()const{if(!PStringData)return 0;return PStringData->Data.GetLength();}
    unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
    bool operator<(const CCharString& other)const{
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWideString ToWideString() const;
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    long GetLength()const{if(!PStringData)return 0;return PStringData->Data.GetLength();}
    CCharString GetSubString(unsigned long,unsigned long)const;
    bool operator==(const char* text)const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
    bool operator==(const CCharString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&)const;
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_BODYGUARD_IDLE
    bool operator==(const char* lpsz) const { if(!PStringData){if(strcmp(lpsz, "")==0)return true;return false;}return PStringData->Data==lpsz; }
#endif
#ifdef FABLE_RETAIL_SUBCOMPONENT_TEXT
    const CCharString& operator=(const CCharString&);
    const char* GetAsCharArray()const {const char* result=&NullChar;if(PStringData)result=PStringData->Data.PeekData();return result;}
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    void LoadFromFile(class CAFile&,class CASuspendableProcess*,long);
    const CCharString& operator=(const CCharString&);
    const char* GetAsCharArray()const{if(!PStringData)return &NullChar;return PStringData->Data.PeekData();}
#endif
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#if defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned long GetCRC() const; // Ego_r00403c00; native00404310 calls the original CRC kernel.
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    bool operator<(const CCharString& other)const {
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
#ifdef FABLE_RETAIL_STATUS_MENU
    __forceinline
#endif
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#endif
    ~CCharString();
protected:
#ifdef FABLE_RETAIL_GFMAIN
 static const char NullChar;
#endif
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    void UnassignString();
    static long NoStrings;
#endif
#if defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    static const char NullChar;
#endif
#ifdef FABLE_RETAIL_SUBCOMPONENT_TEXT
    static const char NullChar;
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#else
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
#if defined(FABLE_RETAIL_BODYGUARD) || defined(FABLE_RETAIL_NAVIGATION_UPDATE) || defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_MINIMAP_TRAITS) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE) || defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_REGION_GRAPH) || defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#define FABLE_RETAIL_CCHARSTRING_DATA
#include "fable_string.h"
#endif
class CCharStringData;
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
class CWideString;
#endif
#ifdef FABLE_RETAIL_FACTIONS
class CCharString;
#include "engine/CCRC.h"
#endif
class CCharString {
public:
#ifdef FABLE_RETAIL_FISTICUFFS
    const CCharString& operator=(const CCharString&);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_CHICKEN_KICKING
    operator const char*()const;
    const CCharString& operator=(const CCharString&);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_INIT_THING_TYPES
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_TOUR_GUIDE
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_ESCORT_TRADER
    const CCharString& operator=(const CCharString&);
    const CCharString& operator=(const char*);
#endif
#ifdef FABLE_RETAIL_BODYGUARD
    long GetLength() const { if(!PStringData)return 0; return PStringData->Data.GetLength(); }
    void Truncate(unsigned long);
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_FACTIONS
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():&NullChar;}
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    unsigned long GetCRC()const{return CCRC::Calc(0,GetAsCharArray(),GetLength());}
    bool operator<(const CCharString& other)const{
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
#endif
#ifdef FABLE_RETAIL_INVENTORY_MAP_SERIALISE
    CWideString ToWideString() const;
#endif
#ifdef FABLE_RETAIL_NAVIGATION_UPDATE
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    CCharString GetSubString(unsigned long,unsigned long)const;
    bool operator==(const char* text)const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
    bool operator==(const CCharString& other)const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&)const;
    const CCharString& operator=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_BODYGUARD_IDLE
    bool operator==(const char* lpsz) const { if(!PStringData){if(strcmp(lpsz, "")==0)return true;return false;}return PStringData->Data==lpsz; }
#endif
#ifdef FABLE_RETAIL_SUBCOMPONENT_TEXT
    const CCharString& operator=(const CCharString&);
    const char* GetAsCharArray()const {const char* result=&NullChar;if(PStringData)result=PStringData->Data.PeekData();return result;}
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    void LoadFromFile(class CAFile&,class CASuspendableProcess*,long);
    const CCharString& operator=(const CCharString&);
    const char* GetAsCharArray()const{if(!PStringData)return &NullChar;return PStringData->Data.PeekData();}
#endif
#ifdef FABLE_RETAIL_BANK_SYMBOLS
    long FindCharPos(char)const;
#endif
#ifdef FABLE_RETAIL_MAP_BRUSHES
    const CCharString& operator=(const char*);
#endif
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
#ifdef FABLE_RETAIL_MINIMAP_TRAITS
    const CCharString& operator=(const CCharString&);
    bool operator!=(const char* text) const { return !(*this==text); }
#endif
#if defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
#if defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    const char* GetAsCharArray() const { return PStringData==0 ? &NullChar : PStringData->Data.PeekData(); }
#else
    const char* GetAsCharArray() const { const char* result=&NullChar; if(PStringData) result=PStringData->Data.PeekData(); return result; }
#endif
#endif
#if defined(FABLE_RETAIL_EFFECT_LOOKUP) || defined(FABLE_RETAIL_CTHINGMANAGER_SAVE)
    unsigned long GetCRC() const; // Ego_r00403c00; native00404310 calls the original CRC kernel.
#endif
#if defined(FABLE_RETAIL_PERSIST_OPINION) || defined(FABLE_RETAIL_MINIMAP_TRAITS)
    bool EqualsCaseInsensitive(const CCharString&) const;
    bool operator==(const char* text) const;
#endif
#ifdef FABLE_RETAIL_ANIMATION_SEQUENCE
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
#endif
#ifdef FABLE_RETAIL_SUBTITLE
    bool operator<(const CCharString& other)const {
        if(PStringData==other.PStringData)return false;
        if(!PStringData)return true;
        if(!other.PStringData)return false;
        return PStringData->Data<other.PStringData->Data;
    }
    long GetLength() const { return PStringData==0 ? 0 : PStringData->Data.GetLength(); }
    inline bool StartsWith(const CCharString& str) const { return strncmp(GetAsCharArray(),str.GetAsCharArray(),str.GetLength())==0; }
    inline bool EndsWith(const CCharString& str) const { long offset=GetLength()-str.GetLength();if(offset>=0)return strcmp(GetAsCharArray()+offset,str.GetAsCharArray())==0;return false; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool operator==(const char* text) const { if(PStringData) return PStringData->Data==text; return strcmp("",text)==0; }
    bool operator!=(const char* text) const { if(!PStringData) return strcmp("",text)!=0; return !(PStringData->Data==text); }
    const CCharString& operator+=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator=(const char*);
#endif
#if defined(FABLE_RETAIL_RELATIVE_PATH) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
#endif
#ifdef FABLE_RETAIL_REGION_GRAPH
#ifdef FABLE_RETAIL_STATUS_MENU
    __forceinline
#endif
    bool operator==(const char* text) const {if(!PStringData){if(strcmp(text,"")==0)return true;return false;}return PStringData->Data==text;}
#endif
    ~CCharString();
protected:
#if defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE) || defined(FABLE_RETAIL_SUBTITLE_LIFETIME) || defined(FABLE_RETAIL_MINIMAP_LIFETIME)
    void UnassignString();
    static long NoStrings;
#endif
#if defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_SCOREBOARD) || defined(FABLE_RETAIL_SUBTITLE) || defined(FABLE_RETAIL_ANIMATION_SEQUENCE)
    static const char NullChar;
#endif
#ifdef FABLE_RETAIL_SUBCOMPONENT_TEXT
    static const char NullChar;
#endif
#ifdef FABLE_RETAIL_EMITTER_DATABASE
    static const char NullChar;
#endif
    CCharStringData* PStringData;
};
#endif

#ifdef FABLE_RETAIL_BANK_SYMBOLS
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
CCharString __fastcall GFIntToCharString(long);
#endif

#endif

#endif
