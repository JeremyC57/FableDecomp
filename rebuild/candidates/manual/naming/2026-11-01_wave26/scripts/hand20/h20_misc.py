# h20_misc.py: naming wave 20, family misc -- the remaining c686 queue items that are single-row respells (name-only / byte-identical):
#   eps:    ?DELTA_NOTIONAL_ZERO@@3MB 0x0129BA3C (ESTABLISHED_SPELLINGS) in the three rows wave 18 left spelling EPSILON (00751e80 twice, 00788617, 00badb40);
#   getlen: ?GetLength@CMemoryDataOutputStream@@QBEKXZ 0x00994B40 (Ego_r / FableWin.pdb) in 009d4a50 / 009d4f10 (were the non-const GetSize);
#   0042b467: the map<CCharString, long> _M_find (c686 queue: the helper-count map of InitFirePoints; the landed 004FE510 names the map
#           <CCharString, long>) -- CUnprovenValue_0042b467 -> long;
#   00781670: the map<CDefString, long> find of CFlammableDef's helper-point map (c686 queue; the landed GetEffectAtHelperPoint 0x007817F0 /
#           GetAllFireHelpers 0x00781B90 walk the same tree: pair<const CDefString, long>; CDefString = long TablePos, Ego_r.pdb) -- the
#           CUnproven* placeholders -> the proven types;
#   007e6ec0: ??$GetPFirstThing@VCThingFilter_HasDefName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasDefName@@PAPAVCThing@@@Z (Ego_r.pdb
#           member template; the callee 0x006A7BE0 is the landed ??RCThingFilter_HasDefName@@QBE_NABVCThing@@@Z) on the landed byte-twin
#           0x0056D050 text (DECISIONS_W19 item 6 verification: not HasTrackName (CThingTrackNode filter) nor HasDefGlobalIndex (0x006A80A0 = HasTC));
#   007e5d20: its caller CTCOracleMinigame::PlayGame names the template with CThingFilter_HasDefName (the four OBJECT_* def names) and the
#           gamesmaster search with CThingFilter_HasScriptName (0x0056D050, the landed leaf) -- was HasTrackName on both;
#   009e6530: the own row of ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb; the comparator of the landed chunk-686 sort rows) -- was the
#           descriptive FastCompare (the 37 callers are the fastcmp family);
#   00ac99f0: ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
#           (Ego_r.pdb; the landed 0x00ACA0C0 names it) over the landed ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@... 0x00AC97D0
#           (chunk 686) -- was the un-namespaced CGroupRemap stand-in with a CRetailVector.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_misc.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())

for a in ('00751e80', '00788617', '00badb40'):
    if a in _X: continue
    he.resub(a, 'src', r'extern const float EPSILON;(\s*)// 0x0129BA3C', r'extern const float DELTA_NOTIONAL_ZERO;\1// 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (ESTABLISHED_SPELLINGS; naming wave 20)')
    he.resub(a, 'src', r'\bEPSILON\b', 'DELTA_NOTIONAL_ZERO')
for a in ('009d4a50', '009d4f10'):
    if a in _X: continue
    he.resub(a, 'src', r'unsigned long GetSize\(\);(\s*)// 0x00994B40', r'unsigned long GetLength() const;\1// 0x00994B40 ?GetLength@CMemoryDataOutputStream@@QBEKXZ (Ego_r / FableWin.pdb; naming wave 20)')
    he.resub(a, 'src', r'->GetSize\(\)', '->GetLength()')
    he.resub(a, 'src', r'CMemoryDataOutputStream::GetSize @', 'CMemoryDataOutputStream::GetLength @')
    he.resub(a, 'src', r'::GetSize @ 0x00994B40', '::GetLength @ 0x00994B40')

a = '0042b467'
if a not in _X:
    he.edit(a, 'src', 'struct CUnprovenValue_0042b467 { };                     // mapped type not recoverable (only the key is read here)\n', '')
    he.resub(a, 'src', r'pair<const CCharString, CUnprovenValue_0042b467>', 'pair<const CCharString, long>')
    he.edit(a, 'src', '// Leaf: ?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@ECUnprovenValue_0042b467@@\n', '// Leaf: ?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@J@std@@\n')
    he.edit(a, 'src', '// Identity (descriptive): a CCharString-keyed tree of this TU (callers 0x0042B8FC / 0x0042BEF2, both propagated labels; reloc_bindings has only the stand-in CSubDefMap::find); the mapped type is not recoverable.\n',
            '// Identity (naming wave 20, chunk 686): the map<CCharString, long> of this TU -- its insert_unique 0x00431606 (landed: "the tree is map<CCharString, long>") and\n//   _M_erase 0x0043020A are the same instantiation; chunk 686 reads it as the helper-count map of CTCFlammable::InitFirePoints 0x007808B0 and the\n//   landed CMap caller 0x004FE510 as its name -> index map<CCharString, long> (was the CUnprovenValue_0042b467 placeholder).\n')
    he.resub(a, 'src', r'(?m)^// std::_Rb_tree<CCharString, pair<const CCharString, CUnprovenValue_0042b467>, \.\.\.>::_M_find', '// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_find')

a = '00781670'
if a not in _X:
    he.edit(a, 'src', 'struct CUnprovenKey_00781670 { long _M_unproven; };\ninline bool operator<(const CUnprovenKey_00781670& __x, const CUnprovenKey_00781670& __y) { return __x._M_unproven < __y._M_unproven; }\nstruct CUnprovenValue_00781670 { CUnprovenKey_00781670 _M_key; };\nstruct CUnprovenKeyOfValue_00781670 { const CUnprovenKey_00781670& operator()(const CUnprovenValue_00781670& __v) const { return __v._M_key; } };\n',
            'class CDefString { public:                                     // Ego_r.pdb: long TablePos (4 bytes, class V)\n    long TablePos;\n    bool operator<(const CDefString& __y) const { return TablePos < __y.TablePos; }   // the signed `jl` / `jge` compare of the walk (no out-of-line operator< in either PDB)\n};\n')
    p, rel, t = he._rw(a, 'src')
    t = re.sub(r'template _Rb_tree<CUnprovenKey_00781670, CUnprovenValue_00781670, CUnprovenKeyOfValue_00781670, less<CUnprovenKey_00781670 >, allocator<CUnprovenValue_00781670 > >::iterator _Rb_tree<CUnprovenKey_00781670, CUnprovenValue_00781670, CUnprovenKeyOfValue_00781670, less<CUnprovenKey_00781670 >, allocator<CUnprovenValue_00781670 > >::find\(const CUnprovenKey_00781670&\);',
               'typedef pair<const CDefString, long> _Val;\ntemplate _Rb_tree<CDefString, _Val, _Select1st<_Val>, less<CDefString>, allocator<_Val> >::iterator _Rb_tree<CDefString, _Val, _Select1st<_Val>, less<CDefString>, allocator<_Val> >::find(const CDefString&);', t)
    L = t.split('\n')
    L[0] = '// std::_Rb_tree<CDefString, std::pair<const CDefString, long>, std::_Select1st<std::pair<const CDefString, long> >, std::less<CDefString>, std::allocator<std::pair<const CDefString, long> > >::find(const CDefString&) -- STLport 4.6 exact-key lookup returning the iterator by value (the header when absent)'
    for i, l in enumerate(L):
        if l.startswith('// Types: unproven'):
            L[i] = '// Types (naming wave 20, chunk 686): the map<CDefString, long> of CFlammableDef (+0x40): the landed GetEffectAtHelperPoint 0x007817F0 and'
            L[i + 1] = '//   GetAllFireHelpers 0x00781B90 walk the same tree (pair<const CDefString, long> at +0x10; CDefString = long TablePos, Ego_r.pdb); was the CUnproven* placeholders.'
        if l.startswith('// Leaf: ?find@?$_Rb_tree@UCUnprovenKey_00781670'):
            L[i] = '// Leaf: ?find@?$_Rb_tree@VCDefString@@U?$pair@$$CBVCDefString@@J@std@@'
    t = '\n'.join(L)
    he._w(p, t); print('ok', a, 'src')
    he.resub(a, 'tst', r'CUnprovenKey_00781670', 'CDefString')

a = '007e6ec0'
if a not in _X:
    T = open('/home/user/FableDecomp/rebuild/src/compiled/00/56/' + [f for f in os.listdir('/home/user/FableDecomp/rebuild/src/compiled/00/56') if f.endswith('_0056d050.cpp')][0], encoding='utf-8', errors='surrogateescape', newline='').read()
    T = T.replace('CThingFilter_HasScriptName', 'CThingFilter_HasDefName').replace('0x006A7E30', '0x006A7BE0').replace('0x0056D050', '0x007E6EC0')
    T = re.sub(r'(?m)^// Identity \(proven\):.*\n', '// Identity (proven, naming wave 20): the filter callee 0x006A7BE0 is the landed ??RCThingFilter_HasDefName@@QBE_NABVCThing@@@Z (CThing::GetDefName\n//   against the filter\'s CCharString); the CThing type mask (0, `xor al,al`); Ego_r.pdb ??$GetPFirstThing@VCThingFilter_HasDefName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasDefName@@PAPAVCThing@@@Z.\n//   Byte twin of the landed 0x0056D050 (HasScriptName) and 0x006D3FB0 (HasDefGlobalIndex); the landed caller CTCOracleMinigame::PlayGame 0x007E5D20\n//   named it with CThingFilter_HasTrackName (a CThingTrackNode filter in the PDB) and the catalog / the old row with CThingFilter_HasDefGlobalIndex\n//   (whose operator() 0x006A80A0 is the landed CThingFilter_HasTC) -- DECISIONS_W19 item 6 resolved.\n', T, count=1)
    T = T.replace('// Shape (family M136, representative 0x007E6EC0)', '// Shape (family M136, representative 0x0056D050)')
    he.put(a, 'src', T)
    he.resub(a, 'tst', r'CThingFilter_HasDefGlobalIndex', 'CThingFilter_HasDefName')

a = '007e5d20'
if a not in _X:
    he.edit(a, 'src', 'class CThingFilter_HasTrackName { public:\n    CCharString Name;\n    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }\n};\nclass CThingSearchTools { public:\n    bool GetPFirstThing(const CThingFilter_HasTrackName& filter, CThing** out) const;         // 0x007E6EC0 (const: Ego_r / FableWin)\n    bool GetPFirstSpeechThing(const CThingFilter_HasTrackName& filter, CThing** out) const;   // 0x0056D050 (second instantiation)\n};\n',
            'class CThingFilter_HasDefName { public:                     // ??0CThingFilter_HasDefName@@QAE@ABVCCharString@@@Z (Ego_r.pdb; inline here)\n    CCharString Name;\n    CThingFilter_HasDefName(const CCharString& name) : Name(name) { }\n};\nclass CThingFilter_HasScriptName { public:                  // ??0CThingFilter_HasScriptName@@QAE@ABVCCharString@@@Z (Ego_r.pdb; inline here)\n    CCharString Name;\n    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }\n};\nclass CThingSearchTools { public:\n    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;\n    // 0x007E6EC0 ??$GetPFirstThing@VCThingFilter_HasDefName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasDefName@@PAPAVCThing@@@Z (the four symbols, by def name)\n    // 0x0056D050 ??$GetPFirstThing@VCThingFilter_HasScriptName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasScriptName@@PAPAVCThing@@@Z (the gamesmaster, by script name)\n    // (Ego_r / FableWin.pdb member template; naming wave 20 -- both were named with CThingFilter_HasTrackName, a CThingTrackNode filter)\n};\n')
    he.resub(a, 'src', r'GetPFirstThing\(CThingFilter_HasTrackName\(name\)', 'GetPFirstThing(CThingFilter_HasDefName(name)', 4)
    he.edit(a, 'src', 'GetPFirstSpeechThing(CThingFilter_HasTrackName(name), &SpeechMarker)', 'GetPFirstThing(CThingFilter_HasScriptName(name), &SpeechMarker)')
    he.edit(a, 'src', '// GetPFirstThing<CThingFilter_HasTrackName, ...> @ 0x007E6EC0 (symbols) and the second instantiation @ 0x0056D050 (the\n// gamesmaster; manifest names both "<CThingFilter_HasDefGlobalIndex,CThing>"), CPlayerManager::GetMainPlayer @ 0x00449970,\n',
            '// GetPFirstThing<CThingFilter_HasDefName, CThing> @ 0x007E6EC0 (the four symbols, OBJECT_* def names) and GetPFirstThing<CThingFilter_HasScriptName, CThing>\n// @ 0x0056D050 (the gamesmaster, a script name; both landed under those leaves), CPlayerManager::GetMainPlayer @ 0x00449970,\n')
    he.resub(a, 'src', r'find the four symbol objects and the gamesmaster by track name', 'find the four symbol objects by def name and the gamesmaster by script name')
    he.resub(a, 'src', r'// Shape: each track name is a NAMED CCharString', '// Shape: each name is a NAMED CCharString')

a = '009e6530'
if a not in _X:
    he.edit(a, 'src', '// CCharString::FastCompare(const CCharString&) const -- cheap strict ordering for sorted containers: by block identity, then by length, then by the text compared one 32-bit word at a time\n// Leaf: ?FastCompare@CCharString\n',
            '// CCharString::operator<(const CCharString&) const -- cheap strict ordering for sorted containers: by block identity, then by length, then by the text compared one 32-bit word at a time\n// Leaf: ??MCCharString@@QBE_NABV0@@Z\n// Name (naming wave 20, chunk 686): FableWin.pdb ??MCCharString@@QBE_NABV0@@Z -- the CCompareString comparator of the landed chunk-686 sort rows\n//   (__introsort_loop 0x00CBDAD0, __unguarded_linear_insert 0x00CBA9B0 ...) and of CVectorMap<CCharString, ...>::LowerBound 0x00CBA1F0 calls it as\n//   `a.first < b.first`; it is not CBasicString<char>::operator< 0x00429950 (the lexicographic one). Was the descriptive FastCompare.\n')
    he.resub(a, 'src', r'\bFastCompare\b', 'operator<')
    he.resub(a, 'tst', r'\bFastCompare\b', 'operator<')

a = '00ac99f0'
if a not in _X:
    he.put(a, 'src', '''// QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups(const std::map<unsigned long, unsigned long>& remap) -- remap the smoothing-group ids of every vertex through the given table
// Leaf: ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC99F0, length 104 bytes (padded to 112).
// Name (naming wave 20, chunk 686): Ego_r.pdb ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z,
//   the spelling the landed caller COptimisedMesh::RemoveBones 0x00ACA0C0 binds; the callee is the landed (chunk 686)
//   ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KK...@Z 0x00AC97D0 (`this` is the vertex, one stack argument).
//   Was the un-namespaced COptimisedPrimitive::RemapGroups(const CGroupRemap&) over a CRetailVector stand-in.
// Layout: retail COptimisedPrimitive::Vertices is a 12-byte STLport vector at +0x10 of 0xb8-byte COptimisedVertex records
//   (the `imul 0xb21642c9 ; sar 7` division by 0xb8).
// Shape: `for (unsigned long i = 0; i < Vertices.size(); ++i)` -- size() re-evaluated every pass (signed pointer
//   difference / sizeof, unsigned `jb` compare), the element address strength-reduced to a running byte offset.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:   // STLport 4.6 vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    _Tp& operator[](unsigned long __n) { return *(_M_start + __n); }
};
}
namespace QuadricOptimiserInternals {
class COptimisedVertex { public:                // retail 0xb8 bytes
    unsigned char _body[0xb8];
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap);     // 0x00AC97D0
};
class COptimisedPrimitive { public:
    long                              MaterialIndex;      // +0x00
    unsigned char                     UniqueVertices[0xc];// +0x04
    std::vector<COptimisedVertex>     Vertices;           // +0x10
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap);
};

void COptimisedPrimitive::RemapGroups(const std::map<unsigned long, unsigned long>& remap)
{
    for (unsigned long i = 0; i < Vertices.size(); ++i)
        Vertices[i].RemapGroups(remap);
}
}
''')
