#pragma optimize("s",on)
struct CSimVoice { int f0; };
extern "C" void __fastcall CSimVoice_dtor(CSimVoice* voice);
extern "C" void __cdecl op_delete(void* p);

void* __fastcall CSimVoice_scalar_deleting_destructor(CSimVoice* voice, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CSimVoice_dtor(voice);
    if (flags & 1) {
        op_delete(voice);
    }
    return voice;
}