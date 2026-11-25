#!/usr/bin/env python3
"""vt429.py <Class...>: retail vftable slots of each class with RTTI-port labels (genmodel's reader)."""
import sys,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import vtlib429 as V
for X in sys.argv[1:]:
    for vt,sl in V.vtable(X):
        print('%s vt=%08X n=%d'%(X,vt,len(sl)))
        for j,t in enumerate(sl): print('  %2d %08X %s'%(j,t,V.L.get('%08x'%t,'-')))
