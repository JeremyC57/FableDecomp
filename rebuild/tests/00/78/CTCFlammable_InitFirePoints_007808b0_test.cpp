#include <vector>
#include <map>
#include <utility>
#include <cstdio>
#define FABLE_RETAIL_ASSERT_ARM(x) ((void)0)
class CCharString { public: int id; CCharString(int n=0):id(n){} bool operator<(const CCharString& r) const {return id<r.id;} };
class CDefString {public: int id; CDefString(int n=0):id(n){} bool operator<(const CDefString& r)const{return id<r.id;} bool operator==(const CCharString& r)const{return id==r.id;} operator CCharString()const{return CCharString(id);} };
template<class T> class CArray:public std::vector<T>{};
class CFireEffectCreationSet {public:typedef std::map<CDefString,long> TContainmentMap;TContainmentMap ContainmentMap;};
class CFlammableDef {public:CFireEffectCreationSet EffectCreationSet; long FireResistance;
void GetAllFireHelpers(std::vector<CDefString>&) const;
bool GetEffectAtHelperPoint(CDefString,long&) const;
bool GetEffectAtHelperPoint(const CCharString&,long&) const;};
class Appearance {public:std::vector<CCharString> mesh;void GetAllHelperPointNames(std::vector<CCharString>& a){a=mesh;}};
class CThing {public:Appearance graphic;Appearance* GetTCGraphicAppearance(){return &graphic;}};
namespace NFire {class CFirePoint {public:
 CCharString HelperName; long HelperIndex,EffectIndex,Resistance;bool Burning;long FireDamageAfterFrame;CThing* PParentThing;
 CFirePoint(CThing&,const CCharString&,long,long,long);};}
class DefPtr {public:CFlammableDef value;CFlammableDef* operator->(){return &value;} CFlammableDef* Get(){return &value;}};
class CTCFlammable {public:std::vector<NFire::CFirePoint> FirePoints; bool HasFirePoint; DefPtr PDef; CThing thing;CThing& GetParentThing(){return thing;} void InitFirePoints();};
inline void CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helper_points) const
{
    if(false) {} else for (CFireEffectCreationSet::TContainmentMap::const_iterator it=EffectCreationSet.ContainmentMap.begin(); it!=EffectCreationSet.ContainmentMap.end(); ++it)
        helper_points.push_back((*it).first);
}

inline NFire::CFirePoint::CFirePoint(CThing& parent_thing, const CCharString& helper_name, long helper_index, long effect, long fire_resistance)
    : HelperName(helper_name), HelperIndex(helper_index), EffectIndex(effect), Resistance(fire_resistance),
      Burning(false), FireDamageAfterFrame(0), PParentThing(&parent_thing)
{
}

inline bool CFlammableDef::GetEffectAtHelperPoint(const CCharString& name,long& effect) const
{
    CFireEffectCreationSet::TContainmentMap::const_iterator it;
    if(false) {} else for(it=EffectCreationSet.ContainmentMap.begin();it!=EffectCreationSet.ContainmentMap.end();++it) {
        if(it->first == name) {
            effect=it->second;
            return true;
        }
    }
    return false;
}

inline bool CFlammableDef::GetEffectAtHelperPoint(CDefString name, long& effect) const
{
    CFireEffectCreationSet::TContainmentMap::const_iterator it = EffectCreationSet.ContainmentMap.find(name);
    if (it != EffectCreationSet.ContainmentMap.end()) {
        effect = it->second;
        return true;
    }
    return false;
}

void CTCFlammable::InitFirePoints()
{
    FirePoints.clear();
    if (!PDef.Get()) { FABLE_RETAIL_ASSERT_ARM(false); }
    CArray<CDefString> fire_names;
    PDef->GetAllFireHelpers(fire_names);
    std::map<CCharString, long> fire_pt_enumerator;
    if (false) {} else for (CArray<CDefString>::iterator it=fire_names.begin(); it!=fire_names.end(); ++it) {
        CCharString name=*it;
        fire_pt_enumerator.insert(std::make_pair(name, 0));
    }
    std::vector<CCharString> all_names;
    GetParentThing().GetTCGraphicAppearance()->GetAllHelperPointNames(all_names);
    if (all_names.empty()) { FABLE_RETAIL_ASSERT_ARM(false); }
    HasFirePoint=false;
    if (false) {} else for (std::vector<CCharString>::iterator it_all=all_names.begin();it_all!=all_names.end();++it_all) {
        const CCharString& name=*it_all;
        std::map<CCharString,long>::iterator it_look_up=fire_pt_enumerator.find(name);
        if (it_look_up!=fire_pt_enumerator.end()) {
            long effect;
            if (PDef->GetEffectAtHelperPoint(name,effect)) {
                FirePoints.push_back(NFire::CFirePoint(GetParentThing(),name,it_look_up->second++,effect,PDef->FireResistance));
                HasFirePoint=true;
            }
        }
    }
    if (!HasFirePoint && !all_names.empty() && !fire_names.empty()) {
        long effect;
        if (PDef->GetEffectAtHelperPoint(fire_names.front(),effect)) {
            FirePoints.push_back(NFire::CFirePoint(GetParentThing(),all_names.front(),0,effect,PDef->FireResistance));
            HasFirePoint=true;
        }
    }
}

int main() {
 for(int cfg=0;cfg<16;++cfg) for(int mesh=0;mesh<125;++mesh) for(int populated=0;populated<2;++populated) {
    CTCFlammable c;CFlammableDef& def=c.PDef.value;
    long resistance=cfg-mesh;
    def.FireResistance=resistance;
    int first=-1, ids[4], count=0, digits=mesh;
    long indexes[4]={0,0,0,0};
    for(int i=0;i<4;++i) if(cfg & (1<<i)) {if(first<0)first=i;def.EffectCreationSet.ContainmentMap[CDefString(i)]=100-i;}
    for(int pos=0;pos<3;++pos) {int id=digits%5; digits/=5;if(id!=4){ids[count++]=id;c.thing.graphic.mesh.push_back(CCharString(id));}}
    c.HasFirePoint=populated!=0;
    if(populated)c.FirePoints.push_back(NFire::CFirePoint(c.thing,CCharString(99),33,44,55));
    std::vector<CDefString> collected; collected.push_back(CDefString(99));
    def.GetAllFireHelpers(collected);
    unsigned int at=1;
    for(int i=0;i<4;++i) {
      bool expected=(cfg&(1<<i))!=0;
      long one=-1,two=-2;
      if(def.GetEffectAtHelperPoint(CDefString(i),one)!=expected || def.GetEffectAtHelperPoint(CCharString(i),two)!=expected)return 1;
      if(one!=(expected?100-i:-1) || two!=(expected?100-i:-2))return 2;
      if(expected && (at>=collected.size() || collected[at++].id!=i))return 3;
    }
    if(collected[0].id!=99 || at!=collected.size())return 4;
    c.InitFirePoints();
    int names[3], effects[3], indices[3], expectedCount=0;
    for(int k=0;k<count;++k) {int id=ids[k];if(cfg&(1<<id)) {names[expectedCount]=id;effects[expectedCount]=100-id;indices[expectedCount++]=indexes[id]++;}}
    if(!expectedCount && first>=0 && count) {names[0]=ids[0];effects[0]=100-first;indices[0]=0;expectedCount=1;}
    if(c.HasFirePoint!=(expectedCount!=0) || c.FirePoints.size()!=static_cast<unsigned>(expectedCount))return 5;
    for(int k=0;k<expectedCount;++k) {const NFire::CFirePoint& f=c.FirePoints[k];if(f.PParentThing!=&c.thing || f.HelperName.id!=names[k] || f.EffectIndex!=effects[k] || f.HelperIndex!=indices[k] || f.Resistance!=resistance || f.Burning || f.FireDamageAfterFrame)return 6;}
 }
 puts("007808b0_TEST PASS");return 0;
}