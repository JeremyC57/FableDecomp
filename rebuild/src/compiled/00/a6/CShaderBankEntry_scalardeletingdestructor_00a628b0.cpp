// Leaf: ?CShaderBankEntry_scalar_deleting_destructor@@YIPAXPAVCShaderBankEntry@@HI@Z
// agentNF182 (2026-11-20), in-place respelling of this row's existing local model: the destructor it calls is ??1CShaderBankEntry@@UAE@XZ 0x00A628D0
//   (Ego_r.pdb + FableWin.pdb; its landed row defines that leaf), declared virtual as in retail and called by qualified name so the
//   call stays direct. The real ??_G is emitted only by a TU that compiles a CShaderBankEntry constructor or destructor (retail TU-mate: the
//   out-of-line constructor, a separate landed row), so the self leaf stays a stand-in (NF182 NOTES).
class CShaderBankEntry { public:
    virtual ~CShaderBankEntry();      // 0x00A628D0
};

void operator delete(void*);

void* __fastcall CShaderBankEntry_scalar_deleting_destructor(CShaderBankEntry* entry, int edx_unused, unsigned int flags)
{
    entry->CShaderBankEntry::~CShaderBankEntry();
    if (flags & 1)
        operator delete(entry);
    return entry;
}