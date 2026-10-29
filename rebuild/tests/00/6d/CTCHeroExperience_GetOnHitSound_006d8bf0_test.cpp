#include <stdio.h>
#include <string.h>
#include <new>
int main();
// 006d8bf0 CTCHeroExperience::GetOnHitSound
// Leaf: ?GetOnHitSound@CTCHeroExperience@@QBE?AVCCharString@@XZ
// Flags:
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define CDefClassBase CDefClassBaseLayout
#include "engine/CDefClassBase.h"
#undef CDefClassBase
class CBaseClass{public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:virtual ~CBaseClassNonCopyable();};
// Layout: retail CTCBase prefix12; this function never reads its reference or bool state.
class CThing;class CTCBase:public CBaseClassNonCopyable{private:unsigned char unobservedComponentState[8];};
#pragma pack(push,1)
class CIVCountedPointeeBase{public:virtual ~CIVCountedPointeeBase();virtual void Delete()const;private:unsigned char unobservedRefCount[4];};
class CResource:public CIVCountedPointeeBase{public:unsigned char unobservedResource[20];};
class CDefPointeeBase:public CResource{};
class CDefClassBase:public CDefPointeeBase{public:unsigned char unobservedDef[sizeof(CDefClassBaseLayout)-sizeof(CDefPointeeBase)];};
class CSubDefClassBase:public CDefClassBase{};
#pragma pack(pop)
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience
class CCharStringData;
class CCharString {friend int ::main();friend class CDefStringTable;protected:CCharStringData*PStringData;public:CCharString();CCharString(const CCharString&);~CCharString();};
// Layout: receiver-only declaration; no claim about the table object extent or unread fields.
class CDefStringTable {public:CCharString GetString(long)const;};
extern CDefStringTable GDefStringTable;
class CDefString {private:long TablePos;public:operator CCharString()const{return GDefStringTable.GetString(TablePos);}};
namespace std {
template<class T> class allocator { };
template<class A,class B> struct pair { A first;B second; };
template<class T> struct less { };
template<class V> struct _Select1st { };
template<class V> struct _Nonconst_traits { typedef V& reference;typedef V*pointer; };
template<class V> struct _Const_traits { typedef const V& reference;typedef const V*pointer; };
struct _Rb_tree_node_base { bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right; };
template<class V> struct _Rb_tree_node : public _Rb_tree_node_base { V _M_value_field; };
template<class Dummy> class _Rb_global { public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x) {
        if(x->_M_right) { x=x->_M_right;while(x->_M_left)x=x->_M_left; }
        else { _Rb_tree_node_base* y=x->_M_parent;while(x==y->_M_right) {x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y; }
        return x;
    }
};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&v)const{return _M_node!=v._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
    typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
    _Rb_tree_iterator(_Rb_tree_node<V>*n){_M_node=n;}
    _Rb_tree_iterator(const iterator&n){_M_node=n._M_node;}
    _Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}
    reference operator*()const{return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field;}
    pointer operator->()const{return &(operator*());}
};
template<class V,class T,class A> class _STLP_alloc_proxy:public A {public:V _M_data;};
template<class T,class A> class _Vector_base {protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};
template<class T,class A=allocator<T> > class vector:public _Vector_base<T,A>{};
template<class V,class A> struct _Rb_tree_base {protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;};
template<class K,class V,class Sel,class C,class A> class _Rb_tree:public _Rb_tree_base<V,A>{
protected:
    unsigned int _M_node_count;C _M_key_compare;
public:
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
    const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}
    const_iterator end()const{return const_iterator(this->_M_header._M_data);}
};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class map{
    typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}
};
}
class CHeroExperienceDef:public CSubDefClassBase {
public:
    std::vector<long> ExperienceLevelThresholds,HeroLevelTitles,FallbackMultipliers;
    std::map<long,CDefString> SoundIntensityThresholds;
};
template<class T>class CDefPointer{friend int ::main();private:CDefPointeeBase*Object;public:T*operator->()const{return static_cast<T*>(Object);}};
class CTCHeroExperience:public CTCBase {
friend int ::main();
private:
    unsigned char unobservedPrefix[0x54-sizeof(CTCBase)];
    CDefPointer<const CHeroExperienceDef> PDef;
public:
    CCharString GetOnHitSound()const;
private:long GetOnHitSoundIntensity()const;
};
CCharString CTCHeroExperience::GetOnHitSound()const
{
    long intensity=GetOnHitSoundIntensity();
    for(std::map<long,CDefString>::const_iterator it=PDef->SoundIntensityThresholds.begin();it!=PDef->SoundIntensityThresholds.end();++it) {
        if(intensity<=it->first)return CCharString(it->second);
    }
    return CCharString(PDef->SoundIntensityThresholds.begin()->second);
}

CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CIVCountedPointeeBase::~CIVCountedPointeeBase(){}
void CIVCountedPointeeBase::Delete()const{}
struct FixtureStringData{unsigned int fixtureRefs;long fixtureCode;};
CDefStringTable GDefStringTable;
static FixtureStringData fixturePool[7];
static const CTCHeroExperience*fixtureHero;
static long fixtureIntensity,fixtureExpected;
static unsigned int fixtureGetCalls,fixtureIntensityCalls,fixtureErrors,fixtureLive,fixtureConstructed,fixtureDestroyed;
// Controlled table/string ownership boundary; every candidate copy and destruction executes.
CCharString::CCharString():PStringData(0){++fixtureLive;++fixtureConstructed;}
CCharString::CCharString(const CCharString&other):PStringData(other.PStringData){++fixtureLive;++fixtureConstructed;if(PStringData)++reinterpret_cast<FixtureStringData*>(PStringData)->fixtureRefs;}
CCharString::~CCharString(){if(PStringData){FixtureStringData*p=reinterpret_cast<FixtureStringData*>(PStringData);if(!p->fixtureRefs)++fixtureErrors;else --p->fixtureRefs;}if(!fixtureLive)++fixtureErrors;else --fixtureLive;++fixtureDestroyed;}
CCharString CDefStringTable::GetString(long code)const{++fixtureGetCalls;if(this!=&GDefStringTable||code!=fixtureExpected||code<100||code>=107)++fixtureErrors;CCharString value;value.PStringData=reinterpret_cast<CCharStringData*>(&fixturePool[(unsigned int)(code-100)%7]);++reinterpret_cast<FixtureStringData*>(value.PStringData)->fixtureRefs;return value;}
long CTCHeroExperience::GetOnHitSoundIntensity()const{++fixtureIntensityCalls;if(this!=fixtureHero)++fixtureErrors;return fixtureIntensity;}
int main(){
    typedef std::pair<const long,CDefString>Value;typedef std::_Rb_tree_node<Value>Node;
    CHeroExperienceDef definition;CTCHeroExperience hero;hero.PDef.Object=&definition;fixtureHero=&hero;
    union Storage{double alignment;unsigned char bytes[sizeof(Node)];}nodes[7],headerSpace;
    Node*header=reinterpret_cast<Node*>(headerSpace.bytes);Node*data[7];const int balanced[7]={3,1,5,0,2,4,6};unsigned int cases=0;
    if((char*)&definition.SoundIntensityThresholds-(char*)&definition!=0x4c||(char*)&hero.PDef-(char*)&hero!=0x54)return 1;
    for(int count=1;count<=7;++count)for(int order=0;order<3;++order){
        header->_M_color=false;header->_M_parent=0;header->_M_left=header->_M_right=header;memcpy(&definition.SoundIntensityThresholds,&header,sizeof(header));
        for(int n=0;n<count;++n){data[n]=reinterpret_cast<Node*>(nodes[n].bytes);CDefString symbol;long code=100+n;memcpy(&symbol,&code,sizeof(code));Value value={(n+1)*10,symbol};new(&data[n]->_M_value_field)Value(value);data[n]->_M_left=data[n]->_M_right=0;}
        for(int step=0;step<(order==2?7:count);++step){int n=order==0?step:(order==1?count-step-1:balanced[step]);if(n>=count)continue;Node*node=data[n];std::_Rb_tree_node_base*parent=header;std::_Rb_tree_node_base*current=header->_M_parent;bool left=false;
            while(current){parent=current;left=node->_M_value_field.first<static_cast<Node*>(current)->_M_value_field.first;current=left?current->_M_left:current->_M_right;}
            node->_M_parent=parent;if(parent==header)header->_M_parent=node;else if(left)parent->_M_left=node;else parent->_M_right=node;
        }
        header->_M_left=data[0];header->_M_right=data[count-1];
        for(long intensity=-5;intensity<=80;intensity+=5){
            fixtureIntensity=intensity;int selected=0;for(int n=0;n<count;++n)if(intensity<=(n+1)*10){selected=n;break;}fixtureExpected=100+selected;
            fixtureErrors=fixtureGetCalls=fixtureIntensityCalls=fixtureLive=fixtureConstructed=fixtureDestroyed=0;for(int n=0;n<7;++n){fixturePool[n].fixtureRefs=0;fixturePool[n].fixtureCode=100+n;}
            {CCharString result=hero.GetOnHitSound();if(result.PStringData!=reinterpret_cast<CCharStringData*>(&fixturePool[selected])||reinterpret_cast<FixtureStringData*>(result.PStringData)->fixtureCode!=fixtureExpected||fixtureGetCalls!=1||fixtureIntensityCalls!=1||fixtureLive!=1||fixtureErrors)return 2;for(int n=0;n<7;++n)if(fixturePool[n].fixtureRefs!=(n==selected?1U:0U))return 3;}
            if(fixtureLive||fixtureErrors||fixtureConstructed!=fixtureDestroyed)return 4;for(int n=0;n<7;++n)if(fixturePool[n].fixtureRefs)return 5;++cases;
        }
        for(int n=0;n<count;++n)data[n]->_M_value_field.~Value();
    }
    if(cases!=378)return 6;puts("006d8bf0_TEST PASS");return 0;
}