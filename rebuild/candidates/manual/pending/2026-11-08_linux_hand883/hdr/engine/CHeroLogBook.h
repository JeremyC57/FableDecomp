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
#ifdef FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN
        // Retail opt-in (chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC): the other two inline accessors,
        // Ego_r.pdb / FableWin.pdb ?GetAbbreviatedName@CHeroLogBookEntry@CHeroLogBook@@QBEABVCWideString@@XZ and
        // ?GetContent@CHeroLogBookEntry@CHeroLogBook@@QBEABVCWideString@@XZ (no out-of-line copy in retail).
        const CWideString& GetAbbreviatedName()const{return AbbreviatedName;}
        const CWideString& GetContent()const{return Content;}
#endif
    private:
        CWideString Name,AbbreviatedName,Content;
    };
    void AddEntry(const CWideString&,const CWideString&,const CWideString&,CHeroLogBookEntry::ECategory,bool);
#ifdef FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN
    // (same opt-in) CTCInventoryMap's logbook screens copy LogEntries straight out of the player's log book (0x00609706:
    // `mov ecx,[eax+0F8h] ; add ecx,10h ; push ecx ; call map copy 0x00619C50`): no PDB accessor returns the map, so the screen
    // class is a friend (the PDBs keep no access specifiers).
    friend class CTCInventoryMap;
#endif
private:
    unsigned char GivenQuests[12]; // Complete vector<CQuestDesc> storage; unused by this member.
    long QuestBeingViewed;
    std::map<CHeroLogBookEntry::ECategory,std::vector<CHeroLogBookEntry> > LogEntries;
};
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook)==28);
FABLE_STATIC_ASSERT(sizeof(CHeroLogBook::CHeroLogBookEntry)==12);
#endif
