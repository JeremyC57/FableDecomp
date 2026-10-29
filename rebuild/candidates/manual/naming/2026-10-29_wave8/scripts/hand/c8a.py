# wave 8 scope 1: the three RELOC_CONFLICTs ChatGPT pass 9 exposed (pass 9 side right; older landed rows respelled).
#  A. 0x004A96F0 stores vtable 0x01238C6C (RTTI .?AV?$CIntelligentPointer@VCThing@@@@) = CIntelligentPointer<CThing>(CThing*);
#     0x0056C860 stores 0x01237F1C (.?AV?$CIntelligentPointer@$$CBVCThing@@@@) = the const-CThing ctor. 0072cba0 and 0071c9f0
#     construct the 0x004A96F0 temporary, so their pointee is CThing (0071c9f0: Ego_r CTCHeroMorph::IdleParticleEmitters is
#     list<CIntelligentPointer<CThing> >).
#  B. CBankFileAsyncData: Ego_r order IsFinished 0xB29550, GetData 0xB295A0, PeekData 0xB295F0, CanRelocateData 0xB29640 and
#     FableWin order (... WaitUntilFinished, GetData, PeekData, RelocateData) = retail IsFinished 0x9D54E0, 0x9D5530 GetData,
#     0x9D5580 PeekData, 0x9D55D0 CanRelocateData. Callers by retail call target: 0x9D5530 from 009c8700 009c8fb0 00a25d20
#     00a42ea0 00bdc510 00bf3b60 009fd9b0 (GetData); 0x9D5580 from 00be04c0 (PeekData).
#  C. 0x006860F0 is STLport vector<bool>::_M_fill_assign (landed defining row STLvectorbool_Mfillassign_006860f0, public,
#     QAEXIAB_N); 006821e0 calls it through the inline assign(0x5C, true) (resize is 0x00467686).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all

# --- A. 0072cba0 CTCSpecialAbilities::SetupBerserk
F = '0072cba0'
edit(F, 'src', '//          CIntelligentPointer<const CThing>(CThing*) 0x004A96F0; the berserk action\'s item setter 0x0072D360 (by-value',
     '//          CIntelligentPointer<CThing>(CThing*) 0x004A96F0 (vtable 0x01238C6C, RTTI CIntelligentPointer<CThing>); the berserk\n'
     '//          action\'s item setter 0x0072D360 (by-value')
edit(F, 'src', '    CIntelligentPointer(T* item);                   // 0x004A96F0 (const CThing instantiation)\n',
     '    CIntelligentPointer(T* item);                   // 0x004A96F0 (CThing instantiation)\n')
edit(F, 'src', '    void SetItem(CIntelligentPointer<const CThing> item);   // 0x0072D360\n',
     '    void SetItem(CIntelligentPointer<CThing> item); // 0x0072D360\n')

# --- A. 0071c9f0 CTCHeroMorph::UpdateIdleParticles
F = '0071c9f0'
edit(F, 'src', '(STLport list of CIntelligentPointer<const CThing>)', '(STLport list of CIntelligentPointer<CThing>, Ego_r)')
edit(F, 'src', '    CIntelligentPointer(T* item);                       // 0x004A96F0 (const CThing instance)\n',
     '    CIntelligentPointer(T* item);                       // 0x004A96F0 (CThing instance: vtable 0x01238C6C)\n')
edit(F, 'src', '        CIntelligentPointer<const CThing> _M_data;\n', '        CIntelligentPointer<CThing> _M_data;\n')
edit(F, 'src', '    void push_back(const CIntelligentPointer<const CThing>& item);   // 0x004BA4E0\n',
     '    void push_back(const CIntelligentPointer<CThing>& item);   // 0x004BA4E0\n')
edit(F, 'src', '        node->_M_data.~CIntelligentPointer<const CThing>();\n', '        node->_M_data.~CIntelligentPointer<CThing>();\n')
edit(F, 'src', '                            CIntelligentPointer<const CThing> item(particle);\n',
     '                            CIntelligentPointer<CThing> item((CThing*)particle);\n')

# --- B. defining rows: 009d5530 is GetData, 009d5580 the const PeekData
F = '009d5530'
edit(F, 'src', '// CBankFileAsyncData::PeekData() -- lock', '// CBankFileAsyncData::GetData() -- lock')
edit(F, 'src', '(0x009D5530 and 0x009D5580 are byte-identical bodies).\n',
     '(0x009D5530 and 0x009D5580 are byte-identical bodies).\n'
     '// Name: Ego_r CBankFileAsyncData order IsFinished, GetData, PeekData, CanRelocateData (FableWin: GetData, PeekData) =\n'
     '//       retail 0x009D54E0, 0x009D5530, 0x009D5580, 0x009D55D0: this is GetData (QAEPAXXZ); PeekData is the const twin.\n')
edit(F, 'src', '    void* PeekData();     // 0x009D5530 / 0x009D5580\n', '    void* GetData();                                // 0x009D5530\n')
edit(F, 'src', 'void* CBankFileAsyncData::PeekData()\n', 'void* CBankFileAsyncData::GetData()\n')
edit(F, 'tst', '    void* PeekData()\n', '    void* GetData()\n')
edit_all(F, 'tst', 'd.PeekData()', 'd.GetData()', 2)
edit(F, 'tst', 'e.PeekData()', 'e.GetData()')
F = '009d5580'
edit(F, 'src', '// CBankFileAsyncData::GetData() -- lock', '// CBankFileAsyncData::PeekData() const -- lock')
edit(F, 'src', '(0x009D5530 and 0x009D5580 are byte-identical bodies).\n',
     '(0x009D5530 and 0x009D5580 are byte-identical bodies).\n'
     '// Name: Ego_r CBankFileAsyncData order IsFinished, GetData, PeekData, CanRelocateData (FableWin: GetData, PeekData) =\n'
     '//       retail 0x009D54E0, 0x009D5530, 0x009D5580, 0x009D55D0: this is the const PeekData (QBEPBXXZ).\n')
edit(F, 'src', '    bool                        m_dataLocked;     // +0x45\n',
     '    mutable bool                m_dataLocked;     // +0x45 (set by the const PeekData)\n')
edit(F, 'src', '    void* GetData();     // 0x009D5530 / 0x009D5580\n', '    const void* PeekData() const;                   // 0x009D5580\n')
edit(F, 'src', 'void* CBankFileAsyncData::GetData()\n', 'const void* CBankFileAsyncData::PeekData() const\n')
edit(F, 'tst', '    bool m_relocatable, m_relocating, m_finished, m_dataLocked, m_completionPending; bool m_setupResult;\n',
     '    bool m_relocatable, m_relocating, m_finished; mutable bool m_dataLocked; bool m_completionPending; bool m_setupResult;\n')
edit(F, 'tst', '    void* GetData()\n', '    const void* PeekData() const\n')
edit_all(F, 'tst', 'd.GetData()', 'd.PeekData()', 2)
edit(F, 'tst', 'e.GetData()', 'e.PeekData()')

# --- B. callers
F = '00be04c0'
edit(F, 'src', 'CBankFileAsyncData::GetData @ 0x009D5580 (on', 'CBankFileAsyncData::PeekData @ 0x009D5580 (const twin of GetData 0x009D5530; on')
edit(F, 'src', '    void* GetData();                               // 0x009D5580\n', '    const void* PeekData() const;                  // 0x009D5580\n')
edit(F, 'src', '    return node->m_pData->GetData();\n', '    return (void*)node->m_pData->PeekData();\n')
for F in ('009c8700', '009c8fb0'):
    edit(F, 'src', '::PeekData @ 0x009D5530 (byte-twin of GetData 0x009D5580, named apart for relocation identity),',
         '::GetData @ 0x009D5530 (Ego_r / FableWin order GetData, PeekData; byte-twin of the const PeekData 0x009D5580),')
    edit(F, 'src', '    const void* PeekData() const;      // 0x009D5530 (byte-identical to GetData 0x009D5580)\n',
         '    void* GetData();                   // 0x009D5530 (byte-identical to the const PeekData 0x009D5580)\n')
edit('009c8700', 'src', 'PGroup->PFileData->PeekData()', 'PGroup->PFileData->GetData()')
edit('009c8fb0', 'src', 'PText->PFileData->PeekData()', 'PText->PFileData->GetData()')
# 00bf3b60 (its header's GetData / IsFinished) is not edited: the row grades DIFFER before and after (DEFERRED).

# --- C. 006821e0 CTCCreatureOpinionOfHero::SetOpinionDeedMask: assign(0x5C, true) -> _M_fill_assign 0x006860F0
F = '006821e0'
edit(F, 'src', '// Callees: vector<bool>::resize(size_type, const bool&) 0x006860F0 (catalog "std::vector_insert_value"),',
     '// Callees: vector<bool>::_M_fill_assign(size_type, const bool&) 0x006860F0 (through the inline assign; landed STLport row;\n'
     '//   catalog "std::vector_insert_value"; resize is 0x00467686),')
edit(F, 'src', '    void resize(size_type __new_size, const _Tp& __x);   // 0x006860F0 (vector<bool>)\n',
     '    void _M_fill_assign(size_type __n, const _Tp& __val);   // 0x006860F0 (vector<bool>)\n'
     '    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }\n')
edit(F, 'src', 'The resize\'s `true` temporary is the high byte', 'The assign\'s `true` temporary is the high byte')
edit(F, 'src', '    DeedMask.resize(OPINION_DEED_TYPE_COUNT, true);\n', '    DeedMask.assign(OPINION_DEED_TYPE_COUNT, true);\n')
edit(F, 'tst', '    void resize(size_type __new_size, const _Tp& __x);   // 0x006860F0 (vector<bool>)\n',
     '    void _M_fill_assign(size_type __n, const _Tp& __val);   // 0x006860F0 (vector<bool>)\n'
     '    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }\n')
edit(F, 'tst', '    DeedMask.resize(OPINION_DEED_TYPE_COUNT, true);\n', '    DeedMask.assign(OPINION_DEED_TYPE_COUNT, true);\n')
edit(F, 'tst', 'void vector<_Tp, _Alloc>::resize(size_type __new_size, const _Tp& __x)\n{                                                          // test stand-in for 0x006860F0: erase or fill-insert at the end\n    ++g_resizes;\n    size_type __size = (size_type)(this->_M_finish - this->_M_start);\n    if (__new_size < __size) { this->_M_finish = this->_M_start + __new_size; return; }\n    if (this->_M_start + __new_size > this->_M_end_of_storage._M_data) {\n        _Tp* __p = (_Tp*)std::malloc(__new_size * sizeof(_Tp));\n        if (__size) std::memcpy(__p, this->_M_start, __size * sizeof(_Tp));\n        if (this->_M_start) std::free(this->_M_start);\n        this->_M_start = __p; this->_M_finish = __p + __size; this->_M_end_of_storage._M_data = __p + __new_size;\n    }\n    for (; this->_M_finish != this->_M_start + __new_size; ++this->_M_finish)\n        *this->_M_finish = __x;\n}\n',
     'void vector<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp& __val)\n{                                                          // test stand-in for 0x006860F0: __n copies of __val\n    ++g_resizes;\n    if (this->_M_start + __n > this->_M_end_of_storage._M_data) {\n        if (this->_M_start) std::free(this->_M_start);\n        this->_M_start = (_Tp*)std::malloc(__n * sizeof(_Tp));\n        this->_M_end_of_storage._M_data = this->_M_start + __n;\n    }\n    for (this->_M_finish = this->_M_start; this->_M_finish != this->_M_start + __n; ++this->_M_finish)\n        *this->_M_finish = __val;\n}\n')
edit(F, 'tst', '    opinion.SetOpinionDeedMask(2);                             // a def of another class: mask untouched beyond the resize\n'
     '    CHECK(!opinion.DeedMask[5] && ',
     '    opinion.SetOpinionDeedMask(2);                             // a def of another class: mask only reset to all enabled\n'
     '    CHECK(opinion.DeedMask[5] && ')
