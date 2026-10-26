// CTCActionUseReadable::OnActionUse(CThing& user) -- base OnActionUse; unless the readable uses the info display, and when it has a text tag, give a live creature user the action of reading that text
// Leaf: ?OnActionUse@CTCActionUseReadable@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E80C0, length 116 bytes (+12 int3 padding). Vtable slot 28 of
// CTCActionUseReadable (0x0123F234). `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified), CWideString::operator!=(const wchar_t*) @ 0x0099B150
// (the empty literal 0x0122D70C), CCreatureAction_Read::CCreatureAction_Read(CThingCreatureBase&, const CWideString&) @ 0x007E7FD0,
// CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped), CWideString::~CWideString @ 0x0099B510,
// ~CCreatureActionBase @ 0x00693EF0 (catalog "CPhysicsMeshInfo::~CPhysicsMeshInfo").
// Shape: &TextTag formed once (`add esi,18h`) and reused for the ctor argument; the user reference cast to a creature pointer is
// null-tested after the def/text tests; the 0xAC-byte action temporary (CCreatureActionBase 0xA8 + TextTag) passed by reference
// straight from the ctor's eax and destroyed inline with no vtable store (a novtable leaf).
// Layout: PDB CTCActionUseReadable: PDef +0x14 (CDefPointer<CReadableDef const>), TextTag +0x18; CReadableDef UseInfoDisplay
// +0x25; retail CThing Dead = bit 0 of +0x91.
typedef unsigned short wchar_t;                     // VC7.1 default: wchar_t is not a native type
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                         // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CWideString { public:
    void* PString;
    bool operator!=(const wchar_t* text) const;    // 0x0099B150
    ~CWideString();                                // 0x0099B510
};
class CReadableDef { public:
    unsigned char _pad00[0x25];
    bool UseInfoDisplay;                           // +0x25
};
class CCreatureActionBase {                        // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();                // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_Read : public CCreatureActionBase {
public:
    CWideString TextTag;                           // +0xa8
    CCreatureAction_Read(CThingCreatureBase& creature, const CWideString& textTag);   // 0x007E7FD0
};

inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};
class CTCActionUseBase : public CTCBase {
public:
    const void* PActionUseDef;                     // +0x0c CDefPointer<CActionUseDef const>
    bool Usable;                                   // +0x10
    virtual void OnActionUse(CThing& user);        // 0x007EAB90
};
class CTCActionUseReadable : public CTCActionUseBase {
public:
    const CReadableDef* PDef;                      // +0x14 CDefPointer<CReadableDef const>
    CWideString TextTag;                           // +0x18
    virtual void OnActionUse(CThing& user);
};

void CTCActionUseReadable::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    if (!PDef->UseInfoDisplay && TextTag != L"") {
        CThingCreatureBase* creature = CastToCreature(&user);
        if (creature && !creature->IsDead())
            creature->SetCurrentAction(CCreatureAction_Read(*creature, TextTag));
    }
}