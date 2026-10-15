// CCreatureAction_DragonBurninateInto::`scalar deleting destructor'(unsigned int) -- implicit destructor inlined: the MovementPoints vector's storage freed, then the action base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCCreatureAction_DragonBurninateInto
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953A40, length 51 bytes.
// Callees: free @ 0x00BFEA14 (the vector storage), the action base destructor @ 0x0062E900 (manifest
//          label "~CTCLightningOrb" is propagated), operator delete @ 0x00BFE9BC.
// Vtable 0x01294374 = CCreatureAction_DragonBurninateInto (rtti-vtables.tsv; this body is its slot 0).
// Layout: PDB CCreatureAction_DragonBurninateInto (0x168) = a 300-byte action base, PreviousPoint,
// MovementPoints (vector<C3DVector>), BurnStart/BurnStop, FlameStartPos/FlameEndPos. Retail is 0x18
// narrower below the vector: PreviousPoint +0x114, MovementPoints (12 bytes) +0x120, BurnStart +0x12c,
// BurnStop +0x130, FlameStartPos +0x134, FlameEndPos +0x140. The base's real name is not in the PDB
// match set (the 300-byte dragon-action base), so it is named descriptively.
// Shape: NO vtable store -- VC7.1 does not reset the vfptr in a compiler-generated (implicit)
// destructor, so the class declares no destructor (a user-declared `~T() {}` stores the vtable,
// see 0x00547AB0); the vector member's inline destructor frees its block under a null test, the
// base destructor (itself vfptr-free) runs out of line. The `new` factory pulls the vtable and the
// deleting destructor into the TU.

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

struct CDragonActionBase {                                                // 300-byte (PDB) creature-action base, 0x114 retail
    virtual ~CDragonActionBase();                                         // 0x0062E900
    unsigned char _body[0x110];                                           // +0x04 .. +0x114
};

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct C3DVectorVector {                                                  // retail vector<C3DVector> (12 bytes)
    C3DVector* m_first;
    C3DVector* m_last;
    C3DVector* m_end;
    C3DVectorVector() : m_first(0), m_last(0), m_end(0) {}
    ~C3DVectorVector()
    {
        if (m_first)
            free(m_first);
    }
};

struct CCreatureAction_DragonBurninateInto : CDragonActionBase {          // vtable 0x01294374
    C3DVector       PreviousPoint;                                        // +0x114
    C3DVectorVector MovementPoints;                                       // +0x120
    float           BurnStart;                                            // +0x12c
    float           BurnStop;                                             // +0x130
    C3DVector       FlameStartPos;                                        // +0x134
    C3DVector       FlameEndPos;                                          // +0x140
    CCreatureAction_DragonBurninateInto() : BurnStart(0.0f), BurnStop(0.0f) {}
};

CCreatureAction_DragonBurninateInto* NewDragonBurninateInto()
{
    return new CCreatureAction_DragonBurninateInto();
}