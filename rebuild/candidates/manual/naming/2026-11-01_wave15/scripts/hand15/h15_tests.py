# test stubs of the LINK_FAIL rows: the behaviour tests link the reconstructed object and define its callees, so a respelled
# callee needs its stub respelled the same way (bodies keep the same checks). Rows: 0080c250 (bcn), 00798540 / 008d6900 /
# 008d69f0 (sai), 006baa70 / 00729c30 / 007a0400 / 0085baf0 (queue).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
SAI = 'class CThingAICreature { public: void SuspendAI(bool suspend, void* suspender); };   // 0x00833A10 (Ego_r.pdb)\n'
GPI = 'class CBaseObject;\nclass CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };   // 0x00A01B50 (Ego_r.pdb)\n'
# bcn: the base's destructor is the protected virtual one (slot 0, was placeholder v00)
edit('0080c250', 'tst', 'struct CBaseClassNonCopyable { virtual void v00(); CBaseClassNonCopyable(); ~CBaseClassNonCopyable(); };',
     'class CBaseClassNonCopyable { protected: virtual ~CBaseClassNonCopyable(); public: CBaseClassNonCopyable(); };')
edit('0080c250', 'tst', 'void CBaseClassNonCopyable::v00() { }\n', '')
# sai
edit('00798540', 'tst', '    void SetSuspendAnimation(bool suspend, void* suspender);\n};\nvoid CThing::SetSuspendAnimation(bool suspend, void* suspender) {',
     '};\n' + SAI + 'void CThingAICreature::SuspendAI(bool suspend, void* suspender) {')
edit('008d6900', 'tst', '    void DoPush(int a, int b);             // __fastcall\n', '')
edit('008d6900', 'tst', 'void CMember::DoPush(int a, int b) {\n    (void)a; (void)b;\n    g_push_called++;\n    g_push_this += (int)(this->flag);',
     SAI + 'void CThingAICreature::SuspendAI(bool suspend, void* suspender) {\n    (void)suspend; (void)suspender;\n    g_push_called++;\n    g_push_this += (int)(((CMember*)this)->flag);')
edit('008d69f0', 'tst', '    void Do(int a, CModeDragonRearUp* self);\n', '')
edit('008d69f0', 'tst', 'void SomeState::Do(int a, CModeDragonRearUp* self) { g_doCalls++; g_lastArg = a; g_lastSelf = self; }',
     SAI + 'void CThingAICreature::SuspendAI(bool suspend, void* suspender) { g_doCalls++; g_lastArg = suspend; g_lastSelf = (CModeDragonRearUp*)suspender; }')
# queue
edit('006baa70', 'tst', 'extern "C" void __fastcall Mapwho_Dtor(CTCMapwho* self) { g_dtor++; }',
     'class CFaction { public: ~CFaction(); };   // 0x006BA6D0 (Ego_r.pdb)\nCFaction::~CFaction() { g_dtor++; }')
edit('00729c30', 'tst', '    void* Resolve();\n', '')
edit('00729c30', 'tst', 'void* Entrance::Resolve()\n{\n    return g_resolveResult;\n}', GPI + 'CBaseObject* CBaseIntelligentPointer::GetPItem() const\n{\n    return (CBaseObject*)g_resolveResult;\n}')
edit('007a0400', 'tst', 'struct CTCSound { unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c); };\nunsigned long CTCSound::PlaySound(const CCharString& name, bool positional, long a, long b, long c)\n{\n    ++g_plays; g_played_id = (long)name.PStringData;\n    return (!positional && a == 0 && b == 0 && c == -1) ? 42UL : 7UL;',
     'class CTCSoundPlayer { public: unsigned long PlayAttachedCriteria(const CCharString& name, bool a1, bool a2, bool a3, long a4); };   // 0x0066BD00 (Ego_r.pdb)\nunsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString& name, bool a1, bool a2, bool a3, long a4)\n{\n    ++g_plays; g_played_id = (long)name.PStringData;\n    return (!a1 && !a2 && !a3 && a4 == -1) ? 42UL : 7UL;')
edit('0085baf0', 'tst', '{ char pad[0x8]; void FABLE_FASTCALL inner(); void FABLE_FASTCALL outer(); };', '{ char pad[0x8]; void FABLE_FASTCALL outer(); };\n' + GPI)
edit('0085baf0', 'tst', 'void FABLE_FASTCALL FableDestVal_0085baf0_t::inner() { g_inner++; }', 'CBaseObject* CBaseIntelligentPointer::GetPItem() const { g_inner++; return 0; }')
edit('007a0400', 'tst', '    CTCSound snd;', '    CTCSoundPlayer snd;')
