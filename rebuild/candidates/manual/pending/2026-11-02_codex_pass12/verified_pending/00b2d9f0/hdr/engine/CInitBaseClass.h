#ifndef FABLE_ENGINE_CINITBASECLASS_H
#define FABLE_ENGINE_CINITBASECLASS_H
#include "engine/CBaseClass.h"
// Layout: original base class4 bytes plus private Valid+4, native size8.
class CInitBaseClass:public CBaseClass {private:bool Valid;public:CInitBaseClass();~CInitBaseClass();
#if defined(FABLE_RETAIL_CAMERA_DRAW) || defined(FABLE_RETAIL_CENTERED_TEXT) || defined(FABLE_RETAIL_TEXTURE_CACHE)
virtual void Validate()const;
#endif
};
#endif
