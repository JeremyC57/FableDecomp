// CThingFilter_IsInQuest::operator()(const CThing& thing) const -- compare the thing string with the requested value.
// Leaf: ??RCThingFilter_IsInQuest@@QBE_NABVCThing@@@Z
// Identity: proven; PDB ??RCThingFilter_IsInQuest@@QBE_NABVCThing@@@Z; own retail field+0x58 matches Ego_r CThing::SerialisationSectionName rebased-0x10; shared CCharString equality uses CBasicString<char>::Compare0x411570.
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