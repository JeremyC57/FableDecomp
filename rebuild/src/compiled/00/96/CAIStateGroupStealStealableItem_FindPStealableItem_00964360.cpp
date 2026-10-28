// CAIStateGroup_StealStealableItem::FindPStealableItem() -- nearest object with the stealable property and a free action point
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964360, length 107 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThing<CFilter_Bind2<CThingFilter_ObjectHasProperty,
// CThingFilter_HasFreeActionPoint>, CThingCompare_Nearest, CThingObject> @ 0x00964230 (manifest name).
//
// Shape: the result pointer is a zero-initialised local returned after the search. The Nearest comparator
// copies the creature's position; the bound filter pair is a CFilter_Bind2 holding two const references: to
// the property filter (one word, property mask 0x08000000, stored as an immediate after the argument pushes)
// and to the EMPTY free-action-point filter, a 1-byte object VC places in the dead high byte of the frame's
// lowest word (`lea edx,[esp+7]`) only when it is a TEMPORARY in the Bind2 construction (a named local takes
// a whole word and shifts the layout). Frame: 0x20 bytes. Tried for the copy order: memberwise copy constructor,
// POD with body assignment, by-value constructor parameter.
#include "engine/CAIStateGroup_StealStealableItem.h"


class C3DVector { public:                              // POD: a whole-record copy loads all three dwords first
    float X, Y, Z;
};

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
};

class CThingObject;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingCompare_Nearest { public:                  // 12 bytes: the reference position
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) { Position = position; }
};

class CThingFilter_ObjectHasProperty { public:         // one word: the property mask
    unsigned long Property;
    CThingFilter_ObjectHasProperty(unsigned long property) : Property(property) {}
};

class CThingFilter_HasFreeActionPoint { public:        // empty filter
    CThingFilter_HasFreeActionPoint() {}
};

template<class A, class B> class CFilter_Bind2 { public:   // two const references
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& first, const B& second) : First(first), Second(second) {}
};

typedef CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint> CStealableFilter;

class CThingSearchTools { public:
    bool GetPBestThing(const CStealableFilter& filter, const CThingCompare_Nearest& compare,
                       CThingObject** result);  // 0x00964230
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the search GetPBestThing runs (returns the best thing or 0)
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThing @ 0x00964230 (compiled in retail's TU): clears the out pointer, stores the best
// thing found, reads the filter and comparator through their const references; no argument escapes.
__declspec(noinline) bool CThingSearchTools::GetPBestThing(const CStealableFilter& filter, const CThingCompare_Nearest& compare, CThingObject** result)
{
    *result = 0;
    long best = NearSink_s7_0(this, *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)result = best;
    return *result != 0;
}
#pragma auto_inline(on)

struct CAIStateGroup_StealStealableItem_Methods : CAIStateGroup_StealStealableItem {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_pad_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CThingObject* FindPStealableItem();
};

CThingObject* CAIStateGroup_StealStealableItem_Methods::FindPStealableItem()
{
    CThingObject* item = 0;
    CThingCompare_Nearest compare(GetPBrain()->PCreature->GetPos());
    CThingFilter_ObjectHasProperty hasProperty(0x08000000);
    PeekThingSearchTools()->GetPBestThing(CStealableFilter(hasProperty, CThingFilter_HasFreeActionPoint()), compare, &item);
    return item;
}