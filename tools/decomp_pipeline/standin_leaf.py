# standin_leaf.py -- the STANDIN_LEAF check (plan phase 3, 2026-11-04; proposed for the coordinator, owner question 1 of phase 2).
# A landed row can grade a STAND-IN symbol: a free function `?CFoo_Bar@@YI...` or a helper class `?Get@T@@...` that has the
# retail body but not the catalog function's name (CFoo::Bar). Its `// Leaf:` is honest about what is graded, so nothing
# else reports it. verify_and_land prints `WARN STANDIN_LEAF <symbol> (catalog <name>)` for such rows. The warning is
# NON-blocking: it changes no grade, no parity and no behaviour result; LANDVERIFY_STRICT_* do not read it.
# The rule: the graded symbol's (method, class) must equal the catalog name's (template arguments dropped; ctor / dtor /
# scalar and vector deleting destructors and operators decoded). A function template symbol (??$name@) only needs the method.
# Rows with no catalog name to compare (sub_XXXXXXXX, `dynamic initializer ...`, _static_init::, a `$E<n>` symbol) and EH funclet
# rows are never flagged. An undecorated C symbol matches with one cdecl `_` or its fastcall/stdcall decoration removed.
import re
_OPS={"??4":"operator=","??8":"operator==","??9":"operator!=","??A":"operator[]","??R":"operator()","??M":"operator<",
      "??O":"operator>","??N":"operator<=","??P":"operator>=","??2":"operator new","??3":"operator delete","??_U":"operator new[]",
      "??_V":"operator delete[]","??Y":"operator+=","??Z":"operator-=","??C":"operator->","??D":"operator*","??H":"operator+",
      "??G":"operator-","??K":"operator/","??X":"operator*=","??_0":"operator/=","??7":"operator!","??B":"operator cast",
      "??E":"operator++","??F":"operator--","??6":"operator<<","??5":"operator>>","??L":"operator%","??I":"operator&","??U":"operator|",
      "??T":"operator^","??S":"operator~","??V":"operator&&","??W":"operator||","??Q":"operator,","??_4":"operator&=","??_5":"operator|=",
      "??_6":"operator^=","??_1":"operator%=","??_2":"operator>>=","??_3":"operator<<="}
def sym_mc(sym):
    """(method, class) of a VC7.1 decorated name; ctor/dtor/sdd/vdd for the special members."""
    s=sym.lstrip('_') if not sym.startswith('?') else sym
    if s.startswith('??$'):   # function template (??$name@<args>@scope@@) or member operator template (??$?R<args>@class@@)
        m=re.match(r'^\?\?\$\?([0-9A-Z]|_[0-9A-Z])',s)
        if m: return _OPS.get('??'+m.group(1),'operator'),None
        m=re.match(r'^\?\?\$([A-Za-z_0-9]+)@',s); return (m.group(1) if m else s),None   # None: scope not decoded
    for code,meth in (('??0','ctor'),('??1','dtor'),('??_G','sdd'),('??_E','vdd')):
        if s.startswith(code):
            m=re.match(r'^'+re.escape(code)+r'\??\$?([A-Za-z_0-9]+)@',s); return meth,(m.group(1) if m else '')
    for code in sorted(_OPS,key=len,reverse=True):
        if s.startswith(code):
            m=re.match(r'^'+re.escape(code)+r'\??\$?([A-Za-z_0-9]+)@',s); return _OPS[code],(m.group(1) if m else '')
    m=re.match(r'^\?([A-Za-z_0-9]+)@(?:@|\??\$?([A-Za-z_0-9]+)@)',s)
    if m: return m.group(1),(m.group(2) or '')
    return re.sub(r'@\d+$','',s.lstrip('@')),''   # C / __fastcall / __stdcall names
def cat_mc(nm):
    nm=re.sub(r'<[^<>]*(?:<[^<>]*(?:<[^<>]*>[^<>]*)*>[^<>]*)*>','',nm)   # drop template arguments (nested)
    mod,meth=nm.rsplit('::',1) if '::' in nm else ('',nm)
    cl=mod.split('::')[-1] if mod and mod!='_global' else ''
    m0=meth.strip()
    if 'scalar deleting' in m0: m0='sdd'
    elif 'vector deleting' in m0: m0='vdd'
    elif m0.startswith('~'): m0='dtor'
    elif cl and m0==cl: m0='ctor'
    elif m0.startswith('operator') and not m0.startswith('operator '): m0=re.sub(r'\s+','',m0)
    return m0,cl
# Owner rule (2026-11-05): a stand-in is fine when it is consistent and named as accurately as the code allows, in Lionhead style
# (a free `Class_Method` function standing for a member, or the real method on a stand-in class named after the real one). Only an
# INACCURATE stand-in warns: a generic placeholder class or method (T, Slot, M ...), an address-suffixed name, or a name that
# carries neither the catalog method nor (for a member) the catalog class. Consistency (one stand-in spelling per retail target)
# is the symbol index's MULTINAME check, not this one.
GENERIC={"T","Sub","Owner","Slot","Info","Obj","S","Self","This","Inner","Foo","Holder","Mid","MidObj","A","B","C","X","Y","P","Q",
         "R","Base","Head","Tail","HeadStr","TailObj","FirstPart","SubA","SubB","Pair","PairType","Elem","Node","Item","W","V","U",
         "D","E","F","G","H","K","Impl","Helper","Wrap","Stub","M","N","Fn","F0","Host","Ptr","List"}
def _n(x): return re.sub(r'[^a-z0-9]','',(x or '').lower())
def _placeholder(nm): return bool(re.search(r'(?:^|_)(?:sub|FUN|vfunc|thunk|cand)_?[0-9a-fA-F]*$|_[0-9a-fA-F]{8}$|^C_?[0-9a-fA-F]{8}$',nm or ''))
def standin(sym,catname):
    """None when the graded symbol names the catalog function or is an accurate stand-in, else '<sym> (catalog <name>; <why>)'."""
    if not sym or 'Unwind@' in catname: return None
    if re.match(r'^(?:[A-Za-z_]+::)?sub_[0-9a-fA-F]{8}$',catname) or catname.startswith(('`','_static_init::')) or re.match(r'^_?\$E\d+$',sym): return None   # no catalog name to compare
    gm,gc=sym_mc(sym); cm,cc=cat_mc(catname)
    if gm==cm and (gc==cc or gc is None or not cc and not gc): return None
    if not sym.startswith('?'):   # an undecorated C name: one cdecl `_`, or the fastcall `@..@N` / stdcall `@N` decoration
        base=re.sub(r'@\d+$','',sym); base=base[1:] if base[:1] in '_@' else base
        if base==cm or base==catname or ('_'+base)==cm: return None
        gm,gc=base,''
    if gm==cm and not cc: return None          # a _global catalog row that names a member: the class is the source's choice
    if cc in ('std','stlp_std','_STL','') or (cc and cc[:1].islower()): cc=''   # a namespace, not a class
    free = sym.startswith('?') and re.match(r'^\?[A-Za-z_0-9]+@@',sym) or not sym.startswith('?')
    if gc in GENERIC or gm in GENERIC: why='generic placeholder'
    elif _placeholder(gm) or _placeholder(gc): why='address-only name'
    elif _placeholder(cm):                     # the catalog name is itself a placeholder: judge the stand-in alone
        return None
    elif free and _n(cm) and _n(cm) in _n(gm) and (not cc or _n(cc) in _n(gm)): return None      # Class_Method free function
    elif gm==cm and gc and cc and (_n(cc) in _n(gc) or _n(gc) in _n(cc)): return None              # real method, stand-in class
    elif free and _n(cm) and _n(cm) in _n(gm): why='free function lacks its class'
    else: why='names another function'
    return '%s (catalog %s; %s)'%(sym,catname,why)
