// CThingNoise::vfunc_8() -- normalises OnCreate()'s byte result to a bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00838550, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  call OnCreate ; test al,al ; setne al ; ret
// Virtual predicate that forwards to the __fastcall helper @ 0x004C9560 on the same object
// (ecx passes through untouched) and returns its byte result as a normalised bool. Callee name
// kept from the manifest only when the label is unique. One of 6 bodies of this shape.
struct CThingNoise {
    virtual void v00();
    virtual bool vfunc_8();
    unsigned char OnCreate();                // 0x004C9560
};

bool CThingNoise::vfunc_8()
{
    return OnCreate() ? true : false;
}