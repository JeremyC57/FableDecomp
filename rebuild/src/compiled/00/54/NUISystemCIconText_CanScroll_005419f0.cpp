// NUISystem::CIconText::CanScroll(bool, C2DBoxF&) -- an icon text never scrolls (returns 0.0f, the pooled literal at 0x0122DEDC); vtable slot 88
// Leaf: ?CanScroll@CIconText@NUISystem@@UAEM_NAAVC2DBoxF@@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/NUISystem_CIconText.h"
namespace NUISystem {
float CIconText::CanScroll(bool, C2DBoxF&) { return 0.0f; }
}
