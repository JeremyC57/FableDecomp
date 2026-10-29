# wave 7 family ipo7: the 24 byte-identical CIntelligentPointer::operator= rows (one 741-byte mass-landed source, all named
# "CIntelligentPointer<CThingCreatureBase>") call CBaseIntelligentPointer::GetPItem 0x00A01B50 and ::SetPItem 0x00A01B90 under
# stand-in names (?GetTarget@CIntelligentPointer@@QAEPAVCThingCreatureBase@@XZ, ?Assign@CIntelligentPointer@@...).
# Ego_r.pdb + FableWin.pdb: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ, ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z.
# Rows whose instance is settled (DECISIONS_W7: 0x004A9760 CThing, 0x0048BD20 CThingPlayerCreature, 0x0050F440 CThingAICreature)
# become the template instance itself (explicit instantiation; ??4?$CIntelligentPointer@VT@@@@QAEAAV0@ABV0@@Z, matching the
# rows' Leaf selector); the other 21 keep their non-template class (pointee unproven) with the callees respelled. Idempotent.
import re, sys, os, hashlib
ORIG = 'ecb63fb7444a02229f4ba04fecde6b30'
KNOWN = {'004a9760': 'CThing', '0048bd20': 'CThingPlayerCreature', '0050f440': 'CThingAICreature'}
BASE = ('class CBaseObject;\n'
        'class CBaseIntelligentPointer\n{\npublic:\n'
        '    CBaseObject* GetPItem() const;          // 0x00A01B50 (Ego_r.pdb)\n'
        '    void SetPItem(CBaseObject* item);       // 0x00A01B90 (Ego_r.pdb)\n};\n\n')
def templ(a, T):
    return ('// CIntelligentPointer<%s>::operator= @ 0x%s (instance settled in naming wave 7, DECISIONS_W7.tsv)\n' % (T, a.upper()) +
            '// Leaf: ??4?$CIntelligentPointer@\n'
            '// Smart-pointer copy assignment: SetPItem(other.GetPItem()); return *this;\n\n' + BASE +
            'class %s;\n\n' % T +
            'template <class T> class CIntelligentPointer : public CBaseIntelligentPointer\n{\npublic:\n'
            '    CIntelligentPointer& operator=(const CIntelligentPointer& other);\n};\n\n'
            'template <class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& other)\n{\n'
            '    SetPItem(other.GetPItem());\n    return *this;\n}\n\n'
            'template class CIntelligentPointer<%s>;\n' % T)
PLAIN = ('// CIntelligentPointer<T>::operator= (pointee unproven; the row is one of 24 byte-identical instance bodies)\n'
         '// Leaf: ??4?$CIntelligentPointer@\n'
         '// Smart-pointer copy assignment: SetPItem(rhs->GetPItem()); return this;\n\n' + BASE +
         'class CThingCreatureBase;\n\n'
         'class CIntelligentPointer : public CBaseIntelligentPointer\n{\npublic:\n'
         '    CIntelligentPointer* operator=(CIntelligentPointer* rhs);\n\n    CThingCreatureBase* m_ptr;\n};\n\n'
         '// Out-of-line so the compiler emits the body into the .obj.\n'
         'CIntelligentPointer* CIntelligentPointer::operator=(CIntelligentPointer* rhs)\n{\n'
         '    this->SetPItem(rhs->GetPItem());\n    return this;\n}\n')
if __name__ == '__main__':
    for p in sys.argv[1:]:
        a = re.search(r'_([0-9a-f]{8})\.cpp$', p).group(1)
        b = open(p, 'rb').read()
        new = templ(a, KNOWN[a]) if a in KNOWN else PLAIN
        if b.decode('utf-8') == new: print('skip', p, 'already'); continue
        if hashlib.md5(b).hexdigest() != ORIG: print('skip', p, 'not the 741-byte source'); continue
        open(p, 'w', encoding='utf-8', newline='').write(new); print('edited', p, 'template' if a in KNOWN else 'plain')
