#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CCharString;
template<class T> class CBasicString { public: T* PData; unsigned long StringLength; protected: static long __fastcall Compare(const T*,const T*); friend class CCharString; };
class CCharStringData { public: CBasicString<char> Data; };
class CCharString { public: CCharStringData* PStringData;
__forceinline bool operator==(const CCharString& other) const {
 if(PStringData==other.PStringData) return true;
 if(PStringData && other.PStringData && PStringData->Data.StringLength==other.PStringData->Data.StringLength)
  return CBasicString<char>::Compare(PStringData->Data.PData,other.PStringData->Data.PData)==0;
 return false;
}
};
class CThing { public: unsigned char _pad00[0x58]; CCharString SerialisationSectionName; };
class CThingFilter_IsInQuest { public: CCharString QuestSectionName; bool operator()(const CThing&) const; };
bool CThingFilter_IsInQuest::operator()(const CThing& thing) const { return thing.SerialisationSectionName == QuestSectionName; }

static int compareCalls=0;
template<class T> long __fastcall CBasicString<T>::Compare(const T* a,const T* b) { ++compareCalls; return strcmp(a,b); }
int main() { CThingFilter_IsInQuest filter; CThing thing; CCharStringData a,b; char aa[]="same",bb[]="same",cc[]="diff"; a.Data.PData=aa; b.Data.PData=bb; a.Data.StringLength=b.Data.StringLength=4;
 filter.QuestSectionName.PStringData=0; thing.SerialisationSectionName.PStringData=0; if(!filter(thing) || compareCalls) goto fail;
 filter.QuestSectionName.PStringData=&a; if(filter(thing) || compareCalls) goto fail;
 thing.SerialisationSectionName.PStringData=&a; if(!filter(thing) || compareCalls) goto fail;
 thing.SerialisationSectionName.PStringData=&b; if(!filter(thing) || compareCalls!=1) goto fail;
 b.Data.StringLength=3; if(filter(thing) || compareCalls!=1) goto fail;
 b.Data.StringLength=4; b.Data.PData=cc; if(filter(thing) || compareCalls!=2) goto fail;
 filter.QuestSectionName.PStringData=0; if(filter(thing) || compareCalls!=2) goto fail;
 puts("006a7c20_TEST PASS"); return 0;
fail: puts("006a7c20_TEST FAIL"); return 1;
}