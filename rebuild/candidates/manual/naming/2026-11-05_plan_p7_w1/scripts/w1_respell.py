# w1_respell.py W : wave 1 (phase 6). Respell every binder of 0x0041E5F2 that spells it as CFrontEndManager::GetInstance or a
# stand-in (GetUIManager / GetUI / Manager@RetailUI / Manager@PhotoInput) to the decided ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
# (coordinator decision 2026-11-05; Ego_r.pdb + FableWin.pdb; 557 landed binders already). Rules:
#  A. `class CFrontEndManager : public CManager { static CFrontEndManager* __fastcall GetInstance(); [other members] };` in namespace
#     NUISystem: the block goes, GetInstance (and the other members) move into the row's local `class CManager {` block, every
#     CFrontEndManager token becomes CManager.
#  B. a standalone CFrontEndManager stand-in (virtual slots): renamed to `class CManager` (public:), wrapped in namespace NUISystem
#     when it sat at global scope; uses qualified.
#  C. per-row patches for the free-function / placeholder stand-ins (GetUIManager, GetUI, PhotoInput::Manager, RetailUI::Manager).
# Writes W/overlay/rebuild/src/compiled/<row>.cpp and W/rows.tsv (address, rule). Comments naming CFrontEndManager::GetInstance are
# respelled too (the callee comment must name the function retail calls).
import sys,os,re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import wavelib as L
W=sys.argv[1]; os.makedirs(W,exist_ok=True)
def blank_comments(t):
    """same length as t, comments and string literals blanked (for scope scanning)"""
    out=list(t); i=0; n=len(t)
    while i<n:
        if t.startswith('//',i):
            j=t.find('\n',i); j=n if j<0 else j
            for k in range(i,j): out[k]=' '
            i=j
        elif t.startswith('/*',i):
            j=t.find('*/',i+2); j=n if j<0 else j+2
            for k in range(i,j): out[k]=' '
            i=j
        elif t[i]=='"':
            j=i+1
            while j<n and t[j]!='"':
                if t[j]=='\\': j+=1
                j+=1
            for k in range(i+1,min(j,n)): out[k]=' '
            i=j+1
        else: i+=1
    return ''.join(out)
def namespace_at(t,pos):
    """names of the namespaces enclosing offset pos"""
    b=blank_comments(t); st=[]; i=0
    for m in re.finditer(r'\bnamespace\s+(\w+)\s*\{|[{}]',b):
        if m.start()>=pos: break
        if m.group(0)=='{': st.append(None)
        elif m.group(0)=='}':
            if st: st.pop()
        else: st.append(m.group(1))
    return [x for x in st if x]
def find_block(t,rx):
    m=re.search(rx,t)
    if not m: return None
    b=blank_comments(t); i=b.find('{',m.start()); d=0; j=i
    while j<len(b):
        if b[j]=='{': d+=1
        elif b[j]=='}':
            d-=1
            if d==0: break
        j+=1
    k=t.find(';',j)+1
    e=t.find('\n',k); e=len(t) if e<0 else e   # take the rest of the line (trailing comment)
    return m.start(),k,e
GI='static CManager* __fastcall GetInstance();'
def rule_A(t):
    r=find_block(t,r'(?m)^[ \t]*class\s+CFrontEndManager\s*:\s*public\s+CManager\s*\{')
    if not r: return None,'no derived block'
    s,k,e=r; blk=t[s:k]
    inner=blk[blk.find('{')+1:blk.rfind('}')]
    members=[]
    for line in inner.split('\n'):
        l=line.strip()
        if not l or l.startswith('//') or re.match(r'^(public|private|protected):\s*$',l): continue
        members.append(line.rstrip())
    members=[re.sub(r'^(\s*)public:\s*','\\1',m) for m in members]
    members=[re.sub(r'static\s+CFrontEndManager\*\s+__fastcall\s+GetInstance\(\);',GI,m) for m in members]
    if not any('GetInstance' in m for m in members): return None,'no GetInstance in block'
    # the local CManager block of the same namespace: the last `class CManager` before the block
    cm=[m for m in re.finditer(r'(?m)^[ \t]*class\s+CManager\s*(?::[^{]*)?\{',t[:s])]
    if not cm: return None,'no local CManager block'
    cmp=cm[-1]; ins=cmp.end()
    close=t.find('}',ins)
    if '\n' in t[ins:close]:
        add='\n'+'\n'.join(('    '+m.strip()) for m in members)+('\n' if t[ins]!='\n' else '')
        t2=t[:ins]+'\npublic:'+add+t[ins:s]+t[e+1:]
    else:   # a one-line class block
        t2=t[:ins]+' public: '+' '.join(m.strip() for m in members)+t[ins:s]+t[e+1:]
    t2=re.sub(r'\bCFrontEndManager\b','CManager',t2)
    return t2,'A'
def rule_B(t):
    r=find_block(t,r'(?m)^[ \t]*(class|struct)\s+CFrontEndManager\s*\{')
    if not r: return None,'no standalone block'
    s,k,e=r; blk=t[s:k]; ns=namespace_at(t,s)
    blk2=re.sub(r'^(\s*)(class|struct)\s+CFrontEndManager\s*\{','\\1class CManager {\npublic:',blk,count=1)
    blk2=blk2.replace('static CFrontEndManager* __fastcall GetInstance();',GI)
    rest=t[:s]+('namespace NUISystem {\n' if 'NUISystem' not in ns else '')+blk2+('\n}' if 'NUISystem' not in ns else '')+t[k:]
    q='CManager' if 'NUISystem' in ns else 'NUISystem::CManager'
    rest=re.sub(r'\bNUISystem::CFrontEndManager\b','NUISystem::CManager',rest)
    rest=re.sub(r'\bCFrontEndManager\b',q,rest)
    return rest,'B'+('' if 'NUISystem' in ns else '-wrapped')
NSDECL='namespace NUISystem { class CManager { public: static CManager* __fastcall GetInstance(); }; }   // 0x0041E5F2 (Ego_r.pdb / FableWin.pdb)\n'
def special(a,t):
    if a=='0041e641':
        # CManager::~CManager: GetInstance and the two bank setters are CManager's own (Ego_r.pdb / FableWin.pdb ?SetMeshBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCMeshDataBank@@@@@Z
        # 0x0041D198 (2 landed binders already spell it so), ?SetGraphicsBank@..VCGraphicDataBank.. 0x0042A9B7); the local CFontBank is CMeshDataBank (PMeshBank +0x18 in NUISystem_CManager.h).
        t=t.replace('class CFontBank;','class CMeshDataBank;')
        t=re.sub(r'class CFrontEndManager \{ public:\n    static CFrontEndManager\* __fastcall GetInstance\(\);[^\n]*\n    void SetFontBank\(CCountedPointer<CFontBank> bank\);[^\n]*\n    void SetGraphicsBank\(CCountedPointer<CGraphicDataBank> bank\);[^\n]*\n\};\n','',t)
        t=t.replace('class CManager : public CManagerBase { public:\n','class CManager : public CManagerBase { public:\n    static CManager* __fastcall GetInstance();                                 // 0x0041E5F2\n    void SetMeshBank(CCountedPointer<CMeshDataBank> bank);                    // 0x0041D198\n    void SetGraphicsBank(CCountedPointer<CGraphicDataBank> bank);             // 0x0042A9B7\n')
        t=t.replace('    SMember10     m_graphicsBank;                  // +0x10\n    SMember18     m_fontBank;                      // +0x18\n','    SMember10     PGraphicsBank;                   // +0x10\n    SMember18     PMeshBank;                       // +0x18\n')
        t=t.replace('CFrontEndManager::GetInstance()->SetFontBank(CCountedPointer<CFontBank>());','GetInstance()->SetMeshBank(CCountedPointer<CMeshDataBank>());')
        t=t.replace('CFrontEndManager::GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());','GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());')
        t=t.replace('CFrontEndManager::GetInstance @ 0x0041E5F2','CManager::GetInstance @ 0x0041E5F2').replace('the bank setters @ 0x0041D198 (counted pointer at +0x18)','SetMeshBank @ 0x0041D198 (counted pointer at +0x18)')
        t=t.replace('hand the front-end manager singleton empty font and graphics banks','hand the manager singleton empty mesh and graphics banks')
        return t,'special'
    if a=='005407b0':
        t=t.replace('struct ManagerMethods:CManager {typedef void(ManagerMethods::*AddObserverMethod)(ObserverNative*);void AddObserver(ObserverNative*p){(this->*static_cast<AddObserverMethod*>(__vftable)[2])(p);}};\nManagerMethods* __fastcall GetUIManager(); //41E5F2 CManager::GetInstance\n',
                    'namespace NUISystem {class CManager:public ::CManager {public:static CManager* __fastcall GetInstance(); //0041E5F2 (Ego_r.pdb / FableWin.pdb)\n typedef void(CManager::*AddObserverMethod)(ObserverNative*);void AddObserver(ObserverNative*p){(this->*static_cast<AddObserverMethod*>(__vftable)[2])(p);}};}\n')
        t=t.replace('GetUIManager()->AddObserver(&Observer);','NUISystem::CManager::GetInstance()->AddObserver(&Observer);')
        t=t.replace('sizeof(ManagerMethods::AddObserverMethod)==4','sizeof(NUISystem::CManager::AddObserverMethod)==4')
        return t,'special'
    if a=='0055a3e0':
        t=t.replace('struct NativeUIManager {unsigned char Prefix[0xbc];void* PDropped;};\nNativeUIManager* __cdecl GetUIManager(); //0041E5F2\n',
                    'namespace NUISystem {class CManager:public ::CManager {public:static CManager* __fastcall GetInstance();};} //0041E5F2 (Ego_r.pdb / FableWin.pdb); PDropped +0xbc from engine/CManager.h\n')
        t=t.replace('GetUIManager()->PDropped','NUISystem::CManager::GetInstance()->PDropped')
        return t,'special'
    if a in ('005fc2d0','005fc8a0'):
        t=t.replace('struct CManager {__declspec(noinline) CComponent* CreateComponent(const CCharString&,int);};CManager* __fastcall GetUI();',
                    'struct CManager {__declspec(noinline) CComponent* CreateComponent(const CCharString&,int);};')
        t=t.replace('CManager* ui=GetUI();','CManager* ui=(CManager*)NUISystem::CManager::GetInstance();')
        # the declaration goes before the row's placeholder namespace
        m=re.search(r'(?m)^namespace NScreen%s \{'%a,t)
        if not m: return None,'no NScreen namespace'
        t=t[:m.start()]+NSDECL+t[m.start():]
        return t,'special'
    if a=='00691ae0':
        t=t.replace('struct Manager {void* VTable;unsigned char BeforeMouseMovement[0xac];Vector2 MouseMovement;static Manager* __fastcall GetInstance();void SetMouseMovement(Vector2& v){MouseMovement=v;}};',
                    'struct Manager {void* VTable;unsigned char BeforeMouseMovement[0xac];Vector2 MouseMovement;static Manager* GetInstance(){return (Manager*)NUISystem::CManager::GetInstance();}void SetMouseMovement(Vector2& v){MouseMovement=v;}};')
        m=re.search(r'(?m)^namespace PhotoInput \{',t)
        if not m: return None,'no PhotoInput namespace'
        t=t[:m.start()]+NSDECL+t[m.start():]
        return t,'special'
    if a in ('005518e0','00551bc0'):
        t=t.replace('struct Manager {static Manager* __fastcall GetInstance();Component* CreateComponent(unsigned long);}; //0041e5f2,0041d21b',
                    'struct Manager {Component* CreateComponent(unsigned long);}; //0041d21b; the instance is NUISystem::CManager::GetInstance() 0x0041e5f2')
        t=t.replace('Manager::GetInstance()->CreateComponent(','((Manager*)NUISystem::CManager::GetInstance())->CreateComponent(')
        m=re.search(r'(?m)^namespace RetailUI \{',t)
        if not m: return None,'no RetailUI namespace'
        t=t[:m.start()]+NSDECL+t[m.start():]
        return t,'special'
    if a=='00597006':
        # the row IS NUISystem::CFrontEndManager (RefreshAvailableProfilesForDelete; GetProfileNames 0x005955AB, CreateFrontEndButtonComponent 0x00596A68 are its
        # own PDB members): only GetInstance moves to CManager, whose instance the method then uses for CreateComponent (0x0041DB1D, a CManager member).
        t=t.replace('class CManager {\npublic:\n    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D\n',
                    'class CManager {\npublic:\n    static CManager* __fastcall GetInstance();              // 0x0041E5F2\n    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D\n')
        t=t.replace('    static CFrontEndManager* __fastcall GetInstance();      // 0x0041E5F2\n','')
        t=t.replace('    CFrontEndManager* manager = GetInstance();','    CManager* manager = CManager::GetInstance();')
        return t,'special'
    return None,'no special rule'
rows=open(W+'/rows.tsv','w'); rows.write('address\trule\n')
for l in open(L.P+'/w1/binders.tsv'):
    a=l.split('\t')[0]; t=L.read(L.ROOT+'/'+L.src_rel(a))
    t2,why=special(a,t)
    if t2 is None: t2,why=rule_A(t)
    if t2 is None: t2,why=rule_B(t)
    if t2 is None or t2==t: print(a,'UNCHANGED',why); rows.write(a+'\tUNCHANGED '+why+'\n'); continue
    t2=re.sub(r'CFrontEndManager::GetInstance','CManager::GetInstance',t2)   # comments
    if re.search(r'\bCFrontEndManager\b',t2) and a!='00597006': print(a,'WARN: CFrontEndManager token left')
    L.write(L.ov_path(W,L.src_rel(a)),t2); rows.write(a+'\t'+why+'\n'); print(a,why)
