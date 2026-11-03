import sys
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import gen_class_headers as G
from pathlib import Path
G.OUT = Path("/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agent719c/gen/engine"); G.QUAR = G.OUT/"_quarantine"
G.INCLUDE = Path("/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agent719c/gen")
sys.argv = ["gen"] + ["--classes"] + sys.argv[1:]
G.main()
