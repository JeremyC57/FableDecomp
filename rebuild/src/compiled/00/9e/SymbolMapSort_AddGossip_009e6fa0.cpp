// SymbolMapSort::LinearInsert -- insert a by-value three-string symbol record into its sorted prefix
// Leaf: LinearInsert
// Retail009E6FA0 length185. Propagated AddGossip label is wrong: fastcall(first,last,value,tag), ret20.
// Native string copy0099EC30,assign0099EFB0,dtor0099EAE0; comparator009E6530; move_backward009E6410; unguarded_insert009E66F0.
#include "fable_string.h"
// Actual string header omits the native copy constructor and comparator. Field-free API view retains its genuine storage.
struct SymbolString:CCharString{SymbolString(const SymbolString&);bool operator<(const SymbolString&)const;};
struct SymbolInfo{SymbolString GroupName;SymbolString EntryName;long Val;};
struct SymbolRecord{SymbolString Name;SymbolInfo Info;};
struct NonscalarTag{NonscalarTag(){}};
SymbolRecord* __fastcall MoveBackward(SymbolRecord*,SymbolRecord*,SymbolRecord*,NonscalarTag&,int); //009E6410
void __fastcall UnguardedInsert(SymbolRecord*,SymbolRecord,NonscalarTag); //009E66F0
void __fastcall LinearInsert(SymbolRecord* first,SymbolRecord* last,SymbolRecord value,NonscalarTag tag){
 if(value.Name<first->Name){MoveBackward(first,last,last+1,tag,0);*first=value;}
 else UnguardedInsert(last,value,tag);
}