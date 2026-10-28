// CAIStateGroup_MinionCuriosity::FindNewCuriosityPos() -- take the position of the thing the creature is
// currently curious about (if any, and if it is not dead) as the new curiosity position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933AA0, length 61 bytes (+ int3 padding).
// Callees: the creature's curiosity-target query @ 0x0082FEC0 (no arguments, returns a CThing* -- the manifest
// "CEngineInternalPrimitiveBase::AddChildPrimitive" is a propagated label on a `xor eax,eax ; ret` stub body),
// CThing::GetPosition @ 0x006A4D00 (const C3DVector&; manifest "CVertexBufferWin32::DoSizeof" is propagated).
// Shape: `t = creature->GetCuriosityThing(); if (t && !(t->flags91 & 1)) { CuriosityPos = t->GetPosition();
// return true; } return false;` -- the dead flag is bit 0 of the byte at +0x91 tested as a branch; the
// C3DVector assignment at +0x14 is the IMPLICIT operator= of a class with a user copy constructor (three
// dword moves through `add esi,14h`); bool result as `mov al,1` / shared `xor al,al`. No engine header for
// this class; base layout local (PDB names), CuriosityPos descriptive.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;         // 0x006A4D00
};
class CThing : public CThingPhysical { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                // +0x91 bit 0: dead
};

class CThingCreatureBase { public:
    CThing* GetCuriosityThing();            // 0x0082FEC0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    C3DVector CuriosityPos;                 // +0x14
    bool FindNewCuriosityPos();
};

bool CAIStateGroup_MinionCuriosity::FindNewCuriosityPos()
{
    CThing* thing = PBrain->Creature->GetCuriosityThing();
    if (thing && !(thing->m_flags91 & 1)) {
        CuriosityPos = thing->PeekPos();
        return true;
    }
    return false;
}