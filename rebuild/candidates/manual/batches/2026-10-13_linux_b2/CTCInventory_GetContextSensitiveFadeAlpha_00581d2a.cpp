// CTCInventory::GetContextSensitiveFadeAlpha(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581D2A, oracle length 36 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+160h] ; imul eax,eax,1Ch ; add ecx,eax ; cmp byte ptr [ecx],0 ; jne C ; fld [__real@1.0] ; jmp R ; C: call CFadeInFadeOutBase::GetAlpha ; R: ret 4
// The 0x1C-byte elements are CFadeInFadeOutBase (engine header: +0x00 Active). Retail keeps the
// ContextSensitiveFadeTimes vector's begin pointer at +0x160 (PDB +0x188: the shipped CTCInventory
// is 0x28 bytes smaller before it). An inactive fader reads as fully opaque (1.0f, .rdata
// 0x0122DED8); otherwise its current alpha comes from the fader (0x004348D0, __thiscall).

struct CFadeInFadeOutBaseCalls {
    bool          Active;                // +0x00
    unsigned char _rest[0x1B];           // 0x1C-byte element
    float GetAlpha();                    // 0x004348D0
};

struct CTCInventory_Retail {
    unsigned char            _pad_0x000[0x160];
    CFadeInFadeOutBaseCalls* ContextSensitiveFadeTimes_First;   // +0x160 vector begin (PDB +0x188)
    float GetContextSensitiveFadeAlpha(long index);
};

float CTCInventory_Retail::GetContextSensitiveFadeAlpha(long index)
{
    CFadeInFadeOutBaseCalls* fader = &this->ContextSensitiveFadeTimes_First[index];
    if (!fader->Active)
        return 1.0f;
    return fader->GetAlpha();
}
