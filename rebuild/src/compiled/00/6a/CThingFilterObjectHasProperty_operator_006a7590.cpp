// CThingFilter_ObjectHasProperty::operator()(const CThingObject& object) const -- whether the object's GetProperties() (virtual slot 48) shares a bit with the filter's Property
// Leaf: ??RCThingFilter_ObjectHasProperty@@QBE_NABVCThingObject@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7590, 27 bytes (`ret 4`).
// Identity: landed as CThingFilter_MarkerHasProperty::operator(), a label shared with its byte-identical neighbour 0x006A75B0. Retail
//   has no ICF, so the two are different filters: every caller of 0x006A7590 that type-tests does so with the CThingObject mask
//   0x807 (GetPBestThing 0x00964230 / 0x009650C0 / 0x00965E80, GetAllThingsInRoughArea 0x0091ADB0), while 0x006A75B0's callers use
//   the CThingMarker mask 0x8007 (0x00964300) and the marker type list 0xB (GetAllLeafThings 0x00964EF0). Ego_r/FableWin name
//   ??RCThingFilter_ObjectHasProperty@@QBE_NABVCThingObject@@@Z and ??RCThingFilter_MarkerHasProperty@@QBE_NABVCThingMarker@@@Z, and
//   GetAllThingsInRoughArea<CThingObject, list<CThingObject*>, CFilter_Bind2<CThingFilter_ObjectHasProperty, CThingFilter_In2DRange>>.
// Callees: the object's virtual slot 48 (+0xC0) = CThingObject::GetProperties 0x00702FF0 (vtable 0x01264DCC slot 48).
// Layout: Ego_r CThingFilter_ObjectHasProperty = { EObjectProperty Property; } (4 bytes).
// Shape: Property is copied to a local before the virtual call (kept in esi, retail's `mov esi,[ecx]`); `!= 0` of the AND gives `neg ; sbb ; neg`.
enum EObjectProperty { };

class CThingObject {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual unsigned long GetProperties() const;      // slot 48 (+0xC0)
};

class CThingFilter_ObjectHasProperty {
public:
    EObjectProperty Property;                         // +0x00
    bool operator()(const CThingObject& object) const;
};

bool CThingFilter_ObjectHasProperty::operator()(const CThingObject& object) const
{
    unsigned long property = Property;
    return (property & object.GetProperties()) != 0;
}
