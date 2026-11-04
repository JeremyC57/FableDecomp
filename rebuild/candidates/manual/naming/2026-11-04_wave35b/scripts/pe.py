import struct, os
EXE = os.environ.get('FABLE_EXE')
class PE:
    def __init__(self, path=EXE):
        self.d = open(path,'rb').read()
        d=self.d
        e=struct.unpack_from('<I',d,0x3c)[0]
        nsec=struct.unpack_from('<H',d,e+6)[0]; opt=struct.unpack_from('<H',d,e+20)[0]
        self.base=struct.unpack_from('<I',d,e+24+28)[0]
        self.secs=[]
        o=e+24+opt
        for i in range(nsec):
            name=d[o:o+8].rstrip(b'\0').decode(); vs,va,rs,ro=struct.unpack_from('<IIII',d,o+8)
            self.secs.append((name,self.base+va,max(vs,rs),ro,rs)); o+=40
    def sec(self,va):
        for s in self.secs:
            if s[1]<=va<s[1]+s[2]: return s
    def off(self,va):
        s=self.sec(va)
        if not s: return None
        r=va-s[1]
        if r>=s[4]: return None
        return s[3]+r
    def u32(self,va):
        o=self.off(va); 
        if o is None: return None
        return struct.unpack_from('<I',self.d,o)[0]
    def bytes(self,va,n):
        o=self.off(va); return self.d[o:o+n]
    def cstr(self,va):
        o=self.off(va); e=self.d.index(b'\0',o); return self.d[o:e].decode('latin1')
    def is_text(self,va):
        s=self.sec(va); return s and s[0]=='.text'
