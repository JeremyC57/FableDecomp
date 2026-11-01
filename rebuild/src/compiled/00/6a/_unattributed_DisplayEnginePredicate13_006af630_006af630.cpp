// _unattributed::DisplayEnginePredicate13_006af630() -- tail-calls virtual slot 13 of the world returned by CTCBase::PeekWorld (a bool predicate); owner unknown
// Leaf: ?DisplayEnginePredicate13_006af630@_unattributed@@
// Naming wave 25 (DECISIONS_W24_RESOLVED item 7): 0x00686A80 is ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (Ego_r / FableWin, 16 bytes after
//   GetWorld; ~100 landed rows bind it), so the object whose slot 13 (+0x34) this tail-calls is the const CWorld; the owner class stays
//   descriptive (_unattributed), now deriving from the empty-base CTCBase the landed PeekWorld rows model. Was CViewBase::GetDisplayEngine.
// Renamed (fistp pass, 2026-10-24) from CThingTrackNode::DrawIsDrawable @ 00/6a/CThingTrackNode_CThingTrackNodeDrawIsDrawableb_006af630.cpp: not in any vtable; its three callers (0x00880205 in NUISystem::CSpellContainerList::AddChild, 0x0088022C/0x0088024E in CTCPhysicsBase::HasCollidedWithLandscapeAt) carry propagated labels themselves; descriptive name, module _unattributed
// (was: _unattributed::DisplayEnginePredicate13_006af630 @ 0x006af630)
// Retail (12 bytes):
//   e8 4b 74 fd ff   call GetDisplayEngine        ; CDisplayEngine* = this->GetDisplayEngine()
//   8b 10            mov edx, [eax]               ; edx = vtable of returned CDisplayEngine
//   8b c8            mov ecx, eax                 ; ecx = this for the virtual call
//   ff 62 34         jmp  [edx+0x34]              ; tail-call virtual slot 0x34 (bool __fastcall(CDisplayEngine*))
//
// This is a one-line forwarding thunk: it asks the CViewBase-derived _unattributed
// for its CDisplayEngine, then tail-calls a virtual predicate on that engine and
// returns its result directly (no local ret -- the jmp reuses the caller's return
// address, which is exactly what "return engine->Virtual34();" compiles to when the
// call is the last statement in the function).
//
// Build: cl /c /O2 /Oy /W3

class CWorld { public:
    // Only the vtable layout matters for byte parity: 13 unused virtual slots
    // (0x00..0x30) ahead of the bool predicate at slot 0x34.
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30();
    virtual bool v34() const; // slot 0x34: the predicate this thunk tail-calls
};

class CTCBase {
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

struct _unattributed : CTCBase {
    bool DisplayEnginePredicate13_006af630();
};

bool _unattributed::DisplayEnginePredicate13_006af630()
{
    return PeekWorld().v34();
}
