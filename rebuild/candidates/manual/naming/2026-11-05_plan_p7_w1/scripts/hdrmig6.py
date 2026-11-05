# hdrmig6.py W [addr...|@file] [--limit N] : the phase 6 CThing migration rewriter (plan agent, 2026-11-05).
# For every landed row that defines CThing locally it writes W/overlay/<row>.cpp with the local model replaced by
# `#include "engine/CThing.h"` (plus the FABLE_RETAIL_CTHING* defines the row's member uses need), or records why the row is
# deferred (W/hdrmig.tsv: address, status, note). The gate decides the bytes; this script only rewrites text:
#   1. the local `class|struct CThing [: bases] { ... };` block and `class CThing;` forward declarations go;
#   2. local models of classes the header's include closure defines go too (CClass, CBaseObject, CBaseClass, CCharString, CWideString,
#      CDefPointer, CDefPointeeBase, CVectorMap, CKeyPairCompareLess, CArray, ETCInterfaceType, a local CInterfaceMap class ->
#      typedef of the header's instance); a local `namespace std {` model defers the row (local_std);
#   3. the stand-in lookup calls are respelled to the header's members: GetInterface*(&v, N) / GetTC(N, &v) -> GetTC(TCI_X, &v)
#      (PeekTC when the local stand-in was const), GetTC<T>(N) returning T* -> (T*)&x->GetTC(TCI_X), HasTC / HasInterface(N) ->
#      HasTC(TCI_X), raw UsingComponents-word tests -> HasTC(TCI_X); interface numbers become the Ego_r enumerators of
#      engine/ETCInterfaceType.h (a local enumerator is mapped by its value);
#   4. member reads of another class's CThing stand-in become the header's public accessors (field_map / accessor_map by offset:
#      +0x10 -> GetType() (CClass::Type, engine/CClass.h), +0x91 bit 0 -> IsDead(), +0x6C -> GetTypeFlags(), +0x60 -> GetPhysicsTC(), +0x18 -> GetUniqueID(), +0x64 ->
#      GetTCGraphicAppearance(), +0x8C -> GetDefGlobalIndex(), +0x8E -> GetCurrentMapNumber(), +0x90 -> GetPlayerNumber());
#      inside CThing's own methods the members are respelled to the header's names instead;
#   5. a local method the header lacks defers the row (header_lacks: listed for NEXT.md); a local `operator new(size_t, bool)`
#      (CThing::operator new 0x004C7380, not in the header) defers it too; the CThingSearchTools search templates (a different
#      inline shape of IsIncludedInThingScans) are deferred as searchtools_template.
#   5b. the lookup's const-ness (GetTC v PeekTC) follows the index: a row binding only 0x004365B0 (CVectorMap::LowerBound const)
#      spells PeekTC, only 0x0040F020 GetTC; a const receiver then needs its out variable const (const T* v).
#   6. a derived local stand-in `class D : public CThing { unsigned char _pad[N]; ... }` is re-based: members below 0x98 are
#      CThing's own (read through the header's accessors), the pad above shrinks by sizeof(CThing); the `// Leaf:` line is
#      corrected to the index's symbol when only the mangling differs (the gate and vland grade the largest function otherwise);
#      the FABLE_RETAIL_* defines of the header's include closure (STLport, strings, CVectorMap, CDefPointer, CClass) are hoisted
#      above the include; a removed local CCharString / CWideString model with the retail constructors asks for
#      FABLE_RETAIL_CCHARSTRING / FABLE_RETAIL_CWIDESTRING; a global local CTCGraphicAppearance stand-in moves into
#      namespace NGraphicAppearance (the PDB's; the header's GetTCGraphicAppearance returns that type).
import sys,os,re,collections,json
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; args=sys.argv[2:]; limit=None
if '--limit' in args: i=args.index('--limit'); limit=int(args[i+1]); del args[i:i+2]
addrs=[]
for a in args: addrs+=[l.split()[0] for l in open(a[1:]) if l.strip() and not l.startswith('address')] if a.startswith('@') else [a]
# --- the header's enumerators (value -> TCI name)
TCI={}; TCI_BY_NAME={}
for m in re.finditer(r'\b(TCI_\w+)\s*=\s*(\d+)',L.read(L.ROOT+'/rebuild/include/engine/ETCInterfaceType.h')): TCI[int(m.group(2))]=m.group(1); TCI_BY_NAME[m.group(1)]=int(m.group(2))
# --- header members and the sub-macro each needs
MACRO_OF={'GetPlayer':'FABLE_RETAIL_CTHING','PeekComponent':'FABLE_RETAIL_CTHING','SetAsRenderedThisFrame':'FABLE_RETAIL_CTHING','SetAsActivated':'FABLE_RETAIL_CTHING',
 'Alloc':'FABLE_RETAIL_CTHING','GetTypeFlags':'FABLE_RETAIL_CTHING','GetTCGraphicAppearance':'FABLE_RETAIL_CTHING','GetPlayerNumber':'FABLE_RETAIL_CTHING',
 'SetInLimbo':'FABLE_RETAIL_CTHING','HasScriptThing':'FABLE_RETAIL_CTHING','SetPersistOnUnloadWhenInLimbo':'FABLE_RETAIL_CTHING','PeekTCDriver':'FABLE_RETAIL_CTHING',
 'PeekPlayer':'FABLE_RETAIL_CTHING_PEEKPLAYER','IsUnderScriptSupervision':'FABLE_RETAIL_CTHING_SCRIPTSUPERVISION',
 'GetPCurrentThingManager':'FABLE_RETAIL_CTHING_MANAGERACCESS','PeekPCurrentThingManager':'FABLE_RETAIL_CTHING_MANAGERACCESS','GetThingSearchTools':'FABLE_RETAIL_CTHING_MANAGERACCESS',
 'PeekThingSearchTools':'FABLE_RETAIL_CTHING_MANAGERACCESS','CheckUID':'FABLE_RETAIL_CTHING_MANAGERACCESS','PCurrentThingManager':'FABLE_RETAIL_CTHING_MANAGERACCESS',
 'AddAndCopyTC':'FABLE_RETAIL_CTHING_ADDANDCOPYTC','GetTCDriver':'FABLE_RETAIL_CTHING_GETTCDRIVER','ConstructFromLevelScript':'FABLE_RETAIL_CTHING_LEVELSCRIPT',
 'GetWorldMap':'FABLE_RETAIL_CTHING_LEVELSCRIPT','StatGetTypeFlags':'FABLE_RETAIL_CTHING_STATTYPEFLAGS'}
for _n in ('AddToRenderList','GetScriptThing','SetAsGameStatePersisted','IsGameStatePersisted','IsLevelPersistent','GetDefName','SetPlayerNumber','SetCurrentMapNumber',
           'SetForceFrameUpdate','DoActivationSequence','RemoveTC','IsAffectedByBulletTime','IsInLimbo','IsActivated','IsDying','IsKillLocked','IsGloballyLoaded','IsDeathPersistent',
           'UpdateThingScanInclusion','PeekPlayerManager'): MACRO_OF[_n]='FABLE_RETAIL_CTHING_STATEACCESS'
NESTED={'FABLE_RETAIL_CTHING_STATEACCESS','FABLE_RETAIL_CTHING_PEEKPLAYER','FABLE_RETAIL_CTHING_SCRIPTSUPERVISION','FABLE_RETAIL_CTHING_MANAGERACCESS','FABLE_RETAIL_CTHING_ADDANDCOPYTC','FABLE_RETAIL_CTHING_GETTCDRIVER','FABLE_RETAIL_CTHING_LEVELSCRIPT','FABLE_RETAIL_CTHING_STATTYPEFLAGS'}
HDR_MEMBERS=set(MACRO_OF)|{'Kill','AddTC','GetPBaseDef','PeekBaseDef','GetDefGlobalIndex','GetUniqueID','GetCurrentMapNumber','IsLevelLess','IsDead','IsIncludedInThingScans','IsAlive','HasTC','GetTC','PeekTC',
 'GetPhysicsTC','CheckRetailLayout','GetPos','GetPhysPos','GetWorld','PeekWorld','GetPWorldSeed','GetPlayerManager','UniqueID','UsingComponents','Components','UpdateComponentsList','SerialisationSectionName','PTCDriver','PhysicsTC',
 'PTCGraphicAppearance','WorldFrameCreatedAt','ThingTypeFlags','PBaseDef','ScriptName','ScriptData','RenderListPrev','RenderListNext','UpdateListPrev','UpdateListNext','DefGlobalIndex','CurrentMapNumber','PlayerNumber',
 'Dead','CreatedByOwnerThingCreation','IncludeInThingScans','Activated','GloballyLoaded','InLimbo','KillLocked','CreatedBySerialisation','DeletedOnOwnerLevelOutOfScope','Drawable','Dying','ForceFrameUpdate','LevelPersistent',
 'GameStatePersisted','PersistOnUnloadWhenInLimbo','PausedDuringCutscenes','NotPausedDuringCutscenes','DeathPersistent','KillOnLevelUnload','ActivatedScript','LevelIsUnloading','IsAffectedByBulletTimeOverride','AddingTCsPermitted','InitialisationStage'}
# local data-member names by retail offset -> header member (own-method rows) and the public accessor (other rows)
FIELD={0x10:('Type','GetType()'),0x18:('UniqueID','GetUniqueID()'),0x20:('UsingComponents',None),0x44:('Components',None),0x54:('UpdateComponentsList',None),0x58:('SerialisationSectionName',None),
 0x5c:('PTCDriver',None),0x60:('PhysicsTC','GetPhysicsTC()'),0x64:('PTCGraphicAppearance','GetTCGraphicAppearance()'),0x68:('WorldFrameCreatedAt',None),0x6c:('ThingTypeFlags','GetTypeFlags()'),
 0x70:('PBaseDef',None),0x74:('ScriptName',None),0x78:('ScriptData',None),0x7c:('RenderListPrev','RenderListPrev'),0x80:('RenderListNext','RenderListNext'),0x84:('UpdateListPrev','UpdateListPrev'),0x88:('UpdateListNext','UpdateListNext'),
 0x8c:('DefGlobalIndex','GetDefGlobalIndex()'),0x8e:('CurrentMapNumber','GetCurrentMapNumber()'),0x90:('PlayerNumber','GetPlayerNumber()')}
# +0x91.. bit -> the public accessor of another class's row (None: no accessor in the header)
BITACC={(0x91,0):'IsDead()',(0x91,3):'IsActivated()',(0x91,4):'IsGloballyLoaded()',(0x91,5):'IsInLimbo()',(0x91,6):'IsKillLocked()',(0x92,2):'IsDying()',(0x92,4):'IsLevelPersistent()',(0x92,5):'IsGameStatePersisted()',(0x93,1):'IsDeathPersistent()'}
BITS91=['Dead','CreatedByOwnerThingCreation','IncludeInThingScans','Activated','GloballyLoaded','InLimbo','KillLocked','CreatedBySerialisation',
 'DeletedOnOwnerLevelOutOfScope','Drawable','Dying','ForceFrameUpdate','LevelPersistent','GameStatePersisted','PersistOnUnloadWhenInLimbo','PausedDuringCutscenes',
 'NotPausedDuringCutscenes','DeathPersistent','KillOnLevelUnload','ActivatedScript','LevelIsUnloading','IsAffectedByBulletTimeOverride','AddingTCsPermitted']
# well-known stand-in names (no offset comment needed)
KNOWN={'m_presentBits':0x20,'PresentBits':0x20,'m_tcMask':0x20,'m_present':0x20,'UsingComponents':0x20,'Interfaces':0x44,'m_interfaces':0x44,'Components':0x44,'m_44':0x44,'m_tcs':0x44,
 'm_flags91':0x91,'Flags91':0x91,'DeadFlags':0x91,'StateBits':0x91,'LifeFlags':0x91,'m_stateFlags':0x91,'ThingTypeFlags':0x6c,'m_flags6c':0x6c,'PhysicsTC':0x60,'m_pPhysics':0x60,'PPhysics':0x60,'Physics':0x60,
 'UniqueID':0x18,'UID':0x18,'PTCGraphicAppearance':0x64,'PGraphicAppearance':0x64,'DefGlobalIndex':0x8c,'DefIndex':0x8c,'PlayerNumber':0x90,'CurrentMapNumber':0x8e,'PTCDriver':0x5c,'PBaseDef':0x70,'PDef':0x70,'m_pDef':0x70,
 'ScriptName':0x74,'ScriptData':0x78,'Dead':'bit0','m_dead':'bit0','InLimbo':'bit5','Activated':'bit3','IncludeInThingScans':'bit2'}
CLOSURE_SIZE={'CBaseObject':0x8,'CClass':0x14,'CBaseClass':0x4,'CDefPointeeBase':0x1c,'CIVCountedPointeeBase':0x8,'CResource':0x1c,'CCharString':4,'CWideString':4}
CLOSURE=['CClass','CBaseObject','CBaseClass','CCharString','CWideString','CDefPointer','CDefPointeeBase','CVectorMap','CKeyPairCompareLess','CArray','CCharStringData','CWideStringData','CIVCountedPointeeBase','CResource']
RX_THING=L.class_rx('CThing')
def strip_block(t,b,s):
    """remove the class block starting at the `{` offset s (and its `;` + trailing same-line comment); returns new text"""
    blk=L.block(t,s,b); k=t.find(';',s+len(blk)); k=k+1 if k>=0 and t[s+len(blk):k].strip()=='' else s+len(blk)
    e=t.find('\n',k); e=len(t) if e<0 else e
    if t[k:e].strip() and not t[k:e].strip().startswith('//'): e=k
    return t[:s]+t[e:]
def remove_class(t,name,template_ok=True):
    """remove every local definition block of class `name` (with any template<> head / `#pragma pack` on the head line left alone)"""
    n=0
    while True:
        b=L.blank_comments(t)
        m=re.search(r'(?<![\w:])(?:template\s*<(?:[^<>]|<[^<>]*>)*>\s*)?(?:(?:class|struct)\s+__declspec\([^)]*\)|class|struct)\s+'+name+r'\b\s*(?:<[^>]*>\s*)?(?::\s*[^{;]*)?\{',b)
        if not m: return t,n
        head=m.start(); s=b.find('{',m.start())
        blk=L.block(t,s,b); k=t.find(';',s+len(blk)); k=k+1 if k>=0 and t[s+len(blk):k].strip()=='' else s+len(blk)
        e=t.find('\n',k); e=len(t) if e<0 else e
        if t[k:e].strip() and not t[k:e].strip().startswith('//'): e=k
        t=t[:head]+'@@CUT@@'+t[e:]; n+=1
def receiver(code,pos):
    """the primary expression that ends at pos (x, x->y, a.b(c)[d], (expr)) : returns its start offset"""
    i=pos; depth=0
    while i>0:
        c=code[i-1]
        if c in ')]':
            depth+=1; i-=1; continue
        if c in '([':
            if depth==0: break
            depth-=1; i-=1; continue
        if depth>0: i-=1; continue
        if c.isalnum() or c=='_' : i-=1; continue
        if code.startswith('->',i-2) : i-=2; continue
        if c=='.' and i>=2 and (code[i-2].isalnum() or code[i-2] in '_)]'): i-=1; continue
        if c=='*' and i>=2 and code[i-2]=='(' : i-=1; continue   # (*p).x
        break
    while i<pos and code[i] in ' \t': i+=1
    return i
class Defer(Exception): pass
def bit_reads(code,n,off):
    """another class's reads of the +0x91 flag byte stand-in n (`X->n & MASK`, `(X->n & MASK)`, bit-field `X->n`) -> the header's
    accessors (receiver found by receiver(): calls and casts included); returns (code, names_left)"""
    out=[]; i=0; left=0
    for mm in re.finditer(r'(->|\.)\s*'+re.escape(n)+r'\b(?!\s*\()',code):
        if mm.start()<i: continue
        rs=receiver(code,mm.start()); recv=code[rs:mm.start()]; j=mm.end()
        ms=re.match(r'\s*&\s*(0x[0-9a-fA-F]+|\d+)\b(?!\s*[&|<>])',code[j:])
        acc=None; e=j
        if ms:
            v=int(ms.group(1),0); bit=v.bit_length()-1
            if v==1<<bit: acc=BITACC.get((0x91,bit)); e=j+ms.end()
        elif off=='bit0' or isinstance(off,str): acc='IsDead()' if off=='bit0' else BITACC.get((0x91,int(off[3:])))
        if acc is None: left+=1; continue
        s0=rs
        if code[rs-1:rs]=='(' and re.match(r'\s*\)',code[e:]): s0=rs-1; e=e+re.match(r'\s*\)',code[e:]).end()
        out.append(code[i:s0]); out.append(recv+mm.group(1)+acc); i=e
    out.append(code[i:]); return ''.join(out),left
def tci_of(expr,enum_map,row_enums,glob_enums={}):
    """an interface-type expression -> the header enumerator text, or None"""
    e=expr.strip()
    m=re.match(r'^\(\s*ETCInterfaceType\s*\)\s*\(?\s*(.+?)\s*\)?$',e)
    if m: e=m.group(1).strip()
    if re.match(r'^(0x[0-9a-fA-F]+|\d+)[uUlL]*$',e):
        v=int(e.rstrip('uUlL'),0); return TCI.get(v)
    if e in TCI_BY_NAME: return e
    if e in enum_map: return TCI.get(enum_map[e])
    if e in glob_enums: return TCI.get(glob_enums[e])
    m=re.match(r'^(\w+)::(\w+)$',e)
    if m and (m.group(1),m.group(2)) in row_enums: return TCI.get(row_enums[(m.group(1),m.group(2))])
    if m and m.group(2)=='StatGetInterfaceType': return e
    return None
def migrate(a):
    src=L.src_rel(a); t=L.read(L.ROOT+'/'+src); orig=t; notes=[]; macros=set()
    if re.search(r'^// Revisit:',t,re.M): raise Defer('revisit_marker')
    if re.search(r'^\s*#\s*define\s+CThing\b',t,re.M): raise Defer('quarantine_model: #define CThing <alias> include')
    if re.search(r'^\s*#\s*define\s+(CClass|CBaseObject|CBaseClass|CCharString|CWideString|CVectorMap|CDefPointer|CDefPointeeBase|CArray)\b',t,re.M): raise Defer('quarantine_model: #define <closure class> alias')
    _isym=L.cache().get(a,{}).get('sym') or ''
    if re.match(r'\?\?\$(GetInterface|GetTC|PeekTC|GetTCConst|GetInterfaceConst)\w*@',_isym): raise Defer('template_instance_row (the row is a GetTC<T> instantiation body)')
    b=L.blank_comments(t); blocks=[m for m in RX_THING.finditer(b)]
    if len(blocks)!=1: raise Defer('blocks=%d'%len(blocks))
    m=blocks[0]; s=b.find('{',m.start()); blk=L.block(t,s,b); bases=(m.group(2) or '').strip()
    # pack region?
    pre=b[:m.start()]
    if pre.count('pack(push')>pre.count('pack(pop'): raise Defer('pack_region')
    if re.search(r'\bnamespace\s+std\s*\{',b): raise Defer('local_std')
    if L.namespace_at(t,m.start(),b): raise Defer('nested_in_namespace '+'::'.join(L.namespace_at(t,m.start(),b)))
    already='engine/CThing.h' in t
    # --- members of the local block
    for bn in re.findall(r'\b(?:public|protected|private)?\s*(\w+)',bases):
        if bn in ('public','protected','private','virtual'): continue
        if bn not in ('CClass','CBaseObject','CBaseClass','CBaseClassNonCopyable'):
            mb=L.class_rx(bn).search(b)
            if mb and re.search(r'\w\s*;',L.blank_comments(L.block(t,b.find('{',mb.start()),b))): raise Defer('local_base '+bn+' (CThing modelled on a local base with members)')
    if re.search(r'\b(CInterfaceMap|CVectorMap\s*<[^;{]*>)\s*::\s*(LowerBound|Find|end|begin)\s*\(',b): raise Defer('own_definition CVectorMap member (same-TU body)')
    if re.search(r'^//\s*Leaf:\s*\?\?\$(GetInterface|GetTC|PeekTC|GetTCConst|GetInterfaceConst)\w*@',t,re.M): raise Defer('template_instance_row (the row is a GetTC<T> instantiation body)')
    if re.search(r'(?<![\w])(class|struct)\s+CDefPointer\s*(:[^{;]*)?\{',b) and not re.search(r'template\s*<[^>]*>\s*(class|struct)\s+CDefPointer\b',b): raise Defer('closure_model CDefPointer (non-template local stand-in)')
    inner=blk[1:-1]; members={}; methods={}; own_enum={}; has_ctor=False; depth=0; mtypes={}
    def statements(inner):
        out=[]
        for line in inner.split('\n'):
            code=line.split('//')[0]; cm=line[len(code):]
            if code.count(';')<=1: out.append(line); continue
            pieces=[]; cur=''; d=0
            for ch in code:
                cur+=ch
                if ch=='{': d+=1
                elif ch=='}': d-=1
                if d==0 and ch in ';}': pieces.append(cur); cur=''
            if cur.strip(): pieces.append(cur)
            for k,pc in enumerate(pieces): out.append(pc+(cm if k==len(pieces)-1 else ''))
        return out
    for line in statements(inner):
        code=line.split('//')[0]; off=re.search(r'//\s*\+?(0x[0-9a-fA-F]+)\b',line)
        d0=depth; depth+=code.count('{')-code.count('}')
        if d0>0 and depth>0: continue   # inside an inline method body
        for mt in re.finditer(r'(?<![\w:])((?:\w+::)*\w+)\s*\*\s*(\w+)\s*;',code): mtypes[mt.group(2)]=mt.group(1)
        if re.search(r'\bCThing\s*\([^)]*\)\s*[;{:]',code) and not re.search(r'\b(operator|new)\b',code): has_ctor=True
        offv=int(off.group(1),16) if off else None
        for mm in re.finditer(r'\b([A-Za-z_]\w*)\s*(\[[^\]]*\]\s*)*(:\s*\d+\s*)?[;,=]',re.sub(r'\b(public|private|protected)\s*:','',code)):
            n=mm.group(1)
            if n in ('const','virtual','static','inline','return','else','enum','unsigned','signed','char','short','long','int','bool','float','double','void','true','false','struct','class','typedef','this'): continue
            if '(' in code.split(n)[0] and not re.search(r'\)\s*(const)?\s*(=\s*0)?\s*;',code): continue
            members.setdefault(n,offv)
        for mm in list(re.finditer(r'\b([A-Za-z_]\w*)\s*\([^()]*\)\s*(const)?\s*(=\s*0)?\s*[;{]',code))+list(re.finditer(r'\b([A-Za-z_]\w*)\s*\([^()]*\)\s*(const)?()\s*$',code.rstrip())):
            n=mm.group(1)
            if n in ('if','for','while','switch','return','sizeof','CThing','__declspec','offsetof','FABLE_STATIC_ASSERT'): continue
            methods[n]=bool(mm.group(2))
    own=bool(re.search(r'\bCThing::\w+\s*\(',b[s+len(blk):])) or bool(re.search(r'\bCThing::\w+\s*\(',b[:m.start()]))
    # --- local enumerators of ETCInterfaceType / INTERFACE_TYPE enums of local TC stand-ins
    enum_map={}; glob_enums={}
    for em in re.finditer(r'\benum\s+ETCInterfaceType\s*\{([^}]*)\}',re.sub(r'//[^\n]*','',t)):
        for x in re.finditer(r'\b(\w+)\s*=\s*(0x[0-9a-fA-F]+|\d+)',em.group(1)): enum_map[x.group(1)]=int(x.group(2),0)
    row_enums={}
    for em in re.finditer(r'(?<![\w])enum\s*(?:\w+\s*)?\{([^}]*)\}\s*;',re.sub(r'//[^\n]*','',b[:m.start()]+b[s+len(blk):])):
        for x in re.finditer(r'\b(\w+)\s*=\s*(0x[0-9a-fA-F]+|\d+)',em.group(1)):
            if re.search(r'\b(TC|TCI|INTERFACE|Interface|kTC|TYPE)',x.group(1)) and x.group(1) not in enum_map: glob_enums[x.group(1)]=int(x.group(2),0)
    for cm in re.finditer(r'(?<![\w:])(?:class|struct)\s+(\w+)\b[^;{]*\{',b):
        cb=L.block(t,b.find('{',cm.start()),b)
        for x in re.finditer(r'\b(INTERFACE_TYPE|InterfaceType|TYPE|Type|kInterfaceType)\s*=\s*(0x[0-9a-fA-F]+|\d+)',cb): row_enums[(cm.group(1),x.group(1))]=int(x.group(2),0)
    if re.search(r'\boperator\s+new\s*\([^)]*,\s*bool\b',blk) or re.search(r'\bnew\s*\(\s*(true|false)\s*\)',b): raise Defer('header_lacks operator_new(size_t,bool)')
    if 'engine/CThingSearchTools.h' in t and re.search(r'\bIsThingOfType\b|\bThingTypeMask\b',b): raise Defer('searchtools_template (IsIncludedInThingScans shape)')
    if has_ctor and re.search(r':\s*CThing\s*\(|\bnew\s+CThing\b|\bCThing\s+\w+\s*[;(]',b[s+len(blk):]): raise Defer('header_lacks ctor')
    for mm in re.finditer(r'\bCThing::(\w+)\s*(<[^>]*>)?\s*\(',b):
        if mm.group(1) in ('GetTCDriver','PeekTCDriver','GetTC','PeekTC','HasTC','IsDead','IsAlive','GetPhysicsTC','GetUniqueID','GetDefGlobalIndex','GetCurrentMapNumber','GetPBaseDef','GetTypeFlags','GetTCGraphicAppearance','GetPlayerNumber','SetPersistOnUnloadWhenInLimbo','IsIncludedInThingScans','GetPCurrentThingManager','PeekPCurrentThingManager','StatGetTypeFlags') and b[mm.end():mm.end()+200].find('{')>=0 and b[mm.end():mm.end()+200].find('{')<b[mm.end():mm.end()+200].find(';') : raise Defer('own_definition_of_header_inline '+mm.group(1))
    # the pointer members' local stand-in types take the header's names (the PDB's): PhysicsTC +0x60 CTCPhysicsBase,
    # PTCGraphicAppearance +0x64 NGraphicAppearance::CTCGraphicAppearance, PTCDriver +0x5C CTCDriverBase
    renames={}
    for n,ty in mtypes.items():
        off=members.get(n); off=KNOWN.get(n,off) if off is None else off
        want={0x60:'CTCPhysicsBase',0x64:'CTCGraphicAppearance',0x5c:'CTCDriverBase'}.get(off)
        tyb=ty.split('::')[-1]
        if want and tyb!=want and ty not in ('void','char','unsigned') and not re.search(r'(?<![\w])(class|struct)\s+'+want+r'\b',b): renames[tyb]=want
    for old,new in renames.items(): t=re.sub(r'(?<![\w])'+re.escape(old)+r'\b',new,t); b=L.blank_comments(t)
    if renames:
        blocks=[m for m in RX_THING.finditer(b)]; m=blocks[0]; s=b.find('{',m.start()); blk=L.block(t,s,b)
    # --- 1. the block goes; the include comes where it stood
    rest=t[:m.start()]+'@@CUT@@'+strip_block(t,b,s)[s:]
    rest=re.sub(r'(?m)^[^\n]*\boperator new\s*\(\s*(unsigned int|size_t|unsigned long)\s*,\s*void\s*\*[^\n]*\n','',rest)
    rest=re.sub(r'(?m)^[^\n]*\boperator delete\s*\(\s*void\s*\*\s*,\s*void\s*\*[^\n]*\n','',rest)
    # a local GetPlayer declared at 0x004C7A10 is PeekPlayer (the catalog / both PDBs); respell the calls
    if re.search(r'\bGetPlayer\s*\([^)]*\)[^\n]*0x004[cC]7[aA]10',blk):
        rest=re.sub(r'(->|\.)\s*GetPlayer\s*\(',r'\1PeekPlayer(',rest); methods.pop('GetPlayer',None); methods['PeekPlayer']=True
    rest=re.sub(r'\bstruct\s+CThing\s*;','class CThing;',rest)
    # --- 2. local models of the closure classes
    removed=[]
    if re.search(r'\bCCharString\s*\(\s*const\s+char\s*\*\s*\w+\s*,\s*long\s+\w+\s*=\s*-1',b) or re.search(r'\bCCharString\s*\(\s*const\s+char\s*\*\s*\w*\s*\)',b): macros.add('FABLE_RETAIL_CCHARSTRING')
    if re.search(r'\bCWideString\s*\(\s*const\s+(unsigned\s+short|wchar_t)\s*\*',b): macros.add('FABLE_RETAIL_CWIDESTRING')
    if re.search(r'template\s*<[^>]*>\s*(class|struct)\s+CDefPointer\b[^;]*\{',b): raise Defer('closure_model CDefPointer (local pointee stand-ins)')
    for c in CLOSURE:
        rest,n=remove_class(rest,c)
        if n: removed.append(c)
    # fable_string.h keeps the retail member sets (operators, GetLength, the literal constructor) behind the opt-ins
    if 'CCharString' in removed: macros.add('FABLE_RETAIL_CCHARSTRING')
    if 'CWideString' in removed: macros.add('FABLE_RETAIL_CWIDESTRING')
    # a local class derived from a removed closure model: its leading pad counted from the local model's size; re-based on the header's
    for c in removed:
        hs=CLOSURE_SIZE.get(c)
        if hs is None: continue
        bb=L.blank_comments(rest)
        for md in list(re.finditer(r'(?<![\w:])(?:class|struct)\s+(\w+)\s*:\s*(?:public\s+)?'+c+r'\s*\{',bb))[::-1]:
            sd=bb.find('{',md.start()); db=L.block(rest,sd,bb)
            mp=re.search(r'(?m)^(\s*)unsigned char\s+(\w*pad\w*)\s*\[\s*([^\]]+)\]\s*;([^\n]*)',db)
            pre=db[:mp.start()] if mp else db
            if not mp or re.search(r'\w\s*;',L.blank_comments(pre)): raise Defer('closure_derived '+md.group(1)+':'+c)
            ex=mp.group(3); me=re.match(r'^\s*(0x[0-9a-fA-F]+|\d+)\s*-\s*(0x[0-9a-fA-F]+|\d+)\s*$',ex); mh=re.search(r'(?i)pad_?(?:0x)?([0-9a-f]+)$',mp.group(2))
            try:
                if me: end=int(me.group(1),0); start=int(me.group(2),0)
                elif mh: start=int(mh.group(1),16); end=start+int(ex,0)
                else: raise ValueError
            except ValueError: raise Defer('closure_derived '+md.group(1)+':'+c+' (pad '+ex+')')
            if end-hs<0: raise Defer('closure_derived '+md.group(1)+':'+c+' (end 0x%x < header 0x%x)'%(end,hs))
            nl='' if end==hs else mp.group(1)+'unsigned char %s[0x%x - 0x%x];%s'%(mp.group(2),end,hs,mp.group(4))
            db2=db[:mp.start()]+nl+db[mp.end():]; rest=rest[:sd]+db2+rest[sd+len(db):]
    # a global local CTCGraphicAppearance stand-in: the PDB's namespace (the header forward-declares NGraphicAppearance::CTCGraphicAppearance)
    bb=L.blank_comments(rest); mg=L.class_rx('CTCGraphicAppearance').search(bb)
    if mg and not L.namespace_at(rest,mg.start(),bb) and not re.search(r'namespace\s+NGraphicAppearance\b',bb):
        sg=bb.find('{',mg.start()); gb=L.block(rest,sg,bb); ge=rest.find(';',sg+len(gb))+1
        rest=rest[:mg.start()]+'namespace NGraphicAppearance {\n'+rest[mg.start():ge]+'\n}\nusing NGraphicAppearance::CTCGraphicAppearance;'+rest[ge:]
        rest=re.sub(r'(?m)^[ \t]*class\s+CTCGraphicAppearance\s*;[ \t]*(//[^\n]*)?\n','',rest)
    rest,n=remove_class(rest,'CInterfaceMap')
    if n: removed.append('CInterfaceMap'); rest=rest.replace('@@CUT@@','@@CUT@@\ntypedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;',1)
    rest=re.sub(r'(?m)^[ \t]*enum\s+ETCInterfaceType\s*\{[^}]*\}\s*;[ \t]*(//[^\n]*)?\n','@@CUT@@\n',rest)
    rest=re.sub(r'\benum\s+ETCInterfaceType\s*\{[^}]*\}\s*;','@@CUT@@',rest)
    rest=re.sub(r'(?m)^[ \t]*enum\s+ETCInterfaceType\s*;[ \t]*(//[^\n]*)?\n','',rest)
    for mname in ('typedef\\s+std::pair<ETCInterfaceType,\\s*CTCBase\\*>\\s+InterfaceEntry;',):
        rest=re.sub(mname,'',rest)
    if 'template <class K, class V, class TCompare> class CVectorMap;' in rest: rest=rest.replace('template <class K, class V, class TCompare> class CVectorMap;','')
    rest=re.sub(r'(?m)^[ \t]*template\s*<[^>]*>\s*class\s+(CKeyPairCompareLess|CVectorMap|CArray|CDefPointer)\s*;[ \t]*(//[^\n]*)?\n','',rest)
    # a local enumerator of the removed enum used in code -> the header's name
    def enum_sub(mm):
        n=mm.group(0); v=enum_map.get(n); return TCI.get(v,n) if v is not None else n
    if enum_map: rest=re.sub(r'\b('+'|'.join(re.escape(k) for k in enum_map)+r')\b',enum_sub,rest)
    # --- 3. the lookup calls
    code=rest; out=[]; i=0; unresolved=[]; peek_vars=set()
    rxcall=re.compile(r'(?:(->|\.)\s*|(?<![\w.>:~]))(GetInterface\w*|GetTC\w*|PeekTC\w*|HasTC|HasInterface|FindTC)\s*(<\s*(?:const\s+)?(\w+)\s*(?:\*\s*)?>)?\s*\(')
    # a local `T* GetTC<T>(N)` wrapper that zeroes an out pointer and calls GetInterface: the PDB form is the out-pointer call itself
    zeroed_wrapper=bool(re.search(r'GetTC\s*\(\s*long\s+\w+\s*\)\s*(const)?\s*\{[^}]*=\s*0\s*;[^}]*GetInterface\w*\s*\(\s*&',blk))
    if zeroed_wrapper:
        def zsub(mm):
            ind,var,recv,T,arg=mm.group(1),mm.group(2),(mm.group(3) or '')+mm.group(4)+'->',mm.group(5),mm.group(6)
            tci=tci_of(arg,enum_map,row_enums,glob_enums)
            if not tci: return mm.group(0)
            macros.add('FABLE_RETAIL_CTHING_GETTC_EXPANDED')
            return ind+T+'* '+var+' = 0;\n'+ind+recv+'GetTC('+tci+', &'+var+');'
        code=re.sub(r'(?m)^([ \t]*)(?:\w+)\*\s+(\w+)\s*=\s*((?:\w+(?:->|\.))*?)?(\w+)\s*->\s*GetTC\s*<\s*(\w+)\s*>\s*\(([^()]*)\)\s*;',zsub,code)
        if re.search(r'->\s*GetTC\s*<\s*\w+\s*>\s*\(',code): unresolved.append('GetTC<T>(N) zeroed wrapper (not a declaration)')
    # the index's binds decide the lookup's const-ness: retail's CVectorMap::LowerBound is 0x0040F020 (non-const: GetTC) or
    # 0x004365B0 (const: PeekTC); a row binding only one of them spells every lookup that way (one target, one spelling)
    _bt=[x[1] for x in L.cache().get(a,{}).get('binds',[])]
    force_const=True if ('004365b0' in _bt and '0040f020' not in _bt) else (False if ('0040f020' in _bt and '004365b0' not in _bt) else None)
    bcode=L.blank_comments(code)
    while True:
        mm=rxcall.search(bcode,i)
        if not mm: out.append(code[i:]); break
        name=mm.group(2); targ=mm.group(4); p0=mm.end()   # after '('
        if name in ('GetTCGraphicAppearance','GetTCDriver','GetInterfaceType','PeekTCDriver') or (not mm.group(1) and name not in ('GetInterface','GetInterfaceConst','GetTC','PeekTC','HasTC','HasInterface')):
            out.append(code[i:p0]); i=p0; continue
        # the argument list
        d=1; j=p0
        while j<len(code) and d>0:
            if code[j]=='(': d+=1
            elif code[j]==')': d-=1
            j+=1
        argtxt=code[p0:j-1]; argl=[x.strip() for x in re.split(r',(?![^()]*\))',argtxt)] if argtxt.strip() else []
        sep=mm.group(1) or ''
        if sep: rs=receiver(code,mm.start()); recv=code[rs:mm.start()]
        else: rs=mm.start(); recv=''
        castpre=''
        mc=re.match(r'\(\s*(?:const\s+)?\w+(?:::\w+)*\s*\*+\s*\)\s*',recv)
        if mc: castpre=mc.group(0); recv=recv[mc.end():]; rs+=mc.end()
        isconst=methods.get(name,False)
        root=re.match(r'\s*\(?\*?\s*(\w+)',recv); root=root.group(1) if root else None
        if not sep:
            heads=list(re.finditer(r'(?m)^\w[^;{}()]*\([^;{}]*\)\s*(const)?\s*\{',code[:mm.start()]))
            if heads and heads[-1].group(1): isconst=True
        elif recv.rstrip().endswith(')'):
            # a getter: const when the row declares it returning a const CThing (a const chain root says nothing about it)
            mg=re.search(r'(\w+)\s*\(\s*\)\s*$',recv.rstrip())
            if mg and re.search(r'\bconst\s+CThing\s*[*&]\s*(?:\w+::)*'+re.escape(mg.group(1))+r'\s*\(',code): isconst=True
            if re.search(r'\(\s*const\s+CThing\s*[*&]\s*\)|static_cast\s*<\s*const\s+CThing\s*[*&]\s*>',recv): isconst=True
        else:
            if root and not isconst and re.search(r'\bconst\s+\w+\s*[*&]\s*(const\s*)?'+re.escape(root)+r'\b',code): isconst=True   # a const receiver: PeekTC
            if root and not isconst and re.search(r'\bconst\s+CThing\s*\*\s*(const\s*)?\b'+re.escape(root)+r'\b|\bconst\s+CThing\s*&\s*'+re.escape(root)+r'\b',code): isconst=True
            if re.search(r'\(\s*const\s+CThing\s*[*&]\s*\)',recv): isconst=True
        if force_const is not None and name not in ('HasTC','HasInterface'): isconst=force_const
        def t_of(var):
            mv=re.search(r'(?<![\w])(?:const\s+)?(\w+)\s*\*\s*(?:const\s*)?'+re.escape(var)+r'\s*(?:[;,=)]|\=)',code); return mv.group(1) if mv else None
        def tci_of_T(T):
            for (c,k),v in row_enums.items():
                if c==T: return TCI.get(v)
            return None
        rep=None
        if name in ('HasTC','HasInterface') and len(argl)==1:
            tci=tci_of(argl[0],enum_map,row_enums,glob_enums)
            if tci: rep=recv+sep+'HasTC('+tci+')'
            elif re.match(r'^[\w.>-]+$',argl[0]) and argl[0] not in TCI_BY_NAME: rep=recv+sep+'HasTC((ETCInterfaceType)'+argl[0]+')'
        elif name.startswith('GetInterface') or (name in ('GetTC','PeekTC','FindTC','GetTCIfPresent') and len(argl)==2):
            if len(argl)==2:
                pa=[x for x in argl if x.startswith('&')]; ta=[x for x in argl if not x.startswith('&')]
                if len(pa)==1 and len(ta)==1:
                    tci=tci_of(ta[0],enum_map,row_enums,glob_enums)
                    if tci: rep=recv+sep+('PeekTC' if isconst or name.endswith('Const') or name=='PeekTC' else 'GetTC')+'('+tci+', '+pa[0]+')'; macros.add('FABLE_RETAIL_CTHING_GETTC_EXPANDED')
            elif len(argl)==1 and argl[0].startswith('&'):
                T=t_of(argl[0][1:].strip()); tci=tci_of_T(T) if T else None
                rep=recv+sep+('PeekTC' if isconst else 'GetTC')+'('+((tci+', ') if tci else '')+argl[0]+')'; macros.add('FABLE_RETAIL_CTHING_GETTC_EXPANDED')
        elif name in ('GetTC','PeekTC') and len(argl)==1 and argl[0].startswith('&'):
            T=t_of(argl[0][1:].strip()); tci=tci_of_T(T) if T else None
            rep=recv+sep+('PeekTC' if (isconst or name=='PeekTC') else 'GetTC')+'('+((tci+', ') if tci else '')+argl[0]+')'; macros.add('FABLE_RETAIL_CTHING_GETTC_EXPANDED')
        elif name in ('GetTC','PeekTC','GetTCIfPresent','FindTC') and len(argl)==1:
            tci=tci_of(argl[0],enum_map,row_enums,glob_enums)
            if tci:
                cst=('const ' if (isconst or name=='PeekTC') else '')
                if code[rs-1:rs]=='&' and not castpre: rs-=1   # the local returned a reference and the row took its address
                if targ: rep='(('+cst+targ+'*)&'+recv+sep+'GetTC('+tci+'))'
                else: rep='('+castpre+'&'+recv+sep+'GetTC('+tci+'))'
                macros.add('FABLE_RETAIL_CTHING')
        if rep is None:
            unresolved.append(name+'('+argtxt[:30]+')'); out.append(code[i:j]); i=j
        else:
            out.append(code[i:rs]); out.append(rep); i=j
            if rep.find(sep+'PeekTC(')>=0 and sep:
                pa=[x for x in argl if x.startswith('&')]
                if pa: peek_vars.add(pa[0][1:].strip())
    rest=''.join(out)
    # the type-flag test of a thing class: a local `enum { ThingTypeMask = N };` is the PDB's static StatGetTypeFlags() (CThing's
    # is 0, engine/CThing.h FABLE_RETAIL_CTHING_STATTYPEFLAGS; FableWin.pdb ?StatGetTypeFlags@<class>@@SIKXZ)
    if re.search(r'\bThingTypeMask\b',rest):
        rest=re.sub(r'enum\s*\{\s*ThingTypeMask\s*=\s*([^}]+?)\s*\}\s*;','static unsigned long __fastcall StatGetTypeFlags() { return \\1; }',rest)
        rest=re.sub(r'\b(\w+)::ThingTypeMask\b',r'\1::StatGetTypeFlags()',rest); macros.add('FABLE_RETAIL_CTHING_STATTYPEFLAGS')
    # raw word tests of the UsingComponents bitset: X->m_presentBits[i] & (1 << j) / & 0xMASK  -> X->HasTC(TCI)
    def bitsub(mm):
        idx=int(mm.group(3)); mask=mm.group(4)
        if mask.startswith('('): bit=int(re.search(r'<<\s*(\d+)',mask).group(1))
        else:
            v=int(mask.rstrip('uUlL'),0); bit=v.bit_length()-1
            if v!=1<<bit: return mm.group(0)
        tci=TCI.get(idx*32+bit); return (mm.group(1)+'HasTC('+tci+')') if tci else mm.group(0)
    rest=re.sub(r'((?:\w|->|\.|\)|\])*(?:->|\.))(m_presentBits|PresentBits|m_tcMask|m_present|UsingComponents|Interfaces)\s*\[\s*(\d+)\s*\]\s*&\s*(0x[0-9a-fA-F]+[uUlL]*|\d+[uUlL]*|\(\s*1\s*(?:UL|U|L|u|l)?\s*<<\s*\d+\s*\))',bitsub,rest)
    # --- 4. member reads
    used_names=set()
    for n in list(members)+list(methods):
        if re.search(r'(->|\.)\s*'+re.escape(n)+r'\b',rest) or (own and re.search(r'(?<![\w>.])'+re.escape(n)+r'\s*[\(\[;&|=!<>+\-*/,)]',rest)): used_names.add(n)
    lacks=[]
    for n in sorted(used_names):
        if n in methods and n in HDR_MEMBERS:
            if n in MACRO_OF: macros.add(MACRO_OF[n])
            continue
        if n in methods:
            if n in ('GetInterface','GetInterfaceT','GetInterfaceV','GetInterfaceX','GetInterfaceConst','HasInterface','FindTC','GetTCIfPresent','GetPhysics','GetDef','GetDefName'):
                if re.search(r'(->|\.)\s*'+re.escape(n)+r'\s*\(',L.blank_comments(rest)):
                    if n=='GetPhysics': rest=re.sub(r'(->|\.)\s*GetPhysics\s*\(\s*\)',r'\1GetPhysicsTC()',rest); continue
                    lacks.append(n)
                continue
            lacks.append(n); continue
        off=members.get(n); off=KNOWN.get(n,off) if off is None else off
        if n in HDR_MEMBERS and own: continue
        if off is None: lacks.append(n+'(no offset)'); continue
        if off=='bit0' or off==0x91:
            if not own:
                rest,left=bit_reads(rest,n,off)
                if left: lacks.append(n+'(other bits)')
                if 'IsInLimbo()' in rest or 'IsActivated()' in rest or 'IsGloballyLoaded()' in rest or 'IsKillLocked()' in rest or 'IsDying()' in rest or 'IsLevelPersistent()' in rest or 'IsGameStatePersisted()' in rest or 'IsDeathPersistent()' in rest: macros.add('FABLE_RETAIL_CTHING_STATEACCESS')
                continue
            # X->flags & 1 -> X->IsDead(); !(X->flags & 1) -> !X->IsDead(); X->Dead (bitfield) -> X->IsDead()
            if own:
                rest=re.sub(r'(?<![\w>.])'+re.escape(n)+r'\s*&\s*1\b','Dead',rest); rest=re.sub(r'(?<![\w>.])'+re.escape(n)+r'\b(?!\s*&)','Dead',rest) if off=='bit0' else rest
            else:
                rest=re.sub(r'\(\s*((?:\w|->|\.|\)|\])*(?:->|\.))'+re.escape(n)+r'\s*&\s*1\s*\)',r'\1IsDead()',rest)
                rest=re.sub(r'((?:\w|->|\.|\)|\])*(?:->|\.))'+re.escape(n)+r'\s*&\s*1\b(?!\s*[&|<>])',r'\1IsDead()',rest)
                if off=='bit0': rest=re.sub(r'((?:\w|->|\.|\)|\])*(?:->|\.))'+re.escape(n)+r'\b(?!\s*\()',r'\1IsDead()',rest)
            if not own:
                def accsub(mm):
                    v=int(mm.group(3).rstrip('uUlL'),0); bit=v.bit_length()-1
                    if v!=1<<bit: return mm.group(0)
                    acc=BITACC.get((0x91,bit)); return (mm.group(1)+acc) if acc else mm.group(0)
                rest=re.sub(r'\(\s*((?:\w|->|\.|\)|\])*(?:->|\.))'+re.escape(n)+r'\s*&\s*(0x[0-9a-fA-F]+|\d+)\s*\)',lambda mm:accsub(type('M',(),{'group':lambda self,i,mm=mm:[mm.group(0),mm.group(1),None,mm.group(2)][i]})()),rest)
                rest=re.sub(r'((?:\w|->|\.|\)|\])*(?:->|\.))'+re.escape(n)+r'\s*&\s*(0x[0-9a-fA-F]+|\d+)\b(?!\s*[&|<>])',lambda mm:accsub(type('M',(),{'group':lambda self,i,mm=mm:[mm.group(0),mm.group(1),None,mm.group(2)][i]})()),rest)
                if 'IsInLimbo()' in rest or 'IsActivated()' in rest or 'IsGloballyLoaded()' in rest or 'IsKillLocked()' in rest: macros.add('FABLE_RETAIL_CTHING_STATEACCESS')
            if re.search(r'(->|\.)\s*'+re.escape(n)+r'\b',rest): lacks.append(n+'(other bits)')
            continue
        if isinstance(off,str):
            bit=int(off[3:]); acc=BITACC.get((0x91,bit))
            if acc and not own:
                rest=re.sub(r'((?:->|\.)\s*)'+re.escape(n)+r'\b(?!\s*\()',r'\1'+acc,rest)
                if acc!='IsDead()': macros.add('FABLE_RETAIL_CTHING_STATEACCESS')
                continue
            lacks.append(n+'(bit)'); continue
        hdr,acc=FIELD.get(off,(None,None))
        if hdr is None: lacks.append('%s@0x%x'%(n,off)); continue
        if own: rest=re.sub(r'(?<![\w>.])'+re.escape(n)+r'\b',hdr,rest) if n!=hdr else rest; continue
        if acc is None: lacks.append('%s@0x%x(no accessor)'%(n,off)); continue
        rest=re.sub(r'((?:->|\.)\s*)'+re.escape(n)+r'\b(?!\s*\()',r'\1'+acc,rest)
        if acc in ('GetTypeFlags()','GetTCGraphicAppearance()','GetPlayerNumber()'): macros.add('FABLE_RETAIL_CTHING')
    for mv in re.finditer(r'(?<![\w])(\w+)\s*\*\s*(\w+)\s*=\s*(\(?\*?\s*\w+)\s*(?:->|\.)\s*GetPhysicsTC\s*\(\s*\)',rest):
        root=re.sub(r'[(*\s]','',mv.group(3))
        if mv.group(1)!='const' and (re.search(r'\bconst\s+\w+\s*[*&]\s*(const\s*)?'+re.escape(root)+r'\b',rest) or (own and root=='this' and False)): peek_vars.add(mv.group(2))
    for v in sorted(peek_vars):
        mv=re.search(r'(?<![\w])(\w+)(\s*\*\s*)'+re.escape(v)+r'\s*(=[^;]*)?;',rest)
        if not mv or mv.group(1)=='const': continue
        T=mv.group(1); rest=rest[:mv.start()]+'const '+rest[mv.start():]
        # the methods the row calls on it, declared (prototype only) in a local T block, become const
        bb=L.blank_comments(rest); mt=L.class_rx(T).search(bb)
        if mt:
            s0=bb.find('{',mt.start()); tb=L.block(rest,s0,bb); nb=tb
            for mc2 in re.finditer(r'(?<![\w])'+re.escape(v)+r'\s*->\s*(\w+)\s*\(',rest):
                nb=re.sub(r'(\b'+re.escape(mc2.group(1))+r'\s*\([^;{}]*\))(\s*;)',r'\1 const\2',nb)
            if nb!=tb: rest=rest[:s0]+nb+rest[s0+len(tb):]
    if lacks: raise Defer('header_lacks '+','.join(lacks[:6]))
    if unresolved: raise Defer('unresolved_call '+','.join(unresolved[:4]))
    # explicit instantiations of the one-argument GetTC<T> / PeekTC<T>: T needs StatGetInterfaceType() (injected from the local
    # stand-in's interface enumerator); the include must precede that class
    for mi in re.finditer(r'template\s+bool\s+CThing::(GetTC|PeekTC)\s*<\s*(const\s+)?(\w+)\s*>\s*\(',rest):
        T=mi.group(3); tci=None
        for (c,k),v in row_enums.items():
            if c==T: tci=TCI.get(v)
        if not tci: raise Defer('unresolved_call explicit '+mi.group(1)+'<'+T+'>')
        bb=L.blank_comments(rest); mt=L.class_rx(T).search(bb)
        if not mt: raise Defer('unresolved_call explicit '+mi.group(1)+'<'+T+'> (no local class)')
        st=bb.find('{',mt.start())
        macros.add('FABLE_RETAIL_CTHING_GETTC_EXPANDED')
        if 'StatGetInterfaceType' not in L.block(rest,st,bb):
            rest=rest[:st+1]+'\npublic:\n    static ETCInterfaceType StatGetInterfaceType() { return '+tci+'; }     // '+mi.group(1)+'<T> (the PDB form) asks T for its interface type\n'+rest[st+1:]
        cut=rest.find('@@CUT@@')
        if cut>mt.start(): rest=rest[:cut]+rest[cut+7:]; rest=rest[:mt.start()]+'@@CUT@@\n'+rest[mt.start():]
    # --- derived local stand-ins: `class D : public CThing { unsigned char _pad[N]; <members with offsets> }`
    bb=L.blank_comments(rest)
    for md in list(re.finditer(r'(?<![\w:])(class|struct)\s+(\w+)\s*:\s*(?:public\s+)?CThing\s*\{',bb))[::-1]:
        sd=bb.find('{',md.start()); db=L.block(rest,sd,bb); lines=db[1:-1].split('\n'); first=None; newl=[]; absorbed=[]; bad=False
        for ln in lines:
            mp=re.match(r'(\s*)unsigned char\s+(\w*pad\w*)\s*\[\s*([^\]]+)\]\s*;',ln)
            mo=re.search(r'//\s*\+?(0x[0-9a-fA-F]+)\b',ln)
            if mp and first is None:
                try: n=eval(mp.group(3).replace('0x','0x'),{},{})
                except Exception: bad=True; break
                first=n
                if n>=0x98: newl.append(mp.group(1)+'unsigned char %s[0x%x];   // own members from +0x98'%(mp.group(2),n-0x98) if n>0x98 else '')
                continue
            if mo and int(mo.group(1),16)<0x98 and re.search(r'\w\s*;',ln.split('//')[0]):
                nm=re.search(r'(\w+)\s*(\[[^\]]*\])?\s*;',ln.split('//')[0]).group(1); absorbed.append((nm,int(mo.group(1),16))); continue
            newl.append(ln)
        if bad or first is None or (first<0x98 and not absorbed): continue
        if first<0x98 and not all(o>=first for n,o in absorbed): continue
        for nm,o in absorbed:
            hdr,acc=FIELD.get(o,(None,None))
            if acc is None: lacks.append('%s@0x%x(derived)'%(nm,o)); continue
            rest2=re.sub(r'((?:->|\.)\s*)'+re.escape(nm)+r'\b(?!\s*\()',r'\1'+acc,rest); 
            if rest2!=rest: rest=rest2; macros.add('FABLE_RETAIL_CTHING') if acc in ('GetTypeFlags()','GetTCGraphicAppearance()','GetPlayerNumber()') else None
            bb=L.blank_comments(rest); md2=re.search(r'(?<![\w:])(class|struct)\s+'+md.group(2)+r'\s*:\s*(?:public\s+)?CThing\s*\{',bb); sd=bb.find('{',md2.start()); db=L.block(rest,sd,bb)
        rest=rest[:sd]+'{'+'\n'.join(newl)+'}'+rest[sd+len(db):]
    if lacks: raise Defer('header_lacks '+','.join(lacks[:6]))
    for em in list(re.finditer(r'(?m)^[ \t]*enum\s*\{([^}]*)\}\s*;[ \t]*(//[^\n]*)?\n',rest))[::-1]:
        names=re.findall(r'\b(\w+)\s*=',em.group(1))
        if names and all(n in glob_enums for n in names) and not any(re.search(r'(?<![\w])'+re.escape(n)+r'\b',rest[:em.start()]+rest[em.end():]) for n in names):
            rest=rest[:em.start()]+rest[em.end():]
    # offsetof asserts on local names -> the header's names (or dropped when the header has no such member)
    def offsub(mm):
        n=mm.group(1)
        if n in HDR_MEMBERS: return mm.group(0)
        off=members.get(n); off=KNOWN.get(n,off) if off is None else off
        hdr=FIELD.get(off,(None,None))[0] if isinstance(off,int) else None
        return 'offsetof(CThing, '+hdr+')' if hdr else '@@DROPLINE@@'
    rest=re.sub(r'(?m)^[^\n]*FABLE_STATIC_ASSERT\s*\(\s*offsetof\s*\(\s*CThing\s*,[^\n]*\n','',rest)
    rest=re.sub(r'offsetof\(\s*CThing\s*,\s*(\w+)\s*\)',offsub,rest)
    rest=re.sub(r'(?m)^[^\n]*@@DROPLINE@@[^\n]*\n','',rest)
    # the Leaf line: the index's symbol when only the mangling differs (e.g. IBE vs QBE)
    ml=re.search(r'(?m)^//\s*Leaf:\s*(\S+)',rest); isym=L.cache().get(a,{}).get('sym') or ''
    if ml and isym and ml.group(1)!=isym and re.match(r'\?[^@?]+@',ml.group(1)) and re.match(r'\?[^@?]+@',isym) and re.match(r'\?[^@?]+@',ml.group(1)).group(0)==re.match(r'\?[^@?]+@',isym).group(0) and re.sub(r'@@[QIA][AB]E',"@@XXE",ml.group(1))==re.sub(r'@@[QIA][AB]E',"@@XXE",isym):
        rest=rest[:ml.start(1)]+isym+rest[ml.end(1):]
    if not ml and isym and isym.startswith('?') and 'A0x' not in isym:
        # no Leaf line: the gate and vland grade the largest function, which can now be an out-of-line copy of a header template
        # instance (VC7.1 emits one beside the expansion); the index's symbol names the row's function
        ml0=re.match(r'//[^\n]*\n',rest); k=ml0.end() if ml0 else 0
        rest=rest[:k]+'// Leaf: '+isym+'\n'+rest[k:]
    # hoist the closure headers' FABLE_RETAIL_* defines above the include
    hoist=[]
    def hsub(mm):
        hoist.append(mm.group(1)); return ''
    cut=rest.find('@@CUT@@')
    if cut>=0:
        after=re.sub(r'(?m)^[ \t]*(#define\s+FABLE_RETAIL_(?:STLP|CCHARSTRING|CWIDESTRING|CBASICSTRING|CVECTORMAP|CARRAY|CDEFPOINTER|DEF_CLASS_BASE|DEF_|CCLASS|BASE_OBJECT|CBASECLASS)\w*)[ \t]*(//[^\n]*)?\n',hsub,rest[cut:])
        rest=rest[:cut]+after
    for h in hoist:
        if not re.search(r'(?m)^[ \t]*'+re.escape(h)+r'\b',rest[:cut]): rest=rest[:cut]+h+'\n'+rest[cut:]; cut+=len(h)+1
    cut=rest.find('@@CUT@@')
    if cut>=0 and re.search(r'#include\s*"engine/(CClass|CBaseObject|CBaseClass)\.h"',rest[:cut]):
        top=''
        for mac in ('FABLE_RETAIL_BASE_OBJECT','FABLE_RETAIL_CCLASS'):
            if not re.search(r'(?m)^[ \t]*#define\s+'+mac+r'\b',rest[:cut]): top+='#define '+mac+'\n'
        if top:
            mi0=re.search(r'(?m)^[ \t]*#(include|define)',rest); k=mi0.start() if mi0 else 0
            rest=rest[:k]+top+rest[k:]
    # --- the include
    if 'IsIncludedInThingScans' in used_names and re.search(r'IsIncludedInThingScans[^\n]*0x0048[bB][cC]40',blk): macros.add('FABLE_RETAIL_CTHING_SCANINCLUSION_OUTOFLINE')
    if any(x in NESTED for x in macros): macros.add('FABLE_RETAIL_CTHING')
    if own and bases.startswith('public CClass'): macros.add('FABLE_RETAIL_CTHING')
    inc=''.join('#define %s\n'%x for x in sorted(macros,key=lambda x:(x!='FABLE_RETAIL_CTHING',x)))+('#include "engine/CThing.h"' if not already else '')
    if already:
        for x in macros:
            if not re.search(r'#define\s+'+x+r'\b',rest): rest=re.sub(r'(?m)^(#include "engine/CThing\.h")','#define %s\n\\1'%x,rest,count=1)
    rest=rest.replace('@@CUT@@',inc,1).replace('@@CUT@@','')
    if m.group(1)=='struct':
        rest=re.sub(r'(?m)^(// Leaf:.*)$',lambda mm: re.sub(r'([PAB])UCThing@@',r'\1VCThing@@',mm.group(1)),rest)
    rest=re.sub(r'\n{3,}','\n\n',rest)
    if rest==orig: raise Defer('unchanged')
    return rest,'migrated'+(' removed:'+'+'.join(removed) if removed else '')+(' macros:'+'+'.join(sorted(macros)) if macros else '')+(' own' if own else '')
if not addrs:
    addrs=[r['address'] for r in L.tsv(L.P+'/survey/CThing_rows.tsv') if r['kind']=='local']
if limit: addrs=addrs[:limit]
done={r['address']:r for r in L.tsv(W+'/hdrmig.tsv')} if os.path.exists(W+'/hdrmig.tsv') else {}
res=[]
for a in addrs:
    try: txt,note=migrate(a); L.write(L.ov_path(W,L.src_rel(a)),txt); res.append((a,'rewritten',note))
    except Defer as d: res.append((a,'deferred',str(d)))
    except Exception as ex: res.append((a,'error',type(ex).__name__+': '+str(ex)[:150]))
for a,s,n in res: done[a]={'address':a,'status':s,'note':n}
with open(W+'/hdrmig.tsv','w') as o:
    o.write('address\tstatus\tnote\n')
    for a in sorted(done): o.write('\t'.join([a,done[a]['status'],done[a]['note']])+'\n')
c=collections.Counter(s for a,s,n in res); print('hdrmig6',dict(c))
d=collections.Counter(n.split(' ')[0] for a,s,n in res if s!='rewritten'); print('deferred/error by kind:',d.most_common(12))
