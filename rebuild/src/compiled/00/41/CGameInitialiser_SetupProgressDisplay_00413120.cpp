// CGameInitialiser::SetupProgressDisplay() -- GFInitialise's loading-screen step: run the CInitBaseClass Init, reset two start-up fields, publish a fresh CProgressDisplay through a temporary counted pointer
// Leaf: ?SetupProgressDisplay@CGameInitialiser@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00413120, 128 bytes. Callees: CInitBaseClass::Init
// 0x0099A350 (`Valid = true; return true`; manifest label CXMVPlaybackInfo is propagated),
// operator new 0x00BFEA1A (0x88 bytes), CProgressDisplay::CProgressDisplay 0x00499CE0,
// CCountedPointer<CProgressDisplay>::CCountedPointer(T*) 0x00403D20, SetProgressDisplay 0x009E9FD0
// (__fastcall, the counted pointer by reference), operator delete 0x00BFE9BC.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; call Init ; test al,al ; je F ; push 0x88 ;
//   mov byte [esi+0x20c],0 ; mov dword [esi+8],0 ; call new ; add esp,4 ; test eax,eax ; je Z ;
//   mov ecx,eax ; call CProgressDisplay ; jmp J ; Z: xor eax,eax ; J: push eax ; lea ecx,[p] ;
//   call CCountedPointer ; lea ecx,[p] ; call SetProgressDisplay ; <inline ~CCountedPointer:
//   Info re-read from the frame: dec [Info] ; cmp [Info],0 ; jne ; mov ecx,[Info+8] ;
//   call [Info+4] ; push Info ; call delete> ; mov al,1 ; ... ret ; F: xor al,al ; ... ret
// `this` is the global start-up object at 0x013B83D0 (GFInitialise 0x004022B0 calls this with it;
// GFUninitialise calls CInitBaseClass::Uninitialise on the same object), a CInitBaseClass
// {vptr, Valid +4} subclass with a dword at +8 and a byte at +0x20C that this step clears. The
// manifest name GFInitialise_SetupProgressDisplay is descriptive; the class name here is too.
// PDB CCPPointerInfo: RefCount +0, DeleteFunc +4 (called with the object in ecx), Data +8. An
// earlier lane landed this body as Global_GFInitialiseSetupProgressDisplay_00413120.cpp
// (fable_boot.h); this TU is self-contained.
struct CProgressDisplay {
    unsigned char m_body[0x88];
    CProgressDisplay();                           // 0x00499CE0
};

struct CCPPointerInfo {
    unsigned long RefCount;                       // +0
    void (__fastcall* DeleteFunc)(void* data);    // +4
    void* Data;                                   // +8
};

void __cdecl operator delete(void* p);            // 0x00BFE9BC

struct CCountedProgressDisplayPointer {
    CProgressDisplay* Data;                       // +0
    CCPPointerInfo* Info;                         // +4
    CCountedProgressDisplayPointer(CProgressDisplay* object);   // 0x00403D20
    ~CCountedProgressDisplayPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
};

void __fastcall SetProgressDisplay(const CCountedProgressDisplayPointer& display);   // 0x009E9FD0

struct CInitBaseClass {
    virtual void v00();
    bool Valid;                                   // +4
    bool Init();                                  // 0x0099A350
};

struct CGameInitialiser : CInitBaseClass {
    unsigned long m_startupValue;                 // +8
    unsigned char _pad0c[0x200];
    unsigned char m_startupFlag;                  // +0x20C
    bool SetupProgressDisplay();
};

bool CGameInitialiser::SetupProgressDisplay()
{
    if (Init())
    {
        m_startupFlag = 0;
        m_startupValue = 0;
        CCountedProgressDisplayPointer display(new CProgressDisplay);
        SetProgressDisplay(display);
        return true;
    }
    return false;
}