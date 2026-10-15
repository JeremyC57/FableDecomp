// std::_Dest_val<std::allocator<CEditLevelMerger::CConflict>, CEditLevelMerger::CConflict>(CConflict*) -- destroy one CConflict in place
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403510, 61 bytes. Callees: member destructor A 0x00598000 (x4), member destructor B 0x0059B5D0 (x2).
//
// STLport's allocator "destroy" hook as instantiated for a vector<CEditLevelMerger::CConflict>: it
// simply runs the CConflict destructor on the element. CConflict is the level-merge tool's record of one
// clash between two edits of a map; it owns six sub-objects of two kinds, which the synthesised
// destructor tears down in reverse declaration order (+0xBC, +0xB8, +0x94, +0x30, +0x2C, then +0x0C).
// Only the level editor's merge path creates these, so in the shipped game the element count is
// normally zero and this is never reached.
//
// Matching: a straight run of __fastcall member-destructor calls; the last one becomes a
// `pop esi; jmp dtorA` tail. Mirrors _Dest_val<allocator<CMacro>,CMacro> at 0x004047A0.

struct MemberA { char pad[4]; };   // sub-object type destroyed by dtorA (four of them)
struct MemberB { char pad[4]; };   // sub-object type destroyed by dtorB (two of them)

extern "C" void __fastcall dtorA(MemberA* p);   // destructor of the MemberA sub-objects (0x00598000)
extern "C" void __fastcall dtorB(MemberB* p);   // destructor of the MemberB sub-objects (0x0059B5D0)

// CEditLevelMerger::CConflict, as far as the destructor touches it.
struct Host {
    char     pad0[0x0c];   // leading plain data
    MemberA  m_0c;         // first owned sub-object (destroyed last, as the tail call)
    char     pad1[0x2c - 0x10];
    MemberB  m_2c;         // owned sub-object
    MemberB  m_30;         // owned sub-object
    char     pad2[0x94 - 0x34];
    MemberA  m_94;         // owned sub-object
    char     pad3[0xb8 - 0x98];
    MemberA  m_b8;         // owned sub-object
    MemberA  m_bc;         // last declared, destroyed first
};

void __fastcall Dest_val(Host* self)
{
    // Reverse declaration order.
    dtorA(&self->m_bc);
    dtorA(&self->m_b8);
    dtorA(&self->m_94);
    dtorB(&self->m_30);
    dtorB(&self->m_2c);
    dtorA(&self->m_0c);
}
