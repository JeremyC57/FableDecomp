# rev.py <path>...: restore pristine copies saved by ed2.py
import sys, os, shutil
S = os.environ["S"]; O = S + "/wave34/orig/"
for p in sys.argv[1:]:
    shutil.copy(O + p, p); os.remove(O + p); print("reverted", p)
