from pathlib import Path
import sys,subprocess,json
sys.path.insert(0,'tools/decomp_pipeline');import verify_and_land as V
D=Path('work/codex-lane/w22/coordinator_retry184').resolve();e=V.env();o=D/'00cab370_eh.obj';exe=D/'00cab370_eh.exe';args=['/nologo','/c','/O2','/GX','/W3',f'/Fo{o}',str(D/'00cab370_test.cpp')]
p=V.cl(args,e);(D/'native_eh_compile.log').write_text(json.dumps(args)+'\n'+p.stdout+p.stderr);assert p.returncode==0
p=V.run_tool(V.VC/'bin/link.exe',['/nologo',f'/out:{exe}',str(o)],e);(D/'native_eh_link.log').write_text(p.stdout+p.stderr);assert p.returncode==0
p=subprocess.run([V.WINE,str(exe)],env=e,capture_output=True,text=True);(D/'native_eh_run.log').write_text(p.stdout+p.stderr);print(p.returncode,p.stdout);assert p.returncode==0 and 'CAB370 PASS'in p.stdout
