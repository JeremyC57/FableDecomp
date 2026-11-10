// Isolated fixture for the selected body and original search/filter semantics.
#include <cstdio>
#include <cassert>
namespace std {template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};}
namespace std {
template <class _Ret, class _Tp, class _Arg>
class const_mem_fun1_ref_t : public binary_function<_Tp,_Arg,_Ret> {
  typedef _Ret (_Tp::*__fun_type)(_Arg) const;
public:
  explicit const_mem_fun1_ref_t(__fun_type __pf) : _M_f(__pf) {}
  _Ret operator()(const _Tp& __r, _Arg __x) const { return (__r.*_M_f)(__x); }
private:
  __fun_type _M_f;
};
template <class _Result, class _Tp, class _Arg>
inline const_mem_fun1_ref_t<_Result,_Tp,_Arg>
mem_fun_ref(_Result (_Tp::*__f)(_Arg) const) { return const_mem_fun1_ref_t<_Result,_Tp,_Arg>(__f); }
template <class _Operation> 
class binder2nd
  : public unary_function<typename _Operation::first_argument_type,
                          typename _Operation::result_type> {
protected:
  _Operation _M_op;
  typename _Operation::second_argument_type value;
public:
  binder2nd(const _Operation& __x,
            const typename _Operation::second_argument_type& __y) 
      : _M_op(__x), value(__y) {}

  typename _Operation::result_type
  operator()(const typename _Operation::first_argument_type& __x) const {
    return _M_op(__x, value); 
  }

  typename _Operation::result_type
  operator()(typename _Operation::first_argument_type& __x) const {
    return _M_op(__x, value); 
  }
};

template <class _Operation, class _Tp>
inline binder2nd<_Operation> 
bind2nd(const _Operation& __fn, const _Tp& __x) 
{
  typedef typename _Operation::second_argument_type _Arg2_type;
  return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}
}

enum ETCInterfaceType { TCI_VILLAGE=34 };
class C3DVector { public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){} };
class CTCVillage {public:int marker;};
class CThing {public:
 bool Dead,IncludeInThingScans,Activated,InLimbo,componentPresent;CTCVillage* component;C3DVector position;
 CThing():Dead(false),IncludeInThingScans(true),Activated(true),InLimbo(false),componentPresent(true),component(0){}
 bool IsDead()const{return Dead;}
 bool IsIncludedInThingScans()const{return IncludeInThingScans&&Activated&&!Dead&&!InLimbo;}
 bool HasTC(ETCInterfaceType t)const{return t==TCI_VILLAGE&&componentPresent;}
 const C3DVector& GetPos()const{return position;}
 template<class T> bool GetTC(T** out){if(HasTC(TCI_VILLAGE)){*out=static_cast<T*>(component);return true;}return false;}
 template<class T> bool PeekTC(T** out)const{if(HasTC(TCI_VILLAGE)){*out=static_cast<T*>(component);return true;}return false;}
};
class CThingVillage:public CThing{};
class CThingPlayerCreature:public CThing {public:mutable int positionReads;CThingPlayerCreature():positionReads(0){}const C3DVector& PeekPos()const{++positionReads;return position;}};
class CThingCompare_Nearest {
 C3DVector Pos;
 static float Squared(const C3DVector& a,const C3DVector& b){float x=a.X-b.X,y=a.Y-b.Y,z=a.Z-b.Z;return x*x+y*y+z*z;}
public:CThingCompare_Nearest(const C3DVector& p):Pos(p){}
 template<class T>T* operator()(T* first,T* second)const{if(!first)return second;if(!second)return first;return Squared(first->GetPos(),Pos)<Squared(second->GetPos(),Pos)?first:second;}
};
class CThingSearchTools {public:
 CThingVillage* things[3];int count;mutable int calls;CThingSearchTools():count(0),calls(0){}
 template<class Filter,class Compare,class T> bool GetPBestLeafThing(const Filter& filter,const Compare& compare,T** out)const {
  ++calls;T* best=0;
  for(int i=0;i<count;++i){T* thing=things[i];if(thing->IsIncludedInThingScans()&&filter(*thing))best=compare(best,thing);}
  *out=best;return best!=0;
 }
};
template<class T> bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
class CPlayer {public:
 CThingPlayerCreature* character;CThingSearchTools* tools;mutable int pointerReads;
 CPlayer():character(0),tools(0),pointerReads(0){}
 const CThingPlayerCreature* PeekPPlayerCharacter()const{++pointerReads;return character;}
 CThingPlayerCreature* GetPPlayerCharacter()const{++pointerReads;return character;}
 CThingSearchTools& GetThingSearchTools()const{return *tools;}
 CTCVillage* GetPNearestTCVillage();const CTCVillage* GetPNearestTCVillage()const;
};
CTCVillage* CPlayer::GetPNearestTCVillage()
{
    if(IS_THING_ALIVE(PeekPPlayerCharacter())){
        CThingVillage* pvillage=0;
        if(GetThingSearchTools().GetPBestLeafThing(std::bind2nd(std::mem_fun_ref(&CThing::HasTC),TCI_VILLAGE),
            CThingCompare_Nearest(GetPPlayerCharacter()->PeekPos()),&pvillage)){
            CTCVillage* ptcvillage;
            pvillage->GetTC(&ptcvillage);
            return ptcvillage;
        }
    }
    return 0;
}

int main(){
 CTCVillage components[3];CThingVillage villages[3];CThingPlayerCreature character;
 CThingSearchTools tools;CPlayer player;player.tools=&tools;
 for(int i=0;i<3;++i){components[i].marker=i;villages[i].component=&components[i];tools.things[tools.count++]=&villages[i];}
 assert(player.GetPNearestTCVillage()==0);assert(tools.calls==0&&player.pointerReads==1);
 player.character=&character;character.Dead=true;assert(player.GetPNearestTCVillage()==0);assert(tools.calls==0&&character.positionReads==0);
 character.Dead=false;
 for(int mode=0;mode<3;++mode)for(int mask=0;mask<8;++mask)for(int flags=0;flags<16;++flags)for(int coordinate=0;coordinate<3;++coordinate){
  character.position=C3DVector(float(coordinate-1),0,0);
  for(int i=0;i<3;++i){villages[i].position=C3DVector(float(i-1),float(mode==1?i:0),float(mode==2?2-i:0));villages[i].componentPresent=(mask&(1<<i))!=0;villages[i].Dead=false;villages[i].IncludeInThingScans=true;villages[i].Activated=true;villages[i].InLimbo=false;}
  villages[1].Dead=(flags&1)!=0;villages[1].IncludeInThingScans=(flags&2)==0;villages[1].Activated=(flags&4)==0;villages[1].InLimbo=(flags&8)!=0;
  int expected=-1;float distance=0;
  for(int i=0;i<3;++i){if(!villages[i].componentPresent||!villages[i].IsIncludedInThingScans())continue;float x=villages[i].position.X-character.position.X,y=villages[i].position.Y,z=villages[i].position.Z;float d=x*x+y*y+z*z;if(expected<0||d<=distance){expected=i;distance=d;}}
  int priorReads=player.pointerReads,priorCalls=tools.calls,priorPositions=character.positionReads;
  assert(player.GetPNearestTCVillage()==(expected<0?0:&components[expected]));assert(player.pointerReads==priorReads+2&&tools.calls==priorCalls+1&&character.positionReads==priorPositions+1);
 }
 tools.count=0;assert(player.GetPNearestTCVillage()==0);
 std::puts("00488620_TEST PASS");return 0;
}