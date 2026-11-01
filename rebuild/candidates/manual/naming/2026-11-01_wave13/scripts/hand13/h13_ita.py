# family ita (hand): IS_THING_ALIVE 0x0069CE20 is ??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z
# (Ego_r / FableWin instantiation; its two callers pass a CIntelligentPointer<const CThing>: CThingSoundEmitter::PAttachedThing
# and CAIStateGroup_StealStealableItem::PEscapePoint are CIntelligentPointer<CThing const> in struct_layouts_egor). The
# 0x00836AD0 caller already binds it; 0x009647C0 is respelled here. HE_OV=<overlay>
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '009647c0'
edit(A, 'src', 'struct CIntelligentPointerConstThing : CBaseIntelligentPointer {\n};',
     'template <class T> class CIntelligentPointer;\n'
     'template <> class CIntelligentPointer<const CThing> : public CBaseIntelligentPointer { public:   // Ego_r: PEscapePoint is CIntelligentPointer<CThing const>\n};\n'
     'typedef CIntelligentPointer<const CThing> CIntelligentPointerConstThing;')
edit(A, 'src', 'bool __fastcall IS_THING_ALIVE(const CIntelligentPointerConstThing& p);   // 0x0069CE20',
     'template <class T> bool __fastcall IS_THING_ALIVE(const T& p);              // 0x0069CE20 = IS_THING_ALIVE<CIntelligentPointer<const CThing> > (Ego_r / FableWin)')
