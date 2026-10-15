// CLockedAudioObject00cafd90::~CLockedAudioObject00cafd90() -- delete the object's critical section, then run the destructor of its second base (at +4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAFD90, length 34 bytes. Callees: KERNEL32 DeleteCriticalSection through its
// import slot 0x0143FDE4, the second base's destructor @ 0x00CAEFB0 (EH-framed: releases the COM-style interface at its +0x10
// through `__stdcall` slot 2, then its own bases; manifest "CVolume::ReleaseVolume") as a tail jump.
// Label correction: the manifest calls this CRenderManager2D::AddLineToDrawBuffer (propagated). It is the plain destructor
// called by the scalar deleting destructor at 0x00CAFD70 (whose `sub ecx,8` adjustor thunk 0x00CAFD60 heads the vtable at
// 0x012BF9A0 -- a no-RTTI module, the audio streaming code around 0x00CAxxxx), so the class is named by address.
// Layout: a primary polymorphic base at +0x00, the second base (0x18 bytes, vptr at +0x04) at +0x04, the CRITICAL_SECTION at
// +0x1c.
// Shape: the class is `__declspec(novtable)` -- its destructor stores no vtable -- and with novtable VC7.1 converts `this`
// to the +4 base with an explicit null test (`test esi,esi ; je ; lea ecx,[esi+4]`, the null arm `xor ecx,ecx`), each arm
// ending in the base-destructor jump; the single import call goes straight through the slot (`call [__imp_..]`).
// Leaf: ??1CLockedAudioObject00cafd90
struct CRITICAL_SECTION_00cafd90 {                      // RTL_CRITICAL_SECTION, 24 bytes
    unsigned char _body[0x18];
};
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION_00cafd90* section);   // 0x0143FDE4

struct CAudioRoot00cafd90 {                             // primary base (vptr at +0x00)
    virtual void v00();
};
struct CAudioObjectBase00caefb0 {                       // second base (vptr at +0x04 of the object)
    virtual ~CAudioObjectBase00caefb0();                // 0x00CAEFB0
    unsigned char _body[0x14];                          // +0x04 .. +0x17 (its own base at +4, an interface pointer at +0x10)
};

struct __declspec(novtable) CLockedAudioObject00cafd90 : CAudioRoot00cafd90, CAudioObjectBase00caefb0 {
    CRITICAL_SECTION_00cafd90 m_lock;                   // +0x1c
    virtual ~CLockedAudioObject00cafd90();
};

CLockedAudioObject00cafd90::~CLockedAudioObject00cafd90()
{
    DeleteCriticalSection(&m_lock);
}