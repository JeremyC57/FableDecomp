// NScript::CPredicate_And<CIsThingAlive,CIsThingAlive>::Clone() -- Clone the conjunction with independent predicate copies.
// Leaf: ?Clone@?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@UBEPAVCPredicateBase@2@XZ
// Identity: original NScript CPredicate_And and original CIsThingAlive/CIsThingAlive names in both PDB name sets; own aggregate vtable012e1b70 contains evaluation/Clone/destructor slots0/1/2, embedded tables012c32f0/012c32f0 identify separate operand types. Original CScriptThing(const CScriptThing&)004abe90.
// Layout: active CScriptThing header aliased for actual CBaseClass inheritance and real counted pointer. Direct original CPredicateBase interface order is operator()/Clone/dtor (no invented CBaseClass root); nested Predicate1+4,Predicate2+20, each Thing+4. PC conjunction size36.
#include <new>
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace NScript{
class CPredicateBase{public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase{public:CScriptThing Thing;virtual bool operator()()const;virtual CPredicateBase* Clone()const;};
class CIsThingConscious:public CPredicateBase{public:CScriptThing Thing;virtual bool operator()()const;virtual CPredicateBase* Clone()const;};
template<class P1,class P2>class CPredicate_And:public CPredicateBase{public:P1 Predicate1;P2 Predicate2;CPredicate_And(const P1& p,const P2& q):Predicate1(p),Predicate2(q){}virtual bool operator()()const{return Predicate1()&&Predicate2();}virtual CPredicateBase* Clone()const{return new CPredicate_And(Predicate1,Predicate2);}};
}
template class NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>;
#ifdef _MSC_VER
typedef char ExactSize[sizeof(NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>)==36?1:-1];
#endif