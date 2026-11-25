// CTCMapwho::OnDie  retail 0x006b5870  __fastcall void(CTCMapwho*)

struct SubObjAt90 {
};

// cdecl free function, takes the object pointer, cleans up 4 bytes
void operator delete(void*);   // call 0xbfe9bc

class CTCMapwho { public:
    char pad[0x90];
    SubObjAt90 sub;   // at +0x90
    void OnDie();
};

// Naming (NF449 gen, 2026-11-25): callee-only respell of this row's destructor stand-in calls to the PDB destructor (T1 tracker;
//   target self rows' Leaf), as a qualified destructor call on a bodiless declaration (no layout; rulings r2/r3):
//   0x008814C0 ??1CScreenEffectPriorityStack@NScreenEffect@@UAE@XZ (was ?Release@SubObjAt90@@QAEXXZ)
namespace NScreenEffect { class CScreenEffectPriorityStack { public: virtual ~CScreenEffectPriorityStack(); }; }
void CTCMapwho::OnDie()
{
    if (this) {
        reinterpret_cast<NScreenEffect::CScreenEffectPriorityStack*>(&(this->sub))->NScreenEffect::CScreenEffectPriorityStack::~CScreenEffectPriorityStack();
        operator delete(this);
    }
}