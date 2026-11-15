# agentP6F tools
- g.sh <addr> <src> [overlay] [-v]: grade with q.py (Leaf/Flags from the source), masked diff count (cmpq.py).
- mkpay.py <out.json> <addr>...: one-row vland payload from final/.
- hk.sh <gdb-script> <src> <out.json> [json-config]: run a c2 hook script (sched/host.py) on a row source.
- cnt.sh <src>: c2 constant records at the free walk (value:count:id).  f3ids.sh <src>: root candidate ids (c2prio).
- sched/tw.py: watchpoints on a constant record's use count.  sched/tu.py + showtu.py: tuples seen by the counting walk (opcode, flag, constants).
- sched/tnew.py: constant sym creation call chains.  sched/trst.py: rotation-pointer resets (0x2d3).  sched/tflag.py: c2 debug flags.
- sched/t13.py + showdag.py: block scheduler DAG (agentP6's).  rot.py <q.py output>: scratch-register choices retail v built.
- mc.py <src.cpp> [flags]: compile a micro TU with the verifier's base flags and disassemble it (run under `timeout`).
