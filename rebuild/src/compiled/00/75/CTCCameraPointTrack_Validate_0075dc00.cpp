// CTCCameraPointTrack::Validate(CCharString& error_text) const -- with the parent's shape manager (interface 0x44)
// present, preload the "no track" error text and report whether the manager has a best track
// Leaf: ?Validate@CTCCameraPointTrack@@UBE_NAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075DC00, length 88 bytes. CTCCameraPointTrack vtable 0x0123B0E4 slot 17.
// Source correction (FINDINGS chunk 393: "0x75DC00 CTCCameraPointTrack::Validate (says vfunc_17)"): the catalog name was
//   already Validate but the landed body was `bool vfunc_17(CCharString*)`; Ego_r.pdb
//   `?Validate@CTCCameraPointTrack@@UBE_NAAVCCharString@@@Z` (public virtual const, the text by reference; the
//   CTCCameraPointBuilding / CTCCameraPointScripted overrides sit in the same slot 17 of their vtables, 0x0075D9A0 /
//   0x0075CDF0). The interface-0x44 component is CTCShapeManager and the callee 0x00761B50 is its
//   `?GetBestTrack@CTCShapeManager@@QBE_NPAPBVCTrack@NShape@@@Z` (chunk 393 label fix of "Map_FindByKey").
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0, CCharString::operator=(const char*)
//   @ 0x0099EFE0 (Ego_r `??4CCharString@@QAEABV0@PBD@Z`), CTCShapeManager::GetBestTrack @ 0x00761B50.
// Literal @ 0x0126A060: "There is a CAMERA_POINT_TRACK with no track associated with it. Have a nice day."
// Layout: CTCBase ParentThing +4; CThing bitset +0x20 / interface map +0x44 as the landed family. CTCBase is declared
//   locally with a real vptr (engine/CTCBase.h spells the vptr as a data word, which would add a second one under the
//   virtual Validate).
// Shape: the inline out-parameter GetInterface (key in the `push ecx` slot, map in esi, component in eax -> esi); the
//   error string is the caller's object (`mov ecx,[esp+0Ch]`); the out-pointer local `track` is homed in the now-dead
//   argument slot (`lea ecx,[esp+0Ch]`) and the lookup's bool is returned directly.
class CCharString {
public:
    const CCharString& operator=(const char* text); // 0x0099EFE0
};

namespace NShape { class CTrack; }

class CTCShapeManager {                             // interface type 0x44
public:
    bool GetBestTrack(const NShape::CTrack** out) const;   // 0x00761B50
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCShapeManager* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;           // 0x004365B0
};

class CThing {
public:
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

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
};

class CTCCameraPointTrack : public CTCBase {
public:
    virtual bool Validate(CCharString& error_text) const;
};

bool CTCCameraPointTrack::Validate(CCharString& error_text) const
{
    CTCShapeManager* shapes;
    if (ParentThing->GetInterface(0x44, &shapes)) {
        error_text = "There is a CAMERA_POINT_TRACK with no track associated with it. Have a nice day.";
        const NShape::CTrack* track;
        return shapes->GetBestTrack(&track);
    }
    return false;
}
