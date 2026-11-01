import sys
# mk.py <src> <dst> <old> <new>  (literal replace, must occur once)
s=open(sys.argv[1]).read(); o=sys.argv[3].encode().decode('unicode_escape'); n=sys.argv[4].encode().decode('unicode_escape')
assert s.count(o)==1, ('count', s.count(o)); open(sys.argv[2],'w').write(s.replace(o,n))
