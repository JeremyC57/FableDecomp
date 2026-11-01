# h19_own_gif.py: h19_own.py restricted to the chunk-684 own row 0x009FF6B0 (family gif): OWN_FAM=gif.
import os, sys, runpy
os.environ['OWN_FAM'] = 'gif'
runpy.run_path(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'h19_own.py'), run_name='__main__')
