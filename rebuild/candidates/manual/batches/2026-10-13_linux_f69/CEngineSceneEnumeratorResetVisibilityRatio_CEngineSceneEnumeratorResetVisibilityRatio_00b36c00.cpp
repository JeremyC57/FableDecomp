// CEngineSceneEnumeratorResetVisibilityRatio::CEngineSceneEnumeratorResetVisibilityRatio(void*) -- base CBase_0099a2f0(), the argument stored at +0x4, then this vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B36C00, oracle length 27 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0099a2f0::CBase_0099a2f0 ; mov eax,[esp+8] ; mov [esi+0x4],eax ; mov dword ptr [esi],offset CEngineSceneEnumeratorResetVisibilityRatio::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor: base CBase_0099a2f0() @ 0x0099A2F0, the argument kept at +0x4 and
// this class's vtable (0x012A12F4, named through rtti-vtables.tsv). The argument type is not
// recoverable from the body; `void*` models it. One of 15 bodies of this shape.
struct CBase_0099a2f0 {
    virtual void v00();
    CBase_0099a2f0();                           // 0x0099A2F0
};

struct CEngineSceneEnumeratorResetVisibilityRatio : CBase_0099a2f0 {
    void* m_argument;                      // +0x4
    virtual void v00();
    CEngineSceneEnumeratorResetVisibilityRatio(void* argument);
};

CEngineSceneEnumeratorResetVisibilityRatio::CEngineSceneEnumeratorResetVisibilityRatio(void* argument)
    : CBase_0099a2f0()
    , m_argument(argument)
{
}
