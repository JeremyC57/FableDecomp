# tc.py <test.cpp>: compile a behaviour test like behaviour_of and print the compiler output
import sys,os
from pathlib import Path
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
t=Path(sys.argv[1]).read_text()
work=Path(os.environ["W"])/"tmp"/"tc"; work.mkdir(parents=True,exist_ok=True)
tp=work/"t.cpp"; tp.write_text(t)
r=V.cl(["/nologo","/c","/Od","/W3",f"/Fo{work/'t.obj'}",str(tp)],V.env())
print(r.stdout[-4000:], r.stderr[-2000:] if r.stderr else "")
