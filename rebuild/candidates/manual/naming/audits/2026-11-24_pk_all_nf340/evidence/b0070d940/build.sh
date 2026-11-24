#!/bin/bash
# build.sh <name> <src> [flags...]: compile in the background-safe way, one cl at a time
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh; export S; unset TMPDIR TMP
A=$S/w22/agentNF340; cd $A/work/bred; n=$1; src=$2; shift 2
python3 -I -B -c "
import sys; sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline'); sys.dont_write_bytecode=True
import verify_and_land as V, subprocess
def rt(exe,args,e,_t=600):
    cmd=[V.WINE,str(exe)]+V.tool_args(args)
    try: return subprocess.run(cmd,capture_output=True,text=True,errors='replace',env=e,timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd,124,'','timeout')
open('$A/work/bred/$n.cpp','w').write(V.vc71(open('$A/work/bred/$src').read()))
r=rt(V.CL_EXE,['/nologo','/c','/O2','/Oy','/W3']+sys.argv[1:]+['/Fo$A/work/bred/$n.obj','$A/work/bred/$n.cpp'],V.env()); print('rc',r.returncode); print(r.stdout[-800:])
" "$@" > $n.log 2>&1
python3 -I -B $A/tools/pk_all.py $A/work/bred/$n.obj 00000000 --leaf=NF340_Probe --no-leaf-relocs --addrs=$A/work/bred/addrs.tsv >> $n.log 2>&1
echo DONE >> $n.log
