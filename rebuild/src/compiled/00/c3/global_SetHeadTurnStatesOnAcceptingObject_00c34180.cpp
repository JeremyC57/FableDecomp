// SetHeadTurnStatesOnAcceptingObject(CLookTarget*, long, long) -- asks the object (virtual slot 1) whether it accepts, then calls the two setters @ 0x00C41530 / 0x00C41550 on the object or on null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34180, length 46 bytes. The chunk label "CDisplayViewManager::SetView" is propagated: the body is a
// free three-argument `__stdcall` function (`ret 0Ch`, ecx never read) in the 0x00C3xxxx module (sibling of the landed 0x00C33F20).
// Callees: the object's virtual slot 1 (bool), the setters @ 0x00C41530 and @ 0x00C41550 (manifest "CTCLook::SetHeadTurnState(long)" for both).
// Shape: the masked pointer (`neg al ; sbb eax,eax ; and esi,eax`) is kept in esi for both calls; each argument is pushed just before its call.
struct CLookTarget {
    virtual void v00();
    virtual bool Accepts();                        // slot 1
    void SetHeadTurnState(long state);             // 0x00C41530
    void SetHeadTurnTarget(long target);           // 0x00C41550
};

void __stdcall SetHeadTurnStatesOnAcceptingObject(CLookTarget* object, long state, long target)
{
    CLookTarget* accepting = object->Accepts() ? object : 0;
    accepting->SetHeadTurnState(state);
    accepting->SetHeadTurnTarget(target);
}