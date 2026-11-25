import struct,os
EXE=os.environ.get('FABLE_EXE')
class PE:
    def __init__(s,p=EXE):
        s.d=open(p,'rb').read(); d=s.d
        e=struct.unpack_from('<I',d,0x3c)[0]
        n=struct.unpack_from('<H',d,e+6)[0]; osz=struct.unpack_from('<H',d,e+20)[0]
        s.base=struct.unpack_from('<I',d,e+24+28)[0]
        so=e+24+osz; s.secs=[]
        for i in range(n):
            nm,vs,va,rs,ro=struct.unpack_from('<8sIIII',d,so+40*i)
            s.secs.append((nm.rstrip(b'\0').decode(),va+s.base,vs,ro,rs))
    def text(s):
        for nm,va,vs,ro,rs in s.secs:
            if nm=='.text': return va,s.d[ro:ro+min(vs,rs)]
    def read(s,a,n):
        for nm,va,vs,ro,rs in s.secs:
            if va<=a<va+rs: return s.d[ro+a-va:ro+a-va+n]
