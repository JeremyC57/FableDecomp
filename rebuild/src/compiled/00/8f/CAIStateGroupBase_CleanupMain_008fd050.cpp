// CAIStateGroupBase::CleanupMain() -- OnReplace, Cleanup, then tells the creature's interface-7 component the state group is gone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD050, length 77 bytes.
// Callees: slot 10 OnReplace (+0x28) and slot 9 Cleanup (+0x24) on `this` (rtti-vtables.tsv),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (landed callers 0x007BEA08 ..),
// and the component's method @ 0x006CB6B0 (thiscall, no arguments: a forwarder to its +0xc
// object's slot 18; the manifest's "CScriptThing::operator==" label is propagated and rejected).
// engine/CAIStateGroupBase.h: PBrain +0x04; engine/CAIBrain.h: Creature +0x18. The creature's
// component presence bitset lives at +0x20 (type 7 -> word 0, bit 7: retail tests the sign of
// the byte) and its sorted interface map at +0x44 -- the inline GetInterface shape of the landed
// 0x007BEA08 family (lower bound, end or larger key -> the end entry). The type key lives in the
// `push ecx` frame slot; the map pointer is a named local (esi) so the end is re-read through it.
struct CTCBase;

struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CTCBase {
    void OnStateGroupRemoved();             // 0x006CB6B0
};

struct CThingAICreature {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }   // the raw masked word: a bool result adds a shift
};

struct CAIBrain {
    void*             __vftable;            // +0x00
    unsigned char     _pad[0x14];           // +0x04..+0x18
    CThingAICreature* Creature;             // +0x18
};

struct CAIStateGroupBase {                  // vptr +0x00
    CAIBrain* PBrain;                       // +0x04
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual void Cleanup();                 // slot 9 (+0x24)
    virtual void OnReplace();               // slot 10 (+0x28)
    void CleanupMain();
};

void CAIStateGroupBase::CleanupMain()
{
    OnReplace();
    Cleanup();
    CThingAICreature* creature = PBrain->Creature;
    if (creature->HasInterface(7)) {
        long type = 7;
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 7 < it->type)
            it = map->last;
        it->component->OnStateGroupRemoved();
    }
}