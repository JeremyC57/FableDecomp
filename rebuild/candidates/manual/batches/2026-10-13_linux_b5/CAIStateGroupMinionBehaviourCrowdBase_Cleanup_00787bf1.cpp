// CAIStateGroup_MinionBehaviourCrowdBase::Cleanup()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787BF1, oracle length 13 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  mov byte ptr [ecx+45h],0 ; mov byte ptr [ecx+44h],1 ; mov eax,[ecx] ; jmp [eax+7Ch]
// Clears the "in crowd" flag (+0x45), marks the group finished (+0x44) and tail-calls the base
// Cleanup through virtual slot 31 (+0x7C). The class is vtable-shaped here (no engine header).
struct CAIStateGroup_MinionBehaviourCrowdBase_Virtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78();
    virtual void BaseCleanup();             // slot 31 (+0x7C)
    unsigned char _pad_0x04[0x40];
    bool          Finished;                 // +0x44 (name from use)
    bool          InCrowd;                  // +0x45 (name from use)
    void Cleanup();
};

void CAIStateGroup_MinionBehaviourCrowdBase_Virtuals::Cleanup()
{
    this->InCrowd  = false;
    this->Finished = true;
    BaseCleanup();
}
