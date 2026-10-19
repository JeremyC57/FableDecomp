import runpy,sys
from pathlib import Path
sys.path.insert(0,str(Path('tools/decomp_pipeline').resolve()))
import verify_land_jumptable as j
j.val.EXTRA_FLAGSETS=[]
j.val.PRAGMAS=[]
j.val.main()
