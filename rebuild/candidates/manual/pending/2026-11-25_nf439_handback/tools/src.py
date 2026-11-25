import sys; sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439/tools'); import nf
for a in sys.argv[1:]:
    print(a, nf.src_of(a), nf.test_of(a), nf.lock_owner(a))
