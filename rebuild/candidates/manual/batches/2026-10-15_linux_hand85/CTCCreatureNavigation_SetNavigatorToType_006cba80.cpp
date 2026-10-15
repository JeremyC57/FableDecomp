// CTCCreatureNavigation::SetNavigatorToType(long type) -- switch to the possible navigator of the given type (interrupting the old one)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CBA80, length 63 bytes (+1 int3).
// Callee: CNavigatorBase::Interrupt (virtual slot 18, `call [edx+48h]`).
// Shape: the navigator type is the byte at CNavigatorBase +0x10 (`movzx`), compared with the enum argument held in edi.
// The search over PossibleNavigators is an inline find returning `last` when nothing matches (`mov eax,ecx`), and the
// result is dereferenced unconditionally (as retail does).
// Model: CTCCreatureNavigation PDB names (PNavigator +0x0c, PossibleNavigators +0x10 as the retail 12-byte vector);
// navigator slots from the CNavigatorAStar/CNavigatorFlyer vtables (9 CanNavigateToPos, 10 IsNavLineOfSightClear,
// 18 Interrupt) and the external interface vtables (slot 4 FindPosZAt, float).

struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0 (PhysicsTC +0x60, position at +0x0c)
};

struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool flag) const;    // slot 4
};

struct CNavigatorBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float proximity, bool flag) const;   // slot 9
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

struct CTCCreatureNavigation {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CNavigatorBase* PNavigator;             // +0x0c
    CNavigatorVector PossibleNavigators;    // +0x10
    static CNavigatorBase** FindNavigator(CNavigatorBase** first, CNavigatorBase** last, long type)
    {
        for (; first != last; ++first)
            if ((*first)->NavigatorType == type)
                return first;
        return last;
    }
    void SetNavigatorToType(long type);
};

void CTCCreatureNavigation::SetNavigatorToType(long type)
{
    if (PNavigator->NavigatorType != type) {
        PNavigator->Interrupt();
        PNavigator = *FindNavigator(PossibleNavigators.first, PossibleNavigators.last, type);
    }
}
