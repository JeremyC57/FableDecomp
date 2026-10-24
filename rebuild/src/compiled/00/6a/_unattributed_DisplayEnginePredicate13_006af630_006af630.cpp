// _unattributed::DisplayEnginePredicate13_006af630() -- tail-calls virtual slot 13 of the display engine returned by CViewBase::GetDisplayEngine (a bool predicate); owner unknown
// Leaf: ?DisplayEnginePredicate13_006af630@_unattributed@@
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

struct CDisplayEngine {
    // Only the vtable layout matters for byte parity: 13 unused virtual slots
    // (0x00..0x30) ahead of the bool predicate at slot 0x34.
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30();
    virtual bool v34(); // slot 0x34: the predicate this thunk tail-calls
};

struct CViewBase {
    CDisplayEngine *GetDisplayEngine();
};

struct _unattributed : CViewBase {
    bool DisplayEnginePredicate13_006af630();
};

bool _unattributed::DisplayEnginePredicate13_006af630()
{
    return GetDisplayEngine()->v34();
}
