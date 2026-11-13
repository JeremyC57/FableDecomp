#ifndef FABLE_ENGINE_CHEROLOGBOOK_H
#define FABLE_ENGINE_CHEROLOGBOOK_H
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
// Original Ego_r/FableWin classes and native47a9f3 LogEntries+10.
// Original GivenQuests vector shrinks16 to12 and LogEntries map16 to12.
class CHeroLogBook {
public:
    class CQuestDesc;
    class CHeroLogBookEntry {
    public:
        enum ECategory {CATEGORY_QUEST,CATEGORY_STORY,CATEGORY_TUTORIAL,CATEGORY_BASICS,CATEGORY_OBJECTS,CATEGORY_TOWNS,CATEGORY_HERO,CATEGORY_COMBAT};
        CHeroLogBookEntry(const CWideString& name_tag,const CWideString& abbreviated_name,const CWideString& text_tag):Name(name_tag),AbbreviatedName(abbreviated_name),Content(text_tag){}
        CHeroLogBookEntry(const CHeroLogBookEntry& entry):Name(entry.Name),AbbreviatedName(entry.AbbreviatedName),Content(entry.Content){}
        const CWideString& GetName()const{return Name;}
#ifdef FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS
        // Retail opt-in (row 0x00607330): FableWin.pdb ?GetAbbreviatedName@ / ?GetContent@CHeroLogBookEntry@CHeroLogBook@@QBEABVCWideString@@XZ, inline.
        const CWideString& GetAbbreviatedName()const{return AbbreviatedName;}
        const CWideString& GetContent()const{return Content;}
#endif
    private:
        CWideString Name,AbbreviatedName,Content;
    };
    void AddEntry(const CWideString&,const CWideString&,const CWideString&,CHeroLogBookEntry::ECategory,bool);
#ifdef FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS
    // FableWin.pdb ?GetLogEntries@CHeroLogBook@@QBE... (const accessor, inline in retail: 0x0060735B reads +0x10 in place).
    const std::map<CHeroLogBookEntry::ECategory,std::vector<CHeroLogBookEntry> >& GetLogEntries()const{return LogEntries;}
#endif
private:
    unsigned char GivenQuests[12]; // Complete vector<CQuestDesc> storage; unused by this member.
    long QuestBeingViewed;
    std::map<CHeroLogBookEntry::ECategory,std::vector<CHeroLogBookEntry> > LogEntries;
};
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook)==28);
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook::CHeroLogBookEntry)==12);
#endif
