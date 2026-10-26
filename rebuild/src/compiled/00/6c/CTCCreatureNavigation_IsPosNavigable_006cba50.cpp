// CTCCreatureNavigation::IsPosNavigable(const C3DVector& pos) -- can the current navigator route from the thing's position to pos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CBA50, length 38 bytes (+10 int3). Twin of the landed IsPosNavLineOfSightTo
// (0x006CBA20, slot 10).
// Callees: CThing::GetPosition @ 0x004C73D0, CNavigatorBase::CanNavigateToPos (virtual slot 9, `call [edi+24h]`) with
// proximity 0.0f (`push 0`) and `false`.
// Shape: the navigator's vtable is fetched before the position call (edi) and the object reloaded into ecx after it;
// the thing is reached through an inline GetParentThing() accessor -- with the plain member VC7.1 loads pos after the
// pushes, with the accessor it loads pos first (edx) and interleaves the pushes as retail.
// Model: CTCCreatureNavigation PDB names (PNavigator +0x0c, PossibleNavigators +0x10 as the retail 12-byte vector);
// navigator slots from the CNavigatorAStar/CNavigatorFlyer vtables (9 CanNavigateToPos, 10 IsNavLineOfSightClear,
// 18 Interrupt) and the external interface vtables (slot 4 FindPosZAt, float).

class C3DVector { public: float X, Y, Z; };

class CThing { public:
    const C3DVector& GetPosition() const;   // 0x004C73D0 (PhysicsTC +0x60, position at +0x0c)
};

class CNavigatorExternalInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool flag) const;    // slot 4
};

class CNavigatorBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float proximity = 0.0f, bool flag = false) const;   // slot 9
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;                          // slot 10
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17();
    virtual void Interrupt();               // slot 18
    unsigned char _pad04[0x0c];
    unsigned char NavigatorType;            // +0x10
    unsigned char _pad11[3];
    CNavigatorExternalInterface* PInterface;   // +0x14
};

struct CNavigatorVector {                   // retail 12-byte vector<CNavigatorBase*>
    CNavigatorBase** first;
    CNavigatorBase** last;
    CNavigatorBase** end;
};

class CTCCreatureNavigation { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CNavigatorBase* PNavigator;             // +0x0c
    CNavigatorVector PossibleNavigators;    // +0x10
    bool IsPosNavigable(const C3DVector& pos) const;
    CNavigatorBase* GetNavigator() const { return PNavigator; }
    CThing* GetParentThing() const { return ParentThing; }
};

bool CTCCreatureNavigation::IsPosNavigable(const C3DVector& pos) const
{
    return PNavigator->CanNavigateToPos(GetParentThing()->GetPosition(), pos);
}