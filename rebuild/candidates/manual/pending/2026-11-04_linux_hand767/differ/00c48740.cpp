// CUnproven_00c48740::~CUnproven_00c48740() -- libpk: destructor of an owner whose member at +0x14 frees its buffer (0x00C167C0)
// Grade: DIFFER(name) -- name unproven: libpk has no PDB / RTTI; row binding: ??1SDecoderMessage@@QAE@XZ; the callee 0x00C167C0 is bound under eight different landed placeholder spellings (one retail address, one name not yet resolved)
// Leaf: ??1CUnproven_00c48740@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C48740, 8 bytes: `add ecx,14h ; jmp 0x00C167C0`. The target frees [this] with operator delete
//   (0x00BFE9BC) when set and zeroes it (landed as CActiveFile_OnReadFinished, a free __fastcall placeholder; reloc bindings also
//   call it ??1CBaseIntelligentPointer_00c48300..00c48560, ??1CMember_00c2d3b0, ?Release@CBuffer_00c161a0, ?Cons_val_worker@AllocSub).
//   The bytes reproduce under this placeholder; parked until 0x00C167C0 has one name.
// Next idea: settle 0x00C167C0 (an owning-buffer holder's destructor, `if (p) { delete p; p = 0; }`) in a naming wave, then land
//   this as the owner's destructor under the name its callers use.
class CBufferHolder_00c167c0 { public: ~CBufferHolder_00c167c0(); void* Buffer; };   // 0x00C167C0
class CUnproven_00c48740 {
public:
    ~CUnproven_00c48740();
private:
    unsigned char Unmodeled_0x00[0x14];
    CBufferHolder_00c167c0 Member;                        // +0x14
};
CUnproven_00c48740::~CUnproven_00c48740()
{
}
