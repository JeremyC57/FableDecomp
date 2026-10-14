// SExpSpendUndo_00c3d7d0::SExpSpendUndo() -- EH-framed default constructor: vtable 0x012B5870 over a polymorphic base (dtor @ 0x00C3C830, state 0), four 0x14-byte list members at +0x1c / +0x30 / +0x44 / +0x58 (ctor @ 0x00C3C8A0, states 1-4), the three indices at +0x6c.. set to -1, the counters at +4..+0xc, +0x7c..+0x84 and +0xa4 zeroed, the five scales at +0x8c.. and +0x9c.. set to 1.0f, +0x78 to 1, +0x88 to 5000 and +0x98 to 1/330 (all scalar stores in the body, after the list constructors)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D7D0, oracle length 212 bytes. Built under /EHsc.
// Retail bytes:  ... mov esi,ecx ; push edi ; mov [esp+8],esi ; xor edi,edi ; lea ecx,[esi+1Ch] ; mov [esp+14h],edi ; mov dword ptr [esi],12B5870h ; call CList::CList ; lea ecx,[esi+30h] ; mov byte ptr [esp+14h],1 ; call CList::CList ; lea ecx,[esi+44h] ; mov byte ptr [esp+14h],2 ; call CList::CList ; lea ecx,[esi+58h] ; mov byte ptr [esp+14h],3 ; call CList::CList ; mov ecx,[esp+0Ch] ; or eax,-1 ; mov [esi+6Ch],eax ; mov [esi+70h],eax ; mov [esi+74h],eax ; mov eax,3F800000h ; mov [esi+4],edi ; ... mov dword ptr [esi+78h],1 ; mov dword ptr [esi+88h],1388h ; mov dword ptr [esi+98h],3B46980Ch ; pop edi ; mov eax,esi ; ... ret
// The manifest label names the PDB class; the list member is named by address.
struct CList_00c3d7d0 {                    // 0x14-byte list member
    unsigned char _body[0x14];
    CList_00c3d7d0();                      // 0x00C3C8A0
    ~CList_00c3d7d0();                     // 0x00C3C8C0
};

struct CUndoBase_00c3d7d0 {                // polymorphic base: its vtable store is dead (overwritten before any call) and dropped
    virtual void v00();
    CUndoBase_00c3d7d0() { }
    ~CUndoBase_00c3d7d0();                 // 0x00C3C830 (state 0 unwind)
};

struct SExpSpendUndo_00c3d7d0 : CUndoBase_00c3d7d0 {
    long m_4;
    long m_8;
    long m_c;
    unsigned char _pad[0xc];               // +0x10
    CList_00c3d7d0 m_1c;
    CList_00c3d7d0 m_30;
    CList_00c3d7d0 m_44;
    CList_00c3d7d0 m_58;
    long m_6c;
    long m_70;
    long m_74;
    long m_78;
    long m_7c;
    long m_80;
    long m_84;
    long m_88;
    float m_8c;
    float m_90;
    float m_94;
    float m_98;
    float m_9c;
    float m_a0;
    long m_a4;
    SExpSpendUndo_00c3d7d0();
};

SExpSpendUndo_00c3d7d0::SExpSpendUndo_00c3d7d0()
    : m_1c(), m_30(), m_44(), m_58()
{
    m_6c = -1;
    m_70 = -1;
    m_74 = -1;
    m_4 = 0;
    m_8 = 0;
    m_c = 0;
    m_a4 = 0;
    m_80 = 0;
    m_84 = 0;
    m_7c = 0;
    m_8c = 1.0f;
    m_90 = 1.0f;
    m_94 = 1.0f;
    m_9c = 1.0f;
    m_a0 = 1.0f;
    m_78 = 1;
    m_88 = 5000;
    m_98 = 1.0f / 330.0f;
}
