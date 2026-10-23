// CTCCameraPointTrack::vfunc_17(CCharString* errorText) -- with the parent's interface-0x44 component present, preloads the "no track" error text and returns whether the component has a track
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DC00, length 88 bytes. CTCCameraPointTrack vtable 0x0123B0E4 slot 17.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CCharString::operator=(const char*)
// @ 0x0099EFE0 (landed callers), the component's track lookup @ 0x00761B50 (`bool (T** out)`: a find over its
// counted-pointer list that writes the hit through the out pointer; the sibling 0x0075DC60 reads it back).
// Literal @ 0x0126A060: "There is a CAMERA_POINT_TRACK with no track associated with it. Have a nice day."
// Layout: engine/CTCBase.h (ParentThing +4); CThing bitset +0x20 / interface map +0x44 as the landed family.
// Shape: the inline out-parameter GetInterface (key in the `push ecx` slot, map in esi, component in
// eax -> esi); the error string is the caller's object (`mov ecx,[esp+0Ch]` = the pointer argument);
// the out-pointer local `track` is homed in the now-dead errorText argument slot (`lea ecx,[esp+0Ch]`)
// and the lookup's bool is returned directly.
#include "engine/CTCBase.h"

struct CCharString {
    const CCharString& operator=(const char* text); // 0x0099EFE0
};

struct CCameraTrack;

struct CTCCameraTrackOwner {                        // the interface-0x44 component (class not established)
    bool GetTrack(CCameraTrack** out);              // 0x00761B50
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCCameraTrackOwner* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};

struct CTCCameraPointTrack : CTCBase {
    bool vfunc_17(CCharString* errorText);
};

bool CTCCameraPointTrack::vfunc_17(CCharString* errorText)
{
    CTCCameraTrackOwner* owner;
    if (ParentThing->GetInterface(0x44, &owner)) {
        *errorText = "There is a CAMERA_POINT_TRACK with no track associated with it. Have a nice day.";
        CCameraTrack* track;
        return owner->GetTrack(&track);
    }
    return false;
}