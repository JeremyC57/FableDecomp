# h20_c686.py: naming wave 20, family c686 -- the chunk 686 NAME_FIXES rows (QUEUE_W20_input.tsv), name-only respells:
#   00502170 CWorldMap::CreateWorldLevelWad: the CWADBank local's 0x00A3A410 is ?AddFile@CWADBank@@QAEXABVCWideString@@@Z (Ego_r / FableWin.pdb;
#            was CBankFileAsync::UpdateEntryFromFile);
#   00780c80 / 00780d20 CTCFlammable::ExtinguishFire / OnInitialActivate: 0x007808B0 is the private ?InitFirePoints@CTCFlammable@@AAEXXZ
#            (Ego_r / FableWin.pdb; the queue's InitialiseFirePoints is in neither PDB);
#   00a13c60 C3DVector::Rotate: 0x00A55F90 is ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z (Ego_r / FableWin.pdb) on a default-constructed
#            matrix (the bound ??0CMatrix3x4@@QAE@ABVC3DVector@@M@Z exists in neither PDB);
#   007817f0 own row: ?GetEffectAtHelperPoint@CFlammableDef@@QBE_NABVCCharString@@AAJ@Z (Ego_r / FableWin.pdb; was the descriptive
#            CNamedValueTreeOwner::FindAttributeByName with a long* out parameter);
#   00781b90 own row: ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z (Ego_r / FableWin.pdb;
#            was Row00781b90::TreeOwner::BuildVectorFromTree over a local vector<unsigned>);
#   00c632e0 / 00c63350 own rows: CEaxContext::SetRoomHF / SetRoomLF (unsigned long slot, long value) -- the EAX reverb lRoomHF / lRoomLF setters the
#            landed SetEnvironment 0x00C68A50 (chunk 686) calls 2nd / 3rd in its preset walk (descriptive, libpk: no PDB; were CLight::SetIntegerProperty2 / 3).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c686.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())

a = '00502170'
if a not in _X:
    he.edit(a, 'src', '    void UpdateEntryFromFile(const CWideString& fileName);   // 0x00A3A410\n', '')
    he.edit(a, 'src', 'class CWADBank : public CBankFileAsync {                // vtable 0x012442DC\npublic:\n    CWADBank() { }\n',
            'class CWADBank : public CBankFileAsync {                // vtable 0x012442DC\npublic:\n    CWADBank() { }\n    void AddFile(const CWideString& fileName);          // 0x00A3A410 ?AddFile@CWADBank@@QAEXABVCWideString@@@Z (Ego_r / FableWin.pdb; naming wave 20, chunk 686)\n')
    he.resub(a, 'src', r'bank\.UpdateEntryFromFile\(', 'bank.AddFile(', 2)

for a in ('00780c80', '00780d20'):
    if a in _X: continue
    he.resub(a, 'src', r'(?m)^    void InitialiseFirePoints\(\);(\s*)// 0x007808B0\n', r'private:\n    void InitFirePoints();\1// 0x007808B0 ?InitFirePoints@CTCFlammable@@AAEXXZ (Ego_r / FableWin.pdb, private; naming wave 20, chunk 686)\npublic:\n', 1)
    he.resub(a, 'src', r'\bInitialiseFirePoints\b', 'InitFirePoints')

a = '00a13c60'
if a not in _X:
    he.edit(a, 'src', '    CMatrix3x4(const C3DVector& axis, float angle);      // 0x00A55F90\n',
            '    void SetFromVectorAndAngle(const C3DVector& axis, float angle);   // 0x00A55F90 ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z (Ego_r / FableWin.pdb; naming wave 20, chunk 686)\n')
    he.edit(a, 'src', '    CMatrix3x4 rotation(axis, angle);\n', '    CMatrix3x4 rotation;\n    rotation.SetFromVectorAndAngle(axis, angle);\n')
    he.edit(a, 'src', '    float X, Y, Z;\n    C3DVector(const C3DVector& v)', '    float X, Y, Z;\n    C3DVector() { }                                              // the engine\'s empty default constructor\n    C3DVector(const C3DVector& v)')
    he.edit(a, 'src', '// Callees: 0x00A55F90 CMatrix3x4::CMatrix3x4(const C3DVector& axis, float angle) (the axis-angle rotation; manifest\n// label "Matrix_RotationAroundAxis"), building the 48-byte matrix in the frame.\n',
            '// Callees: 0x00A55F90 CMatrix3x4::SetFromVectorAndAngle(const C3DVector& axis, float angle) (the axis-angle rotation; manifest\n// label "Matrix_RotationAroundAxis"; the PDB name set has no such constructor -- naming wave 20), building the 48-byte matrix in the frame.\n')

a = '007817f0'
if a not in _X:
    he.edit(a, 'src', '// CNamedValueTreeOwner::FindAttributeByName(const CCharString& name, long* value) -- walk the owner\'s def-string -> value map in order and return the value of the first entry whose name equals `name`\n// Leaf: FindAttributeByName\n',
            '// CFlammableDef::GetEffectAtHelperPoint(const CCharString& helperPoint, long& effect) const -- walk the def\'s helper-point -> effect map in order and return the effect of the first entry whose name equals `helperPoint`\n// Leaf: ?GetEffectAtHelperPoint@CFlammableDef@@QBE_NABVCCharString@@AAJ@Z\n// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb ?GetEffectAtHelperPoint@CFlammableDef@@QBE_NABVCCharString@@AAJ@Z -- the CFlammableDef\n//   (CTCFlammable::PDef at +0x0c) of InitFirePoints 0x007808B0, whose map at +0x40 the landed GetAllFireHelpers 0x00781B90 walks;\n//   was the descriptive CNamedValueTreeOwner::FindAttributeByName with a long* out parameter.\n')
    he.resub(a, 'src', r'\bCNamedValueTreeOwner\b', 'CFlammableDef')
    he.resub(a, 'src', r'FindAttributeByName\(const CCharString& name, long\* value\) const', 'GetEffectAtHelperPoint(const CCharString& helperPoint, long& effect) const')
    he.edit(a, 'src', '        if ((*it).first == name) {\n            *value = (*it).second;\n', '        if ((*it).first == helperPoint) {\n            effect = (*it).second;\n')
    he.resub(a, 'src', r'map<CDefString, long>   m_attributes;                // \+0x40', 'map<CDefString, long>   m_helperPointEffects;         // +0x40 (descriptive member name)')
    he.resub(a, 'src', r'\bm_attributes\b', 'm_helperPointEffects')
    he.resub(a, 'tst', r'\bCNamedValueTreeOwner\b', 'CFlammableDef')
    he.resub(a, 'tst', r'\bFindAttributeByName\b', 'GetEffectAtHelperPoint')

a = '00781b90'
if a not in _X:
    p, rel, t = he._rw(a, 'src')
    if 'GetAllFireHelpers' not in t:
        t = t.replace('// 00781b90 Row00781b90::TreeOwner::BuildVectorFromTree (catalog CTCAvoidRegion_BuildVectorFromTree; no CTCAvoidRegion class in the PDBs, owner kept)\n// Flags: /O2 /Oy /W3\n// Leaf: BuildVectorFromTree\n',
                      '// CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helpers) const -- push every key of the def\'s helper-point -> effect map (+0x40) onto the vector, in tree order\n// Flags: /O2 /Oy /W3\n// Leaf: ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z\n// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@V?$allocator@VCDefString@@@std@@@std@@@Z --\n//   the CFlammableDef (CTCFlammable::PDef) of InitFirePoints 0x007808B0, the same map the landed GetEffectAtHelperPoint 0x007817F0 walks;\n//   was Row00781b90::TreeOwner::BuildVectorFromTree over a local vector<unsigned> (catalog CTCAvoidRegion_BuildVectorFromTree, propagated).\n//   The vector is the row\'s inline STLport push_back model (three pointers; _M_insert_overflow), spelled as the std::vector instantiation.\n')
        t = t.replace('namespace Row00781b90 {\n', 'class CDefString { public: unsigned long CRC; };               // 4 bytes (the map key; Ego_r.pdb class V)\ntemplate <class K, class V> struct pair { K first; V second; };\nnamespace Row00781b90 {\n')
        t = t.replace('struct _Rb_tree_node {unsigned _M_color;_Rb_tree_node*_M_parent,*_M_left,*_M_right;unsigned _M_value_field;};',
                      'struct _Rb_tree_node {unsigned _M_color;_Rb_tree_node*_M_parent,*_M_left,*_M_right;pair<const CDefString, long> _M_value_field;};')
        t = t.replace(' __forceinline unsigned&operator*()const{return _M_node->_M_value_field;}', ' __forceinline const pair<const CDefString, long>&operator*()const{return _M_node->_M_value_field;}')
        # the vector becomes the std::vector<CDefString, std::allocator<CDefString> > instantiation (its members stay the row's inline model)
        t = t.replace('template<class T> struct vector {\n', '}\nnamespace std {\ntemplate<class T> class allocator { };\ntemplate<class T, class A = allocator<T> > class vector { public:\n')
        t = t.replace('   T*oldLast=_M_finish;T*buffer=cap?(T*)malloc(cap*sizeof(T)):0;\n   T*out=__uninitialized_copy(_M_start,oldLast,buffer);\n   for(unsigned i=0;i<count;++i){_Construct(out,v);++out;}',
                      '   T*oldLast=_M_finish;T*buffer=cap?(T*)malloc(cap*sizeof(T)):0;\n   T*out=Row00781b90::__uninitialized_copy(_M_start,oldLast,buffer);\n   for(unsigned i=0;i<count;++i){Row00781b90::_Construct(out,v);++out;}')
        t = t.replace('   unsigned old=_M_finish-_M_start;unsigned cap=old+max(old,count);', '   unsigned old=_M_finish-_M_start;unsigned cap=old+Row00781b90::max(old,count);')
        t = t.replace('  if(_M_finish!=_M_end_of_storage){_Construct(_M_finish,v);++_M_finish;}', '  if(_M_finish!=_M_end_of_storage){Row00781b90::_Construct(_M_finish,v);++_M_finish;}')
        t = t.replace('\n};\ntypedef vector<unsigned> Vector;\nstruct TreeOwner {\n char pad[0x40];_Rb_tree_node*_M_header;\n __forceinline _Rb_tree_iterator begin(){return _Rb_tree_iterator(_M_header->_M_left);}\n __forceinline _Rb_tree_iterator end(){return _Rb_tree_iterator(_M_header);}\n void BuildVectorFromTree(Vector*);\n};\nvoid TreeOwner::BuildVectorFromTree(Vector*out){for(_Rb_tree_iterator it=begin();it!=end();++it)out->push_back(*it);}\n}',
                      '\n};\n}\nclass CFlammableDef { public:\n char pad[0x40];Row00781b90::_Rb_tree_node*_M_header;      // +0x40: the helper-point -> effect map (STLport map<CDefString, long>, header pointer first)\n __forceinline Row00781b90::_Rb_tree_iterator begin()const{return Row00781b90::_Rb_tree_iterator(_M_header->_M_left);}\n __forceinline Row00781b90::_Rb_tree_iterator end()const{return Row00781b90::_Rb_tree_iterator(_M_header);}\n void GetAllFireHelpers(std::vector<CDefString>& helpers) const;\n};\nvoid CFlammableDef::GetAllFireHelpers(std::vector<CDefString>& helpers)const{for(Row00781b90::_Rb_tree_iterator it=begin();it!=end();++it)helpers.push_back((*it).first);}')
        he._w(p, t); print('ok', a, 'src')
    else: print('already', a, 'src')

for a, nm, old, lvl, bit in (('00c632e0', 'SetRoomHF', 'SetIntegerProperty2', 'Level2', '0x10'), ('00c63350', 'SetRoomLF', 'SetIntegerProperty3', 'Level3', '0x20')):
    if a in _X: continue
    p, rel, t = he._rw(a, 'src')
    if nm not in t:
        t = re.sub(r'(?m)^// CLight::%s\(long voice, long level\) -- validate a level in millibels \(-10000\.\.0, else last error -2, result -3\), store it in the voice\'s record \(%s\) when it changed and, when that voice is the current, live one, commit level \(m_committed%s\) and mark dirty bit %s; returns 0\n'
                   % (old, lvl, lvl, bit),
                   '// CEaxContext::%s(unsigned long slot, long value) -- validate a value in millibels (-10000..0, else last error -2, result -3), store it in the slot\'s reverb record (%s) when it changed and, when that slot is the current, live one, commit it (m_committed%s) and mark dirty bit %s; returns 0\n'
                   '// Leaf: ?%s@CEaxContext@@QAEJKJ@Z\n'
                   '// Name (naming wave 20, chunk 686): the EAX 4.0 reverb-slot property setter the landed CEaxContext::SetEnvironment 0x00C68A50 calls %s in its\n'
                   '//   preset walk (lRoomHF / lRoomLF of the EAXREVERBPROPERTIES block: the -10000..0 millibel range, dirty bit %s); descriptive member name (libpk, no\n'
                   '//   PDB); the class is the landed CEaxContext (SetEnvironment / SetDecayTime 0x00C633C0 share its `this`); was the propagated CLight::%s.\n'
                   % (nm, lvl.replace('Level2', 'RoomHF').replace('Level3', 'RoomLF'), lvl.replace('Level2', 'RoomHF').replace('Level3', 'RoomLF'), bit, nm, '2nd' if nm == 'SetRoomHF' else '3rd', bit, old), t, count=1)
        t = t.replace('struct CLight {', 'class CEaxContext { public:').replace('long CLight::%s(long voice, long level)' % old, 'long CEaxContext::%s(unsigned long slot, long value)' % nm)
        t = t.replace('    long %s(long voice, long level);' % old, '    long %s(unsigned long slot, long value);' % nm)
        t = t.replace('m_currentVoice', 'm_currentSlot').replace('m_voices[voice]', 'm_slotProperties[slot]').replace('m_slots[voice]', 'm_slotRecords[slot]')
        t = t.replace('SVoiceSlot       m_slots[3];', 'SSlotRecord      m_slotRecords[3];').replace('SVoiceProperties m_voices[5];', 'SSlotProperties  m_slotProperties[5];')
        t = t.replace('struct SVoiceSlot {', 'struct SSlotRecord {').replace('struct SVoiceProperties {', 'struct SSlotProperties {')
        t = t.replace('    long          Level2;                          // +0x08', '    long          RoomHF;                          // +0x08').replace('    long          Level3;                          // +0x0c', '    long          RoomLF;                          // +0x0c')
        t = re.sub(r'\bm_committedLevel2\b', 'm_committedRoomHF', t); t = re.sub(r'\bm_committedLevel3\b', 'm_committedRoomLF', t)
        t = re.sub(r'\bLevel2\b', 'RoomHF', t); t = re.sub(r'\bLevel3\b', 'RoomLF', t)
        t = re.sub(r'\blevel\b', 'value', t.split('long CEaxContext::')[0]) + 'long CEaxContext::' + re.sub(r'\blevel\b', 'value', t.split('long CEaxContext::')[1])
        t = re.sub(r'\bvoice\b', 'slot', t)
        he._w(p, t); print('ok', a, 'src')
    else: print('already', a, 'src')
    he.resub(a, 'tst', r'\bCLight\b', 'CEaxContext')
    he.resub(a, 'tst', r'\b%s\b' % old, nm)
