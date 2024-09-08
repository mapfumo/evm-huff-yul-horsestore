# EVM Assembly, Opcodes, Yul, Huff and Formal Verification

## Generate bytecode

```sh
$huffc src/horseStoreV1/HorseStore.huff --bin-runtime
```

## Deploying Huff Smart Contract in Foundry & Debugging

```sh
$forge install huff-language/foundry-huff --no-commit
```

In foundry.toml

```toml
ffi = true

remappings = ['foundry-huff/=lib/foundry-huff/src/']
```

```sh
$forge test --match-path *Huff*  --debug testWriteValue -vvvv
```

## Horse Store

1. Write a basic simplestorage/horse store
2. Write horse store in huff
   1. Where did this data come from? How did Remix know to send this data
      1. 0xcdfead2e0000000000000000000000000000000000000000000000000000000000000001
      2. How does the EVM know how to interpret with data?
   2. How does remx know how to update the number of horses with ths data?

# NOTES

1. Every smart contract starts with a function dispatcher - this is when you check the call data sent to the contract. This is how you know which function to call.
2. Once you find the matching function you route the call to code associated with that function

## Function Interface Declaration:

```solidity
#define function updateHorseNumber(uint256) nonpayable returns ()
#define function readNumberOfHorses() view returns (uint256)
```

## Storage Slot Definition:

````solidity
#define constant NUMBER_OF_HORSES_STORAGE_SLOT = FREE_STORAGE_POINTER()```
````

Defines a constant for the storage slot where the number of horses will be stored. `FREE_STORAGE_POINTER()` is used to assign slot 0 as the storage location for `numberOfHorses`.

## Main Macro: MAIN()

```solidity
#define macro MAIN() = takes(0) returns(0) {
    0x00 calldataload 0xe0 shr // [function_selector
```

### Function Selector Matching

```solidity
dup1 __FUNC_SIG(updateHorseNumber) eq updateJump jumpi // [function_selector]
    __FUNC_SIG(readNumberOfHorses) eq readJump jumpi // [function_selector]
```

- `dup1`: Duplicates the function selector on the stack.
- `__FUNC_SIG(updateHorseNumber)`: Inserts the function selector for updateHorseNumber.
- `eq`: Compares the duplicated function selector with \_\_FUNC_SIG(updateHorseNumber).
- `updateJump jumpi: Jumps to the updateJump label if the function selector matches updateHorseNumber.
- The same pattern is used to check for the readNumberOfHorses function selector.
- If no function selector matches, the code reverts:

### HorseStore Solidity & Huff Key Differences and Optimizations

- Manual Storage Management: Huff requires explicit storage slot management, while Solidity handles this automatically.
- Low-level Operations: Huff uses EVM opcodes directly, allowing for more gas-efficient code.
- Function Dispatching: The Huff code implements manual function dispatching, which Solidity does automatically.
- No High-level Constructs: Huff lacks high-level constructs like variables or control structures, requiring everything to be implemented using stack operations and jumps.
- Gas Optimization: The Huff implementation can potentially be more gas-efficient due to its low-level nature, allowing for fine-grained control over operations.

## Reference

https://updraft.cyfrin.io/courses/formal-verification
