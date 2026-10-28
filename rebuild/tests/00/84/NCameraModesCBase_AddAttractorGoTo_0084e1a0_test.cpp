#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);
#else
#include <stdlib.h>
#endif

namespace std {
template<class A,class B> struct pair { A first; B second; pair(const A& a,const B& b):first(a),second(b) {} pair(const pair& p):first(p.first),second(p.second) {} };
class _List_node_base { public: _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class T> class _List_node:public _List_node_base { public: T _M_data; };
template<class T> class allocator {};
template<class T,class Alloc=allocator<T> > class list { public: _List_node_base* _M_node; void push_back(const T& value) { insert(_M_node,value); } void insert(_List_node_base* position,const T& value) { _List_node<T>* node=(_List_node<T>*)malloc(sizeof(_List_node<T>)); new(&node->_M_data) T(value); _List_node_base* previous=position->_M_prev; node->_M_next=position; node->_M_prev=previous; previous->_M_next=node; position->_M_prev=node; } };
}
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}};
class CPhysicsState{public:C3DVector Position,Velocity,Acceleration;std::list<std::pair<C3DVector,float> > Attractors,IncidentalAttractors;bool CullPhysics;};
class CGameCameraManager;namespace NCameraModes{class CBase{public:virtual ~CBase(){}float DefaultFOV,DefaultRollAngle,DefaultObservedPositionZOffset;CPhysicsState LookAt;CGameCameraManager* CameraManager;void* PCameraModeDef;unsigned char SubComponents[12];CPhysicsState GoTo;protected:void AddAttractorGoTo(const C3DVector&,float);};}
void NCameraModes::CBase::AddAttractorGoTo(const C3DVector& pos,float strength){GoTo.Attractors.push_back(std::pair<C3DVector,float>(pos,strength));}
int main(){class TestMode:public NCameraModes::CBase{public:using NCameraModes::CBase::AddAttractorGoTo;};TestMode obj;std::_List_node_base root;root._M_next=root._M_prev=&root;obj.GoTo.Attractors._M_node=&root;C3DVector p;p.X=1.5f;p.Y=-2.0f;p.Z=7.0f;obj.AddAttractorGoTo(p,3.5f);obj.AddAttractorGoTo(p,-0.5f);{typedef std::_List_node<std::pair<C3DVector,float> > Node;Node* first=(Node*)root._M_next;Node* last=(Node*)root._M_prev;if(first==last || first->_M_prev!=&root || first->_M_next!=last || last->_M_prev!=first || last->_M_next!=&root || first->_M_data.first.X!=1.5f || first->_M_data.first.Y!=-2.0f || first->_M_data.first.Z!=7.0f || first->_M_data.second!=3.5f || last->_M_data.second!=-0.5f)goto fail;free(first);free(last);}puts("0084e1a0_TEST PASS");return 0;fail:puts("0084e1a0_TEST FAIL");return 1;}