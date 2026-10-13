// CTCVillage::SetCropTaskAsAcceptedByThing(CVillageTaskPickCrops*, CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658561, oracle length 37 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; mov eax,[esi] ; mov ecx,esi ; call [eax+10h] ; test al,al ; je F ; push [esp+0Ch] ; mov eax,[esi] ; mov ecx,esi ; call [eax+20h] ; mov al,1 ; jmp R ; F: xor al,al ; R: pop esi ; ret 8
// `this` is unused. A crop task can be handed to a villager only if the task's virtual slot 4
// says it is still available; slot 8 then records the acceptor. Returns whether it was accepted.

struct CThing;

struct CVillageTaskVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual bool IsAvailable();                   // slot 4 (+0x10)
    virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void SetAcceptedBy(CThing* thing);    // slot 8 (+0x20)
};

struct CTCVillage_Methods {
    bool SetCropTaskAsAcceptedByThing(CVillageTaskVirtuals* task, CThing* thing);
};

bool CTCVillage_Methods::SetCropTaskAsAcceptedByThing(CVillageTaskVirtuals* task, CThing* thing)
{
    if (!task->IsAvailable())
        return false;
    task->SetAcceptedBy(thing);
    return true;
}
