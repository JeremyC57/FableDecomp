# survey_cthing2.py ROWS.tsv IDX: feature stats over the local-CThing rows (what the migration rewriter must handle):
# local stand-ins that clash with engine/CThing.h's include closure, the GetTC call forms used, the module (CThing's own methods
# or another class), and the index's own symbol / LowerBound spelling per row. Prints counters and writes ROWS.features.tsv.
import sys,re,collections,json
sys.path.insert(0,'/home/user/FableDecomp/rebuild/candidates/manual/naming/2026-11-03_wave33/scripts')
from common import ps1_entries, ROOT
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1])][1:]; IDX=sys.argv[2]
ent=ps1_entries(); cache=json.load(open(IDX+'/CACHE.json'))
F=collections.OrderedDict([
 ('local_enum_ETCInterfaceType',r'\benum\s+ETCInterfaceType\b'),('inc_ETCInterfaceType_h',r'engine/ETCInterfaceType\.h'),
 ('inc_CVectorMap_h',r'engine/CVectorMap\.h'),('local_CInterfaceMap_class',r'(class|struct)\s+CInterfaceMap\b[^;]*\{'),
 ('local_CClass',r'(class|struct)\s+CClass\b[^;]*\{'),('local_CCharString',r'(class|struct)\s+CCharString\b[^;]*\{'),
 ('local_CDefPointer',r'(class|struct)\s+CDefPointer\b'),('local_CBaseObject',r'(class|struct)\s+CBaseObject\b[^;]*\{'),
 ('local_CTCBase',r'(class|struct)\s+CTCBase\b[^;]*\{'),('inc_CTCBase_h',r'engine/CTCBase\.h'),
 ('INTERFACE_TYPE_enum',r'\bINTERFACE_TYPE\s*='),('StatGetInterfaceType',r'StatGetInterfaceType'),('TCI_names',r'\bTCI_[A-Z_]+\b'),
 ('call_GetTC_tpl',r'GetTC\s*<'),('call_GetTC_addr',r'GetTC\s*\(\s*&'),('call_GetInterface',r'GetInterface\s*(<|\()'),('call_PeekTC',r'PeekTC\s*(<|\()'),
 ('call_HasTC',r'HasTC\s*\('),('bit_test_presentBits',r'(m_presentBits|UsingComponents|m_tcMask|m_present|Interfaces)\s*\[[^\]]*>>\s*5'),
 ('bitset_member',r'bitset<274>'),('flags91',r'(m_flags91|Flags91|StateBits|m_flags)\b'),('Dead_bitfield',r'\bDead\s*:\s*1'),
 ('pragma_pack',r'#pragma\s+pack'),('CIntelligentPointer_CThing',r'CIntelligentPointer\s*<\s*(const\s+)?CThing\s*>'),
 ('inc_CIntelligentPointer_h',r'engine/CIntelligentPointer\.h'),('local_CBaseIntelligentPointer',r'(class|struct)\s+CBaseIntelligentPointer\b[^;]*\{'),
 ('Revisit_marker',r'// Revisit:'),('LowerBound_decl',r'LowerBound\s*\('),('inline_depth',r'inline_depth'),
])
C=collections.Counter(); mods=collections.Counter(); own=collections.Counter(); lb=collections.Counter()
o=open(sys.argv[1].replace('.tsv','.features.tsv'),'w'); o.write('address\tmodule\town_symbol_class\t'+'\t'.join(F)+'\n')
for r in rows:
    if r[8]!='local': continue
    a=r[0]; txt=open(ROOT+'/rebuild/src/compiled/'+r[1],errors='replace').read()
    f=[('1' if re.search(p,txt) else '') for p in F.values()]
    for k,v in zip(F,f):
        if v: C[k]+=1
    m=ent[a].get('Module',''); mods[m]+=1
    e=cache.get(a,{}); s=e.get('sym','')
    oc=re.search(r'@([A-Za-z_0-9]+)@@',s or ''); oc=oc.group(1) if oc else ''
    own['CThing' if oc=='CThing' else 'other']+=1
    for b in e.get('binds',[]):
        if 'LowerBound' in b[0]: lb[re.sub(r'@.*','',b[0][:40])+'...'+b[1]]+=1
    o.write('\t'.join([a,m,oc]+f)+'\n')
print('features:'); [print('  %5d %s'%(n,k)) for k,n in C.items()]
print('modules top:',mods.most_common(25)); print('own symbol class:',own); print('LowerBound binds:',lb.most_common(8))
