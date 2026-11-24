// CChestDef::CChestDef() -- base CSubDefClassBase(), this vtable, then the embedded CMember_0099e4b0 member at +0x30
// Name fix (NF387, T3 nf381 0x0044C0C0): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CChestDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; class key V in both name sets); was the made-up CBase_0044c0c0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D3826, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CSubDefClassBase::CSubDefClassBase ; lea ecx,[esi+0x30] ; mov dword ptr [esi],offset CChestDef::`vftable' ; call CMember_0099e4b0::CMember_0099e4b0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CSubDefClassBase() @ 0x0044C0C0, this class's vtable (0x0123ACDC, named
// through rtti-vtables.tsv), then the member object at +0x30 is constructed out of line
// @ 0x0099E4B0. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
class CSubDefClassBase { public:
    virtual void v00();
    CSubDefClassBase();                           // 0x0044C0C0
};

// Name fix (NF199, 2026-11-20, q4456): the CMember_0099e4b0 stand-in for the CCharString member is replaced by the shared fable_string.h
//   CCharString, so the constructor call binds ??0CCharString@@QAE@XZ (Ego_r.pdb; 0x0099E4B0).
#include "fable_string.h"

class CChestDef : public CSubDefClassBase { public:
    unsigned char _pad[0x2c];
    CCharString m_member;                            // +0x30
    virtual void v00();
    CChestDef();
};

CChestDef::CChestDef()
    : CSubDefClassBase()
    , m_member()
{
}