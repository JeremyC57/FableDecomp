// Standalone exact target plus expanded active production headers.
// External tree and string implementations are deterministic fixture dependencies.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public



#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CREATURE_ACTIONS
#define FABLE_RETAIL_ACTION_EVENT_REGISTRY
#define FABLE_ENGINE_CCREATUREACTIONBASE_H
#define FABLE_ENGINE_CBASEOBJECT_H
















#define FABLE_ENGINE_CBASECLASS_H








class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};

class CBaseObjectPointer;
class CBaseObject : public CBaseClass {
protected:
    CBaseObject();                                                       
    CBaseObject(const CBaseObject&);                                     
    CBaseObject& operator=(const CBaseObject&);                          
public:
    void ClearIntelligentPointers();                                     
    virtual void Validate() const;                                       
protected:
    virtual ~CBaseObject();                                              
    void InternalValidate() const;                                       
public:
    CBaseObjectPointer* IntelligentPointer;                              
};

#define FABLE_ENGINE_C3DVECTOR_H


#define FABLE_VECTOR_INLINE inline
class C3DVector {
public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector& other):X(other.X),Y(other.Y),Z(other.Z){}
    FABLE_VECTOR_INLINE void Initialise(float x,float y,float z){X=x;Y=y;Z=z;}
    FABLE_VECTOR_INLINE const C3DVector operator-(const C3DVector& other)const{return C3DVector(X-other.X,Y-other.Y,Z-other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator+(const C3DVector& other)const{return C3DVector(X+other.X,Y+other.Y,Z+other.Z);}
    FABLE_VECTOR_INLINE const C3DVector operator*(float scale)const{return C3DVector(X*scale,Y*scale,Z*scale);}
    FABLE_VECTOR_INLINE bool operator==(const C3DVector& other)const{return X==other.X&&Y==other.Y&&Z==other.Z;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}
    FABLE_VECTOR_INLINE float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
    FABLE_VECTOR_INLINE const float Dot(const C3DVector& other)const{return X*other.X+Y*other.Y+Z*other.Z;}
    FABLE_VECTOR_INLINE const C3DVector Cross(const C3DVector& other)const{return C3DVector(Y*other.Z-Z*other.Y,Z*other.X-X*other.Z,X*other.Y-Y*other.X);}
    C3DVector& AccurateNormalise();
    void Rotate(const C3DVector&,float);
};
#undef FABLE_VECTOR_INLINE



#define FABLE_ENGINE_CCOUNTEDPOINTER_H










































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};

#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};



#define FABLE_EACTIONREGISTEREDTYPE_H

enum EActionRegisteredType {
    ACTION_NULL = 0,
    ACTION_JACK_OF_BLADES_HIT_RESPONSE = 1,
    ACTION_SCORPION_KING_HIT_RESPONSE = 2,
    ACTION_MAZE_CAUSE_FORCE_PUSH_HIT_RESPONSE = 3,
    ACTION_ROCK_TROLL_DEATH = 4,
    ACTION_KING_SCORPION_DEATH = 5,
    ACTION_WASP_QUEEN_DEATH = 6,
    ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_AIM = 7,
    ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_FIRE = 8,
    ACTION_COMBAT_GENERIC_PROJECTILE_WEAPON_LOAD = 9,
    ACTION_COMBAT_GENERIC_LEADER_COMMAND = 10,
    ACTION_COMBAT_GENERIC_BOAST = 11,
    ACTION_COMBAT_UNBLOCKABLE_ATTACK = 12,
    ACTION_COMBAT_HOP_BACK = 13,
    ACTION_COMBAT_STRAFE_FORWARD = 14,
    ACTION_COMBAT_STRAFE_BACKWARD = 15,
    ACTION_COMBAT_STRAFE_LEFT = 16,
    ACTION_COMBAT_STRAFE_RIGHT = 17,
    ACTION_COMBAT_STRAFE_BACK_LEFT = 18,
    ACTION_COMBAT_STRAFE_BACK_RIGHT = 19,
    ACTION_COMBAT_CHARGE = 20,
    ACTION_COMBAT_IDLE = 21,
    ACTION_BREAK_INTO_MELEE = 22,
    ACTION_COMBAT_SUMMON_CREATURES = 23,
    ACTION_COMBAT_ATTACK_LUNGE = 24,
    ACTION_COMBAT_ATTACK_MAIN = 25,
    ACTION_COMBAT_ATTACK_KNOCKDOWN = 26,
    ACTION_COMBAT_ATTACK_SIDE = 27,
    ACTION_COMBAT_ATTACK_SHORT_RANGE = 28,
    ACTION_BANDIT_KING_HIT_RESPONSE = 29,
    ACTION_BANDIT_KING_STUCK_HIT_RESPONSE = 30,
    ACTION_HOBBE_SPELLCASTER_AIM = 31,
    ACTION_HOBBE_SPELLCASTER_FIRE = 32,
    ACTION_HOBBE_LUNGE = 33,
    ACTION_TENTACLE_HIT_RESPONSE = 34,
    ACTION_SCREAMER_DIE = 35,
    ACTION_SCREAMER_DRAIN_ATTACK = 36,
    ACTION_SCREAMER_DRAIN_OUT_OF = 37,
    ACTION_SCREAMER_ADVANCE = 38,
    ACTION_SCREAMER_BACK_OFF = 39,
    ACTION_SCREAMER_IDLE = 40,
    ACTION_COMBAT_BODGE_SIDE_ATTACK = 41,
    ACTION_COMBAT_TURN_STRIKE_LEFT = 42,
    ACTION_COMBAT_TURN_STRIKE_RIGHT = 43,
    ACTION_COMBAT_CHARGE_STRIKE = 44,
    ACTION_NYMPH_GET_HIT = 45,
    ACTION_NYMPH_GET_HIT_DIE = 46,
    ACTION_BALVERINE_LUNGE_ATTACK = 47,
    ACTION_BALVERINE_BREAK_OFF_FROM_COMBAT = 48,
    ACTION_BALVERINE_BREAK_OFF_FROM_COMBAT_LONG = 49,
    ACTION_SUMMONER_FLAME_SLICE = 50,
    ACTION_SUMMONER_UNSHEATHE_STRIKE = 51,
    ACTION_SUMMONER_STRIKE = 52,
    ACTION_BATTLE_CHARGE = 53,
    ACTION_SUMMONER_DIE = 54
};
class CThing;class CThingCreatureBase;class CTCWeaponTrail;


#define FABLE_STLPORT46_GEOMETRY_H

#include <stddef.h>
#include <stdlib.h>
inline void* __cdecl operator new(size_t,void* p){return p;}
namespace std {
struct __false_type {};
template<class T>class allocator {
public:
 allocator(){}
 T* allocate(size_t n,const void* =0){return n?(T*)malloc(n*sizeof(T)):0;}
 void deallocate(T* p,size_t){if(p)free(p);}
};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {
public:V _M_data;_STLP_alloc_proxy(const A& a,V p):A(a),_M_data(p){}
};
template<class A,class B>struct pair {
 typedef A first_type;typedef B second_type;
 A first;B second;
 pair():first(A()),second(B()){}
 pair(const A&a,const B&b):first(a),second(b){}
 template<class C,class D>pair(const pair<C,D>&p):first(p.first),second(p.second){}
};
template<class A,class B>inline pair<A,B> make_pair(const A&a,const B&b){return pair<A,B>(a,b);}
template<class A,class B,class R>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef R result_type;};
template<class A,class R>struct unary_function{typedef A argument_type;typedef R result_type;};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&v)const{return v.first;}};
template<class T>inline const T& (max)(const T&a,const T&b){return a<b?b:a;}
template<class T>inline const T& (min)(const T&a,const T&b){return b<a?b:a;}
template<class T>inline void swap(T&a,T&b){T temporary=a;a=b;b=temporary;}
template<class T,class U>inline void _Construct(T*p,const U&v){new(p)T(v);}
template<class T>inline void _Destroy(T*p){p->~T();}
template<class I>inline void _Destroy(I first,I last){for(;first!=last;++first)_Destroy(&*first);}
template<class T,class A>class _Vector_base {
public:
 _Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}
 ~_Vector_base(){_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}
protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;
};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{
public:
 typedef T value_type;typedef T*iterator;typedef const T*const_iterator;typedef unsigned int size_type;typedef A allocator_type;
 explicit vector(const A&a=A()):_Vector_base<T,A>(a){}
 ~vector(){_Destroy(this->_M_start,this->_M_finish);}
 iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}
 iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}
 size_type size()const{return size_type(this->_M_finish-this->_M_start);}
 size_type capacity()const{return size_type(this->_M_end_of_storage._M_data-this->_M_start);}
 T&operator[](size_type i){return this->_M_start[i];}const T&operator[](size_type i)const{return this->_M_start[i];}
 void reserve(size_type);
 void push_back(const T&v){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,v);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,v,__false_type(),1,true);}
protected:
 void _M_insert_overflow(T*,const T&,const __false_type&,size_type,bool=false);
};
struct _Rb_tree_node_base {
 typedef bool _Color_type;typedef _Rb_tree_node_base*_Base_ptr;
 _Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;
};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class Dummy>class _Rb_global{
public:typedef _Rb_tree_node_base*_Base_ptr;
 static _Base_ptr __cdecl _M_increment(_Base_ptr);
};
template<class Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<Dummy>::_M_increment(_Base_ptr node){
 if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}
 else{_Base_ptr y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}
 return node;
}
typedef _Rb_global<bool> _Rb_global_inst;
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class T>struct _Const_traits{typedef T value_type;typedef const T&reference;typedef const T*pointer;};
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;_Base_ptr _M_node;
 bool operator==(const _Rb_tree_base_iterator& y)const{return _M_node==y._M_node;}
 bool operator!=(const _Rb_tree_base_iterator& y)const{return _M_node!=y._M_node;}
};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef _Rb_tree_iterator<T,Traits>_Self;typedef _Rb_tree_iterator<T,_Nonconst_traits<T> >iterator;
 typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;
 _Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*n){_M_node=n;}
 _Rb_tree_iterator(const iterator&i){_M_node=i._M_node;}
 reference operator*()const{return ((_Rb_tree_node<T>*)_M_node)->_M_value_field;}
 pointer operator->()const{return &(operator*());}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return *this;}
 _Self operator++(int){_Self old=*this;_M_node=_Rb_global_inst::_M_increment(_M_node);return old;}
};
template<class T,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<T>_Node;
 _Rb_tree_base(const A&a):_M_header(allocator<_Node>(),(_Node*)0){_M_header._M_data=_M_header.allocate(1);}
 ~_Rb_tree_base(){_M_header.deallocate(_M_header._M_data,1);}
protected:_STLP_alloc_proxy<_Node*,_Node,allocator<_Node> >_M_header;
};
template<class K,class V,class KeyOfValue,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{
public:
 typedef K key_type;typedef V value_type;typedef _Rb_tree_node<V>*_Link_type;typedef _Rb_tree_node_base*_Base_ptr;
 typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 _Rb_tree():_Rb_tree_base<V,A>(A()),_M_node_count(0),_M_key_compare(Compare()){
  this->_M_header._M_data->_M_color=false;_M_root()=0;_M_leftmost()=this->_M_header._M_data;_M_rightmost()=this->_M_header._M_data;
 }
 _Rb_tree(const _Rb_tree&);
 ~_Rb_tree(){clear();}
 iterator begin(){return iterator(_M_leftmost());}const_iterator begin()const{return const_iterator(_M_leftmost());}
 iterator end(){return iterator(this->_M_header._M_data);}const_iterator end()const{return const_iterator(this->_M_header._M_data);}
 bool empty()const{return _M_node_count==0;}unsigned int size()const{return _M_node_count;}
 void clear(){if(_M_node_count){_M_erase(_M_root());_M_leftmost()=this->_M_header._M_data;_M_root()=0;_M_rightmost()=this->_M_header._M_data;_M_node_count=0;}}
 pair<iterator,bool>insert_unique(const V&);
 iterator find(const K&);
 void erase(iterator);
protected:
 unsigned int _M_node_count;Compare _M_key_compare;
 _Link_type&_M_root()const{return (_Link_type&)this->_M_header._M_data->_M_parent;}
 _Link_type&_M_leftmost()const{return (_Link_type&)this->_M_header._M_data->_M_left;}
 _Link_type&_M_rightmost()const{return (_Link_type&)this->_M_header._M_data->_M_right;}
protected:
 static _Link_type& __cdecl _S_left(_Base_ptr x){return (_Link_type&)x->_M_left;}
 static _Link_type& __cdecl _S_right(_Base_ptr x){return (_Link_type&)x->_M_right;}
private:void _M_erase(_Link_type);
};
template<class K,class V,class KeyOfValue,class Compare,class A>
void _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_erase(_Link_type x){
 while(x){_M_erase(_S_right(x));_Link_type y=_S_left(x);_Destroy(&x->_M_value_field);this->_M_header.deallocate(x,1);x=y;}
}
template<class K,class Compare=less<K>,class A=allocator<K> >class set{
 typedef _Rb_tree<K,K,_Identity<K>,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef typename _Rep_type::const_iterator iterator;typedef iterator const_iterator;
 iterator begin()const{return _M_t.begin();}iterator end()const{return _M_t.end();}
 unsigned int size()const{return _M_t.size();}bool empty()const{return _M_t.empty();}void clear(){_M_t.clear();}
 pair<iterator,bool>insert(const K&v){typedef typename _Rep_type::iterator _Rep_iterator;pair<_Rep_iterator,bool>p=_M_t.insert_unique(v);return pair<iterator,bool>(p.first,p.second);}
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class map{public:
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:
 typedef pair<const K,T>value_type;typedef typename _Rep_type::iterator iterator;typedef typename _Rep_type::const_iterator const_iterator;
 iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}
 pair<iterator,bool>insert(const value_type&v){return _M_t.insert_unique(v);}
 iterator find(const K&k){return _M_t.find(k);}void erase(iterator i){_M_t.erase(i);}T&operator[](const K&);
};
template<class K,class T,class Compare=less<K>,class A=allocator<pair<const K,T> > >class multimap{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,Compare,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;void erase(iterator i){_M_t.erase(i);}
};
}

namespace NCreatureAction {class CActionEventBase;}


class CCreatureActionBase : public CBaseObject {
public:
 static void __fastcall RegisterActionEventTypes();
private:
 static std::map<CCharString,CCountedPointer<NCreatureAction::CActionEventBase> > RegisteredActionEvents;
 static bool DebugEventTrace;
public:
    static CCountedPointer<CCreatureActionBase> __fastcall GetPActionFromEnum(EActionRegisteredType,CThingCreatureBase&);
public:
    virtual ~CCreatureActionBase();
public:
    virtual void EditUpdate(float);
public:
    virtual void PerformAction() =0;
public:
    virtual void FinishAction();
public:
    virtual bool IsPlayingAnimation() const;
public:
    virtual long GetAnimDelayTime() const;
public:
    virtual long GetAnimationIndex() const;
public:
    virtual long GetAnimationTransitionInTime() const;
public:
    virtual long GetAnimationTransitionOutTime() const;
public:
    virtual unsigned long GetAnimExtraFlags() const;
public:
    virtual CCreatureActionBase * Clone() const =0;
public:
    virtual void Initialise();
public:
    virtual bool IsStrikeEvent(long) const;
public:
    virtual bool IsStrikeEventActive() const;
public:
    virtual bool IsStrikeAction() const;
public:
    virtual bool IsPartOfComboChain() const;
public:
    virtual bool IsStrikeEventFinishedBeingActive() const;
public:
    virtual bool IsVitalStrike() const;
public:
    virtual bool IsKnockdownStrike() const;
public:
    virtual bool IsBlockCounter() const;
public:
    virtual bool IsMeleeFlourish() const;
public:
    virtual bool IsActionWithStrikeEventHelperName() const;
public:
    virtual CCharString GetStrikeEventHelperName() const;
public:
    virtual bool IsOverrideableByActionOfSamePriority() const;
public:
    virtual void OnFrameUpdate(float);
public:
    virtual bool IsQueuable() const;
public:
    virtual bool IsActionReadyForInterruption() const;
public:
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector &) const;
public:
    virtual bool IsBlockable() const;
public:
    virtual bool IsAffectedByHaste() const;
public:
    virtual bool IsAffectedByHeroStats() const;
public:
    virtual bool IsPhysicsPushableByHero() const;
public:
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
public:
    virtual bool IsAlwaysUseAnimationMovement() const;
public:
    virtual bool IsClearingMovementVector() const;
public:
    virtual bool IsToHaveEmptyHands() const;
public:
    virtual bool IsToReplaceItemIfRemoved() const;
public:
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;
public:
    virtual bool IsPreventingQuickAccessItemUsage() const;
public:
    virtual void SetHitter(const CThing &);
public:
    virtual void SetHitDirection(C3DVector);
public:
    virtual void SetHitPosition(C3DVector);
public:
    virtual CTCWeaponTrail * GetWeaponTrailUsedInStrikeAction(long &) const;
public:
    virtual float GetTotalTimeSeconds() const;
public:
    virtual bool IsToEvadeAllHits() const;
public:
    virtual bool IsToCauseRecoil() const;
public:
    virtual bool IsAbleToBeAttacked() const;
public:
    virtual bool IsPlayHitResponse() const;
public:
    virtual bool IsPreventingNormalMovement() const;
public:
    virtual bool IsPreventingLookTurning() const;
public:
    virtual void OnBlowBlocked(bool);
public:
    virtual void OnFollowUpActionFailedToAdd();
public:
    virtual void UpdateActionRunningTime();
public:
    virtual bool IsDecapitatingAttack() const;
public:
    virtual bool PreventSneaking() const;
protected:
    virtual void FrameUpdate();
protected:
    virtual bool IsUsingAnimationMovement() const;
protected:
    virtual bool IsUsingForcedMovement() const;
protected:
    virtual bool IsMovementUsingPhysics() const;
protected:
    virtual void UpdateActionMoveToPos();
protected:
    virtual void InitialiseActionMovement();
protected:
    virtual float GetFrameUpdateTime() const;
protected:
    virtual float GetActionSpeedParameter() const;
protected:
    virtual float GetActionStartRunningTimeSeconds() const;
protected:
    virtual bool OverrideFaceWithDefault() const;
protected:
    virtual bool AllowsCombinationAnims() const;
protected:
    virtual bool CanAnimationTransitionOut() const;
protected:
    virtual void OnAnimationLoaded();
protected:
    virtual bool WaitForMeshToLoad() const;
protected: unsigned char _pad_0x08[0xa0];
};
#define FABLE_ENGINE_ACTION_EVENTS_H






class CCreatureActionBase;class CThingCreatureBase;class CGameCameraManager;
enum EQuakeStrength {QUAKE_STRENGTH_WEAK=0, QUAKE_STRENGTH_MEDIUM=1, QUAKE_STRENGTH_STRONG=2, QUAKE_STRENGTH_MADNESS=3};
enum EQuakeLength {QUAKE_LENGTH_SHORT=0, QUAKE_LENGTH_MEDIUM=1, QUAKE_LENGTH_LONG=2, QUAKE_LENGTH_CONTINUOUS=3};
namespace NCreatureAction {
class CActionEventBase:public CBaseClass {
public:
 virtual void Dispatch(CCreatureActionBase&)=0;
 virtual CCharString GetHook()const=0;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const=0;
 virtual bool SetDataFromString(const CCharString&)=0;
protected:
 CActionEventBase():Finished(false){}
private:
 bool Finished;long ActivationFrame;CThingCreatureBase*PThing;CGameCameraManager*PCameraManager;
};
class CActionEventTriggerParticle:public CActionEventBase {
public:
 CActionEventTriggerParticle(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 long AttachmentFlags; 
 CCharString AttachmentObject; 
 CCharString EffectName; 
 bool Weapon; 
 bool DropRatherThanAttach; 
};
class CActionEventCameraShake:public CActionEventBase {
public:
 CActionEventCameraShake(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 EQuakeStrength Strength; 
 EQuakeLength Length; 
 bool Global; 
};
class CActionEventSound:public CActionEventBase {
public:
 CActionEventSound(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
};
class CActionEventShot:public CActionEventBase {
public:
 CActionEventShot(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
};
class CActionEventExplosion:public CActionEventBase {
public:
 CActionEventExplosion(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString AttachmentObject; 
 long ExplosionDefIndex; 
 bool Dummy; 
 bool UseActionHelper; 
};
class CActionEventAddObject:public CActionEventBase {
public:
 CActionEventAddObject(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString Object; 
 CCharString Hand; 
 bool DestroyOnExit; 
};
class CActionEventRemoveObject:public CActionEventBase {
public:
 CActionEventRemoveObject(){}
 virtual void Dispatch(CCreatureActionBase&);
 virtual CCharString GetHook()const;
 virtual CActionEventBase* Alloc(long,CThingCreatureBase&,CGameCameraManager&)const;
protected:
 virtual bool SetDataFromString(const CCharString&);
private:
 CCharString Hand; 
};
}
void __fastcall CCreatureActionBase::RegisterActionEventTypes()
{
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventTriggerParticle;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventCameraShake;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventSound;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventShot;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventExplosion;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventAddObject;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 {
  CCountedPointer<NCreatureAction::CActionEventBase> cp_event = new NCreatureAction::CActionEventRemoveObject;
  RegisteredActionEvents.insert(std::make_pair(cp_event->GetHook(),cp_event));
 }
 DebugEventTrace=false;
}

static long live_events=0,live_strings=0,hook_calls=0,last_hook_type=-1;
static unsigned hook_mode=0;
static const char*event_names[]={"PARTICLE","RUMBLE","SOUND","SHOT","EXPLOSION","CARRYTHINGADD","CARRYTHINGREMOVE"};
static unsigned key_for(unsigned type){return hook_mode==0?type:hook_mode==1?type%3:0;}
CBaseClass::CBaseClass(){++live_events;}CBaseClass::~CBaseClass(){--live_events;}
class CCharStringData {public:unsigned references;char text[32];};
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*text,long length):PStringData(new CCharStringData){++live_strings;PStringData->references=1;unsigned n=length<0?(unsigned)strlen(text):(unsigned)length;assert(n<32);memcpy(PStringData->text,text,n);PStringData->text[n]=0;}
CCharString::CCharString(const CCharString&other):PStringData(other.PStringData){if(PStringData)++PStringData->references;}
CCharString::~CCharString(){if(PStringData&&!--PStringData->references){delete PStringData;--live_strings;}}
static const char*text_of(const CCharString&value){return value.PStringData?value.PStringData->text:"";}
namespace std {
template<class K,class V,class Key,class Compare,class A>
pair<typename _Rb_tree<K,V,Key,Compare,A>::iterator,bool>_Rb_tree<K,V,Key,Compare,A>::insert_unique(const V&value){
 _Rb_tree_node<V>*header=this->_M_header._M_data;_Rb_tree_node_base*parent=header;_Rb_tree_node_base*current=header->_M_parent;bool left=true;
 while(current){parent=current;const V&old=((_Rb_tree_node<V>*)current)->_M_value_field;int order=strcmp(text_of(value.first),text_of(old.first));if(order<0){left=true;current=current->_M_left;}else if(order>0){left=false;current=current->_M_right;}else return pair<iterator,bool>(iterator((_Rb_tree_node<V>*)current),false);}
 _Rb_tree_node<V>*node=(_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));assert(node);new(&node->_M_value_field)V(value);node->_M_parent=parent;node->_M_left=node->_M_right=0;node->_M_color=false;
 if(parent==header){header->_M_parent=node;header->_M_left=header->_M_right=node;}else if(left){parent->_M_left=node;if(header->_M_left==parent)header->_M_left=node;}else{parent->_M_right=node;if(header->_M_right==parent)header->_M_right=node;}
 ++_M_node_count;return pair<iterator,bool>(iterator(node),true);
}
}
typedef CCountedPointer<NCreatureAction::CActionEventBase> EventPointer;
typedef std::map<CCharString,EventPointer> EventMap;
EventMap CCreatureActionBase::RegisteredActionEvents;
bool CCreatureActionBase::DebugEventTrace=false;

void NCreatureAction::CActionEventTriggerParticle::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventTriggerParticle::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventTriggerParticle::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventTriggerParticle::GetHook()const{++hook_calls;last_hook_type=0;return CCharString(event_names[key_for(0)]);}

void NCreatureAction::CActionEventCameraShake::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventCameraShake::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventCameraShake::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventCameraShake::GetHook()const{++hook_calls;last_hook_type=1;return CCharString(event_names[key_for(1)]);}

void NCreatureAction::CActionEventSound::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventSound::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventSound::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventSound::GetHook()const{++hook_calls;last_hook_type=2;return CCharString(event_names[key_for(2)]);}

void NCreatureAction::CActionEventShot::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventShot::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventShot::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventShot::GetHook()const{++hook_calls;last_hook_type=3;return CCharString(event_names[key_for(3)]);}

void NCreatureAction::CActionEventExplosion::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventExplosion::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventExplosion::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventExplosion::GetHook()const{++hook_calls;last_hook_type=4;return CCharString(event_names[key_for(4)]);}

void NCreatureAction::CActionEventAddObject::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventAddObject::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventAddObject::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventAddObject::GetHook()const{++hook_calls;last_hook_type=5;return CCharString(event_names[key_for(5)]);}

void NCreatureAction::CActionEventRemoveObject::Dispatch(CCreatureActionBase&){assert(false);}
NCreatureAction::CActionEventBase*NCreatureAction::CActionEventRemoveObject::Alloc(long,CThingCreatureBase&,CGameCameraManager&)const{assert(false);return 0;}
bool NCreatureAction::CActionEventRemoveObject::SetDataFromString(const CCharString&){assert(false);return false;}
CCharString NCreatureAction::CActionEventRemoveObject::GetHook()const{++hook_calls;last_hook_type=6;return CCharString(event_names[key_for(6)]);}

static NCreatureAction::CActionEventBase*make_event(unsigned type){switch(type){
case 0:return new NCreatureAction::CActionEventTriggerParticle;
case 1:return new NCreatureAction::CActionEventCameraShake;
case 2:return new NCreatureAction::CActionEventSound;
case 3:return new NCreatureAction::CActionEventShot;
case 4:return new NCreatureAction::CActionEventExplosion;
case 5:return new NCreatureAction::CActionEventAddObject;
case 6:return new NCreatureAction::CActionEventRemoveObject;
default:assert(false);return 0;}}

static void run(unsigned seed,unsigned repeats,unsigned mode,bool initial_debug){
 EventMap&map=CCreatureActionBase::RegisteredActionEvents;map._M_t.clear();assert(live_events==0&&live_strings==0);hook_mode=mode;
 int expected[7];for(unsigned i=0;i<7;++i)expected[i]=-1;
 for(unsigned i=0;i<seed;++i){unsigned type=6-i,key=key_for(type);EventPointer value(make_event(type));map.insert(std::make_pair(CCharString(event_names[key]),value));if(expected[key]==-1)expected[key]=(int)type;}
 CCreatureActionBase::DebugEventTrace=initial_debug;hook_calls=0;
 for(unsigned n=0;n<repeats;++n){CCreatureActionBase::RegisterActionEventTypes();assert(!CCreatureActionBase::DebugEventTrace);for(unsigned i=0;i<7;++i){unsigned key=key_for(i);if(expected[key]==-1)expected[key]=(int)i;}}
 assert(hook_calls==(long)(repeats*7));unsigned expected_size=0;for(unsigned i=0;i<7;++i)if(expected[i]!=-1)++expected_size;assert(map._M_t.size()==expected_size);assert(live_events==(long)expected_size);
 unsigned seen=0;for(EventMap::iterator it=map.begin();it!=map.end();++it){unsigned key=0;while(key<7&&strcmp(text_of(it->first),event_names[key]))++key;assert(key<7&&expected[key]!=-1);assert(it->second.Data&&it->second.Info);assert(it->second.Info->RefCount==1);assert(it->second.Info->Data==it->second.Data);assert(it->second.Info->DeleteFunc==&EventPointer::DeleteData);assert(!it->second.Data->Finished);CCharString hook=it->second->GetHook();assert(last_hook_type==expected[key]);assert(strcmp(text_of(hook),text_of(it->first))==0);seen|=1u<<key;}
 for(unsigned key=0;key<7;++key)assert(bool(seen&(1u<<key))==(expected[key]!=-1));map._M_t.clear();assert(live_events==0&&live_strings==0);
}
int main(){
#ifdef _MSC_VER
 assert(sizeof(NCreatureAction::CActionEventBase)==20&&sizeof(NCreatureAction::CActionEventTriggerParticle)==36&&sizeof(NCreatureAction::CActionEventCameraShake)==32&&sizeof(NCreatureAction::CActionEventSound)==20&&sizeof(NCreatureAction::CActionEventShot)==20&&sizeof(NCreatureAction::CActionEventExplosion)==32&&sizeof(NCreatureAction::CActionEventAddObject)==32&&sizeof(NCreatureAction::CActionEventRemoveObject)==24&&sizeof(EventMap)==12&&sizeof(EventPointer)==8);
#endif
 for(unsigned seed=0;seed<=7;++seed)for(unsigned repeats=1;repeats<=4;++repeats)for(unsigned mode=0;mode<3;++mode)for(unsigned debug=0;debug<2;++debug)run(seed,repeats,mode,debug!=0);
 puts("006932b0_TEST PASS");return 0;
}
