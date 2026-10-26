// CThingManager::GetPhysicsDebugInfo(CPhysicsDebugInfo* info) -- walks every thing in the manager's thing list and lets each one's physics object (if any) add to the debug info
// Grade: MATCH (retry 10, RTM 3077) -- the thing in a named local and `thing->PPhysics` written twice (the null test and the call; VC7.1 CSEs the load): the vtable then loads into eax as retail; a named physics local keeps eax and puts the vtable in edx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E4F0, length 49 bytes.
// Callees: the physics object's virtual slot 89 (+0x164).
// Layout: the thing list is a VC6-era Dinkumware std::list<CThing*> whose head-node pointer sits
// at +0x18 (node: +0 next, +4 prev, +8 value); CThing's physics object pointer at +0x60.
// Shape: `end()` is re-read from the member after every virtual call, `begin()` once; the
// argument is loaded into ebx inside the non-empty path; the physics pointer is null-tested.
class CPhysicsDebugInfo;
struct CPhysicsObject {
    virtual void v000();
    void GetPhysicsDebugInfoSlot();
};
struct CThingPhysics {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void GetPhysicsDebugInfo(CPhysicsDebugInfo* info);     // slot 89 (+0x164)
};
class CThing { public:
    unsigned char  _pad00[0x60];
    CThingPhysics* PPhysics;                   // +0x60
};
struct SThingListNode {
    SThingListNode* PNext;                     // +0x00
    SThingListNode* PPrev;                     // +0x04
    CThing*         PThing;                    // +0x08
};
struct CThingList {                            // VC6 Dinkumware list: allocator byte + head node
    SThingListNode* PHead;
    SThingListNode* begin() const { return PHead->PNext; }
    SThingListNode* end() const { return PHead; }
};
class CThingManager { public:
    unsigned char _pad00[0x14];
    unsigned char _alloc14[4];                 // +0x14 (list allocator word)
    CThingList    Things;                      // +0x18
    void GetPhysicsDebugInfo(CPhysicsDebugInfo* info);
};

void CThingManager::GetPhysicsDebugInfo(CPhysicsDebugInfo* info)
{
    for (SThingListNode* it = Things.begin(); it != Things.end(); it = it->PNext) {
        CThing* thing = it->PThing;
        if (thing->PPhysics)
            thing->PPhysics->GetPhysicsDebugInfo(info);
    }
}