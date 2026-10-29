# wave 5 hand edits (exact single replacements; HE_OV=<overlay>): rows that already model the CVectorMap template but
# emit a non-final LowerBound spelling. Final (DECISIONS.tsv, proven, STLport 4.6):
#   0x0040F020 ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
#   0x004365B0 ...@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
# 005943a2: two template parameters (the comparator is missing) and a non-const result of the const member
#           -> CKeyPairCompareLess<K, V> as the defaulted third parameter, `const value_type*`.
# 008dd6e0: the entry is a nested `struct Entry {K first; V second;}` -> `typedef std::pair<K,V> Entry;` (source+test:
#           the test defines the template LowerBound out of line).
# 00896df0: the entry is a `CKeyPair<K,V>` template and the key a pointer -> std::pair and `const TKey&`
#           (source; the test is a self-contained copy).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
PAIR = 'namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>\n'
edit('005943a2', 'src', 'template <class K, class V> class CVectorMap {\n',
     'template <class K, class V> class CKeyPairCompareLess;\n'
     'template <class K, class V, class TCompare = CKeyPairCompareLess<K, V> > class CVectorMap {\n')
edit('005943a2', 'src', '    value_type* LowerBound(const K& key) const;      // 0x004365B0\n',
     '    const value_type* LowerBound(const K& key) const;   // 0x004365B0\n')
for k in ('src', 'tst'):
    edit('008dd6e0', k, 'enum ETCInterfaceType {TC_SOUND_PLAYER=16};\ntemplate<class K,class V> class CKeyPairCompareLess { public:};\n',
         'enum ETCInterfaceType {TC_SOUND_PLAYER=16};\n' + PAIR + 'template<class K,class V> class CKeyPairCompareLess { public:};\n')
    edit('008dd6e0', k, '    struct Entry {K first;V second;};\n', '    typedef std::pair<K,V> Entry;\n')
edit('00896df0', 'src', 'template <typename TKey, typename TValue>\nstruct CKeyPair\n{\n    TKey m_Key;\n    TValue m_Value;\n};\n\n'
     'typedef CKeyPair<ETCInterfaceType, CTCBase*> KickOffCreditsScreenPair;',
     PAIR + '\ntypedef std::pair<ETCInterfaceType, CTCBase*> KickOffCreditsScreenPair;')
edit('00896df0', 'src', '    CKeyPair<TKey, TValue>* m_pBegin;\n    CKeyPair<TKey, TValue>* m_pEnd;\n\n    CKeyPair<TKey, TValue>* LowerBound(const TKey* pKey);',
     '    std::pair<TKey, TValue>* m_pBegin;\n    std::pair<TKey, TValue>* m_pEnd;\n\n    std::pair<TKey, TValue>* LowerBound(const TKey& key);')
edit('00896df0', 'src', 'map->LowerBound(&interfaceType);', 'map->LowerBound(interfaceType);')
edit('00896df0', 'src', '(ETCInterfaceType)0x101 < entry->m_Key)', '(ETCInterfaceType)0x101 < entry->first)')
edit('00896df0', 'src', 'creditsUI = (CTCCreditsUI*)entry->m_Value;', 'creditsUI = (CTCCreditsUI*)entry->second;')
