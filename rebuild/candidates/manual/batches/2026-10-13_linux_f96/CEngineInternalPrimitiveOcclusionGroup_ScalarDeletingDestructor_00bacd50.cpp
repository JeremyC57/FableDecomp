// CEngineInternalPrimitiveOcclusionGroup::`scalar deleting destructor'(unsigned int) -- empty virtual destructor over CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase, this copy from one translation unit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACD50, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov dword ptr [esi],offset CEngineInternalPrimitiveOcclusionGroup::`vftable' ; call CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Compiler-generated deleting destructor for an empty inline `virtual ~CEngineInternalPrimitiveOcclusionGroup() {}`: installs this
// class's vtable (0x012A5364, rtti-vtables.tsv), runs the base destructor @ 0x00B9C660 (its
// manifest label is kept only when unique) and frees the object when bit 0 of the flag is set
// (operator delete @ 0x00BFE9BC, landed-trusted). Re-emitted per translation unit; this file is the
// copy at this address. One of 12 bodies of this shape.
struct CEngineInternalPrimitiveOcclusionBase {
    virtual void v00();
    ~CEngineInternalPrimitiveOcclusionBase();                          // 0x00B9C660
};

struct CEngineInternalPrimitiveOcclusionGroup : CEngineInternalPrimitiveOcclusionBase {
    virtual ~CEngineInternalPrimitiveOcclusionGroup() { }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CEngineInternalPrimitiveOcclusionGroup;
}
