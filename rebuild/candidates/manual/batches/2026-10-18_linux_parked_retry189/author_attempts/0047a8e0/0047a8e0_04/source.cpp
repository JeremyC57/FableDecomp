// CHeroLogBook::AddObjectiveToQuest(ulong,ulong,const CCharString&) -- PDB00487960
// Leaf: AddObjectiveToQuest
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
#include <string.h>
#include <new>
// Actual header lacks explicit copy/append/Sprintf methods; native string remains one pointer.
struct BasicString {char*Text;bool operator==(const char*)const;}; //004115A0/PDB0040C910 CBasicString<char>::operator==
extern const char EmptyString[]; //0122D70E
struct CCharString {CCharStringData*Data;CCharString();CCharString(const CCharString&);~CCharString();const CCharString&operator=(const char*);const CCharString&operator+=(const CCharString&);void __cdecl Sprintf(const char*,...);__forceinline bool operator!=(const char*t)const{if(Data)return !(*reinterpret_cast<const BasicString*>(Data)==t);return strcmp("",t)!=0;}}; //Sprintf member __cdecl(PDB00AFB180), not thiscall/free function
struct Objective {unsigned long ID;CCharString Description;bool Completed;};
struct Allocator {};
struct ObjectiveVector {ObjectiveVector();Objective*First,*Last,*End;void InsertOverflow(Objective*,const Objective&,const Allocator&,unsigned long,bool); //0047C030 native five-stack-argument vector growth helper
 __forceinline void push_back(const Objective&o){if(Last!=End){if(Last)new(Last)Objective(o);++Last;}else{Allocator a;InsertOverflow(Last,o,a,1,true);}}
};
struct Quest {unsigned long ID;CCharString Description;long Status;ObjectiveVector Objectives;}; //native24 vs donor28 due vector12/16
struct QuestVector {Quest*First,*Last,*End;};
struct CHeroLogBook {CHeroLogBook();QuestVector GivenQuests;bool AddObjectiveToQuest(unsigned long,unsigned long,const CCharString&);}; //actual default PDB01ACA8D0; only accessed prefix represented
bool CHeroLogBook::AddObjectiveToQuest(unsigned long questID,unsigned long objectiveID,const CCharString& description){
 Objective objective;objective.Completed=false;objective.ID=objectiveID;
 if(description!=""){objective.Description.Sprintf("%d-",objectiveID);objective.Description+=description;}
 else objective.Description="This quest objective has no description!";
 unsigned long i=0;unsigned long n=GivenQuests.Last-GivenQuests.First;
 for(;i<n;++i)if(GivenQuests.First[i].ID==questID){GivenQuests.First[i].Objectives.push_back(objective);break;}
 return true;
}
