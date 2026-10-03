import json,sys,bisect,subprocess,os
fs=json.load(open(os.environ.get('FABLE_FUNCS_JSON', 'funcs.json'))); st=sorted(int(f['addr'],16) for f in fs)
for arg in sys.argv[1:]:
    a=int(arg,16); e=st[bisect.bisect_right(st,a)]
    f=next(x for x in fs if int(x['addr'],16)==a)
    print(f"### {f['module']}::{f['name']} @ {a:08x} {f['cc']} {f['ret_type']} ({f['params']})")
    out=subprocess.run(['objdump','-d','-M','intel','--no-show-raw-insn',f'--start-address={a:#x}',f'--stop-address={e:#x}',os.environ['FABLE_EXE']],capture_output=True,text=True).stdout
    lines=[l for l in out.splitlines()[7:] if l.strip() and 'int3' not in l]
    print("\n".join(l.split(':',1)[0].strip()[-4:]+':'+l.split(':',1)[1] for l in lines if ':' in l))
