// CThingFilter_IsNavLineOfSightToThing::operator()(CThing* thing) -- true when the creature-navigation component reports a navigation line of sight from Pos to the thing's position
// Leaf: ??RCThingFilter_IsNavLineOfSightToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7640, length 28 bytes.
// Callees: CThing::GetPos @ 0x004C73D0 (out of line), CTCCreatureNavigation line-of-sight query @ 0x006CB610
// (`__fastcall(const C3DVector* from, const C3DVector& to)`; the manifest's "CScriptThing::operator==" is propagated).
// Layout: PDB CThingFilter_IsNavLineOfSightToThing { C3DVector* Pos; CTCCreatureNavigation* PTCNav; }.
// Shape: right-to-left: the thing's position first, then PTCNav into ecx, the pushes, Pos last.
class C3DVector { public:
    float x, y, z;
};

class CThing { public:
    const C3DVector& GetPos() const;           // 0x004C73D0
};

class CTCCreatureNavigation { public:
    bool IsNavLineOfSight(const C3DVector* from, const C3DVector& to);   // 0x006CB610
};

class CThingFilter_IsNavLineOfSightToThing { public:
    C3DVector*             Pos;                     // +0x00
    CTCCreatureNavigation* PTCNav;                  // +0x04
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsNavLineOfSightToThing::operator()(CThing* thing) const
{
    return PTCNav->IsNavLineOfSight(Pos, thing->GetPos());
}