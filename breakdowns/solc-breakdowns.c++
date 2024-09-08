// 0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220c276cfaa26a978c7cbd6472fe8f6998f4608429a6fb1e97f12f1b1fd0b52111d64736f6c63430008140033

// 3 sections:
// 1. Contract Creation
// 2. Runtime Code
// 3. Metadata

// Contract Creation Code
// Free Memory Pointer 0x40 is pointing to free memory slot at 0x80
// 0x40 always points to the free memory slot
PUSH1 0x80          // [0x80]
PUSH1 0x40          // [0x40, 0x80]
MSTORE              // [] // memory 0x40 -> 0x80

// what's this chunk do?
// If someone sent value with this call, revert
// otherwise, jump, to continue execution
CALLVALUE           // [msg.value] - check msg.value and put it on the stack
DUP1                // [msg.value, msg.value] - duplicate top of the stack
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0, msg.value]
JUMPI               // [msg.value] - jumping if value == 0
PUSH0               // [0x00, msg.value] - pushing 0 to the stack
DUP1                // [0x00, 0x00, msg.value] - duplicate first stack item
REVERT              // [msg.value]

// Jump dest if msg.value == 0
// Sticks the runtime code on chain
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0xa5          // [0xa5]
DUP1                // [0xa5, 0xa5]
PUSH2 0x001b        // [0x001b, 0xa5, 0xa5]
PUSH0               // [0x00, 0x00, 0xa5, 0xa5]
CODECOPY            // [0xa5]  Memory:[runtime code]
PUSH0               // [0x00, 0xa5]
RETURN              // []
INVALID             // [] doesn't do anything - end of contract creation

// Runtime Code
// Entry point for all calls
// free memory pointer
PUSH1 0x80
PUSH1 0x40
MSTORE

// Checking for msg.value, and if given, reverting
CALLVALUE           // [msg.value]
DUP1                // [msg.value, msg.value]
ISZERO              // [msg.value == 0, msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0, msg.value]
JUMPI               // [msg.value]  
//jumtp to "continue!" if msg.value == 0
PUSH0               // [0x00, msg.value]    
DUP1                // [0x00, 0x00, msg.value]
REVERT              // [msg.value] 

// if msg.value == 0 start here!
// continue
// This is checking to see if there is enough calldata for a function selector
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0x04          // [0x04]
CALLDATASIZE        // [calldata_size, 0x04]
LT                  // [calldata_size < 0x04]
PUSH1 0x30          // [0x30, calldata_size < 0x04]
JUMPI               // []
// if calldata_size < 0x04 -> calldata_jump

// function dispatching in Solidity
PUSH0               // [0x00]
CALLDATALOAD        // [32 bytes of calldata]
PUSH1 0xe0          // [0xe0, 32 bytes of calldata]
SHR                 // [calldata[0:4]] - function selector

// Dispatching for setNumberOfHorses
DUP1                // [func_selector, func_selector]
PUSH4 0xcdfead2e    // [0xcdfead2e, func_selector, func_selector]
EQ                  // [func_selector == 0xcdfead2e, func_selector]
PUSH1 0x34          // [0x34, func_selector == 0xcdfead2e, func_selector]
JUMPI               // [func_selector]
// if func_selector == 0xcdfead2e -> updateHorseNumber (set_number_of_horses)

// function dispatching for read number of horses
DUP1                // [func_selector, func_selector]
PUSH4 0xe026c017    // [0xe026c017, func_selector, func_selector]
EQ                  // [func_selector == 0xe026c017, func_selector]
PUSH1 0x45          // [0x45, func_selector == 0xe026c017, func_selector]
JUMPI               // [func_selector]
// if func_selector == 0xe026c017 -> readNumberOfHorses (get_number_of_horses)

// calldata_jump
// revert jumpdest
JUMPDEST            // []
PUSH0               // [0x00]
DUP1                // [0x00, 0x00]
REVERT              // []

// updateHorseNumber jump dest 1
// Setup jumping program counters in the stack
JUMPDEST            // [func_selector]
PUSH1 0x43          // [0x43, func_selector]
PUSH1 0x3f          // [0x3f, 0x43, func_selector]
CALLDATASIZE        // [calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x04          // [0x04, calldata_size, 0x3f, 0x43, func_selector] 
PUSH1 0x59          // [0x59, 0x04, calldata_size, 0x3f, 0x43, func_selector]
JUMP                // [0x04, calldata_size, 0x3f, 0x43, func_selector]

// Just dest 4
// We can finally run an SSTORE to save our value to storage
// 1. Function dispatch
// 2. Checked for msg.value
// 3. Checked that calldata is long enough
// 4. Recieved the number to use from the caldata
JUMPDEST            // [calldata (of numberToUpdate), 0x43, func_selector]
PUSH0               // [0x00, calldata (of numberToUpdate), 0x43, func_selector]
SSTORE              // [0x43, func_selector]
JUMP                // [func_selector]
// Jump to dest 5

// Jump to dest 5
JUMPDEST            // [func_selector]
STOP                // [func_selector]


JUMPDEST
PUSH0
SLOAD
PUSH1 0x40
MLOAD
SWAP1
DUP2
MSTORE
PUSH1 0x20
ADD
PUSH1 0x40
MLOAD
DUP1
SWAP2
SUB
SWAP1
RETURN


// updateHorseNumber jump dest 2
// Check to see if there is a value to update the horse number to
// 4 bytes for function selector, 32 (ox20) bytes for horse number
JUMPDEST            // [0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH0               // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x20          // [0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP3                // [0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP5                // [calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
SUB                 // [calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
// is there mopre calldata than just the function selector?
SLT                 // [calldata_size - 0x04 < 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
ISZERO              // [more_calldata_than_function_selector?, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH1 0x68          // [0x68, more_calldata_than_function_selector?, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
JUMPI               // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
// We are going to jump to jump dest 3 if there is more calldata than:
// function selector + 0x20

// revert if there isn't enough calldata!
PUSH0               // [0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
DUP1                // [0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]
REVERT              // [0x00, 0x04, calldata_size, 0x3f, 0x43, func_selector]


// updateHorseNumber jump dest 3
// Grab the call data for updating the horse number
// Delete some stuff in the stack
JUMPDEST            // [0, 0x04, calldata_size, 0x3f, 0x43, func_selector]
POP                 // [0x04, calldata_size, 0x3f, 0x43, func_selector]
// Ignore the function selector, and grab the data
//0xCDFEAD2E  0000000000000000000000000000000000000000000000000000000000000007
CALLDATALOAD        // [calldata (of numberToUpdate), calldata_size, 0x3f, 0x43, func_selector]
SWAP2               // [0x3f, calldata_size, calldata (of numberToUpdate), 0x43, func_selector]
SWAP1               // [calldata_size, 0x3f, calldata (of numberToUpdate), 0x43, func_selector]
POP                 // [0x3f, calldata (of numberToUpdate), 0x43, func_selector]
JUMP
// jump to dest 4

INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
INVALID
PUSH23 0xcfaa26a978c7cbd6472fe8f6998f4608429a6fb1e97f12
CALL
INVALID
REVERT
SIGNEXTEND
MSTORE
GT
SAR
PUSH5 0x736f6c6343
STOP
ADDMOD
EQ
STOP
CALLER