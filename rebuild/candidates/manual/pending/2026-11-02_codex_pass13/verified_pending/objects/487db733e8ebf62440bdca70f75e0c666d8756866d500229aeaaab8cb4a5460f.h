#if defined(FABLE_RETAIL_REGION_CONNECTIVITY_TYPES) || defined(FABLE_RETAIL_RENDER2D_TYPES)
#ifndef FABLE_ENGINE_CINITBASECLASS_H
#define FABLE_ENGINE_CINITBASECLASS_H
#include "engine/CBaseClass.h"
// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();virtual ~CInitBaseClass();
virtual void Validate() const;
#ifdef FABLE_RETAIL_RENDER2D_TYPES
protected:bool Initialise();void Uninitialise();public:bool IsValid() const;
#endif
};
#endif
#else
#ifndef FABLE_ENGINE_CINITBASECLASS_H
#define FABLE_ENGINE_CINITBASECLASS_H
#include "engine/CBaseClass.h"
// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
};
#endif
#endif
