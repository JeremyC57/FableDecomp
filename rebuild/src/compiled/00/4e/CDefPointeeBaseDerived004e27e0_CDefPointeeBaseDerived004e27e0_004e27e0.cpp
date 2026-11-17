// CDefPointeeBase_Derived_004e27e0::~CDefPointeeBase_Derived_004e27e0() -- destroy the three container members at +0x44, +0x38, +0x2c, then the inlined CDefPointeeBase destructor stores its vtable and tail-calls the CMemoryBlock base destructor
// Leaf: ??1CDefPointeeBase_Derived_004e27e0@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E27E0, length 41 bytes.
// Callees: CContainer_004dd42f::~CContainer_004dd42f @ 0x004DD42F (x2: +0x44, +0x2c),
// std::_Rb_tree<long, pair<const long,long>, ...>::~_Rb_tree @ 0x004DD2B1 (+0x38, the map<long,long>) -- both destroy their elements
// and free the buffer (manifest labels "CActiveFile::OnReadFinished" are propagated garbage);
// CBase_009fc550::~CBase_009fc550 @ 0x009FC550 tail-called. Vtable 0x01230BA0 = CDefPointeeBase.
// The manifest label "~CBossDef" is propagated (the PDB CBossDef has its vectors at +0x28/+0x38/
// +0x48, not +0x2c/+0x38/+0x44); the derived class is named by its address. Same shape as the
// landed 0x004D9861: `__declspec(novtable)` derived class, members destroyed in reverse order.
struct CContainer_004dd42f {
    unsigned char _body[0x0c];
    ~CContainer_004dd42f();                     // 0x004DD42F
};

// NF64 (round 7, q10672): the +0x38 member is a std::map<long,long> (stlport/fable_map.h): its tree destructor 0x004DD2B1 is the
//   STLport ~_Rb_tree of that map, kept out of line (FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE) as retail calls it; was the stand-in
//   CContainer_004dd2b1.
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#include "stlport/fable_map.h"

class CResource { public:
    virtual void v00();
    virtual ~CResource();                          // 0x009FC550
};

class CDefPointeeBase : public CResource { public:       // vtable 0x01230BA0
    virtual void v00();
    ~CDefPointeeBase() { }                      // inline: inlined into every derived destructor
};

struct __declspec(novtable) CDefPointeeBase_Derived_004e27e0 : CDefPointeeBase {
    unsigned char        _pad0[0x28];           // +0x04..+0x2b
    CContainer_004dd42f  m_items0;              // +0x2c
    std::map<long, long> m_items1;              // +0x38
    CContainer_004dd42f  m_items2;              // +0x44
    virtual void v00();
    ~CDefPointeeBase_Derived_004e27e0();
};

CDefPointeeBase_Derived_004e27e0::~CDefPointeeBase_Derived_004e27e0()
{
}