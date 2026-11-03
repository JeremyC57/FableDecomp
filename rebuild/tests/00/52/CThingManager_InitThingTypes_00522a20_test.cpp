// Standalone C++03 behavior test; original registration body and constructors pasted unchanged.
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
typedef std::string CCharString;
#ifndef _MSC_VER
#define __fastcall
#endif
class CClass {public:unsigned long Kind;explicit CClass(unsigned long kind):Kind(kind){} };
class CThingPlayerCreature {public:static CClass* __fastcall Alloc(){static CClass instance(2);return &instance;} };
class CThingAICreature {public:static CClass* __fastcall Alloc(){static CClass instance(1);return &instance;} };
class CThingVillage {public:static CClass* __fastcall Alloc(){static CClass instance(4);return &instance;} };
class CThingBuilding {public:static CClass* __fastcall Alloc(){static CClass instance(3);return &instance;} };
class CThingObject {public:static CClass* __fastcall Alloc(){static CClass instance(5);return &instance;} };
class CThingSoundEmitter {public:static CClass* __fastcall Alloc(){static CClass instance(6);return &instance;} };
class CThingHolySite {public:static CClass* __fastcall Alloc(){static CClass instance(7);return &instance;} };
class CThingSwitch {public:static CClass* __fastcall Alloc(){static CClass instance(9);return &instance;} };
class CThingTrackNode {public:static CClass* __fastcall Alloc(){static CClass instance(10);return &instance;} };
class CThingMarker {public:static CClass* __fastcall Alloc(){static CClass instance(11);return &instance;} };
class CThingPhysicalSwitch {public:static CClass* __fastcall Alloc(){static CClass instance(12);return &instance;} };
class CThingDeadCreature {public:static CClass* __fastcall Alloc(){static CClass instance(13);return &instance;} };
class CThingNoise {public:static CClass* __fastcall Alloc(){static CClass instance(14);return &instance;} };
class CThing {public:static CClass* __fastcall Alloc(){static CClass instance(15);return &instance;} };

class CClass;
// Original Ego_r TPI CClassTypeInit; native registration frame offsets 0,4,8,c.
class CClassTypeInit {
public:
    unsigned char TypeID;
    CClass* (__fastcall *AllocFunc)();
    bool Local;
    CCharString Name;
    CClassTypeInit(long type,CClass*(__fastcall *alloc)(),bool local,const CCharString& name)
      : TypeID((unsigned char)type),AllocFunc(alloc),Local(local),Name(name) {}
};

// Original public base and original fields; native stack offsets +10/+14.
class CThingTypeInit:public CClassTypeInit {
public:
    unsigned long ThingTypeFlags;
    CCharString DefClassType;
    CThingTypeInit(long type,const CCharString& def,unsigned long flags,CClass*(__fastcall *alloc)(),bool local,const CCharString& name)
      :CClassTypeInit(type,alloc,local,name),ThingTypeFlags(flags),DefClassType(def) {}
};

// Original PDB class and field order; native stride12, stores +4/+8.
class CThingTypeInfo {
public:
    long DefType;
    unsigned long ThingTypeFlags;
    CCharString DefClassType;
};

// Original struct key and fields; native vector element stride8.
struct ThingLList { CThing* Head; CThing* Tail; };

class CClassTypeInfo {public:CClass* (__fastcall *AllocFunc)();bool Local;CCharString Name;};
class CClassFactory {public:
 std::vector<CClassTypeInfo> ClassTypeInfo;std::vector<unsigned char> Registered;
 void AddNewClassType(const CClassTypeInit& init){
  Registered.push_back(init.TypeID);if(ClassTypeInfo.size()<=init.TypeID)ClassTypeInfo.resize(init.TypeID+1);
  ClassTypeInfo[init.TypeID].AllocFunc=init.AllocFunc;ClassTypeInfo[init.TypeID].Local=init.Local;ClassTypeInfo[init.TypeID].Name=init.Name;
 }
};
class CThingManager:public CClassFactory {public:
 std::vector<CThingTypeInfo> ThingTypeInfo;std::vector<ThingLList> UpdateLists;
 void AddNewThingType(const CThingTypeInit& init) {
        CClassFactory::AddNewClassType(init);
        ThingTypeInfo[init.TypeID].ThingTypeFlags=init.ThingTypeFlags;
        ThingTypeInfo[init.TypeID].DefClassType=init.DefClassType;
    }
 void InitThingTypes();
};
void CThingManager::InitThingTypes()
{
    ThingTypeInfo.resize(16);
    AddNewThingType(CThingTypeInit(2,"CREATURE",0x2f,CThingPlayerCreature::Alloc,false,"PlayerCreature"));
    AddNewThingType(CThingTypeInit(1,"CREATURE",0x1f,CThingAICreature::Alloc,false,"AICreature"));
    AddNewThingType(CThingTypeInit(4,"VILLAGE",0x407,CThingVillage::Alloc,false,"Village"));
    AddNewThingType(CThingTypeInit(3,"BUILDING",0x47,CThingBuilding::Alloc,false,"Building"));
    AddNewThingType(CThingTypeInit(5,"OBJECT",0x807,CThingObject::Alloc,false,"Object"));
    AddNewThingType(CThingTypeInit(6,"",0x1001,CThingSoundEmitter::Alloc,false,"Sound Emitter"));
    AddNewThingType(CThingTypeInit(7,"HOLY_SITE",0x2007,CThingHolySite::Alloc,false,"Holy Site"));
    AddNewThingType(CThingTypeInit(9,"SWITCH",0x20007,CThingSwitch::Alloc,false,"Switch"));
    AddNewThingType(CThingTypeInit(10,"THING",0x10007,CThingTrackNode::Alloc,false,"TrackNode"));
    AddNewThingType(CThingTypeInit(11,"MARKER",0x8007,CThingMarker::Alloc,false,"Marker"));
    AddNewThingType(CThingTypeInit(12,"PHYSICAL_SWITCH",0x40007,CThingPhysicalSwitch::Alloc,false,"Physical Switch"));
    AddNewThingType(CThingTypeInit(13,"CREATURE",0x87,CThingDeadCreature::Alloc,false,"Dead Creature"));
    AddNewThingType(CThingTypeInit(14,"NOISE",0x101,CThingNoise::Alloc,false,"Noise"));
    AddNewThingType(CThingTypeInit(15,"THING",0x0,CThing::Alloc,false,"Thing"));
    // Ego_r thing_manager.obj local data record0x50a0 proves the name and type.
    static ThingLList dummyList={0,0};
    UpdateLists.resize(ClassTypeInfo.size(),dummyList);
}

int main(){
 const unsigned char ids[]={2,1,4,3,5,6,7,9,10,11,12,13,14,15};
 const unsigned long flags[]={0x2f,0x1f,0x407,0x47,0x807,0x1001,0x2007,0x20007,0x10007,0x8007,0x40007,0x87,0x101,0x0};
 const char* names[]={"PlayerCreature","AICreature","Village","Building","Object","Sound Emitter","Holy Site","Switch","TrackNode","Marker","Physical Switch","Dead Creature","Noise","Thing"};
 const char* defs[]={"CREATURE","CREATURE","VILLAGE","BUILDING","OBJECT","","HOLY_SITE","SWITCH","THING","MARKER","PHYSICAL_SWITCH","CREATURE","NOISE","THING"};
 CClass*(__fastcall *callbacks[])()={CThingPlayerCreature::Alloc,CThingAICreature::Alloc,CThingVillage::Alloc,CThingBuilding::Alloc,CThingObject::Alloc,CThingSoundEmitter::Alloc,CThingHolySite::Alloc,CThingSwitch::Alloc,CThingTrackNode::Alloc,CThingMarker::Alloc,CThingPhysicalSwitch::Alloc,CThingDeadCreature::Alloc,CThingNoise::Alloc,CThing::Alloc};
 const unsigned counts[]={0,1,8,16,20};
 const unsigned listCounts[]={0,2,16,24};
 for(unsigned ci=0;ci<5;++ci)for(unsigned ui=0;ui<4;++ui)for(unsigned ti=0;ti<3;++ti){
  CThingManager manager;manager.ClassTypeInfo.resize(counts[ci]);manager.UpdateLists.resize(listCounts[ui]);
  manager.ThingTypeInfo.resize(ti==0?0:(ti==1?16:20));
  for(unsigned i=0;i<manager.ThingTypeInfo.size();++i){manager.ThingTypeInfo[i].DefType=123;manager.ThingTypeInfo[i].ThingTypeFlags=777;manager.ThingTypeInfo[i].DefClassType="preserve";}
  CThing head,tail;
  for(unsigned i=0;i<manager.UpdateLists.size();++i){manager.UpdateLists[i].Head=&head;manager.UpdateLists[i].Tail=&tail;}
  manager.InitThingTypes();
  assert(manager.ThingTypeInfo.size()==16);assert(manager.ClassTypeInfo.size()==(counts[ci]<16?16:counts[ci]));assert(manager.UpdateLists.size()==manager.ClassTypeInfo.size());
  assert(manager.Registered.size()==14);
  for(unsigned i=0;i<14;++i){unsigned ident=ids[i];assert(manager.Registered[i]==ident);
   assert(manager.ClassTypeInfo[ident].Name==names[i]);assert(!manager.ClassTypeInfo[ident].Local);
   assert(manager.ClassTypeInfo[ident].AllocFunc==callbacks[i]);assert(callbacks[i]()->Kind==ident);
   assert(manager.ThingTypeInfo[ident].ThingTypeFlags==flags[i]);assert(manager.ThingTypeInfo[ident].DefClassType==defs[i]);
  }
  if(ti){for(unsigned i=0;i<16;++i)assert(manager.ThingTypeInfo[i].DefType==123);assert(manager.ThingTypeInfo[0].ThingTypeFlags==777&&manager.ThingTypeInfo[8].ThingTypeFlags==777);assert(manager.ThingTypeInfo[0].DefClassType=="preserve"&&manager.ThingTypeInfo[8].DefClassType=="preserve");}
  for(unsigned i=0;i<manager.UpdateLists.size();++i){assert(manager.UpdateLists[i].Head==(i<listCounts[ui]?&head:0));assert(manager.UpdateLists[i].Tail==(i<listCounts[ui]?&tail:0));}
  manager.Registered.clear();manager.InitThingTypes();assert(manager.Registered.size()==14);for(unsigned i=0;i<14;++i)assert(manager.Registered[i]==ids[i]);
 }
 std::puts("00522a20_TEST PASS");return 0;
}