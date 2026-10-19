// Grade: HELD(bytes match retail) -- test hand-transcribed with renamed types; #pragma pack(1) only to reach offsets | Codex source 00/a3/CVideoSys_Close_00a3b380.cpp, flags , pass pattern 00a3b380_TEST PASS
// CVideoSys::Close() -- release DirectShow interfaces and close the render-event handles
#pragma pack(push,1)
struct CVideoSys { void* GBuilder; void* MControl; void* MPosition; void* MSeeking; void* MEvent; void* BasicAudio; void* PAsfReader; void* PFileSource; void* TexRenderer; unsigned char pad[0x44]; void* pTextBank; void* CriticalSection_DebugInfo; long CriticalSection_LockCount; long CriticalSection_RecursionCount; void* CriticalSection_OwningThread; void* CriticalSection_LockSemaphore; unsigned long CriticalSection_SpinCount; void* RenderEvent; void* PRenderTexture; long Height; long Width; };
#pragma pack(pop)
struct IVideoRef { virtual void A(); virtual void B(); virtual void __stdcall Release(); };
typedef int (__stdcall *CloseFn)(void*);
typedef void (__stdcall *DeleteFn)(void*);
CloseFn* const g_Close = (CloseFn*)0x0143fe30;
DeleteFn* const g_Delete = (DeleteFn*)0x0143fde4;
void __fastcall Close(CVideoSys* self)
{
    IVideoRef* p;
    p=(IVideoRef*)self->MControl; if(p){p->Release(); self->MControl=0;}
    p=(IVideoRef*)self->MPosition; if(p){p->Release(); self->MPosition=0;}
    p=(IVideoRef*)self->MSeeking; if(p){p->Release(); self->MSeeking=0;}
    p=(IVideoRef*)self->BasicAudio; if(p){p->Release(); self->BasicAudio=0;}
    p=(IVideoRef*)self->PFileSource; if(p){p->Release(); self->PFileSource=0;}
    p=(IVideoRef*)self->PAsfReader; if(p){p->Release(); self->PAsfReader=0;}
    p=(IVideoRef*)self->MEvent; if(p){p->Release(); self->MEvent=0;}
    p=(IVideoRef*)self->GBuilder; if(p){p->Release(); self->GBuilder=0;}
    if(self->CriticalSection_LockSemaphore){ (*g_Close)(self->CriticalSection_LockSemaphore); self->CriticalSection_LockSemaphore=0; (*g_Delete)((char*)self+0x64); }
}