#ifndef FABLE_ENGINE_CNAVIGATORQUEUEMANAGER_H
#define FABLE_ENGINE_CNAVIGATORQUEUEMANAGER_H
#include "stlport/fable_containers.h"
class CNavigatorBase;
// Both original PDBs: private const reference Navigator; actual predicate base.
class CIsNavigator:public std::unary_function<std::pair<long,const CNavigatorBase*>,bool> {
 const CNavigatorBase& Navigator;
public:
 CIsNavigator(const CNavigatorBase& navigator):Navigator(navigator){}
 bool operator()(const std::pair<long,const CNavigatorBase*>& value) const {return value.second==&Navigator;}
};
// Both PDBs: sole private list Navigators; native TLC list occupies one word.
class CNavigatorQueueManager {
private:
 std::list<std::pair<long,const CNavigatorBase*> > Navigators;
public:
 void RemoveNavigatorFromQueue(const CNavigatorBase& navigator) {Navigators.remove_if(CIsNavigator(navigator));}
 void AddNavigatorToQueue(const CNavigatorBase&,bool,float);
 bool IsNavigatorAllowedToCalculateRoute(const CNavigatorBase&) const;
 void FrameUpdate();
};
FABLE_STATIC_ASSERT(sizeof(CNavigatorQueueManager)==4);
#endif
