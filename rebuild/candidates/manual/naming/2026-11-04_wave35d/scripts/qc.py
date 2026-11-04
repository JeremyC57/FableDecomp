# qc.py <addr> <leaf> <src> [flags...]: quick parity with lib35.qcheck
import sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
a,leaf,src=sys.argv[1:4]
st,obj=qcheck(a,open(src).read(),leaf,sys.argv[4:])
print(a,st,obj)
