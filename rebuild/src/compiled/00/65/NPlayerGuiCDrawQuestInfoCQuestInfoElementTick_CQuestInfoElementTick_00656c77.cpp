// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(const CWideString& text, bool ticked, float scale) -- base element(scale), Graphic = 0, the text copied, Ticked kept, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656C77, length 51 bytes.
// Leaf: ??0CQuestInfoElementTick@CDrawQuestInfo@NPlayerGui
// Retail bytes: fld [esp+0Ch] ; push esi ; push ecx ; fstp [esp] ; mov esi,ecx ; call CQuestInfoElement::CQuestInfoElement ; push dword ptr [esp+8] ;
//               and dword ptr [esi+1Ch],0 ; lea ecx,[esi+20h] ; mov dword ptr [esi],offset vftable(0x0125A518) ; call CWideString::CWideString(const CWideString&) ;
//               mov al,[esp+0Ch] ; mov [esi+24h],al ; mov eax,esi ; pop esi ; ret 0Ch
// Callees: the quest-info element base constructor @ 0x00656795 (one float argument, the scale; the manifest's
// "CVsyncCallbackTimerInfo" is propagated), the CWideString copy constructor @ 0x0099B720.
// The manifest label "CScreenFilterSThingByPass::CScreenFilterSThingByPass" is propagated; the vtable 0x0125A518 is
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTick (rtti-vtables.tsv) and its one caller is the quest-info Update path
// (0x0064FBD2). Layout (PDB, less the 4 bytes of the retail 12-byte PrimitiveHandles vector in the 0x1c-byte base): Graphic +0x1c,
// Text +0x20, Ticked +0x24. Shape: a size-optimised TU (`and dword ptr [..],0`, the pass-through push) as the landed sibling
// CQuestInfoElementCounterList constructor 0x00656C0A.
#pragma optimize("s", on)
#include <stddef.h>

class CWideString { public:
    wchar_t* m_pData;
    CWideString(const CWideString& other);     // 0x0099B720
};

namespace NPlayerGui {
class CDrawQuestInfo { public:

class CQuestInfoElement { public:                     // vtable 0x0125A2B4
    virtual void v00();
    unsigned char _body[0x1c - 4];             // +0x04 ID, ScriptID, Scale, PrimitiveHandles
    CQuestInfoElement(float scale);            // 0x00656795
};

class CQuestInfoElementTick : public CQuestInfoElement { public:   // vtable 0x0125A518
    virtual void v00();
    long        Graphic;                       // +0x1c
    CWideString Text;                          // +0x20
    bool        Ticked;                        // +0x24
    CQuestInfoElementTick(const CWideString& text, bool ticked, float scale);
};

};

CDrawQuestInfo::CQuestInfoElementTick::CQuestInfoElementTick(const CWideString& text, bool ticked, float scale)
    : CQuestInfoElement(scale), Graphic(0), Text(text), Ticked(ticked)
{
}

}