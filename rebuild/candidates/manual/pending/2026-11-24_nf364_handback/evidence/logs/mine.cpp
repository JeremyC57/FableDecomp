#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }
void f(CCountedPointer<NUISystem::CComponent>& p, NUISystem::CComponent* c) { p.Reset(c); }
