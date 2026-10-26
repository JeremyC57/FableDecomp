#pragma optimize("s",on)
// CTCVillage::SetCropTaskAsAcceptedByThing(CVillageTaskPickCrops*, CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658561, oracle length 37 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; mov eax,[esi] ; mov ecx,esi ; call [eax+10h] ; test al,al ; je F ; push [esp+0Ch] ; mov eax,[esi] ; mov ecx,esi ; call [eax+20h] ; mov al,1 ; jmp R ; F: xor al,al ; R: pop esi ; ret 8
// `this` is unused. A crop task can be handed to a villager only if the task's virtual slot 4
// says it is still available; slot 8 then records the acceptor. Returns whether it was accepted.
//
// Lane feedback: byte-exact only under #pragma optimize("s",on) (shared epilogue, pass-through push);
// the lander sweep finds it. MATCH["s"].
class CThing;

// Vtable-shaped view of the crop task; only slots 4 and 8 are called here.
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
    // Retail passes `thing` straight through (`push dword ptr [esp+0Ch]`) and shares one
    // epilogue between the true/false arms: the size-optimised layout, selected by the
    // lander's #pragma optimize("s",on) sweep. Under plain /O2 the epilogue is duplicated.
    if (task->IsAvailable()) {
        task->SetAcceptedBy(thing);
        return true;
    }
    return false;
}