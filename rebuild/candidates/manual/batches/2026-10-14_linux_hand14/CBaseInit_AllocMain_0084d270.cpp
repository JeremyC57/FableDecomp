// CBaseInit::AllocMain(CGameCameraManager*, CDefPointer<CCameraModeDef const>&) -- allocate the mode, seed three fields from the def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084D270, oracle length 47 bytes.
// Callees: virtual slot 0 of `this` (the allocator).
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx] ; push esi ; mov esi,[esp+0Ch] ; push esi ; push edx ; call [eax] ;
//                mov ecx,[esi] ; mov edx,[ecx+4Ch] ; mov [eax+0Ch],edx ; mov ecx,[esi] ; fld dword ptr [ecx+50h] ;
//                fmul dword ptr [__real@1/360] ; fstp dword ptr [eax+4] ; mov edx,[esi] ; mov ecx,[edx+54h] ; mov [eax+8],ecx ; pop esi ; ret 8
// The def pointer is a reference whose pointee is re-read before each field (the stores into the
// new mode may alias it); the float is scaled by the literal 1/360 (`fld member ; fmul literal`).
struct CGameCameraManager;

struct CCameraModeDef {
    unsigned char _pad[0x4c];
    unsigned long m_flags;          // +0x4c
    float         m_turnDegrees;    // +0x50
    unsigned long m_style;          // +0x54
};

struct CDefPointer_CCameraModeDef {
    const CCameraModeDef* m_pDef;   // +0x00
};

struct CBase {
    unsigned char _pad[4];
    float         m_turnFraction;   // +0x04
    unsigned long m_style;          // +0x08
    unsigned long m_flags;          // +0x0c
};

struct CBaseInit {
    virtual CBase* Alloc(CGameCameraManager* manager, CDefPointer_CCameraModeDef& def);   // slot 0
    CBase* AllocMain(CGameCameraManager* manager, CDefPointer_CCameraModeDef& def);       // 0x0084D270
};

CBase* CBaseInit::AllocMain(CGameCameraManager* manager, CDefPointer_CCameraModeDef& def)
{
    CBase* mode = Alloc(manager, def);
    mode->m_flags = def.m_pDef->m_flags;
    mode->m_turnFraction = def.m_pDef->m_turnDegrees * (1.0f / 360.0f);
    mode->m_style = def.m_pDef->m_style;
    return mode;
}
