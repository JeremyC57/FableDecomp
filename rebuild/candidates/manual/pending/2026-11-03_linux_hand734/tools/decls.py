import sys,re,subprocess,os
# for each address, grep landed sources for a declaration line mentioning it
R='/home/user/FableDecomp/rebuild/src'
for a in sys.argv[1:]:
    A=a.upper().zfill(8)
    try:
        out=subprocess.run(['grep','-rhi','-m1','0x'+A,R,'--include=*.cpp'],capture_output=True,text=True).stdout
    except Exception as e: out=''
    lines=[l.strip() for l in out.splitlines() if '(' in l or 'extern' in l]
    seen=[]
    for l in lines:
        k=re.sub(r'\s+',' ',l)[:160]
        if k not in seen: seen.append(k)
    print('==',A)
    for k in seen[:4]: print('   ',k)
