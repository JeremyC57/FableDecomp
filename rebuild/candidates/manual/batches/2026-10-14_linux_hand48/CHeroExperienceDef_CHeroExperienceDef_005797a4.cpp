// CHeroExperienceDef::CHeroExperienceDef() -- construct the four 4-byte lists at +0..+0xc (std::list @ 0x0042AB58, one 1-byte allocator temporary shared), then clear the word at +0x10 and the two 16-byte records at +0x14 and +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005797A4, length 86 bytes.
// Shape: the EBP frame with the 1-byte temporary at [ebp-1], the records cleared last-word-first, `mov eax,esi`. The 0x0057xxxx module is size-optimised with the frame pointer kept.
#pragma optimize("s", on)
#pragma optimize("y", off)
struct SAllocator_0042ab58 { SAllocator_0042ab58() { } };   // 1 byte: the list allocator (a user constructor: the temporary is not zero-filled)
struct CList_0042ab58 {                            // a 4-byte circular list (one sentinel node)
    void* m_pNode;
    CList_0042ab58(const SAllocator_0042ab58& alloc);   // 0x0042AB58
};
struct SRecord16_005797a4 {                        // four words, cleared
    long m_a, m_b, m_c, m_d;
    SRecord16_005797a4() { m_d = 0; m_c = 0; m_b = 0; m_a = 0; }
};
struct CHeroExperienceDef {
    CList_0042ab58     m_list0;                    // +0x00
    CList_0042ab58     m_list1;                    // +0x04
    CList_0042ab58     m_list2;                    // +0x08
    CList_0042ab58     m_list3;                    // +0x0c
    long               m_10;                       // +0x10
    SRecord16_005797a4 m_record14;                 // +0x14
    SRecord16_005797a4 m_record24;                 // +0x24
    CHeroExperienceDef();
};

CHeroExperienceDef::CHeroExperienceDef()
    : m_list0(SAllocator_0042ab58()), m_list1(SAllocator_0042ab58()), m_list2(SAllocator_0042ab58()), m_list3(SAllocator_0042ab58()), m_10(0)
{
}
