#pragma optimize("s",on)
class CSimVoice { public: int f0; };
extern "C" void __fastcall CSimVoice_dtor(CSimVoice* voice);

void* __fastcall CSimVoice_scalar_deleting_destructor(CSimVoice* voice, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CSimVoice_dtor(voice);
    if (flags & 1) {
        operator delete(voice);
    }
    return voice;
}