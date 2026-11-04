# entity class model from the landed Alloc row's TU (wave 33 D1 precedent)
import re
from common35 import *
from allocs import alloc_map
_AM=None
def amap():
    global _AM
    if _AM is None: _AM=alloc_map()
    return _AM
SIZES={'bool':1,'char':1,'unsigned char':1,'signed char':1,'short':2,'unsigned short':2,'long':4,'unsigned long':4,'int':4,'unsigned int':4,'float':4,
       'CTimer':4,'CCharString':4,'CScriptThing':0xc,'CScriptGameResourceObjectScriptedThingBase':8,'C3DVector':0xc,'double':8}
ALIGN={'double':8,'C3DVector':1}
def model(inner,outer,method):
    """-> (text, alloc_addr, alloc_src, err). text = Alloc TU without its header comment and Alloc body, with <method> declared
    private virtual; the marker /*@@DEF@@*/ marks where the Alloc definition was (namespace-relative qualified)."""
    v=amap().get(inner+'@'+outer)
    if not v: return None,None,None,'no landed Alloc row for %s::%s'%(outer,inner)
    aa,asrc,aleaf,aname=v[0]
    s=rowsrc(asrc)
    lines=s.split('\n'); i=0
    while i<len(lines) and lines[i].startswith('//'): i+=1
    body='\n'.join(lines[i:])
    # Alloc definition
    m=re.search(r'^(CCountedPointer<CEntityScriptBase>\s+(?:__fastcall|FABLE_FASTCALL)\s+([\w:]+)::Alloc\([^)]*\))\s*\n\{.*?\n\}[ \t]*\n',body,re.M|re.S)
    if not m: return None,aa,asrc,'Alloc definition not found in '+asrc
    qual=m.group(2)
    if not qual.endswith(inner): return None,aa,asrc,'Alloc qualifier %s'%qual
    body=body[:m.start()]+'/*@@DEF@@*/\n'+body[m.end():]
    pat=re.compile(r'^[ \t]*virtual ~%s\(\);[^\n]*\n'%re.escape(inner),re.M)
    nd=len(pat.findall(body))
    if nd>1: return None,aa,asrc,'Alloc TU declares ~%s %d times'%(inner,nd)
    body=pat.sub('',body)
    if re.search(r'~%s\s*\('%re.escape(inner),body): return None,aa,asrc,'another ~%s in Alloc TU'%inner
    # method declaration
    mm_=re.search(r'^([ \t]*)virtual void Main\(\);[^\n]*\n',body,re.M)
    if method!='Main' and not re.search(r'virtual void %s\(\);'%method,body) and mm_:
        pre=body[:mm_.start()]; acc=re.findall(r'\b(public|private|protected)\s*:',pre[pre.rfind('class %s '%inner):])
        if acc and acc[-1]=='private':
            ind=mm_.group(1)
            body=body[:mm_.end()]+'%svirtual void %s();%s// PDB ?%s@%s@%s@NScript@@EAEXXZ (slot 2)\n'%(ind,method,' '*max(1,37-len(method)),method,inner,outer)+body[mm_.end():]
    if not re.search(r'virtual void %s\(\);'%method,body):
        am=re.search(r'^([ \t]*)static CCountedPointer<CEntityScriptBase>\s+(?:__fastcall|FABLE_FASTCALL)\s+Alloc\([^\n]*\);[^\n]*\n',body,re.M)
        if not am: return None,aa,asrc,'Alloc declaration not found'
        ind=am.group(1)
        ins='%sprivate:\n%svirtual void %s();%s// PDB ?%s@%s@%s@NScript@@EAEXXZ (slot %d)\n%spublic:\n'%(ind,ind,method,' '*max(1,37-len(method)),method,inner,outer,{'Main':1,'Init':2}[method],ind)
        body=body[:am.end()]+ins+body[am.end():]
    else:
        # must be private
        dm=re.search(r'virtual void %s\(\);'%method,body)
        pre=body[:dm.start()]
        acc=re.findall(r'\b(public|private|protected)\s*:',pre[pre.rfind('class %s '%inner):])
        if not acc or acc[-1]!='private': return None,aa,asrc,'%s declared but not private in Alloc TU'%method
    i=body.find('class CCPPointerInfo'); t_=body.find('template <class T> class CCountedPointer')
    if i>=0 and t_>=0 and 'engine/CCountedPointer.h' not in body:
        k=body.find('\n};\n',t_)+4
        pre=re.sub(r'^void __fastcall Destroy_00cdee00\(void\* object\);[^\n]*\n\n?','',body[:i],flags=re.M)
        body=pre+'// CCountedPointer<T>: the shared retail model (engine/CCountedPointer.h; 0x00CDEE00 is CCountedPointer<CEntityScriptBase>::DeleteData);\n//   the Alloc row\'s local stand-in (deleter Destroy_00cdee00) is dropped here as wave 33 D4 did.\n#include "engine/CCountedPointer.h"\n'+body[k:]
    return body,aa,asrc,qual
def members(text,inner):
    """offset -> (name,type) of the inner class's own data members (start +0x14), simple types only"""
    i=text.find('class %s : public CEntityScriptBase'%inner)
    if i<0: return None
    j=text.find('{',i); depth=0; k=j
    while True:
        c=text[k]
        if c=='{': depth+=1
        elif c=='}':
            depth-=1
            if depth==0: break
        k+=1
    blk=text[j+1:k]
    off=0x14; res={}
    for ln in blk.split('\n'):
        t=ln.split('//')[0].strip()
        if not t or t.endswith(':') or '(' in t or t.startswith(('virtual','static','typedef','class','struct','enum','friend','public','private','protected','}','{',':')): continue
        mm=re.match(r'^([\w:<>\* ]+?)\s*(\*?)\s*(\w+)(\[(\w+)\])?\s*;$',t)
        if not mm: return None
        ty=(mm.group(1)+mm.group(2)).strip(); nm=mm.group(3); n=int(mm.group(5),0) if mm.group(5) else 1
        if ty.endswith('*') or ty.endswith('&'): sz=4; al=4
        elif ty in SIZES: sz=SIZES[ty]; al=ALIGN.get(ty,min(sz,4))
        else: return None
        if off%al: off+=al-off%al
        res[off]=(nm,ty,n)
        cm=re.search(r'\+0x([0-9a-fA-F]+)',ln)
        if cm and int(cm.group(1),16)!=off: return None
        off+=sz*n
    return res
