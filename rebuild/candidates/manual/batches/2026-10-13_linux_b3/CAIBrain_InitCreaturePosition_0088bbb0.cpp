// CAIBrain::InitCreaturePosition()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088BBB0, oracle length 42 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; call <prepare> ; mov esi,[edi+28h] ; cmp esi,[edi+2Ch] ; je R ; L: mov ecx,[esi+4] ; mov eax,[ecx] ; call [eax+0Ch] ; test al,al ; jne R ; mov eax,[edi+2Ch] ; add esi,8 ; cmp esi,eax ; jne L ; R: pop edi ; pop esi ; ret
// engine/CAIBrain.h (PDB): +0x28 CVectorMap<long, CAIStateGroupBase*> StateGroupList. After the
// non-virtual preparation step at 0x0088BB60 each state group gets a chance to place the creature
// through its virtual slot 3; the first group that reports success ends the walk.
#include "engine/CAIBrain.h"

struct CAIStateGroupVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08();
    virtual bool InitCreaturePosition();        // slot 3 (+0x0C)
};

struct CStateGroupEntry {
    long                   Key;      // +0x00
    CAIStateGroupVirtuals* Group;    // +0x04
};

struct CStateGroupVectorMap {
    CStateGroupEntry* Begin;         // +0x00
    CStateGroupEntry* End;           // +0x04
};

struct CAIBrain_Methods : CAIBrain {
    void PrepareCreaturePosition();  // 0x0088BB60
    void InitCreaturePosition();
};

void CAIBrain_Methods::InitCreaturePosition()
{
    PrepareCreaturePosition();
    CStateGroupVectorMap* groups = reinterpret_cast<CStateGroupVectorMap*>(&this->StateGroupList[0]);
    for (CStateGroupEntry* it = groups->Begin; it != groups->End; ++it) {
        if (it->Group->InitCreaturePosition())
            return;
    }
}
