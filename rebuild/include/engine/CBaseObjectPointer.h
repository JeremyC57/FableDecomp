#ifndef FABLE_ENGINE_CBASEOBJECTPOINTER_H
#define FABLE_ENGINE_CBASEOBJECTPOINTER_H
class CBaseObject;
class CBaseObjectPointer { public: CBaseObject* object; unsigned long refcount; };
#endif
