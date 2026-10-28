// StaticInitialization::_$E1 -- construct the wide string at0143e908 and register its destruction.
// Leaf: _$E1
// Identity: proven CWideString/default lifetime, descriptive global; Own native static initializer loads global0143e908, calls default CWideString constructor0099aed0 then registers destructor callback0122ae6a, which calls CWideString destructor0099b510. Original default constructorbody clears storage_ and incrementswide-instance counter013bca20, independently distinguishing wide from narrow. Shared real fable_string.h reused unchanged; original global spelling unproven.
// Layout: genuine original four-byte CWideString and native compiler-generated static lifetime; no handwritten initializer or atexit wrapper.
#include "fable_string.h"
CWideString g_UnprovenWideString_0143E908;