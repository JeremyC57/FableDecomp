import glob
def F(a): return glob.glob("rebuild/src/compiled/"+a[0:2]+"/"+a[2:4]+"/*_"+a+".cpp")[0]
lit={"00aa9650":("19521484","HLPR"),"00aae090":("19521484","HLPR"),"00aae2a0":("19523492","MVEC"),"00abfb90":("19521652","MTLS")}
EDITS=[]
for a,(num,s) in lit.items():
    EDITS += [
    (F(a),"""struct CBase_00aa8360 {
    virtual void v00();
    CBase_00aa8360(long typeCode);              // 0x00AA8360
};""","""class CChunkedFileSuperChunk { public:        // RTTI direct base (was the stand-in CBase_00aa8360)
    virtual void v00();
protected:
    CChunkedFileSuperChunk(const char* type);  // 0x00AA8360 (Ego_r / FableWin protected ??0CChunkedFileSuperChunk@@IAE@PBD@Z)
public:
};"""),
    (F(a),"CBase_00aa8360("+num+")",'CChunkedFileSuperChunk("'+s+'")',0),
    (F(a),"CBase_00aa8360","CChunkedFileSuperChunk",0)]
EDITS += [
(F("00ac02b0"),"    CChunkedFileChunk(long typeCode);              // 0x00AA7590","protected:\n    CChunkedFileChunk(const char* type);           // 0x00AA7590 (Ego_r / FableWin protected ??0CChunkedFileChunk@@IAE@PBD@Z)\npublic:"),
(F("00ac02b0"),"CChunkedFileChunk(19525068)",'CChunkedFileChunk("MTLE")',0),
]
