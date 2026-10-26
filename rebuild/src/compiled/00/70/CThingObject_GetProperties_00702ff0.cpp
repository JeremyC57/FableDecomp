// CThingObject::GetProperties() const -- the object's property bits: its def's Property word (CThingObjectDef::Property)
// Leaf: ?GetProperties@CThingObject@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00702FF0, 13 bytes: mov eax,[ecx+0D0h] ; mov eax,[eax+90h] ; ret (vtable 0x01264DCC slot 48).
// Identity: landed as CThingObject::vfunc_48 (placeholder). Ego_r/FableWin name ?GetProperties@CThingObject@@UBEKXZ, the
//   CThingObject override of the slot-48 property query that CThingMarker::GetProperties 0x0083D280 (CThingMarker vtable 0x01245524
//   slot 48) and CThingFilter_AICreatureHasProperty 0x006A75D0 also use; CThingFilter_ObjectHasProperty 0x006A7590 calls it on
//   CThingObjects (callers type-test with mask 0x807).
// Layout: CThingObject::PDef (CDefPointer<const CThingObjectDef>) Ego_r +0xE8, retail +0xD0; CThingObjectDef::Property (unsigned long)
//   Ego_r +0x94, retail +0x90 (the 4-byte shift CThingMarkerDef::Properties shows, +0x88 -> +0x84).
class CThingObjectDef {
public:
    unsigned char _pad_0x00[0x90];
    unsigned long Property;                           // +0x90
};

template <class T> class CDefPointer {
public:
    T* PDef;
    T* operator->() const { return PDef; }
};

class CThingObject {
public:
    virtual unsigned long GetProperties() const;      // vtable slot 48 in retail (the earlier slots are not modelled)
    unsigned char _pad_0x04[0xd0 - 0x04];
    CDefPointer<const CThingObjectDef> PDef;          // +0xD0
};

unsigned long CThingObject::GetProperties() const
{
    return PDef->Property;
}
