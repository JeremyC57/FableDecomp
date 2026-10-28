# Chunk 533 lessons (GOTCHAS one-liners)

- (wave 25, chunk 533) An out-of-line slerp in a TU that is not its class's (0x00A22590, CQuaternion::SetSlerp, in the CRightHandedSet TU) has the same body as the landed CalculateSlerpWeights / SetSlerpFromIdentity retail_asm rows. Reuse their GFFastArcCos / GFFastSin / FloatToLong helpers and cite the dword `fistp` addresses. VC7.1 reorders the four-term dot product itself, so the source term order does not matter.
- (wave 25, chunk 533) A 4-byte `jmp` thunk (0x00A155C0) is an Ego_r forwarder: `RemoveNavigatorFromQueue(nav) { Navigators.remove_if(CIsNavigator(&nav)); }`. Its by-value one-pointer predicate tail-jumps. Name it after the forwarder, not the list member it jumps to.
- (wave 25, chunk 533) STLport list `push_front` reads `_M_node->_M_next` as the insert position (`mov edi,[ecx]` after loading the list head). The plain `push_back` shape loads the head itself (0x00A1CEB0). A two-point route built from an empty list still tells them apart.
- (wave 25, chunk 533) An if/else whose fall-through is the plain assignment (`cmp eax,2 ; je L ; mov [esi+78h],edi ; jmp`) is `if (type != 2) a; else {...}`. Write the test the way retail lays out the fall-through.
- (wave 25, chunk 533) An empty comparator temporary (`CAStarStateTotalGreaterThan()`) needs a user-declared empty constructor. Value-initialisation stores a zero byte into its frame slot otherwise.
- (wave 25, chunk 533) `std::pair<iterator,bool> result = map.insert(make_pair(k, v)); Push(&result.first->second)` computes the element address AFTER the temporaries' destructors, homed in a dead argument slot. Taking `&insert(...).first->second` inline computes it before them.
- (wave 25, chunk 533) Frame-slot order of C3DVector locals is not declaration order in VC7.1 (0x00A18B50: retail puts `step` above edge1/edge2/to). Moving the declaration to function scope rewrote the whole register allocation (815 bytes) instead of moving the slot. Park after the 783v783 best.
- (wave 25, chunk 533) A hidden-pointer `std::pair<C2DVector, C2DVector>` return:
  - Retail builds the first point in a frame temporary, keeps the second on the x87 stack, and tail-merges every gradient branch.
  - Neither named locals, nor temporaries in the return, nor make_pair (by const& or by value), with or without a C2DVector user copy constructor, reproduce it (0x00A1A7E0, best 895v862).
  - Retail also calls GetSize before computing the point X, which suggests a different expression shape.
- (wave 25, chunk 533) Ego_r.pdb is a Dinkumware build (`_Tree`, `_Tmap_traits`) while retail is STLport 4.6. Use Ego_r for function identity and order only. Take container member spellings from the STLport 4.6 text and the landed STLport models (0x00A22360).

## Why the earlier attempts stalled
- 9 of the 10 catalog labels named the wrong function or class (FindValidNavNode, Collision_TestTriangleSphere, GetConnectingPoint, CNavigatorBase_Constructor, AStar_initialize_search, ApplyPartialAnimationAlpha, BuildEulerRotationMatrix, CGameWorld_Destructor). The rows were attempted without their PDB signatures, so neither the twins nor the arities lined up.
- Once the Ego_r / FableWin donor order fixed each identity, the landed twins came into reach:
  - InitialiseFromPreCalculatedRoute for Initialise
  - SetSlerpFromIdentity for SetSlerp
  - the landed ctor for ~CMeshDataBank
  These took three rows to a match and one to 992v992.
- The remaining DIFFERs are x87 scheduling, frame-slot placement and the return construction of a hidden-pointer pair. These are lane gaps that source changes did not move within the 4-grade budget.
