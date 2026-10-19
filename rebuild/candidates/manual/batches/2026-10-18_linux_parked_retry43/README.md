# Ice-troll icicle assault lifecycle

009627c0 (260): actual state/base/brain and CCreatureAction_IceTrollIcicleAssaultInto headers, native combat-action size114 versus donor12c. RTTI and deleting-destructor callee establish the real base destructor and implicit derived cleanup. Named action result reproduces retail preservation across temporary destruction; explicit completion path merges to the native termination sequence.

Verbatim-source fixtures pass on host and VC7.1 across eight scenarios: absent/dead target, optional stealth component and sentinel, idle cancellation, target death during waiting, action completion/incompletion and repeated waits. Action construction/destruction is balanced before termination. Host -fpermissive models the native VC temporary-address extension. Ten total grades, including final shared action-header integration after grade9 match.

All independently RELOCATION_MATCH + PASS, with verbatim-source host and canonical VC7.1 fixtures.
