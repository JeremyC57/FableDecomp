#pragma optimize("s",on)
// CTCGift::CTCGift(CEngineInternalPrimitive2DClockSprite*) -- base CEngineSubPrimitive2DClockSprite(argument) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D416D, oracle length 24 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite ; mov dword ptr [esi],offset CTCGift::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00686800; the compiler then stores this class's vtable (0x0123B83C, named
// through rtti-vtables.tsv) and returns `this`. One of 68 bodies of this shape.
struct CEngineInternalPrimitive2DClockSprite;

struct CEngineSubPrimitive2DClockSprite {
    virtual void v00();
    CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* argument);            // 0x00686800
};

struct CTCGift : CEngineSubPrimitive2DClockSprite {
    virtual void v00();
    CTCGift(CEngineInternalPrimitive2DClockSprite* argument);
};

CTCGift::CTCGift(CEngineInternalPrimitive2DClockSprite* argument)
    : CEngineSubPrimitive2DClockSprite(argument)
{
}