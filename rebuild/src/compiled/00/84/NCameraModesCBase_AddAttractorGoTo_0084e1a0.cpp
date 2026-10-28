// NCameraModes::CBase::AddAttractorGoTo(const C3DVector& pos, float strength) -- append the camera physics attractor.
// Leaf: ?AddAttractorGoTo@CBase@NCameraModes@@IAEXABVC3DVector@@M@Z
// Identity: proven; both PDB method declaration and named CPhysicsState::Attractors; retail GoTo.Attractors sentinel+120; first two have camera mode OnInit/Update callers, third unreferenced but exact matching API/order and LookAt offset.
// Layout: no scoped base/CPhysicsState header; generated CBase namesake is unrelated. Actual CPhysicsState48 vs donor64 comes from two list12->4 contractions, giving LookAt16/GoTo84. C3DVector actual user copy and STLport list/pair bodies retained.
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