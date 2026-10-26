// NCreatureMode::CModeSitFloor::CModeSitFloor(CThing*, const CCharString&) -- CModeSitBase(thing), own vtable, the CAnimFilterOfGroup member built from the group name
// Leaf: ??0CModeSitFloor@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC3C0, length 36 bytes (+ int3 padding).
// Callees: NCreatureMode::CModeSitBase::CModeSitBase(CThing*) @ 0x008D7CA0, CAnimFilterOfGroup::
// CAnimFilterOfGroup(const CCharString&) @ 0x0073E8F0 (out of line here; the manifest calls it
// "CConsole::SendMessageToUser", a propagated name). Vtable 0x01267BBC = NCreatureMode::CModeSitFloor
// (rtti-vtables.tsv); the manifest's "CConsoleCommandBase" label is propagated.
// Layout = Ego_r.pdb: CModeSitBase is 0x2c bytes (IdleCounter +0x1c, PlayingIdle +0x20, PRemoveAction
// +0x24), CModeSitFloor adds the Filter member at +0x2c (size 0x38). Two stack arguments (`ret 8`):
// the owning creature for the base and the animation-group name for the filter.
class CThing;

class CCharString { public: void* PStringData; };

class CAnimFilterOfGroup { public:
    virtual void v00();                                         // vtable 0x01267568
    CCharString   Group;                                        // +4
    unsigned long GroupCRC;                                     // +8
    CAnimFilterOfGroup(const CCharString& name);                // 0x0073E8F0
};

struct CModeSitBase {
    virtual void v00();
    unsigned char _body[0x28];                                  // +0x04..+0x2b
    CModeSitBase(CThing* thing);                                // 0x008D7CA0
};

struct CModeSitFloor : CModeSitBase {
    CAnimFilterOfGroup Filter;                                  // +0x2c
    virtual void v00();                                         // own vtable 0x01267BBC
    CModeSitFloor(CThing* thing, const CCharString& group);
};

CModeSitFloor::CModeSitFloor(CThing* thing, const CCharString& group)
    : CModeSitBase(thing)
    , Filter(group)
{
}