// CCreatureInteraction::RemoveCreature(CThingCreatureBase*) -- drops a creature from the interaction's participant list
// Leaf: RemoveCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008631B0, length 21 bytes (the 32-byte catalog row ends in int3 padding).
// Callees: std::list<CThing*>::remove(CThing* const&) @ 0x00863130 (the list sits at +0 of `this`).
// Callers: CThingCreatureBase::RemoveFromCreatureInteraction @ 0x006629B0 (`mov ecx,[esi+12Ch] ; push esi ;
//          call`): the interaction object the creature is registered with removes the creature.
// Label: the manifest's "CTCInventoryAbilities::AddLearnedExpression" is propagated over the landed 0x006404B0
//        shape (that one adds 4 to ecx); the caller names the owner, so the class name is descriptive.
// Shape: the argument is converted to the list's value type (derived-to-base pointer, offset 0), so VC builds
//        the conversion temporary in the argument's own slot (`mov eax,[esp+4] ; ... ; mov [esp+8],eax`) and
//        passes its address.

class CThing { public:
    void* __vftable;
};

class CThingCreatureBase : public CThing { public:
};

struct CThingPointerList {                        // std::list<CThing*> (STLport, one node pointer)
    void* m_pNode;
    void remove(CThing* const& value);            // 0x00863130
};

class CCreatureInteraction { public:
    CThingPointerList Participants;               // +0x00
    void RemoveCreature(CThingCreatureBase* creature);
};

void CCreatureInteraction::RemoveCreature(CThingCreatureBase* creature)
{
    Participants.remove(creature);
}