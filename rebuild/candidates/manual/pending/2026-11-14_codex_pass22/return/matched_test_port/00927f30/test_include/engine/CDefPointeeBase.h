#pragma once
class CDefPointeeBase {
public:unsigned long IVRefCount;bool Tracked;
 CDefPointeeBase():IVRefCount(1),Tracked(false){}virtual~CDefPointeeBase();
 unsigned long AddRef()const{return ++const_cast<CDefPointeeBase*>(this)->IVRefCount;}
 unsigned long GetRefCount()const{return IVRefCount;}
 virtual void Delete(){assert(IVRefCount==0);delete this;}
};
