# files.py <addr..>: addr <TAB> lock owner <TAB> HEAD source <TAB> HEAD test (absolute) from the catalog block (nf.py blk)
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319/tools')
import nf
for a in sys.argv[1:]:
    print('%s\t%s\t%s\t%s' % (a, nf.lock_owner(a), nf.src_of(a), nf.test_of(a)))
