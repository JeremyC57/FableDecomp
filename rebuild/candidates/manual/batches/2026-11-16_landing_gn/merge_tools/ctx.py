# For each occurrence of a code line in fin/<rel>, print its line number and the enclosing directive stack.
import sys, re
rel, pat = sys.argv[1], sys.argv[2]
t = open('fin/'+rel, encoding='latin-1').read().replace('\r\n','\n').split('\n')
st = []
for i,l in enumerate(t,1):
    s=l.strip()
    if re.match(r'#\s*if', s): st.append(s[:70])
    elif re.match(r'#\s*(elif|else)', s) and st: st[-1]=st[-1]+' |'+s[:50]
    elif re.match(r'#\s*endif', s) and st: st.pop()
    elif pat in s: print(i, ' > '.join(x for x in st if 'INCLUDED' not in x and '_H' not in x[-6:]) or '(top)')
