// Variant (pick-any, NF310 D1 form) of 0x00428B10: explicit instantiation of engine/CCountedPointer.h's default DeleteData body over a
// local polymorphic CComponent slot-0 model.
// Leaf: ?DeleteData@?$CCountedPointer@VCComponent@NUISystem@@@@SIXPAX@Z
#include "engine/CCountedPointer.h"

namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }

template void FABLE_FASTCALL CCountedPointer<NUISystem::CComponent>::DeleteData(void*);
