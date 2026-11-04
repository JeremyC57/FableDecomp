// NScript::CV_MurderTwistFinalScript::Init() -- clear the sword/crowd state and give the two per-slot vectors two empty entries each (CScriptThing at +0x60, scripted-thing handles at +0x7c)
// Leaf: ?Init@CV_MurderTwistFinalScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB6A00, length 83 bytes.
// Callees: CScriptThing::CScriptThing() @ 0x006E7B40 and ~CScriptThing @ 0x004AA840, the thing vector's
// resize(unsigned, const CScriptThing&) @ 0x00D34AC0; CScriptGameResourceObjectScriptedThingBase::
// CScriptGameResourceObjectScriptedThingBase() @ 0x007E72A0 and its destructor @ 0x007E74D0, the handle
// vector's resize @ 0x00CD3CCB (the manifest labels on all four resize/ctor rows are propagated template names).
// Respell (chunk 639, same bytes): the vectors are the STLport std::vector<CScriptThing> / std::vector<CScriptGameResourceObjectScriptedThingBase>
//   specialisations (resize 0x00D34AC0 / 0x00CD3CCB, spelled as in CQ_FireHeartScript::DisableFollowers 0x00D3A050) and CScriptThing is the
//   global polymorphic class (engine/CScriptThing.h: ctor 0x006E7B40, virtual ~CScriptThing 0x004AA840); the former
//   CScriptThingVector / CScriptedThingVector / N_006e7b40::CScriptThing stand-in spellings are gone.
// Shape: a 16-byte frame shared by the two full-expression temporaries (12 and 16 bytes); each explicit
// temporary `T()` reuses the constructor's returned pointer for the push (`push eax`), is destroyed right after
// its resize, and the four scalar clears (two words, two bytes from the zero register) come first.
// Naming wave 35: identity outer script vtable 012ef978 slot 3 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00eaea20 (slot 2) registers 4 entity scripts whose PDB Alloc outer is CV_MurderTwistFinalScript; PDB ?Init@CV_MurderTwistFinalScript@NScript@@; refs=1; replaces CV_SwordInTheStoneScript::Init: current name also held by 00ed4b10; CV_SwordInTheStoneScript::Init is at 00ed4b10 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Init@CV_MurderTwistFinalScript@NScript@@UAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CV_SwordInTheStoneScript@@QAEXXZ`; this source now defines the PDB member (class CV_SwordInTheStoneScript renamed CV_MurderTwistFinalScript); same bytes.
class CScriptThing { public:                       // 12 bytes, vtable 0x01238C8C (RTTI CScriptThing : CBaseClass)
    void* PImp_Data;                               // +0x04
    void* PImp_Info;                               // +0x08
    CScriptThing();                                // 0x006E7B40
    virtual ~CScriptThing();                       // 0x004AA840
};

class CScriptGameResourceObjectScriptedThingBase { public:   // 16 bytes
    unsigned char _body[0x10 - 4];
    CScriptGameResourceObjectScriptedThingBase();     // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();    // 0x007E74D0
};

namespace std {
template <class T> class allocator;
template <class T, class A> class vector;
template <> class vector<CScriptThing, allocator<CScriptThing> > {                 // retail 12 bytes
public:
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    void resize(unsigned int new_size, const CScriptThing& x);                      // 0x00D34AC0
};
template <> class vector<CScriptGameResourceObjectScriptedThingBase, allocator<CScriptGameResourceObjectScriptedThingBase> > {   // retail 12 bytes
public:
    CScriptGameResourceObjectScriptedThingBase* _M_start;
    CScriptGameResourceObjectScriptedThingBase* _M_finish;
    CScriptGameResourceObjectScriptedThingBase* _M_end_of_storage;
    void resize(unsigned int new_size, const CScriptGameResourceObjectScriptedThingBase& x);   // 0x00CD3CCB
};
}

namespace NScript {
class CV_MurderTwistFinalScript {
public:
    virtual void v00();
    unsigned char        _pad04[0x44];             // +0x04..+0x47
    long                 m_swordState;             // +0x48
    long                 m_crowdState;             // +0x4c
    unsigned char        m_swordTaken;             // +0x50
    unsigned char        m_crowdGathered;          // +0x51
    unsigned char        _pad52[0xe];              // +0x52..+0x5f
    std::vector<CScriptThing, std::allocator<CScriptThing> > m_things;                 // +0x60
    unsigned char        _pad6c[0x10];             // +0x6c..+0x7b
    std::vector<CScriptGameResourceObjectScriptedThingBase, std::allocator<CScriptGameResourceObjectScriptedThingBase> > m_scriptedThings;         // +0x7c
public:
    virtual void Init();                          // PDB ?Init@CV_MurderTwistFinalScript@NScript@@UAEXXZ
};
}
using NScript::CV_MurderTwistFinalScript;

void NScript::CV_MurderTwistFinalScript::Init()
{
    m_swordState = 0;
    m_crowdState = 0;
    m_swordTaken = 0;
    m_crowdGathered = 0;
    m_things.resize(2, CScriptThing());
    m_scriptedThings.resize(2, CScriptGameResourceObjectScriptedThingBase());
}