#!/usr/bin/env python3
"""ann.py file.cpp [mask]: annotate each return site (t2 order) of the row function with its context:
 L=loop nesting kinds (F=for(;;)/for, W=while, D=do), I=if depth inside innermost loop, top=first stmt of a loop body,
 aw=right after a while loop, inw=inside a while body"""
import sys,re
s=open(sys.argv[1]).read(); mask=sys.argv[2] if len(sys.argv)>2 else ''
m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',s,re.M)); start=m[-1].start()
C=r'((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)'
pat=re.compile(r'(?:do \{ )?\bif \('+C+r'\)\s*(\{\s*return;\s*\}|return;)(?: \} while \(0\);)?')
body=s[start:]
# build a stack by scanning tokens
toks=re.finditer(r'\bfor\s*\(|\bwhile\s*\(|\bdo\b|\bif\s*\(|\belse\b|\{|\}|;',body)
k=0
for mm in pat.finditer(body):
    pre=body[:mm.start()]
    # crude: count loop headers whose braces are still open
    stack=[]; i=0; pend=None
    for t in re.finditer(r'\bfor\s*\((?:[^()]|\([^()]*\))*\)\s*\{|\bwhile\s*\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)\s*\{|\bdo\s*\{|\bif\s*\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)\s*\{|\belse\s*\{|\{|\}',pre):
        x=t.group(0)
        if x=='}':
            if stack: stack.pop()
        elif x.startswith('for'): stack.append('F')
        elif x.startswith('while'): stack.append('W')
        elif x.startswith('do'): stack.append('D')
        elif x.startswith('if') or x.startswith('else'): stack.append('i')
        else: stack.append('b')
    tail=pre.rstrip()
    top=tail.endswith('{') and stack and stack[-1] in 'FWD'
    aw=bool(re.search(r'\}\s*$',tail)) and re.search(r'while\s*\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)\s*\{[^{}]*\}\s*$',tail) is not None
    st=''.join(stack[1:]) if stack else ''
    w=mask[k] if k<len(mask) else '?'
    print(f"{k}\t{w}\t{st}\t{'top' if top else ''}\t{'aw' if aw else ''}")
    k+=1
