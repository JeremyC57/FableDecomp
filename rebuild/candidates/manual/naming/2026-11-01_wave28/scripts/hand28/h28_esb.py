# h28_esb.py: wrapper -- h28_dtorfam.py esb
import os, sys, runpy
sys.argv = ["h28_dtorfam.py", "esb"]
runpy.run_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), "h28_dtorfam.py"), run_name="__main__")
