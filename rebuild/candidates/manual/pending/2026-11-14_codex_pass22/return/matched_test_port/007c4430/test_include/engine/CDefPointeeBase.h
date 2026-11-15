#ifndef FABLE_ENGINE_CDEFPOINTEEBASE_H
#define FABLE_ENGINE_CDEFPOINTEEBASE_H
class CDefPointeeBase {public:mutable unsigned long IVRefCount;CDefPointeeBase():IVRefCount(1){}virtual~CDefPointeeBase(){}void AddRef()const{++IVRefCount;}virtual void Delete()const{assert(false);}unsigned long GetRefCount()const{return IVRefCount;}};
#endif
